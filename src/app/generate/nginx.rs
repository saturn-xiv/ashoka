use std::fs;
use std::io::prelude::*;
use std::os::unix::fs::OpenOptionsExt;

use askama::Template;

use super::super::super::errors::Result;

#[derive(Template)]
#[template(path = "nginx.conf", escape = "none")]
struct Config<'a> {
    domain: &'a str,
    port: u16,
}

pub fn run(domain: &str, port: u16) -> Result<()> {
    let tpl = Config {
        domain: domain,
        port,
    }
    .render()?;

    let file = format!("{}.conf", domain);
    info!("generate file {}", file);
    let mut fd = fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .mode(0o644)
        .open(file)?;
    fd.write_all(tpl.as_bytes())?;
    info!("please copy it into /etc/nginx/sites-enabled/ folder");
    Ok(())
}
