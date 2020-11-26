use std::fmt;
use std::result::Result as StdResult;

use actix_web::{dev::HttpResponseBuilder, error::ResponseError, http::StatusCode, HttpResponse};

pub type Result<T> = StdResult<T, Error>;

#[derive(Debug)]
pub enum Error {
    StdIo(std::io::Error),
    StdNetAddrParse(std::net::AddrParseError),
    StdNumParseInt(std::num::ParseIntError),
    StdStrUtf8(std::str::Utf8Error),
    StdStringFromUtf8(std::string::FromUtf8Error),

    Askama(askama::Error),
    ActixMultipart(actix_multipart::MultipartError),
    ActixWebBlockingSerdeJson(actix_web::error::BlockingError<serde_json::Error>),
    Base64Decode(base64::DecodeError),
    DieselMigrationsRun(diesel_migrations::RunMigrationsError),
    DieselResult(diesel::result::Error),
    IniParse(ini::ParseError),
    JsonWebToken(jsonwebtoken::errors::Error),
    Lapin(lapin::Error),
    LettreEmail(lettre_email::error::Error),
    LettreSmtp(lettre::smtp::error::Error),
    MimeFromStr(mime::FromStrError),
    Nix(nix::Error),
    HandlebarsRender(handlebars::RenderError),
    HandlebarsTemplate(handlebars::TemplateError),
    HandlebarsTemplateRender(handlebars::TemplateRenderError),
    R2d2(r2d2::Error),
    Redis(redis::RedisError),
    Reqwest(reqwest::Error),
    RusotoCoreRegionParse(rusoto_core::region::ParseRegionError),
    RusotoCoreRequestTls(rusoto_core::request::TlsError),
    RusotoCoreS3DeleteObject(rusoto_core::RusotoError<rusoto_s3::DeleteObjectError>),
    RusotoCoreS3GetBucketLocation(rusoto_core::RusotoError<rusoto_s3::GetBucketLocationError>),
    RusotoCoreS3PutObject(rusoto_core::RusotoError<rusoto_s3::PutObjectError>),
    RusotoCoreS3CreateBucket(rusoto_core::RusotoError<rusoto_s3::CreateBucketError>),
    SerdeJson(serde_json::Error),
    SerdeXml(serde_xml_rs::Error),
    TomlDe(toml::de::Error),
    TomlSer(toml::ser::Error),
    UrlParse(url::ParseError),
    Validator(validator::ValidationErrors),

    Http(StatusCode, Option<String>),
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::StdIo(v) => v.fmt(f),
            Self::StdNetAddrParse(v) => v.fmt(f),
            Self::StdNumParseInt(v) => v.fmt(f),
            Self::StdStrUtf8(v) => v.fmt(f),
            Self::StdStringFromUtf8(v) => v.fmt(f),

            Self::Askama(v) => v.fmt(f),
            Self::ActixMultipart(v) => v.fmt(f),
            Self::ActixWebBlockingSerdeJson(v) => v.fmt(f),
            Self::Base64Decode(v) => v.fmt(f),
            Self::DieselMigrationsRun(v) => v.fmt(f),
            Self::DieselResult(v) => v.fmt(f),
            Self::IniParse(v) => v.fmt(f),
            Self::JsonWebToken(v) => v.fmt(f),
            Self::Lapin(v) => v.fmt(f),
            Self::LettreEmail(v) => v.fmt(f),
            Self::LettreSmtp(v) => v.fmt(f),
            Self::MimeFromStr(v) => v.fmt(f),
            Self::Nix(v) => v.fmt(f),
            Self::HandlebarsRender(v) => v.fmt(f),
            Self::HandlebarsTemplate(v) => v.fmt(f),
            Self::HandlebarsTemplateRender(v) => v.fmt(f),
            Self::R2d2(v) => v.fmt(f),
            Self::Redis(v) => v.fmt(f),
            Self::Reqwest(v) => v.fmt(f),
            Self::RusotoCoreRegionParse(v) => v.fmt(f),
            Self::RusotoCoreRequestTls(v) => v.fmt(f),
            Self::RusotoCoreS3DeleteObject(v) => v.fmt(f),
            Self::RusotoCoreS3GetBucketLocation(v) => v.fmt(f),
            Self::RusotoCoreS3PutObject(v) => v.fmt(f),
            Self::RusotoCoreS3CreateBucket(v) => v.fmt(f),
            Self::SerdeJson(v) => v.fmt(f),
            Self::SerdeXml(v) => v.fmt(f),
            Self::TomlDe(v) => v.fmt(f),
            Self::TomlSer(v) => v.fmt(f),
            Self::UrlParse(v) => v.fmt(f),
            Self::Validator(v) => v.fmt(f),

            Self::Http(v, r) => match r {
                Some(r) => r.fmt(f),
                None => v.fmt(f),
            },
        }
    }
}

impl From<std::string::FromUtf8Error> for Error {
    fn from(err: std::string::FromUtf8Error) -> Self {
        Self::StdStringFromUtf8(err)
    }
}

impl From<std::io::Error> for Error {
    fn from(err: std::io::Error) -> Self {
        Self::StdIo(err)
    }
}

impl From<std::num::ParseIntError> for Error {
    fn from(err: std::num::ParseIntError) -> Self {
        Self::StdNumParseInt(err)
    }
}

impl From<std::str::Utf8Error> for Error {
    fn from(err: std::str::Utf8Error) -> Self {
        Self::StdStrUtf8(err)
    }
}

impl From<std::net::AddrParseError> for Error {
    fn from(err: std::net::AddrParseError) -> Self {
        Self::StdNetAddrParse(err)
    }
}

impl From<r2d2::Error> for Error {
    fn from(err: r2d2::Error) -> Self {
        Self::R2d2(err)
    }
}

impl From<lapin::Error> for Error {
    fn from(err: lapin::Error) -> Self {
        Self::Lapin(err)
    }
}

impl From<rusoto_core::region::ParseRegionError> for Error {
    fn from(err: rusoto_core::region::ParseRegionError) -> Self {
        Self::RusotoCoreRegionParse(err)
    }
}

impl From<rusoto_core::request::TlsError> for Error {
    fn from(err: rusoto_core::request::TlsError) -> Self {
        Self::RusotoCoreRequestTls(err)
    }
}

impl From<rusoto_core::RusotoError<rusoto_s3::DeleteObjectError>> for Error {
    fn from(err: rusoto_core::RusotoError<rusoto_s3::DeleteObjectError>) -> Self {
        Self::RusotoCoreS3DeleteObject(err)
    }
}

impl From<rusoto_core::RusotoError<rusoto_s3::GetBucketLocationError>> for Error {
    fn from(err: rusoto_core::RusotoError<rusoto_s3::GetBucketLocationError>) -> Self {
        Self::RusotoCoreS3GetBucketLocation(err)
    }
}

impl From<rusoto_core::RusotoError<rusoto_s3::PutObjectError>> for Error {
    fn from(err: rusoto_core::RusotoError<rusoto_s3::PutObjectError>) -> Self {
        Self::RusotoCoreS3PutObject(err)
    }
}

impl From<rusoto_core::RusotoError<rusoto_s3::CreateBucketError>> for Error {
    fn from(err: rusoto_core::RusotoError<rusoto_s3::CreateBucketError>) -> Self {
        Self::RusotoCoreS3CreateBucket(err)
    }
}

impl From<diesel::result::Error> for Error {
    fn from(err: diesel::result::Error) -> Self {
        Self::DieselResult(err)
    }
}

impl From<diesel_migrations::RunMigrationsError> for Error {
    fn from(err: diesel_migrations::RunMigrationsError) -> Self {
        Self::DieselMigrationsRun(err)
    }
}

impl From<serde_json::Error> for Error {
    fn from(err: serde_json::Error) -> Self {
        Self::SerdeJson(err)
    }
}

impl From<askama::Error> for Error {
    fn from(err: askama::Error) -> Self {
        Self::Askama(err)
    }
}

impl From<ini::ParseError> for Error {
    fn from(err: ini::ParseError) -> Self {
        Self::IniParse(err)
    }
}

impl From<redis::RedisError> for Error {
    fn from(err: redis::RedisError) -> Self {
        Self::Redis(err)
    }
}

impl From<base64::DecodeError> for Error {
    fn from(err: base64::DecodeError) -> Self {
        Self::Base64Decode(err)
    }
}

impl From<jsonwebtoken::errors::Error> for Error {
    fn from(err: jsonwebtoken::errors::Error) -> Self {
        Self::JsonWebToken(err)
    }
}

impl From<url::ParseError> for Error {
    fn from(err: url::ParseError) -> Self {
        Self::UrlParse(err)
    }
}

impl From<lettre::smtp::error::Error> for Error {
    fn from(err: lettre::smtp::error::Error) -> Self {
        Self::LettreSmtp(err)
    }
}
impl From<lettre_email::error::Error> for Error {
    fn from(err: lettre_email::error::Error) -> Self {
        Self::LettreEmail(err)
    }
}

impl From<toml::de::Error> for Error {
    fn from(err: toml::de::Error) -> Self {
        Self::TomlDe(err)
    }
}

impl From<toml::ser::Error> for Error {
    fn from(err: toml::ser::Error) -> Self {
        Self::TomlSer(err)
    }
}

impl From<reqwest::Error> for Error {
    fn from(err: reqwest::Error) -> Self {
        Self::Reqwest(err)
    }
}

impl From<actix_web::error::BlockingError<serde_json::Error>> for Error {
    fn from(err: actix_web::error::BlockingError<serde_json::Error>) -> Self {
        Self::ActixWebBlockingSerdeJson(err)
    }
}

impl From<serde_xml_rs::Error> for Error {
    fn from(err: serde_xml_rs::Error) -> Self {
        Self::SerdeXml(err)
    }
}

impl From<nix::Error> for Error {
    fn from(err: nix::Error) -> Self {
        Self::Nix(err)
    }
}

impl From<validator::ValidationErrors> for Error {
    fn from(err: validator::ValidationErrors) -> Self {
        Self::Validator(err)
    }
}

impl From<mime::FromStrError> for Error {
    fn from(err: mime::FromStrError) -> Self {
        Self::MimeFromStr(err)
    }
}

impl From<actix_multipart::MultipartError> for Error {
    fn from(err: actix_multipart::MultipartError) -> Self {
        Self::ActixMultipart(err)
    }
}

impl From<handlebars::RenderError> for Error {
    fn from(err: handlebars::RenderError) -> Self {
        Self::HandlebarsRender(err)
    }
}

impl From<handlebars::TemplateError> for Error {
    fn from(err: handlebars::TemplateError) -> Self {
        Self::HandlebarsTemplate(err)
    }
}

impl From<handlebars::TemplateRenderError> for Error {
    fn from(err: handlebars::TemplateRenderError) -> Self {
        Self::HandlebarsTemplateRender(err)
    }
}
impl ResponseError for Error {
    fn error_response(&self) -> HttpResponse {
        HttpResponseBuilder::new(self.status_code())
            .content_type(mime::TEXT_PLAIN_UTF_8.to_string())
            .body(self.to_string())
    }
    fn status_code(&self) -> StatusCode {
        match *self {
            Self::Http(it, _) => it,
            _ => StatusCode::INTERNAL_SERVER_ERROR,
        }
    }
}
