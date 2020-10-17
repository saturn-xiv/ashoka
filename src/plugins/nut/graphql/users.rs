use std::ops::Deref;

use actix_web::http::StatusCode;
use chrono::{Duration, NaiveDate, NaiveDateTime};
use diesel::Connection;
use juniper::{GraphQLInputObject, GraphQLObject};
use validator::Validate;

use super::super::super::super::{
    crypto::Crypto,
    errors::{Error, Result},
    graphql::{context::Context, Pager, Pagination, I64},
    i18n::I18n,
    jwt::Jwt,
    orm::Connection as Db,
    queue::{RabbitMQ, Task},
};
use super::super::{
    models::{
        log::{Dao as LogDao, Item as LogItem},
        policy::{Dao as PolicyDao, Item as PolicyItem, Role},
        user::{Dao as UserDao, Item as UserItem},
    },
    request::{Action, Token},
    tasks::send_email,
};

#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersSignIn")]
pub struct SignIn {
    #[validate(length(min = 1))]
    pub email: String,
    #[validate(length(min = 1))]
    pub password: String,
}

impl SignIn {
    pub fn execute(&self, ctx: &Context) -> Result<String> {
        self.validate()?;
        let db = ctx.db.deref();
        let user: Result<UserItem> = self.select(db).ok_or(__i18n_e!(
            db,
            &ctx.locale,
            "nut.errors.user.is-not-exist",
            &json!({"email": self.email})
        ));
        let user = user?;
        if let Err(e) = user.auth::<Crypto>(&self.password) {
            __i18n_l!(
                db,
                user.id,
                &ctx.client_ip,
                &ctx.locale,
                "nut.logs.user.sign-in.wrong-password"
            )?;
            return Err(e);
        }
        user.available()?;

        let uid = user.uid.clone();
        let name = user.real_name();
        db.transaction::<_, Error, _>(move || {
            UserDao::sign_in(db, user.id, &ctx.client_ip)?;
            __i18n_l!(
                db,
                user.id,
                &ctx.client_ip,
                &ctx.locale,
                "nut.logs.user.sign-in.success"
            )?;
            Ok(())
        })?;
        let (nbf, exp) = Jwt::timestamps(Duration::weeks(1));
        let token = ctx.jwt.sum(
            None,
            &Token {
                uid,
                sub: name,
                act: Action::SignIn,
                nbf,
                exp,
            },
        )?;

        Ok(token)
    }

    fn select(&self, db: &Db) -> Option<UserItem> {
        if let Ok(it) = UserDao::by_email(db, &self.email) {
            return Some(it);
        }
        None
    }
}

#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersSignUp")]
pub struct SignUp {
    #[validate(length(min = 2, max = 32))]
    pub first_name: String,
    #[validate(length(min = 2, max = 32))]
    pub last_name: String,
    #[validate(length(min = 1, max = 255), email)]
    pub email: String,
    #[validate(length(min = 6, max = 32))]
    pub password: String,
    #[validate(length(min = 6))]
    pub home: String,
}

impl SignUp {
    pub async fn execute(&self, ctx: &Context) -> Result<()> {
        let user = self.save(ctx)?;

        let db = ctx.db.deref();
        EmailForm::send_email(
            db,
            &ctx.locale,
            &ctx.jwt,
            &ctx.queue,
            &user,
            &Action::Confirm,
            &self.home,
        )
        .await?;
        Ok(())
    }
    pub fn save(&self, ctx: &Context) -> Result<UserItem> {
        self.validate()?;
        let db = ctx.db.deref();
        if UserDao::by_email(db, &self.email).is_ok() {
            return Err(__i18n_e!(
                db,
                &ctx.locale,
                "nut.errors.user.already-exist",
                &json!({"email": self.email})
            ));
        }

        let user = db.transaction::<_, Error, _>(move || {
            UserDao::sign_up::<Crypto>(
                db,
                &self.first_name,
                &self.last_name,
                &self.email,
                &self.password,
            )?;
            let it = UserDao::by_email(db, &self.email)?;
            __i18n_l!(
                db,
                it.id,
                &ctx.client_ip,
                &ctx.locale,
                "nut.logs.user.sign-up"
            )?;
            Ok(it)
        })?;
        Ok(user)
    }
}

#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersEmailForm")]
pub struct EmailForm {
    #[validate(length(min = 1))]
    pub email: String,
    #[validate(length(min = 1))]
    pub home: String,
}

impl EmailForm {
    pub async fn confirm(&self, ctx: &Context) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();
        let it = UserDao::by_email(db, &self.email)?;
        if it.confirmed_at.is_some() {
            return Err(__i18n_e!(
                db,
                &ctx.locale,
                "nut.errors.user.already-confirm"
            ));
        }
        Self::send_email(
            db,
            &ctx.locale,
            &ctx.jwt,
            &ctx.queue,
            &it,
            &Action::Confirm,
            &self.home,
        )
        .await?;
        Ok(())
    }

    pub async fn unlock(&self, ctx: &Context) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();
        let it = UserDao::by_email(db, &self.email)?;
        if None == it.locked_at {
            return Err(__i18n_e!(db, &ctx.locale, "nut.errors.user.is-not-lock"));
        }
        Self::send_email(
            &db,
            &ctx.locale,
            &ctx.jwt,
            &ctx.queue,
            &it,
            &Action::Unlock,
            &self.home,
        )
        .await?;
        Ok(())
    }

    pub async fn forgot_password(&self, ctx: &Context) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();
        let it = UserDao::by_email(db, &self.email)?;
        Self::send_email(
            db,
            &ctx.locale,
            &ctx.jwt,
            &ctx.queue,
            &it,
            &Action::ResetPassword,
            &self.home,
        )
        .await?;
        Ok(())
    }

    async fn send_email(
        db: &Db,
        lang: &str,
        jwt: &Jwt,
        queue: &RabbitMQ,
        user: &UserItem,
        act: &Action,
        home: &str,
    ) -> Result<()> {
        let expire = 1;
        let (nbf, exp) = Jwt::timestamps(Duration::hours(expire));
        let token = jwt.sum(
            None,
            &Token {
                uid: user.uid.clone(),
                sub: user.real_name(),
                act: act.clone(),
                nbf,
                exp,
            },
        )?;

        let args =
            Some(json!({ "name": user.first_name, "home": home, "expire":expire, "token":token }));
        let subject = I18n::t(db, lang, format!("nut.mailer.users.{}.subject", act), &args);
        let body = I18n::t(db, lang, format!("nut.mailer.users.{}.body", act), &args);

        queue
            .publish(
                send_email::NAME,
                Task::new(&send_email::Task {
                    email: user.email.clone(),
                    name: user.first_name.clone(),
                    subject,
                    body,
                })?,
            )
            .await?;
        Ok(())
    }
}

#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersTokenForm")]
pub struct TokenForm {
    #[validate(length(min = 1))]
    pub token: String,
}

impl TokenForm {
    pub fn confirm(&self, ctx: &Context) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();

        let token = ctx.jwt.parse::<Token>(&self.token)?.claims;
        if token.act != Action::Confirm {
            return Err(Error::Http(StatusCode::NOT_ACCEPTABLE, None));
        }

        let it = UserDao::by_uid(db, &token.uid)?;
        if it.confirmed_at.is_some() {
            return Err(__i18n_e!(
                db,
                &ctx.locale,
                "nut.errors.user.already-confirm"
            ));
        }

        db.transaction::<_, Error, _>(move || {
            UserDao::confirm(db, it.id)?;
            __i18n_l!(
                db,
                it.id,
                &ctx.client_ip,
                &ctx.locale,
                "nut.logs.user.confirm"
            )?;
            Ok(())
        })?;
        Ok(())
    }

    pub fn unlock(&self, ctx: &Context) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();
        let token = ctx.jwt.parse::<Token>(&self.token)?.claims;
        if token.act != Action::Unlock {
            return Err(Error::Http(StatusCode::NOT_ACCEPTABLE, None));
        }
        let it = UserDao::by_uid(db, &token.uid)?;
        if None == it.locked_at {
            return Err(__i18n_e!(db, &ctx.locale, "nut.errors.user.is-not-lock"));
        }
        db.transaction::<_, Error, _>(move || {
            UserDao::lock(db, it.id, false)?;
            __i18n_l!(
                db,
                it.id,
                &ctx.client_ip,
                &ctx.locale,
                "nut.logs.user.unlock"
            )?;
            Ok(())
        })?;
        Ok(())
    }
}

#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersResetPassword")]
pub struct ResetPassword {
    #[validate(length(min = 1))]
    pub token: String,
    #[validate(length(min = 6, max = 32))]
    pub password: String,
}

impl ResetPassword {
    pub fn execute(&self, ctx: &Context) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();

        let token = ctx.jwt.parse::<Token>(&self.token)?.claims;
        if token.act != Action::ResetPassword {
            return Err(Error::Http(StatusCode::NOT_ACCEPTABLE, None));
        }

        let it = UserDao::by_uid(db, &token.uid)?;

        UserDao::password::<Crypto>(db, it.id, &self.password)?;
        __i18n_l!(
            db,
            it.id,
            &ctx.client_ip,
            &ctx.locale,
            "nut.logs.user.reset-password"
        )?;
        Ok(())
    }
}

#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersChangePassword")]
pub struct ChangePassword {
    #[validate(length(min = 1))]
    pub current_password: String,
    #[validate(length(min = 6, max = 32))]
    pub new_password: String,
}

impl ChangePassword {
    pub fn execute(&self, ctx: &Context) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();
        let user = ctx.current_user()?;

        user.auth::<Crypto>(&self.current_password)?;
        db.transaction::<_, Error, _>(move || {
            UserDao::password::<Crypto>(db, user.id, &self.new_password)?;
            __i18n_l!(
                db,
                user.id,
                &ctx.client_ip,
                &ctx.locale,
                "nut.logs.user.change-password"
            )?;
            Ok(())
        })?;
        Ok(())
    }
}

pub struct SignOut;

impl SignOut {
    pub fn execute(ctx: &Context) -> Result<()> {
        let db = ctx.db.deref();
        let user = ctx.current_user()?;

        __i18n_l!(
            db,
            user.id,
            &ctx.client_ip,
            &ctx.locale,
            "nut.logs.user.sign-out"
        )?;
        Ok(())
    }
}

#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersUpdateProfile")]
pub struct Profile {
    #[validate(length(min = 2, max = 32))]
    pub first_name: String,
    #[validate(length(min = 2, max = 32))]
    pub last_name: String,
    #[validate(length(min = 1))]
    pub logo: String,
}

impl Profile {
    pub fn execute(&self, ctx: &Context) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();
        let user = ctx.current_user()?;

        UserDao::set_profile(db, user.id, &self.first_name, &self.last_name, &self.logo)?;
        Ok(())
    }
}

#[derive(GraphQLObject)]
pub struct Policy {
    pub role: String,
    pub resource: Option<String>,
    pub nbf: NaiveDate,
    pub exp: NaiveDate,
    pub updated_at: NaiveDateTime,
}

impl From<PolicyItem> for Policy {
    fn from(it: PolicyItem) -> Self {
        Self {
            role: it.role,
            resource: it.resource,
            nbf: it.nbf,
            exp: it.exp,
            updated_at: it.updated_at,
        }
    }
}

impl Policy {
    pub fn index(ctx: &Context, id: i64) -> Result<Vec<Self>> {
        ctx.administrator()?;
        let db = ctx.db.deref();
        Ok(PolicyDao::all(db, id)?
            .into_iter()
            .map(|it| it.into())
            .collect::<_>())
    }
}

#[derive(GraphQLObject)]
pub struct CurrentUser {
    pub first_name: String,
    pub last_name: String,
    pub email: String,
    pub policies: Vec<Policy>,
}

impl CurrentUser {
    pub fn new(ctx: &Context) -> Result<Self> {
        let user = ctx.current_user()?;
        let db = ctx.db.deref();
        Ok(Self {
            first_name: user.first_name.clone(),
            last_name: user.last_name.clone(),
            email: user.email.clone(),
            policies: PolicyDao::all(db, user.id)?
                .into_iter()
                .map(|it| it.into())
                .collect::<_>(),
        })
    }
}

#[derive(GraphQLObject)]
pub struct Log {
    pub id: I64,
    pub ip: String,
    pub message: String,
    pub created_at: NaiveDateTime,
}

impl From<LogItem> for Log {
    fn from(it: LogItem) -> Self {
        Self {
            id: it.id.into(),
            ip: it.ip,
            message: it.message,
            created_at: it.created_at,
        }
    }
}

#[derive(GraphQLObject)]
pub struct Logs {
    pagination: Pagination,
    items: Vec<Log>,
}

impl Logs {
    pub fn new(ctx: &Context, pag: &Pager) -> Result<Self> {
        let user = ctx.current_user()?;
        let db = ctx.db.deref();
        let total = LogDao::count(db, user.id)?;
        Ok(Self {
            pagination: Pagination::new(total, pag),
            items: LogDao::all(db, user.id, pag.offset(total), pag.limit())?
                .into_iter()
                .map(|it| it.into())
                .collect::<_>(),
        })
    }
}

pub struct Lock;

impl Lock {
    pub fn execute(ctx: &Context, id: I64) -> Result<()> {
        ctx.administrator()?;
        let db = ctx.db.deref();

        let user = id.0;
        if PolicyDao::is(db, user, &Role::Root) {
            return Err(Error::Http(StatusCode::FORBIDDEN, None));
        }
        db.transaction::<_, Error, _>(move || {
            UserDao::lock(db, user, true)?;
            __i18n_l!(db, user, &ctx.client_ip, &ctx.locale, "nut.logs.user.lock")?;
            Ok(())
        })?;
        Ok(())
    }
}

#[derive(GraphQLObject)]
pub struct User {
    pub id: I64,
    pub first_name: String,
    pub last_name: String,
    pub email: String,
    pub logo: String,
    pub updated_at: NaiveDateTime,
}

impl From<UserItem> for User {
    fn from(it: UserItem) -> Self {
        Self {
            id: it.id.into(),
            first_name: it.first_name,
            last_name: it.last_name,
            email: it.email,
            logo: it.logo,
            updated_at: it.updated_at,
        }
    }
}

impl User {
    pub fn index(ctx: &Context) -> Result<Vec<Self>> {
        ctx.administrator()?;
        let db = ctx.db.deref();
        Ok(UserDao::all(db)?
            .into_iter()
            .map(|it| it.into())
            .collect::<_>())
    }
}
#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersApplyPolicy")]
pub struct Apply {
    #[validate(length(min = 1))]
    pub role: String,
    pub resource: Option<String>,
    pub nbf: NaiveDate,
    pub exp: NaiveDate,
}

impl Apply {
    pub fn execute(&self, ctx: &Context, id: I64) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();
        ctx.administrator()?;

        let user = UserDao::by_id(db, id.0)?;
        db.transaction::<_, Error, _>(move || {
            PolicyDao::apply(
                db,
                user.id,
                &self.role.parse()?,
                &self.resource,
                &self.nbf,
                &self.exp,
            )?;
            __i18n_l!(
                db,
                user.id,
                &ctx.client_ip,
                &ctx.locale,
                "nut.logs.user.role.apply",
                json!({"role": self.role})
            )?;

            Ok(())
        })
    }
}
#[derive(GraphQLInputObject, Validate)]
#[graphql(name = "UsersDenyPolicy")]
pub struct Deny {
    #[validate(length(min = 1))]
    pub role: String,
    pub resource: Option<String>,
}

impl Deny {
    pub fn execute(&self, ctx: &Context, id: I64) -> Result<()> {
        self.validate()?;
        let db = ctx.db.deref();
        ctx.administrator()?;

        let user = UserDao::by_id(db, id.0)?;
        db.transaction::<_, Error, _>(move || {
            PolicyDao::deny(db, user.id, &self.role.parse()?, &self.resource)?;
            __i18n_l!(
                db,
                user.id,
                &ctx.client_ip,
                &ctx.locale,
                "nut.logs.user.role.deny",
                json!({"role": self.role})
            )?;
            Ok(())
        })?;
        Ok(())
    }
}
