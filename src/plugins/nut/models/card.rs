use chrono::{NaiveDateTime, Utc};
use diesel::{delete, insert_into, prelude::*, update};
use mime::Mime;

use super::super::super::super::{
    errors::Result,
    orm::{schema::cards, Connection},
};

#[derive(Queryable, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct Item {
    pub id: i64,
    pub title: String,
    pub body: String,
    pub media_type: String,
    pub action: String,
    pub href: String,
    pub logo: String,
    pub loc: String,
    pub lang: String,
    pub position: i16,
    pub version: i64,
    pub created_at: NaiveDateTime,
    pub updated_at: NaiveDateTime,
}

pub trait Dao {
    fn by_id(&self, id: i64) -> Result<Item>;
    fn create(
        &self,
        lang: &str,
        title: &str,
        logo: &str,
        body: &str,
        media_type: &Mime,
        href: &str,
        action: &str,
        loc: &str,
        position: i16,
    ) -> Result<()>;
    fn update(
        &self,
        id: i64,
        title: &str,
        logo: &str,
        body: &str,
        media_type: &Mime,
        href: &str,
        action: &str,
        loc: &str,
        position: i16,
    ) -> Result<()>;
    fn all(&self) -> Result<Vec<Item>>;
    fn by_lang(&self, lang: &str) -> Result<Vec<Item>>;
    fn delete(&self, id: i64) -> Result<()>;
}

impl Dao for Connection {
    fn by_id(&self, id: i64) -> Result<Item> {
        let it = cards::dsl::cards
            .filter(cards::dsl::id.eq(id))
            .first::<Item>(self)?;
        Ok(it)
    }
    fn create(
        &self,
        lang: &str,
        title: &str,
        logo: &str,
        body: &str,
        media_type: &Mime,
        href: &str,
        action: &str,
        loc: &str,
        position: i16,
    ) -> Result<()> {
        let now = Utc::now().naive_utc();
        insert_into(cards::dsl::cards)
            .values((
                cards::dsl::lang.eq(lang),
                cards::dsl::title.eq(title),
                cards::dsl::body.eq(body),
                cards::dsl::media_type.eq(&media_type.to_string()),
                cards::dsl::action.eq(action),
                cards::dsl::href.eq(href),
                cards::dsl::logo.eq(logo),
                cards::dsl::loc.eq(loc),
                cards::dsl::position.eq(position),
                cards::dsl::updated_at.eq(&now),
            ))
            .execute(self)?;
        Ok(())
    }

    fn update(
        &self,
        id: i64,
        title: &str,
        logo: &str,
        body: &str,
        media_type: &Mime,
        href: &str,
        action: &str,
        loc: &str,
        position: i16,
    ) -> Result<()> {
        let now = Utc::now().naive_utc();
        update(cards::dsl::cards.filter(cards::dsl::id.eq(id)))
            .set((
                cards::dsl::title.eq(title),
                cards::dsl::body.eq(body),
                cards::dsl::media_type.eq(&media_type.to_string()),
                cards::dsl::action.eq(action),
                cards::dsl::href.eq(href),
                cards::dsl::logo.eq(logo),
                cards::dsl::loc.eq(loc),
                cards::dsl::position.eq(position),
                cards::dsl::updated_at.eq(&now),
            ))
            .execute(self)?;
        Ok(())
    }

    fn all(&self) -> Result<Vec<Item>> {
        let items = cards::dsl::cards
            .order(cards::dsl::updated_at.desc())
            .load::<Item>(self)?;
        Ok(items)
    }

    fn by_lang(&self, lang: &str) -> Result<Vec<Item>> {
        let items = cards::dsl::cards
            .filter(cards::dsl::lang.eq(lang))
            .order(cards::dsl::updated_at.desc())
            .load::<Item>(self)?;
        Ok(items)
    }

    fn delete(&self, id: i64) -> Result<()> {
        delete(cards::dsl::cards.filter(cards::dsl::id.eq(id))).execute(self)?;
        Ok(())
    }
}
