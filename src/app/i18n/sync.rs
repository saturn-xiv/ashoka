use std::collections::BTreeMap;
use std::ops::Deref;

use clap::{App, SubCommand};
use diesel::Connection as DieselConnection;
use ini::Ini;

use super::super::super::{
    env::Config,
    errors::{Error, Result},
    i18n::locale::Dao as LocaleDao,
    orm::Connection,
};

pub const COMMAND_NAME: &str = "i18n:sync";
pub const COMMAND_ABOUT: &str = "Sync locale records to database";

pub fn command<'a, 'b>() -> App<'a, 'b> {
    SubCommand::with_name(COMMAND_NAME).about(COMMAND_ABOUT)
}

pub fn run(cfg: Config) -> Result<()> {
    let db = cfg.database.open()?;
    let db = db.get()?;
    let db = db.deref();

    let (inserted, find) = db.transaction::<_, Error, _>(|| sync(db))?;
    info!("find {} records, insert {}", find, inserted);
    Ok(())
}

fn sync(db: &Connection) -> Result<(usize, usize)> {
    let mut find = 0;
    let mut inserted = 0;

    let locales = {
        let mut items = BTreeMap::new();
        items.insert("en-US", include_str!("en-US.ini"));
        items.insert("zh-Hans", include_str!("zh-Hans.ini"));
        items.insert("zh-Hant", include_str!("zh-Hant.ini"));
        items
    };

    for (lang, file) in locales {
        let item = Ini::load_from_str(file)?;
        for sec in item.sections() {
            for it in item.section_all(sec) {
                for (k, v) in it.iter() {
                    find += 1;
                    let code = format!(
                        "{}{}",
                        match sec {
                            Some(it) => format!("{}.", it),
                            None => "".to_string(),
                        },
                        k,
                    );
                    debug!("find {} = {}", k, v);
                    if let Err(_) = LocaleDao::by_lang_and_code(db, lang, &code) {
                        LocaleDao::create(db, lang, &code, v)?;
                        inserted += 1;
                    }
                }
            }
        }
    }

    Ok((inserted, find))
}
