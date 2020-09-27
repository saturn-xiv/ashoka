use std::ops::Deref;
use std::sync::Arc;

use actix_web::http::StatusCode;

use super::super::{
    cache::Pool as Cache,
    crypto::Crypto,
    errors::{Error, Result},
    jwt::Jwt,
    orm::PooledConnection as Db,
    plugins::nut::models::{
        policy::{Dao as PolicyDao, Role},
        user::Item as User,
    },
    queue::RabbitMQ,
};

pub struct Context {
    pub locale: String,
    pub db: Db,
    pub cache: Cache,
    pub jwt: Arc<Jwt>,
    pub crypto: Arc<Crypto>,
    pub queue: Arc<RabbitMQ>,
    pub token: Option<String>,
    pub client_ip: String,
    pub current_user: Option<User>,
}

impl juniper::Context for Context {}

impl Context {
    pub fn current_user(&self) -> Result<&User> {
        self.current_user
            .as_ref()
            .ok_or_else(|| Error::Http(StatusCode::FORBIDDEN, None))
    }

    pub fn administrator(&self) -> Result<&User> {
        self.can(&Role::Admin)
    }

    pub fn can(&self, role: &Role) -> Result<&User> {
        let db = self.db.deref();
        if let Some(ref it) = self.current_user {
            if PolicyDao::is(db, it.id, role) {
                return Ok(it);
            }
        }
        Err(Error::Http(StatusCode::FORBIDDEN, None))
    }
}
