use std::fmt;
use std::fs::{create_dir_all, read_dir, read_to_string, OpenOptions};
use std::os::unix::fs::OpenOptionsExt;
use std::path::{Path, PathBuf};

use chrono::{NaiveDateTime, Utc};
use diesel::{
    connection::{Connection as DieselConnection, SimpleConnection},
    delete, insert_into,
    prelude::*,
    update,
};

use super::super::errors::{Error, Result};
use super::{schema::schema_migrations, Connection};

#[derive(Queryable, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct Item {
    pub id: i64,
    pub name: String,
    pub version: String,
    pub up: String,
    pub down: String,
    pub run_at: Option<NaiveDateTime>,
    pub created_at: NaiveDateTime,
}

impl fmt::Display for Item {
    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {
        write!(fmt, "{}-{}", self.version, self.name)
    }
}
pub struct Schema<'a> {
    connection: &'a Connection,
    root: PathBuf,
}

impl<'a> Schema<'a> {
    const MIGRATIONS: &'static str = "migrations";
    pub fn new<P: AsRef<Path>>(root: P, connection: &'a Connection) -> Result<Self> {
        let root = root.as_ref().to_path_buf();
        connection.transaction::<_, Error, _>(|| {
            let sql = read_to_string(root.join("schema").join("up.sql"))?;
            connection.batch_execute(&sql)?;
            Ok(())
        })?;
        Ok(Self { root, connection })
    }
    pub fn migrate(&self) -> Result<()> {
        self.load()?;
        let items = Dao::all(self.connection)?;
        for it in items {
            if it.run_at.is_none() {
                info!("migrate {}", it);
                self.connection.transaction::<_, Error, _>(|| {
                    self.connection.batch_execute(&it.up)?;
                    Dao::set_run_at(self.connection, it.id)?;
                    Ok(())
                })?;
            }
        }
        Ok(())
    }
    pub fn rollback(&self) -> Result<()> {
        self.load()?;
        if let Ok(it) = Dao::latest(self.connection) {
            info!("rollback {}", it);
            self.connection.transaction::<_, Error, _>(|| {
                self.connection.batch_execute(&it.down)?;
                Dao::delete(self.connection, it.id)?;
                Ok(())
            })?;
        }
        Ok(())
    }
    pub fn status(&self) -> Result<()> {
        self.load()?;
        Ok(())
    }
    pub fn generate(&self, name: &str) -> Result<()> {
        let version = Utc::now().format("%Y%m%d%H%M%S%3f").to_string();
        let root = self
            .root
            .join(Self::MIGRATIONS)
            .join(format!("{}-{}", version, name))
            .to_path_buf();
        if !root.exists() {
            create_dir_all(&root)?;
        }

        info!("generate up.sql & down.sql under {}", root.display());
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

    fn load(&self) -> Result<()> {
        let root = self.root.join(Self::MIGRATIONS);
        if !root.exists() {
            create_dir_all(&root)?;
        }
        for it in read_dir(root)? {
            let it = it?;
            let it = it.path();
            if it.is_dir() {
                if let Some(name) = it.file_name() {
                    if let Some(name) = name.to_str() {
                        const AT: usize = 17;
                        if name.len() > (AT + 1) {
                            let up = read_to_string(it.join("up.sql"))?;
                            let down = read_to_string(it.join("down.sql"))?;
                            debug!("find migration {}", name);
                            let version = &name[..AT];
                            let name = &name[(AT + 1)..];
                            if Dao::exists(self.connection, version, name)? {
                                continue;
                            }
                            Dao::create(self.connection, name, version, &up, &down)?;
                        }
                    }
                }
            }
        }
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
    fn exists(&self, version: &str, name: &str) -> Result<bool>;
    fn delete(&self, id: i64) -> Result<()>;
    fn set_run_at(&self, id: i64) -> Result<()>;
}

impl Dao for Connection {
    fn all(&self) -> Result<Vec<Item>> {
        let items = schema_migrations::dsl::schema_migrations
            .order(schema_migrations::dsl::created_at.asc())
            .load::<Item>(self)?;
        Ok(items)
    }
    fn latest(&self) -> Result<Item> {
        let it = schema_migrations::dsl::schema_migrations
            .filter(schema_migrations::dsl::run_at.is_not_null())
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
    fn exists(&self, version: &str, name: &str) -> Result<bool> {
        let cnt: i64 = schema_migrations::dsl::schema_migrations
            .count()
            .filter(schema_migrations::dsl::version.eq(version))
            .filter(schema_migrations::dsl::name.eq(name))
            .get_result(self)?;
        Ok(cnt > 0)
    }
    fn set_run_at(&self, id: i64) -> Result<()> {
        let now = Utc::now().naive_utc();
        let it =
            schema_migrations::dsl::schema_migrations.filter(schema_migrations::dsl::id.eq(id));
        update(it)
            .set(schema_migrations::dsl::run_at.eq(&now))
            .execute(self)?;
        Ok(())
    }
    fn delete(&self, id: i64) -> Result<()> {
        delete(schema_migrations::dsl::schema_migrations.filter(schema_migrations::dsl::id.eq(id)))
            .execute(self)?;
        Ok(())
    }
}
