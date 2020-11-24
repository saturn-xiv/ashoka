use std::fs::{create_dir_all, OpenOptions};
use std::io::prelude::*;
use std::os::unix::fs::OpenOptionsExt;
use std::path::{Path, PathBuf};

use chrono::{NaiveDateTime, Utc};
use diesel::{delete, insert_into, prelude::*};

use super::super::errors::Result;
use super::{schema::schema_migrations, Connection};

#[derive(Queryable, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct Item {
    pub id: i64,
    pub name: String,
    pub version: String,
    pub up: String,
    pub down: String,
    pub created_at: NaiveDateTime,
}

pub struct Schema {
    connection: Connection,
    root: PathBuf,
    migrations: Vec<Item>,
}

impl Schema {
    pub fn new<P: AsRef<Path>>(root: P, connection: Connection) -> Self {
        let migrations = Vec::new();
        Self {
            root: root.as_ref().to_path_buf(),
            connection,
            migrations,
        }
    }
    pub fn create(&self, name: &str) -> Result<()> {
        let version = Utc::now().format("%Y%m%d%H%M%S%3f").to_string();
        let root = self
            .root
            .join(format!("{}-{}", version, name))
            .to_path_buf();
        if !root.exists() {
            create_dir_all(&root)?;
        }

        OpenOptions::new()
            .write(true)
            .create_new(true)
            .mode(0o644)
            .open(root.join("up.sql"))?;

        OpenOptions::new()
            .write(true)
            .create_new(true)
            .mode(0o644)
            .open(root.join("down.sql"))?;
        Ok(())
    }
}

#[derive(Insertable)]
#[table_name = "schema_migrations"]
pub struct New<'a> {
    pub name: &'a str,
    pub version: &'a str,
    pub up: &'a str,
    pub down: &'a str,
}

pub trait Dao {
    fn all(&self) -> Result<Vec<Item>>;
    fn latest(&self) -> Result<Item>;
    fn create(&self, name: &str, version: &str, up: &str, down: &str) -> Result<()>;
    fn exists(&self, version: &str) -> Result<bool>;
    fn delete(&self, id: i64) -> Result<()>;
}

impl Dao for Connection {
    fn all(&self) -> Result<Vec<Item>> {
        let items = schema_migrations::dsl::schema_migrations
            .order(schema_migrations::dsl::created_at.desc())
            .load::<Item>(self)?;
        Ok(items)
    }
    fn latest(&self) -> Result<Item> {
        let it = schema_migrations::dsl::schema_migrations
            .order(schema_migrations::dsl::created_at.desc())
            .first::<Item>(self)?;
        Ok(it)
    }
    fn create(&self, name: &str, version: &str, up: &str, down: &str) -> Result<()> {
        insert_into(schema_migrations::dsl::schema_migrations)
            .values(&New {
                name,
                version,
                up,
                down,
            })
            .execute(self)?;
        Ok(())
    }
    fn exists(&self, version: &str) -> Result<bool> {
        let cnt: i64 = schema_migrations::dsl::schema_migrations
            .count()
            .filter(schema_migrations::dsl::version.eq(version))
            .get_result(self)?;
        Ok(cnt > 0)
    }
    fn delete(&self, id: i64) -> Result<()> {
        delete(schema_migrations::dsl::schema_migrations.filter(schema_migrations::dsl::id.eq(id)))
            .execute(self)?;
        Ok(())
    }
}
