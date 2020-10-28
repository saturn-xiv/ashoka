#include "application.h"
#include "server.h"
#include "utils.h"

int ashoka::Application::run(int argc, char **argv)
{
    boost::program_options::options_description desc("Allowed options");

    desc.add_options()("help,h", "display argument help information")("version,v", "print version")("config,c", boost::program_options::value<std::string>()->default_value("config.toml"), "configuration file(toml)");

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

    ashoka::utils::init_logging(false);

    BOOST_LOG_TRIVIAL(info) << ASHOKA_PROJECT_NAME << "(" << ASHOKA_VERSION << ")";
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