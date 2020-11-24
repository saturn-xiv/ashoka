pub mod locale;

use actix_web::http::StatusCode;
use handlebars::Handlebars;
use serde::ser::Serialize;

use super::{errors::Error, orm::Connection};

use self::locale::Dao;

pub trait I18n {
    fn exist(&self, lang: &str) -> bool;
    fn tr<S: Serialize>(&self, lang: &str, code: &str, args: &Option<S>) -> Option<String>;
    fn t<C: Into<String>, S: Serialize>(&self, lang: &str, code: C, args: &Option<S>) -> String;
    fn e<C: Into<String>, S: Serialize>(&self, lang: &str, code: C, args: &Option<S>) -> Error;
}

impl I18n for Connection {
    fn exist(&self, lang: &str) -> bool {
        if let Ok(items) = Dao::languages(self) {
            return items.contains(&lang.to_string());
        }
        false
    }

    fn tr<S: Serialize>(&self, lang: &str, code: &str, args: &Option<S>) -> Option<String> {
        if let Ok(it) = Dao::by_lang_and_code(self, lang, code) {
            let reg = Handlebars::new();
            match reg.render_template(&it.message, args) {
                Ok(msg) => {
                    return Some(msg);
                }
                Err(e) => {
                    error!("{:?}", e);
                }
            };
        }
        None
    }

    fn t<C: Into<String>, S: Serialize>(&self, lang: &str, code: C, args: &Option<S>) -> String {
        let code = code.into();
        match self.tr(lang, &code, args) {
            Some(msg) => msg,
            None => format!("{}.{}", lang, code),
        }
    }

    fn e<C: Into<String>, S: Serialize>(&self, lang: &str, code: C, args: &Option<S>) -> Error {
        Error::Http(
            StatusCode::INTERNAL_SERVER_ERROR,
            Some(self.t(lang, code, args)),
        )
    }
}
