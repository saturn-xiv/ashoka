use std::env::current_dir;
use std::fs;
use std::io::prelude::*;
use std::os::unix::fs::OpenOptionsExt;

use actix_web::http::StatusCode;
use askama::Template;
use nix::unistd::{Gid, Group, Uid, User};

use super::super::super::{
    env::DESCRIPTION,
    errors::{Error, Result},
};

#[derive(Template)]
#[template(path = "systemd/service.conf", escape = "none")]
struct Config<'a> {
    user: &'a str,
    group: &'a str,
    root: &'a str,
    description: &'a str,
}

fn current() -> Result<(String, String, String)> {
    let user = User::from_uid(Uid::current())?;
    let group = Group::from_gid(Gid::current())?;
    if let Some(user) = user {
        if let Some(group) = group {
            return Ok((user.name, group.name, current_dir()?.display().to_string()));
        }
    }
    Err(Error::Http(
        StatusCode::INTERNAL_SERVER_ERROR,
        Some("can't get user&group name".to_string()),
    ))
}
pub fn run(name: &str) -> Result<()> {
    let (user, group, root) = current()?;
    let tpl = Config {
        user: &user,
        group: &group,
        description: DESCRIPTION,
        root: &root,
    }
    .render()?;

    let file = format!("{}.service", name);
    info!("generate file {}", file);
    let mut fd = fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .mode(0o644)
        .open(file)?;
    fd.write_all(tpl.as_bytes())?;
    info!("please copy it into /lib/systemd/system/ folder");
    Ok(())
}
