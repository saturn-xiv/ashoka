#![allow(clippy::too_many_arguments)]

#[macro_use]
extern crate serde_derive;
#[macro_use]
extern crate log;
#[macro_use]
extern crate validator_derive;
#[macro_use]
extern crate diesel;
#[macro_use]
extern crate serde_json;
#[macro_use]
extern crate actix_web;
#[macro_use]
extern crate lazy_static;

extern crate actix_files;
extern crate amq_protocol_uri;
extern crate askama;
extern crate base64;
extern crate bytesize;
extern crate chrono;
extern crate chrono_tz;
extern crate clap;
extern crate csv;
extern crate elasticsearch;
extern crate encoding_rs;
extern crate eui48;
extern crate futures;
extern crate futures_executor;
extern crate futures_util;
extern crate git2;
extern crate hex;
extern crate humantime;
extern crate ini;
extern crate jsonwebtoken;
extern crate lettre;
extern crate lettre_email;
extern crate md5;
extern crate mime;
extern crate nix;
extern crate percent_encoding;
extern crate r2d2;
extern crate rand;
extern crate redis;
extern crate regex;
extern crate reqwest;
extern crate rusoto_core;
extern crate rusoto_credential;
extern crate rusoto_s3;
extern crate serde;
extern crate serde_xml_rs;
extern crate sodiumoxide;
extern crate ssh2;
extern crate tempfile;
extern crate toml;
extern crate url;
extern crate uuid;
extern crate validator;
extern crate xml;

#[macro_use]
pub mod macros;

pub mod app;
pub mod cache;
pub mod crypto;
pub mod dict;
pub mod env;
pub mod errors;
pub mod i18n;
pub mod image_magick;
pub mod jwt;
pub mod oauth;
pub mod orm;
pub mod parser;
pub mod plugins;
pub mod queue;
pub mod request;
pub mod settings;
pub mod storage;
pub mod theme;
pub mod third;
pub mod twilio;

pub const XML_HEADER: &str = r###"<?xml version="1.0" encoding="utf-8" ?>"###;
