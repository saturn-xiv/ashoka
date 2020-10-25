#include "server.h"
#include "router.h"

void ashoka::ServerApplication::initialize(Poco::Util::Application &self)
{
    loadConfiguration();
    Poco::Util::ServerApplication::initialize(self);
}

void ashoka::ServerApplication::uninitialize()
{
    Poco::Util::Application &app = Poco::Util::Application::instance();
    if (command == ashoka::Command::HTTP)
    {
        app.logger().warning("exit.");
    }
}

void ashoka::ServerApplication::defineOptions(Poco::Util::OptionSet &options)
{
    Poco::Util::ServerApplication::defineOptions(options);

    options.addOption(
        Poco::Util::Option("help", "h", "display argument help information")
            .required(false)
            .repeatable(false)
            .callback(Poco::Util::OptionCallback<ServerApplication>(
                this, &ServerApplication::handleHelp)));
    options.addOption(
        Poco::Util::Option("version", "v", "print version")
            .required(false)
            .repeatable(false)
            .callback(Poco::Util::OptionCallback<ServerApplication>(
                this, &ServerApplication::handleVersion)));
}

void ashoka::ServerApplication::handleHelp(const std::string &name, const std::string &value)
{
    command = ashoka::Command::HELP;
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("https://git.pro-lite.com/pro-lite/ashoka");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
}

void ashoka::ServerApplication::handleVersion(const std::string &name, const std::string &value)
{
    command = ashoka::Command::VERSION;
    std::cout << ASHOKA_GIT_VERSION << std::endl;
    stopOptionsProcessing();
}

int ashoka::ServerApplication::main(const std::vector<std::string> &args)
{
    Poco::Util::Application &app = Poco::Util::Application::instance();

    if (!isInteractive())
    {
        Poco::AutoPtr<Poco::SyslogChannel> ch(new Poco::SyslogChannel);
        app.logger().setChannel(ch);
    }

#ifndef NDEBUG
    app.logger().setLevel(Poco::Message::PRIO_TRACE);
#endif
    Poco::LogStream logger(app.logger());

    if (command == Command::HTTP)
    {
        logger.information() << ASHOKA_PROJECT_NAME << "(" << ASHOKA_VERSION << ")" << std::endl;

        unsigned short port = static_cast<unsigned short>(config().getInt("http.port", 80));
        std::string secrets(config().getString("http.secrets"));

        ashoka::Redis *redis = new ashoka::Redis(config(), logger);
        std::string postgresql(config().getString("postgresql.url"));

        logger.information() << "listen on http://127.0.0.1:" << port << std::endl;
        Poco::Net::ServerSocket svs(port);
        Poco::Net::HTTPServer srv(
            new ashoka::RequestHandlerFactory(redis, logger),
            svs,
            new Poco::Net::HTTPServerParams);
        srv.start();
        waitForTerminationRequest();
        srv.stop();
    }
    return Application::EXIT_OK;
}