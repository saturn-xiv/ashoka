// pub mod database;
// pub mod generate;
// pub mod http;
// pub mod i18n;

use actix_web::http::StatusCode;
use clap::{App, Arg};

use super::{
    env,
    errors::{Error, Result},
    parser,
};

pub fn launch() -> Result<()> {
    let matches = App::new(env::NAME)
        .version(env::VERSION)
        .author(env::AUTHORS)
        .about(env::DESCRIPTION)
        .before_help(env::BANNER)
        .after_help(env::HOMEPAGE)
        .arg(
            Arg::new("config")
                .short('c')
                .long("config")
                .about("Config file(.toml)")
                .default_value("config")
                .takes_value(true),
        )
        .arg(
            Arg::new("debug")
                .short('d')
                .long("debug")
                .about("Run on debug mode"),
        )
        .subcommand(
            App::new("db")
                .about("Database operations")
                .arg(
                    Arg::new("folder")
                        .long("folder")
                        .short('f')
                        .default_value("migrations")
                        .about("Migrations folder")
                        .takes_value(true),
                )
                .subcommand(App::new("generate").about("Create a new migration"))
                .subcommand(App::new("migrate").about("Migrate database to latest migration"))
                .subcommand(App::new("rollback").about("Rollback database the last migration"))
                .subcommand(App::new("status").about("Show database schema status")),
        )
        .subcommand(
            App::new("cache")
                .about("Cache operations")
                .subcommand(App::new("list").about("List all cache keys"))
                .subcommand(App::new("clear").about("Remove all keys")),
        )
        .subcommand(
            App::new("generate")
                .about("Generate files")
                .arg(
                    Arg::new("target")
                        .short('t')
                        .long("target")
                        .default_value("tmp")
                        .takes_value(true)
                        .about("Target folder"),
                )
                .subcommand(App::new("config").about("Generate config.toml"))
                .subcommand(App::new("nginx").about("Generate nginx.conf"))
                .subcommand(App::new("systemd").about("Generate systemd service.conf")),
        )
        .subcommand(
            App::new("i18n")
                .about("I18n jobs")
                .subcommand(App::new("sync").about("Sync i18n items from file to db")),
        )
        .subcommand(
            App::new("deploy")
                .about("Run deploy tasks by ssh & rsync")
                .arg(
                    Arg::new("recipe")
                        .short('r')
                        .long("recipe")
                        .required(true)
                        .takes_value(true)
                        .about("Recipe name"),
                )
                .arg(
                    Arg::new("cluster")
                        .short('c')
                        .long("cluster")
                        .required(true)
                        .takes_value(true)
                        .about("Cluster name"),
                ),
        )
        .subcommand(
            App::new("twilio")
                .about("Call twilio api")
                .subcommand(
                    App::new("send")
                        .about("Send sms text message")
                        .arg(
                            Arg::new("to")
                                .short('t')
                                .long("to")
                                .required(true)
                                .takes_value(true)
                                .multiple(true)
                                .about("Sent to"),
                        )
                        .arg(
                            Arg::new("body")
                                .short('b')
                                .long("body")
                                .required(true)
                                .takes_value(true)
                                .about("Message body"),
                        ),
                )
                .subcommand(
                    App::new("push")
                        .about("Push sms text message(from url)")
                        .arg(
                            Arg::new("url")
                                .short('u')
                                .long("url")
                                .required(true)
                                .takes_value(true)
                                .about("Resource url"),
                        ),
                )
                .subcommand(App::new("pull").about("Pull sms text message")),
        )
        .subcommand(
            App::new("http")
                .about("HTTP api")
                .subcommand(App::new("routes").about("List of all of the available routes"))
                .subcommand(App::new("server").about("Start http listener")),
        )
        .get_matches();
    // let cfg = "config.toml";
    // let matches = clap::App::new(env::NAME)
    //     .version(env!("CARGO_PKG_VERSION"))
    //     .author(env::AUTHORS)
    //     .about(env::DESCRIPTION)
    //     .before_help(env::BANNER)
    //     .after_help(env::HOMEPAGE)
    //     .subcommand(generate::nginx::command())
    //     .subcommand(
    //         SubCommand::with_name(generate::config::NAME).about(&*generate::config::help(cfg)),
    //     )
    //     .subcommand(generate::systemd::command())
    //     .subcommand(database::migrate::command())
    //     .subcommand(database::rollback::command())
    //     .subcommand(database::status::command())
    //     .subcommand(i18n::sync::command())
    //     .subcommand(SubCommand::with_name(http::routes::NAME).about(http::routes::ABOUT))
    //     .get_matches();

    // if matches.subcommand_matches(http::routes::NAME).is_some() {
    //     return http::routes::run();
    // }

    // if sodiumoxide::init().is_err() {
    //     return Err(Error::Http(
    //         StatusCode::INTERNAL_SERVER_ERROR,
    //         Some("init sodium".to_string()),
    //     ));
    // }

    // if matches.subcommand_matches(generate::config::NAME).is_some() {
    //     return generate::config::run::<&'static str, env::Config>(cfg);
    // }
    // if matches
    //     .subcommand_matches(generate::systemd::COMMAND_NAME)
    //     .is_some()
    // {
    //     return generate::systemd::run();
    // }

    // info!("load configuration from {}", cfg);
    // let cfg: env::Config = parser::from_toml(cfg)?;

    // if let Some(matches) = matches.subcommand_matches(generate::nginx::COMMAND_NAME) {
    //     let name = matches.value_of(generate::nginx::ARG_SERVER_NAME).unwrap();
    //     return generate::nginx::run(name.to_string(), cfg.http.port);
    // }

    // if matches
    //     .subcommand_matches(i18n::sync::COMMAND_NAME)
    //     .is_some()
    // {
    //     return i18n::sync::run(cfg);
    // }

    // if matches
    //     .subcommand_matches(database::migrate::COMMAND_NAME)
    //     .is_some()
    // {
    //     let db = cfg.database.open()?;
    //     let db = db.get()?;
    //     return database::migrate::run(&db);
    // }
    // if matches
    //     .subcommand_matches(database::rollback::COMMAND_NAME)
    //     .is_some()
    // {
    //     let db = cfg.database.open()?;
    //     let db = db.get()?;
    //     return database::rollback::run(&db);
    // }
    // if matches
    //     .subcommand_matches(database::status::COMMAND_NAME)
    //     .is_some()
    // {
    //     let db = cfg.database.open()?;
    //     let db = db.get()?;
    //     return database::status::run(&db);
    // }

    // http::server::launch(cfg)
    Ok(())
}
