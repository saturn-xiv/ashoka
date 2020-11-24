use std::io::stdout;

use clap::{App, SubCommand};

use super::super::super::{errors::Result, orm::Connection};

pub const COMMAND_NAME: &str = "db:generate";

pub fn command<'a, 'b>() -> App<'a, 'b> {
    SubCommand::with_name(COMMAND_NAME).about("Generate a new migration")
}

pub fn run(db: &Connection) -> Result<()> {
    Ok(())
}
