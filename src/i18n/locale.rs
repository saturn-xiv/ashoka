use chrono::{NaiveDateTime, Utc};
use diesel::{delete, insert_into, prelude::*, update};

use super::super::{
    errors::Result,
    orm::{schema::locales, Connection},
};

#[derive(Queryable, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct Item {
    pub id: i64,
    pub lang: String,
    pub code: String,
    pub message: String,
    pub version: i64,
    pub created_at: NaiveDateTime,
    pub updated_at: NaiveDateTime,
}

#[derive(Insertable)]
#[table_name = "locales"]
pub struct New<'a> {
    pub lang: &'a str,
    pub code: &'a str,
    pub message: &'a str,
    pub updated_at: &'a NaiveDateTime,
}

pub trait Dao {
    fn languages(&self) -> Result<Vec<String>>;
    fn count(&self, lang: &str) -> Result<i64>;
    fn all(&self) -> Result<Vec<Item>>;
    fn by_lang(&self, lang: &str) -> Result<Vec<Item>>;
    fn by_id(&self, id: i64) -> Result<Item>;
    fn by_lang_and_code(&self, lang: &str, code: &str) -> Result<Item>;
    fn delete(&self, id: i64) -> Result<()>;
    fn create(&self, lang: &str, code: &str, message: &str) -> Result<()>;
    fn update(&self, id: i64, code: &str, message: &str) -> Result<()>;
}

impl Dao for Connection {
    fn languages(&self) -> Result<Vec<String>> {
        Ok(locales::dsl::locales
            .select(locales::dsl::lang)
            .distinct()
            .load::<String>(self)?)
    }

    fn count(&self, lang: &str) -> Result<i64> {
        let cnt: i64 = locales::dsl::locales
            .count()
            .filter(locales::dsl::lang.eq(lang))
            .get_result(self)?;
        Ok(cnt)
    }
    fn by_lang(&self, lang: &str) -> Result<Vec<Item>> {
        let items = locales::dsl::locales
            .filter(locales::dsl::lang.eq(lang))
            .order(locales::dsl::code.asc())
            .load::<Item>(self)?;
        Ok(items)
    }
    fn all(&self) -> Result<Vec<Item>> {
        let items = locales::dsl::locales
            .order(locales::dsl::updated_at.desc())
            .load::<Item>(self)?;
        Ok(items)
    }
    fn by_id(&self, id: i64) -> Result<Item> {
        let it = locales::dsl::locales
            .filter(locales::dsl::id.eq(id))
            .first::<Item>(self)?;
        Ok(it)
    }
    fn by_lang_and_code(&self, lang: &str, code: &str) -> Result<Item> {
        let it = locales::dsl::locales
            .filter(locales::dsl::lang.eq(lang))
            .filter(locales::dsl::code.eq(code))
            .first::<Item>(self)?;
        Ok(it)
    }
    fn update(&self, id: i64, code: &str, message: &str) -> Result<()> {
        let now = Utc::now().naive_utc();
        let it = locales::dsl::locales.filter(locales::dsl::id.eq(id));
        update(it)
            .set((
                locales::dsl::code.eq(code),
                locales::dsl::message.eq(message),
                locales::dsl::updated_at.eq(&now),
            ))
            .execute(self)?;
        Ok(())
    }
    fn create(&self, lang: &str, code: &str, message: &str) -> Result<()> {
        let now = Utc::now().naive_utc();
        insert_into(locales::dsl::locales)
            .values(&New {
                lang,
                code,
                message,
                updated_at: &now,
            })
            .execute(self)?;
        Ok(())
    }
    fn delete(&self, id: i64) -> Result<()> {
        delete(locales::dsl::locales.filter(locales::dsl::id.eq(id))).execute(self)?;
        Ok(())
    }
}
