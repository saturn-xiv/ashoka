use std::net::SocketAddr;

use actix_cors::Cors;
use actix_session::CookieSession;
use actix_web::{
    http::{header, Method},
    middleware::Logger,
    web, App, HttpServer,
};
use chrono::Duration as ChronoDuration;

use super::super::super::{
    crypto::Crypto,
    env::{Config, Environment, NAME},
    errors::Result,
    graphql,
    jwt::Jwt,
    plugins::{forum, nut},
};

#[actix_rt::main]
pub async fn launch(cfg: Config) -> Result<()> {
    let db = cfg.database.open()?;

    info!("start send email thread");
    // {
    //     let db = db.clone();
    //     let enc = enc.clone();
    //     let db = db.clone();
    //     let qu = qu.clone();
    //     thread::spawn(move || loop {
    //         if let Ok(e) = qu.consume(
    //             format!(
    //                 "{}-{}-{}",
    //                 env::NAME,
    //                 env::VERSION,
    //                 nut::tasks::send_email::NAME
    //             ),
    //             nut::tasks::send_email::NAME.to_string(),
    //             Box::new(nut::tasks::send_email::Consumer {
    //                 db: db.clone(),
    //                 encryptor: enc.clone(),
    //             }),
    //         ) {
    //             error!("send email thread failed {:?}", e);
    //         }
    //         thread::sleep(Duration::from_secs(30));
    //     });
    // }

    let addr = SocketAddr::from(([127, 0, 0, 1], cfg.http.port));
    let cookie = {
        let key: Result<Vec<u8>> = cfg.secrets.clone().into();
        key?
    };
    let origin = cfg.http.origin.clone();
    let jwt = Jwt::new(cfg.secrets.0.clone());
    let enc = Crypto::new(cfg.secrets.clone())?;
    let mq = cfg.rabbitmq.open();
    let che = cfg.cache.open()?;
    let schema = web::Data::new(graphql::Schema::new(
        graphql::query::Query {},
        graphql::mutation::Mutation {},
    ));
    let env = cfg.env.clone();

    HttpServer::new(move || {
        App::new()
            .data(db.clone())
            .data(che.clone())
            .data(enc.clone())
            .data(jwt.clone())
            .data(mq.clone())
            .app_data(schema.clone())
            .wrap(Logger::default())
            .data(web::JsonConfig::default().limit(1 << 16))
            .wrap(match env {
                Environment::Production => Cors::new()
                    .allowed_origin(&origin)
                    .allowed_methods(vec![
                        Method::GET,
                        Method::POST,
                        Method::PUT,
                        Method::PATCH,
                        Method::DELETE,
                    ])
                    .allowed_headers(vec![
                        header::AUTHORIZATION,
                        header::CONTENT_TYPE,
                        header::ACCEPT,
                    ])
                    .supports_credentials()
                    .max_age(60 * 60)
                    .send_wildcard()
                    .finish(),
                _ => Cors::default(),
            })
            .wrap(
                CookieSession::signed(&cookie)
                    .name(NAME)
                    .http_only(true)
                    .max_age(ChronoDuration::hours(1).num_seconds())
                    .path("/")
                    .secure(cfg!(not(debug_assertions))),
            )
            .configure(nut::html::mount)
            .configure(forum::html::mount)
            .service(web::resource(graphql::SOURCE).route(web::post().to(graphql::post)))
            .service(web::resource("/graphiql").route(web::get().to(graphql::get)))
            .service(actix_files::Files::new("/3rd", "node_modules").use_last_modified(true))
    })
    .bind(addr)?
    .run()
    .await?;

    Ok(())
}
