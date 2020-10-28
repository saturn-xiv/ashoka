#include "application.h"
#include "server.h"
#include "utils.h"

int ashoka::Application::run(int argc, char **argv)
{
    boost::program_options::options_description desc("Allowed options");

    desc.add_options()("config,c", boost::program_options::value<std::string>()->default_value("config.ini"), "configuration file(ini)")("debug,d", boost::program_options::bool_switch(), "debug mode")("version,v", "print version")("help,h", "display argument help information");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

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

    const bool debug = vm["debug"].as<bool>();
    ashoka::utils::init_logging(false, debug);

    BOOST_LOG_TRIVIAL(info) << ASHOKA_PROJECT_NAME << "(" << ASHOKA_VERSION << ")";
    BOOST_LOG_TRIVIAL(debug) << "run in debug mode";
    if (vm.count("config"))
    {
        const std::string config = vm["config"].as<std::string>();
        BOOST_LOG_TRIVIAL(info) << "load from " << config;
    }
    else
    {
        std::cout << "Compression level was not set.\n";
    }

    ashoka::Server server = ashoka::Server(8080);
    server.listen();
    return EXIT_SUCCESS;
}