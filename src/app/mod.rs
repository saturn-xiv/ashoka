pub mod generate;
// pub mod http;
// pub mod i18n;

use std::ops::Deref;

use actix_web::http::StatusCode;
use clap::{App, Arg};

use super::{
    env,
    errors::{Error, Result},
    orm::migration::Schema as MigrationSchema,
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
                .default_value("config.toml")
                .takes_value(true),
        )
        .subcommand(
            App::new("db")
                .about("Database operations")
                .arg(
                    Arg::new("folder")
                        .long("folder")
                        .short('f')
                        .default_value("db")
                        .about("Migrations folder")
                        .takes_value(true),
                )
                .subcommand(
                    App::new("generate")
                        .arg(
                            Arg::new("name")
                                .long("name")
                                .short('n')
                                .required(true)
                                .about("Name")
                                .takes_value(true),
                        )
                        .about("Create a new migration"),
                )
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
            App::new("crawler").about("Run crawler").arg(
                Arg::new("name")
                    .long("name")
                    .short('n')
                    .about("Crawler name")
                    .takes_value(true),
            ),
        )
        .subcommand(
            App::new("generate")
                .about("Generate files")
                .subcommand(
                    App::new("config")
                        .arg(
                            Arg::new("name")
                                .short('n')
                                .long("name")
                                .required(true)
                                .takes_value(true)
                                .about("Name"),
                        )
                        .about("Generate config.toml"),
                )
                .subcommand(
                    App::new("nginx")
                        .arg(
                            Arg::new("domain")
                                .short('d')
                                .long("domain")
                                .required(true)
                                .takes_value(true)
                                .about("Domain name"),
                        )
                        .arg(
                            Arg::new("port")
                                .short('p')
                                .long("port")
                                .required(true)
                                .default_value("8080")
                                .takes_value(true)
                                .about("Listen port"),
                        )
                        .about("Generate nginx.conf"),
                )
                .subcommand(
                    App::new("systemd")
                        .arg(
                            Arg::new("name")
                                .short('n')
                                .long("name")
                                .required(true)
                                .takes_value(true)
                                .about("Name"),
                        )
                        .about("Generate systemd service.conf"),
                ),
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
    if sodiumoxide::init().is_err() {
        return Err(Error::Http(
            StatusCode::INTERNAL_SERVER_ERROR,
            Some("init sodium".to_string()),
        ));
    }

    debug!("run on debug mode");
    if let Some(matches) = matches.subcommand_matches("generate") {
        if let Some(matches) = matches.subcommand_matches("config") {
            let name = matches.value_of("name").unwrap();
            return generate::config::run::<&str, env::Config>(name);
        }
        if let Some(matches) = matches.subcommand_matches("nginx") {
            let domain = matches.value_of("domain").unwrap();
            let port = matches.value_of("port").unwrap().parse()?;
            return generate::nginx::run(domain, port);
        }
        if let Some(matches) = matches.subcommand_matches("systemd") {
            let name = matches.value_of("name").unwrap();
            return generate::systemd::run(name);
        }
    }

    let config = matches.value_of("config").unwrap();
    info!("load configuration from {}", config);
    let config: env::Config = parser::from_toml(config)?;
    if let Some(matches) = matches.subcommand_matches("db") {
        let root = matches.value_of("folder").unwrap();
        let db = config.database.open()?;
        let db = db.get()?;
        info!("using folder {}", root);
        let it = MigrationSchema::new(&root, db.deref())?;
        if let Some(matches) = matches.subcommand_matches("generate") {
            let name = matches.value_of("name").unwrap();
            return it.generate(&name);
        }
        if let Some(_) = matches.subcommand_matches("migrate") {
            return it.migrate();
        }
        if let Some(_) = matches.subcommand_matches("rollback") {
            return it.rollback();
        }
        if let Some(_) = matches.subcommand_matches("status") {
            return it.status();
        }
    }
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

    // if matches.subcommand_matches(generate::config::NAME).is_some() {
    //     return generate::config::run::<&'static str, env::Config>(cfg);
    // }
    // if matches
    //     .subcommand_matches(generate::systemd::COMMAND_NAME)
    //     .is_some()
    // {
    //     return generate::systemd::run();
    // }

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
