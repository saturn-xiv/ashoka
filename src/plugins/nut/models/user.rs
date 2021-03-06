use std::fmt;

use actix_web::http::StatusCode;
use chrono::{NaiveDateTime, Utc};
use diesel::{insert_into, prelude::*, update};
use uuid::Uuid;

use super::super::super::super::{
    crypto::Password,
    errors::{Error, Result},
    oauth::google::openid::IdToken,
    orm::{schema::users, Connection},
};

#[derive(Debug, Serialize, Deserialize, Clone)]
pub enum Type {
    Google,
    Facebook,
    Line,
    Github,
    WeChat,
    Email,
}

impl fmt::Display for Type {
    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Type::Google => fmt.write_str("google"),
            Type::Facebook => fmt.write_str("facebook"),
            Type::Github => fmt.write_str("github"),
            Type::WeChat => fmt.write_str("wechat"),
            Type::Line => fmt.write_str("line"),
            Type::Email => fmt.write_str("email"),
        }
    }
}

#[derive(Queryable, Serialize, Deserialize, Debug, Clone)]
#[serde(rename_all = "camelCase")]
pub struct Item {
    pub id: i64,
    pub first_name: String,
    pub last_name: String,
    pub email: String,
    pub password: Option<Vec<u8>>,
    pub uid: String,
    pub provider_type: String,
    pub provider_id: String,
    pub access_token: Option<String>,
    pub logo: String,
    pub sign_in_count: i64,
    pub current_sign_in_at: Option<NaiveDateTime>,
    pub current_sign_in_ip: Option<String>,
    pub last_sign_in_at: Option<NaiveDateTime>,
    pub last_sign_in_ip: Option<String>,
    pub confirmed_at: Option<NaiveDateTime>,
    pub locked_at: Option<NaiveDateTime>,
    pub deleted_at: Option<NaiveDateTime>,
    pub version: i64,
    pub created_at: NaiveDateTime,
    pub updated_at: NaiveDateTime,
}

impl fmt::Display for Item {
    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {
        write!(
            fmt,
            "{} {}<{}>",
            self.first_name, self.last_name, self.email
        )
    }
}

impl Item {
    pub fn real_name(&self) -> String {
        format!("{} {}", self.first_name, self.last_name)
    }
    pub fn available(&self) -> Result<()> {
        let who = self.to_string();
        if self.deleted_at.is_some() {
            return Err(Error::Http(StatusCode::NOT_FOUND, Some(who)));
        }
        if self.locked_at.is_some() {
            return Err(Error::Http(StatusCode::LOCKED, Some(who)));
        }
        if self.confirmed_at.is_none() {
            return Err(Error::Http(StatusCode::FORBIDDEN, Some(who)));
        }
        Ok(())
    }
    pub fn auth<E: Password>(&self, password: &str) -> Result<()> {
        if let Some(ref v) = self.password {
            if E::verify(v, password.as_bytes()) {
                return Ok(());
            }
        }
        Err(Error::Http(
            StatusCode::UNAUTHORIZED,
            Some(self.to_string()),
        ))
    }
}

#[derive(Insertable)]
#[table_name = "users"]
pub struct New<'a> {
    pub first_name: &'a str,
    pub last_name: &'a str,
    pub email: &'a str,
    pub password: Option<&'a [u8]>,
    pub uid: &'a str,
    pub provider_type: &'a str,
    pub provider_id: &'a str,
    pub logo: &'a str,
    pub updated_at: &'a NaiveDateTime,
}

pub trait Dao {
    fn by_id(&self, id: i64) -> Result<Item>;
    fn by_uid(&self, uid: &str) -> Result<Item>;
    fn by_email(&self, email: &str) -> Result<Item>;
    fn set_profile(&self, id: i64, first_name: &str, last_name: &str, logo: &str) -> Result<()>;
    fn sign_in(&self, id: i64, ip: &str) -> Result<()>;
    fn google(&self, access_token: &str, token: &IdToken, ip: &str) -> Result<Item>;
    fn sign_up<T: Password>(
        &self,
        first_name: &str,
        last_name: &str,
        email: &str,
        password: &str,
    ) -> Result<()>;
    fn lock(&self, id: i64, on: bool) -> Result<()>;
    fn confirm(&self, id: i64) -> Result<()>;
    fn count(&self) -> Result<i64>;
    fn all(&self) -> Result<Vec<Item>>;
    fn password<T: Password>(&self, id: i64, password: &str) -> Result<()>;
}

impl Dao for Connection {
    fn by_id(&self, id: i64) -> Result<Item> {
        let it = users::dsl::users
            .filter(users::dsl::id.eq(id))
            .first(self)?;
        Ok(it)
    }

    fn by_uid(&self, uid: &str) -> Result<Item> {
        let it = users::dsl::users
            .filter(users::dsl::uid.eq(uid))
            .first(self)?;
        Ok(it)
    }

    fn by_email(&self, email: &str) -> Result<Item> {
        let it = users::dsl::users
            .filter(users::dsl::email.eq(&email.trim().to_lowercase()))
            .first(self)?;
        Ok(it)
    }

    fn google(&self, access_token: &str, id_token: &IdToken, ip: &str) -> Result<Item> {
        let now = Utc::now().naive_utc();
        let type_ = Type::Google.to_string();
        let it = match users::dsl::users
            .filter(users::dsl::provider_id.eq(&id_token.sub))
            .filter(users::dsl::provider_type.eq(&type_))
            .first::<Item>(self)
        {
            Ok(it) => {
                // if let Some(ref name) = id_token.name {
                //     update(users::dsl::users.filter(users::dsl::id.eq(it.id)))
                //         .set(users::dsl::first_name.eq(&name))
                //         .execute(self)?;
                // }
                if let Some(ref email) = id_token.email {
                    update(users::dsl::users.filter(users::dsl::id.eq(it.id)))
                        .set(users::dsl::email.eq(&email))
                        .execute(self)?;
                }
                if let Some(ref logo) = id_token.picture {
                    update(users::dsl::users.filter(users::dsl::id.eq(it.id)))
                        .set(users::dsl::logo.eq(&logo))
                        .execute(self)?;
                }
                it
            }
            Err(_) => {
                let email = match id_token.email {
                    Some(ref v) => v.clone(),
                    None => format!("{}@gmail.com", id_token.sub),
                };
                let uid = Uuid::new_v4().to_string();
                insert_into(users::dsl::users)
                    .values(&New {
                        first_name: &match id_token.name {
                            Some(ref v) => v.clone(),
                            None => "Guest".to_string(),
                        },
                        last_name: &type_,
                        email: &email,
                        password: None,
                        provider_type: &Type::Google.to_string(),
                        provider_id: &id_token.sub,
                        logo: &match id_token.picture {
                            Some(ref v) => v.clone(),
                            None => format!(
                                "https://www.gravatar.com/avatar/{}.jpg",
                                gravatar_hash(&email)
                            ),
                        },
                        uid: &uid,
                        updated_at: &now,
                    })
                    .execute(self)?;
                self.by_uid(&uid)?
            }
        };
        update(users::dsl::users.filter(users::dsl::id.eq(it.id)))
            .set(users::dsl::access_token.eq(&Some(access_token)))
            .execute(self)?;
        self.sign_in(it.id, ip)?;

        Ok(it)
    }

    fn sign_in(&self, id: i64, ip: &str) -> Result<()> {
        let now = Utc::now().naive_utc();
        let (current_sign_in_at, current_sign_in_ip, sign_in_count) = users::dsl::users
            .select((
                users::dsl::current_sign_in_at,
                users::dsl::current_sign_in_ip,
                users::dsl::sign_in_count,
            ))
            .filter(users::dsl::id.eq(id))
            .first::<(Option<NaiveDateTime>, Option<String>, i64)>(self)?;
        update(users::dsl::users.filter(users::dsl::id.eq(id)))
            .set((
                users::dsl::current_sign_in_at.eq(&now),
                users::dsl::current_sign_in_ip.eq(&Some(ip)),
                users::dsl::last_sign_in_at.eq(&current_sign_in_at),
                users::dsl::last_sign_in_ip.eq(&current_sign_in_ip),
                users::dsl::sign_in_count.eq(&(sign_in_count + 1)),
                users::dsl::updated_at.eq(&now),
            ))
            .execute(self)?;
        Ok(())
    }
    fn sign_up<T: Password>(
        &self,
        first_name: &str,
        last_name: &str,
        email: &str,
        password: &str,
    ) -> Result<()> {
        let email = email.trim().to_lowercase();
        insert_into(users::dsl::users)
            .values(&New {
                first_name: &first_name.trim().to_lowercase(),
                last_name: &last_name.trim().to_lowercase(),
                email: &email,
                password: Some(&T::sum(password.as_bytes())?),
                provider_type: &Type::Email.to_string(),
                provider_id: &email,
                logo: &format!(
                    "https://www.gravatar.com/avatar/{}.jpg",
                    gravatar_hash(&email)
                ),
                uid: &Uuid::new_v4().to_string(),
                updated_at: &Utc::now().naive_utc(),
            })
            .execute(self)?;
        Ok(())
    }

    fn lock(&self, id: i64, on: bool) -> Result<()> {
        let now = Utc::now().naive_utc();
        let it = users::dsl::users.filter(users::dsl::id.eq(id));
        update(it)
            .set((
                users::dsl::locked_at.eq(&if on { Some(now) } else { None }),
                users::dsl::updated_at.eq(&now),
            ))
            .execute(self)?;
        Ok(())
    }

    fn set_profile(&self, id: i64, first_name: &str, last_name: &str, logo: &str) -> Result<()> {
        let now = Utc::now().naive_utc();
        update(users::dsl::users.filter(users::dsl::id.eq(id)))
            .set((
                users::dsl::first_name.eq(first_name),
                users::dsl::last_name.eq(last_name),
                users::dsl::logo.eq(logo),
                users::dsl::updated_at.eq(&now),
            ))
            .execute(self)?;
        Ok(())
    }

    fn confirm(&self, id: i64) -> Result<()> {
        let now = Utc::now().naive_utc();
        let it = users::dsl::users.filter(users::dsl::id.eq(id));
        update(it)
            .set((
                users::dsl::confirmed_at.eq(&Some(now)),
                users::dsl::updated_at.eq(&now),
            ))
            .execute(self)?;
        Ok(())
    }

    fn count(&self) -> Result<i64> {
        let cnt: i64 = users::dsl::users.count().get_result(self)?;
        Ok(cnt)
    }

    fn all(&self) -> Result<Vec<Item>> {
        let items = users::dsl::users
            .order(users::dsl::updated_at.desc())
            .load::<Item>(self)?;
        Ok(items)
    }

    fn password<T: Password>(&self, id: i64, password: &str) -> Result<()> {
        let now = Utc::now().naive_utc();
        let password = T::sum(password.as_bytes())?;
        let it = users::dsl::users.filter(users::dsl::id.eq(id));
        update(it)
            .set((
                users::dsl::password.eq(&Some(password)),
                users::dsl::updated_at.eq(&now),
            ))
            .execute(self)?;
        Ok(())
    }
}

// https://en.gravatar.com/site/implement/hash/
pub fn gravatar_hash<S: AsRef<str>>(email: &S) -> String {
    format!(
        "{:x}",
        md5::compute(email.as_ref().to_lowercase().trim().as_bytes())
    )
}
