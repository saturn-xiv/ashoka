#include "server.h"
#include "nut.h"
#include "router.h"
#include "utils.h"

void ashoka::HttpServer::listen() {}

ashoka::HttpServer::HttpServer(unsigned short int port) : port(port) {}

ashoka::HttpServer::~HttpServer()
{
  // Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(port));

  // auto opts = Pistache::Http::Endpoint::options().threads(1);
  // Pistache::Http::Endpoint server(addr);
  // server.init(opts);
  // server.setHandler(std::make_shared<ashoka::nut::HomeHandler>());
  // server.serve();
}

// void ashoka::ServerApplication::initialize(Poco::Util::Application &self)
// {
//     loadConfiguration();
//     Poco::Util::ServerApplication::initialize(self);
// }

// void ashoka::ServerApplication::uninitialize()
// {
//     Poco::Util::Application &app = Poco::Util::Application::instance();
//     if (command == ashoka::Command::HTTP)
//     {
//         BOOST_LOG_TRIVIAL(warning) << "exit...";
//     }
// }

// void ashoka::ServerApplication::defineOptions(Poco::Util::OptionSet &options)
// {
//     Poco::Util::ServerApplication::defineOptions(options);

//     options.addOption(
//         Poco::Util::Option("help", "h", "display argument help information")
//             .required(false)
//             .repeatable(false)
//             .callback(Poco::Util::OptionCallback<ServerApplication>(
//                 this, &ServerApplication::handleHelp)));
//     options.addOption(
//         Poco::Util::Option("version", "v", "print version")
//             .required(false)
//             .repeatable(false)
//             .callback(Poco::Util::OptionCallback<ServerApplication>(
//                 this, &ServerApplication::handleVersion)));
// }

// void ashoka::ServerApplication::handleHelp(const std::string &name, const
// std::string &value)
// {
//     command = ashoka::Command::HELP;
//     Poco::Util::HelpFormatter helpFormatter(options());
//     helpFormatter.setCommand(commandName());
//     helpFormatter.setUsage("OPTIONS");
//     helpFormatter.setHeader("https://git.pro-lite.com/pro-lite/ashoka");
//     helpFormatter.format(std::cout);
//     stopOptionsProcessing();
// }

// void ashoka::ServerApplication::handleVersion(const std::string &name, const
// std::string &value)
// {
//     command = ashoka::Command::VERSION;
//     std::cout << ASHOKA_GIT_VERSION << std::endl;
//     stopOptionsProcessing();
// }

// int ashoka::ServerApplication::main(const std::vector<std::string> &args)
// {
//     Poco::Util::Application &app = Poco::Util::Application::instance();

//     ashoka::utils::init_logging(!isInteractive());

//     if (command == Command::HTTP)
//     {
//         BOOST_LOG_TRIVIAL(info) << ASHOKA_PROJECT_NAME << "(" <<
//         ASHOKA_VERSION << ")";

//         unsigned short port = static_cast<unsigned
//         short>(config().getInt("http.port", 80)); std::string
//         secrets(config().getString("http.secrets"));

//         ashoka::Redis *redis = new ashoka::Redis(config());
//         std::string postgresql(config().getString("postgresql.url"));

//         BOOST_LOG_TRIVIAL(info) << "listen on http://127.0.0.1:" << port;
//         Poco::Net::ServerSocket svs(port);
//         Poco::Net::HTTPServer srv(
//             new ashoka::RequestHandlerFactory(redis),
//             svs,
//             new Poco::Net::HTTPServerParams);
//         srv.start();
//         waitForTerminationRequest();
//         srv.stop();
//     }
//     return Application::EXIT_OK;
// }