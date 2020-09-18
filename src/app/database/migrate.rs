use std::io::stdout;

use clap::{App, SubCommand};

use super::super::super::{errors::Result, orm::Connection};
use super::embedded_migrations;

pub const COMMAND_NAME: &str = "database:migrate";

pub fn command<'a, 'b>() -> App<'a, 'b> {
    SubCommand::with_name(COMMAND_NAME).about("Migrate database to latest migration")
}

pub fn run(db: &Connection) -> Result<()> {
    embedded_migrations::run_with_output(db, &mut stdout())?;
    Ok(())
}
