use std::collections::BTreeMap;
use std::fmt;
use std::fs::{create_dir_all, File, OpenOptions};
use std::io::{prelude::*, BufReader, BufWriter};
use std::path::{Path, PathBuf};
use std::process::{Command as ShellCommand, Stdio};

use actix_web::http::StatusCode;
use chrono::Utc;
use handlebars::Handlebars;
use rand::{distributions::Alphanumeric, thread_rng, Rng};
use serde::de::DeserializeOwned;
use toml::Value;
use uuid::Uuid;

use super::super::super::{
    env::VERSION,
    errors::{Error, Result},
};

pub const CONFIG_EXT: &str = "toml";
pub const TEMPLATE_EXT: &str = "hbs";
pub const JOBS: &str = "jobs";

pub type Vars = BTreeMap<String, Value>;
pub type Host = (String, Vars);
pub type Excutor = (Vec<Host>, Vec<String>);

pub struct Cluster {}

pub struct Recipe {}
