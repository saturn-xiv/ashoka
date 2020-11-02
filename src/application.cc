#include "application.h"
#include "crypt.h"
#include "ops.h"
#include "redis.h"
#include "server.h"
#include "utils.h"

int ashoka::Application::run(int argc, char **argv)
{
    ashoka::crypt::init();

    boost::program_options::options_description desc("Allowed options");

    desc.add_options()("recipe,r", boost::program_options::value<std::string>(), "recipe name(json)")("config,c", boost::program_options::value<std::string>()->default_value("config.ini"), "configuration file(ini)")("debug,d", boost::program_options::bool_switch(), "debug mode")("version,v", "print version")("help,h", "display argument help information");

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
        BOOST_LOG_TRIVIAL(info) << "load recipe " << name;
        std::ifstream is(name + ".json");
        nlohmann::json js;
        is >> js;
        auto recipe = js.get<ashoka::ops::deploy::Recipe>();
        BOOST_LOG_TRIVIAL(debug) << recipe;
        recipe.execute();
        return EXIT_SUCCESS;
    }

    BOOST_LOG_TRIVIAL(info) << ASHOKA_PROJECT_NAME << "(" << ASHOKA_VERSION << ")";
    BOOST_LOG_TRIVIAL(debug) << "run in debug mode";
    const std::string config = vm["config"].as<std::string>();
    BOOST_LOG_TRIVIAL(info) << "load from " << config;

    boost::property_tree::ptree cfg;
    boost::property_tree::read_ini(config, cfg);
    std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> redis = ashoka::redis::open(&cfg);

    ashoka::Server server = ashoka::Server(8080);
    server.listen();
    return EXIT_SUCCESS;
}