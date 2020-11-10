#include "application.h"
#include "crypt.h"
#include "ops.h"
#include "redis.h"
#include "server.h"
#include "twilio.h"
#include "utils.h"

ashoka::Config::Config(const toml::table &root)
{
    {
        auto node = root["postgresql"];
        if (node.is_table())
        {
            auto table = node.as_table();
            this->postgresql = ashoka::postgresql::Config(*table);
        }
    }
    {
        auto node = root["redis"];
        if (node.is_table())
        {
            auto table = node.as_table();
            this->redis = ashoka::redis::Config(*table);
        }
    }
    {
        auto node = root["rabbitmq"];
        if (node.is_table())
        {
            auto table = node.as_table();
            this->rabbitmq = ashoka::rabbitmq::Config(*table);
        }
    }
}

int ashoka::Application::run(int argc, char **argv)
{
    ashoka::crypt::init();

    boost::program_options::options_description generic("Generic options");
    generic.add_options()("version,v", "print version")("help,h", "display argument help information");

    boost::program_options::options_description global("Global options");
    global.add_options()("config,c", boost::program_options::value<std::string>()->default_value("config"), "configuration file(toml)")("debug,d", boost::program_options::bool_switch(), "debug mode");

    boost::program_options::options_description twilio("Twilio options");
    twilio.add_options()("twilio-sms-to", boost::program_options::value<std::string>(), "target phone number")("twilio-sms-message", boost::program_options::value<std::string>(), "sms message");

    boost::program_options::options_description ops("Ops options");
    ops.add_options()("recipe,r", boost::program_options::value<std::string>(), "recipe name(toml)");

    boost::program_options::options_description desc("Allowed options");
    desc.add(generic).add(global).add(twilio).add(ops);
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
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
        const auto name = vm["recipe"].as<std::string>();
        ashoka::ops::deploy::Recipe recipe(name);
        BOOST_LOG_TRIVIAL(debug) << recipe;
        recipe.execute();
        return EXIT_SUCCESS;
    }

    BOOST_LOG_TRIVIAL(info) << ASHOKA_PROJECT_NAME << "(" << ASHOKA_VERSION << ")";
    BOOST_LOG_TRIVIAL(debug) << "run in debug mode";
    const std::string config = vm["config"].as<std::string>() + ".toml";
    BOOST_LOG_TRIVIAL(info) << "load from " << config;
    toml::table root = toml::parse_file(config);

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

    // boost::property_tree::ptree cfg;
    // boost::property_tree::read_ini(config, cfg);
    // std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> redis = ashoka::redis::open(&cfg);

    auto cfg = ashoka::Config(root);
    auto redis = cfg.redis.open();

    ashoka::Server server = ashoka::Server(8080);
    server.listen();
    return EXIT_SUCCESS;
}