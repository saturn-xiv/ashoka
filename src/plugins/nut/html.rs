use std::path::Path;

use actix_files::NamedFile;
use actix_web::{web, HttpResponse, Responder, Result as WebResult};
use askama::Template;

use super::super::super::errors::Result;

pub fn mount(cfg: &mut web::ServiceConfig) {
    cfg.service(index)
        .service(robots_txt)
        .service(rss)
        .service(sitemap)
        .service(assets);
}

#[get("/")]
async fn index() -> impl Responder {
    HttpResponse::Ok().body("home")
}

#[derive(Template)]
#[template(path = "robots.txt", escape = "none")]
struct RobotsTxt;

// http://www.robotstxt.org/
#[get("/robots.txt")]
async fn robots_txt() -> Result<impl Responder> {
    Ok(HttpResponse::Ok()
        .content_type(mime::TEXT_PLAIN_UTF_8.to_string())
        .body(RobotsTxt.render()?))
}

#[get("/rss/{lang}.xml")]
async fn rss(path: web::Path<(String,)>) -> impl Responder {
    let path = path.into_inner();
    HttpResponse::Ok().body(format!("rss {}", path.0))
}

#[get("/sitemap.xml.gz")]
async fn sitemap() -> impl Responder {
    HttpResponse::Ok().body("sitemap")
}

#[get("/assets/{theme}/{file:.*}")]
async fn assets(path: web::Path<(String, String)>) -> WebResult<impl Responder> {
    // TODO detect theme from config file
    let path = path.into_inner();
    let file = Path::new("themes").join(path.0).join("assets").join(path.1);
    let file = NamedFile::open(&file)?;
    Ok(file.use_last_modified(true))
}
