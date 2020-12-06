#include "application.h"
#include "router.h"
#include "utils.h"
#include "twilio.h"

int ashoka::Application::run(int argc, char **argv)
{

    boost::program_options::options_description generic("Generic options");
    generic.add_options()("version,v", "print version")(
        "help,h", "display argument help information");

    boost::program_options::options_description global("Global options");
    global.add_options()(
        "config,c",
        boost::program_options::value<std::string>()->default_value("config"),
        "configuration file(toml)")(
        "debug,d", boost::program_options::bool_switch(), "debug mode");

    boost::program_options::options_description db("PostgreSQL options");
    db.add_options()(
        "db-schema",
        boost::program_options::value<std::string>()->default_value("db"),
        "db schema folder")("db-generate",
                            boost::program_options::value<std::string>(),
                            "generate migratation by name")(
        "db-migrate", boost::program_options::bool_switch(),
        "migrate database to latest migration")(
        "db-rollback", boost::program_options::bool_switch(),
        "rollback database the last migration")(
        "db-status", boost::program_options::bool_switch(),
        "show database schema status");

    boost::program_options::options_description crawler("Crawler options");
    crawler.add_options()("crawler-all", boost::program_options::bool_switch(),
                          "run all crawler")(
        "crawler-name", boost::program_options::value<std::string>(),
        "run crawler by name");

    boost::program_options::options_description twilio("Twilio options");
    twilio.add_options()("twilio-sms-to",
                         boost::program_options::value<std::string>(),
                         "target phone number")(
        "twilio-sms-message", boost::program_options::value<std::string>(),
        "sms message");

    boost::program_options::options_description ops("Ops options");
    ops.add_options()("recipe,r", boost::program_options::value<std::string>(),
                      "recipe name(toml)");

    boost::program_options::options_description desc("Allowed options");
    desc.add(generic).add(global).add(db).add(crawler).add(twilio).add(ops);
    boost::program_options::variables_map vm;
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    const bool debug = vm["debug"].as<bool>();
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }
    if (vm.count("version"))
    {
        std::cout << ASHOKA_GIT_VERSION << std::endl;
        return EXIT_SUCCESS;
    }

    ashoka::utils::init_logging(false, debug);
    if (vm.count("recipe"))
    {
        // TODO
        // const auto name = vm["recipe"].as<std::string>();
        // ashoka::ops::deploy::Recipe recipe(name);
        // BOOST_LOG_TRIVIAL(debug) << recipe;
        // recipe.execute();
        return EXIT_SUCCESS;
    }

    BOOST_LOG_TRIVIAL(info) << ASHOKA_PROJECT_NAME << "(" << ASHOKA_VERSION
                            << ")";
    BOOST_LOG_TRIVIAL(debug) << "run in debug mode";
    const std::string config = vm["config"].as<std::string>() + ".toml";
    const std::string db_schema = vm["db-schema"].as<std::string>();
    BOOST_LOG_TRIVIAL(info) << "load from " << config;
    toml::table root = toml::parse_file(config);

    {
        BOOST_LOG_TRIVIAL(debug) << "setup https client";
        Poco::Net::Context::Ptr context =
            new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", Poco::Net::Context::VERIFY_NONE);
        Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> handler =
            new Poco::Net::AcceptCertificateHandler(true);
        Poco::Net::SSLManager::instance().initializeClient(NULL, handler, context);
    }

    if (vm.count("twilio-sms-to") && vm.count("twilio-sms-message"))
    {
        const auto to = vm["twilio-sms-to"].as<std::string>();
        const auto message = vm["twilio-sms-message"].as<std::string>();
        auto node = root["twilio"];
        if (node.is_table())
        {
            auto table = node.as_table();
            const ashoka::twilio::Config cfg(*table);
            const ashoka::twilio::Client cli(cfg);
            auto rst = cli.sms(to, message);
            BOOST_LOG_TRIVIAL(debug) << rst;
            return EXIT_SUCCESS;
        }
    }

    // const auto cfg = ashoka::Config(root);
    // {

    //     auto pg = cfg.postgresql.open(std::nullopt);
    //     auto db = pg->get();
    //     ashoka::postgresql::SchemaDao dao(db->context, std_fs::path(db_schema));
    //     dao.load();

    //     if (vm.count("db-generate"))
    //     {
    //         const auto name = vm["db-generate"].as<std::string>();
    //         dao.generate(name);
    //         return EXIT_SUCCESS;
    //     }

    //     if (vm["db-migrate"].as<bool>())
    //     {
    //         dao.migrate();
    //         return EXIT_SUCCESS;
    //     }
    //     if (vm["db-rollback"].as<bool>())
    //     {
    //         dao.rollback();
    //         return EXIT_SUCCESS;
    //     }
    //     if (vm["db-status"].as<bool>())
    //     {
    //         std::cout << dao << std::endl;
    //         return EXIT_SUCCESS;
    //     }
    // }

    // auto pg = cfg.postgresql.open(std_fs::path(db_schema) / "prepares.toml");

    // {
    //     auto db = pg->get();
    //     auto node = root["crawler"];
    //     if (!node.is_table())
    //     {
    //         BOOST_LOG_TRIVIAL(error) << "con't find crawler config";
    //         return EXIT_FAILURE;
    //     }
    //     ashoka::crawler::Crawler crawler(db->context, *(node.as_table()));

    //     if (vm["crawler-all"].as<bool>())
    //     {
    //         crawler.execute();
    //         return EXIT_SUCCESS;
    //     }
    //     if (vm.count("crawler-name"))
    //     {
    //         const auto name = vm["crawler-name"].as<std::string>();
    //         crawler.execute(name);
    //         return EXIT_SUCCESS;
    //     }
    // }

    // auto redis = cfg.redis.open();
    // {
    //     auto it = redis.get();
    // }

    // auto router = ashoka::api::Router(cfg.http);
    // {
    //     ashoka::nut::Plugin nut;
    //     nut.mount(router);
    // }
    // router.start();
    return EXIT_SUCCESS;
}