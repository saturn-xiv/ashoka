#include "application.h"
#include "crypt.h"
#include "ops.h"
#include "redis.h"
#include "server.h"
#include "utils.h"

ashoka::Config::Config(const std::string &name)
{
    const std::string file = name + ".toml";
    BOOST_LOG_TRIVIAL(info) << "load recipe from " << file;
    toml::table root = toml::parse_file(file);
    load(root);
}

void ashoka::Config::load(const toml::table &root)
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

    boost::program_options::options_description desc("Allowed options");

    desc.add_options()("recipe,r", boost::program_options::value<std::string>(), "recipe name(toml)")("config,c", boost::program_options::value<std::string>()->default_value("config"), "configuration file(toml)")("debug,d", boost::program_options::bool_switch(), "debug mode")("version,v", "print version")("help,h", "display argument help information");

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
    const std::string config = vm["config"].as<std::string>();
    BOOST_LOG_TRIVIAL(info) << "load from " << config;

    // boost::property_tree::ptree cfg;
    // boost::property_tree::read_ini(config, cfg);
    // std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> redis = ashoka::redis::open(&cfg);

    auto cfg = ashoka::Config(config);
    auto redis = cfg.redis.open();

    ashoka::Server server = ashoka::Server(8080);
    server.listen();
    return EXIT_SUCCESS;
}