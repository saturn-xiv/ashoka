use clap::{App, SubCommand};

use super::super::super::{errors::Result, orm::Connection};

pub const COMMAND_NAME: &str = "database:rollback";

pub fn command<'a, 'b>() -> App<'a, 'b> {
    SubCommand::with_name(COMMAND_NAME).about("Rollback database the last migration")
}

pub fn run(_db: &Connection) -> Result<()> {
    // embedded_migrations::revert_latest_migration(db)?;
    warn!("TODO");
    Ok(())
}
