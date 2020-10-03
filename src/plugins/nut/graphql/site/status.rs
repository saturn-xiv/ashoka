use std::fmt;
use std::ops::{Deref, DerefMut};

use chrono::NaiveDateTime;
use diesel::{
    prelude::*,
    sql_query,
    sql_types::{BigInt, Text, Timestamp},
};
use juniper::GraphQLObject;
use redis::cmd;

use super::super::super::super::super::{
    errors::Result, graphql::context::Context, orm::Connection as DbConnection,
};

const MB: u64 = 1024 * 1024;

#[derive(GraphQLObject)]
pub struct Status {
    os: Os,
    redis: String,
    database: Vec<String>,
}

impl Status {
    pub fn new(ctx: &Context) -> Result<Self> {
        ctx.administrator()?;
        let db = ctx.db.deref();

        Ok(Status {
            os: Os::new()?,
            redis: {
                let mut db = ctx.cache.get()?;
                let db = db.deref_mut();
                cmd("info").query::<String>(db)?
            },
            database: Self::db(db)?,
        })
    }
}

#[derive(GraphQLObject)]
pub struct Os {
    pub uts: Uts,
    pub uptime: String,
    pub process_count: i32,
    pub load_average: LoadAverage,
    pub ram: Ram,
    pub swap: Swap,
    pub pid: i32,
}

#[derive(GraphQLObject)]
pub struct LoadAverage {
    pub l1: f64,
    pub l2: f64,
    pub l3: f64,
}
#[derive(GraphQLObject)]
pub struct Uts {
    pub machine: String,
    pub node_name: String,
    pub release: String,
    pub sys_name: String,
    pub version: String,
}
#[derive(GraphQLObject)]
pub struct Ram {
    pub unused: i32,
    pub total: i32,
}
#[derive(GraphQLObject)]
pub struct Swap {
    pub unused: i32,
    pub total: i32,
}

impl Os {
    pub fn new() -> Result<Self> {
        let un = nix::sys::utsname::uname();

        let uts = Uts {
            machine: un.machine().to_string(),
            node_name: un.nodename().to_string(),
            release: un.release().to_string(),
            sys_name: un.sysname().to_string(),
            version: un.version().to_string(),
        };

        let si = nix::sys::sysinfo::sysinfo()?;
        let uptime = format!("{:?}", si.uptime());
        let process_count = si.process_count() as i32;

        let (l1, l2, l3) = si.load_average();

        Ok(Os {
            pid: std::process::id() as i32,
            uts,
            uptime,
            process_count,
            load_average: LoadAverage { l1, l2, l3 },
            ram: Ram {
                unused: (si.ram_unused() / MB) as i32,
                total: (si.ram_total() / MB) as i32,
            },
            swap: Swap {
                unused: (si.swap_free() / MB) as i32,
                total: (si.swap_total() / MB) as i32,
            },
        })
    }
}

#[derive(QueryableByName)]
pub struct PgStatus {
    #[sql_type = "Text"]
    pub version: String,
    #[sql_type = "Timestamp"]
    pub timestamp: NaiveDateTime,
}
impl fmt::Display for PgStatus {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "version: {} timestamp: {}", self.version, self.timestamp)
    }
}
#[derive(QueryableByName)]
pub struct PgDatabase {
    #[sql_type = "Text"]
    pub name: String,
    #[sql_type = "BigInt"]
    pub size: i64,
}
impl fmt::Display for PgDatabase {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "name: {} size: {}MB", self.name, self.size)
    }
}
impl Status {
    fn db(db: &DbConnection) -> Result<Vec<String>> {
        let mut items = Vec::new();
        for it in sql_query(r###"SELECT version() as "version", now() as "timestamp""###)
            .load::<PgStatus>(db)?
        {
            items.push(it.to_string());
        }
        for it in sql_query(r###"SELECT pg_database.datname as "name", pg_database_size(pg_database.datname)/1024/1024 AS "size" FROM pg_database ORDER by "size" DESC"###).load::<PgDatabase>(db)?{
            items.push(it.to_string());
        }
        Ok(items)
    }
}
