use std::default::Default;
use std::fmt;
use std::ops::DerefMut;
use std::time::Duration;

use redis::cmd;
use serde::{de::DeserializeOwned, ser::Serialize};

use super::errors::Result;

pub trait Provider {
    fn get<K, V, F>(&self, key: &K, ttl: Duration, fun: F) -> Result<V>
    where
        F: FnOnce() -> Result<V>,
        K: Serialize,
        V: DeserializeOwned + Serialize;
    fn clear(&self) -> Result<()>;
}

type Connection = redis::Client;
pub type Pool = r2d2::Pool<Connection>;
pub type PooledConnection = r2d2::PooledConnection<Connection>;

#[derive(Serialize, Deserialize, Debug, Clone)]
#[serde(rename_all = "camelCase")]
pub struct Config {
    pub host: String,
    pub port: u16,
    pub db: u8,
}

impl Config {
    pub fn open(&self) -> Result<Pool> {
        let manager = Connection::open(self.to_string())?;
        let pool = r2d2::Pool::builder().build(manager)?;
        Ok(pool)
    }
}

impl Default for Config {
    fn default() -> Self {
        Self {
            host: "127.0.0.1".to_string(),
            port: 6379,
            db: 0,
        }
    }
}

impl fmt::Display for Config {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "redis://{}:{}/{}", self.host, self.port, self.db)
    }
}

impl Provider for Pool {
    #[cfg(debug_assertions)]
    fn get<K, V, F>(&self, _key: &K, _ttl: Duration, fun: F) -> Result<V>
    where
        F: FnOnce() -> Result<V>,
        K: Serialize,
        V: DeserializeOwned + Serialize,
    {
        let val = fun()?;
        Ok(val)
    }

    #[cfg(not(debug_assertions))]
    fn get<K, V, F>(&self, key: &K, ttl: Duration, fun: F) -> Result<V>
    where
        F: FnOnce() -> Result<V>,
        K: Serialize,
        V: DeserializeOwned + Serialize,
    {
        let mut db = self.get()?;
        let db = db.deref_mut();

        let key = format!("cache://{}", serde_json::to_string(key)?);
        if let Ok(buf) = cmd("get").arg(&key).query::<Vec<u8>>(db) {
            if let Ok(val) = serde_json::from_slice(buf.as_slice()) {
                return Ok(val);
            }
        }
        debug!("set {:?} {:?}", key, ttl);
        let val = fun()?;
        let _: String = cmd("set")
            .arg(&key)
            .arg(serde_json::to_vec(&val)?.as_slice())
            .arg("ex")
            .arg(ttl.as_secs())
            .query(db)?;
        Ok(val)
    }

    fn clear(&self) -> Result<()> {
        let mut db = self.get()?;
        let db = db.deref_mut();
        let rst = cmd("flushdb").query::<String>(db)?;
        info!("flushdb {}", rst);
        Ok(())
    }
}
