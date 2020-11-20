#include "router.h"

ashoka::api::Context::Context(web::http::http_request request, const std::string secrets)
    : request(request), secrets(secrets)
{
}
bool ashoka::api::Context::is_get() const
{
    return request.method() == web::http::methods::GET;
}
bool ashoka::api::Context::is_post() const
{
    return request.method() == web::http::methods::POST;
}
bool ashoka::api::Context::is_put() const
{
    return request.method() == web::http::methods::PUT;
}
bool ashoka::api::Context::is_patch() const
{
    return request.method() == web::http::methods::PATCH;
}
bool ashoka::api::Context::is_delete() const
{
    return request.method() == web::http::methods::DEL;
}

std::string ashoka::api::Context::uri() const
{
    return web::http::uri::decode(request.request_uri().to_string());
}

std::string ashoka::api::Context::path() const
{
    return web::http::uri::decode(request.request_uri().path());
}
std::vector<std::string> ashoka::api::Context::paths() const
{
    return web::http::uri::split_path(web::http::uri::decode(request.request_uri().path()));
}
std::map<std::string, std::string> ashoka::api::Context::query() const
{
    return web::http::uri::split_query(web::http::uri::decode(request.request_uri().query()));
}
// --------------------------

ashoka::api::Config::Config() : port(8080), secrets(ashoka::crypt::random_base64_string(32)) {}
ashoka::api::Config::Config(const toml::table &root)
{
    std::optional<unsigned short> port = root["port"].value<unsigned short>();
    this->port = port.value_or(8080);
    std::optional<std::string> host = root["secrets"].value<std::string>();
    this->secrets = host.value();
}
ashoka::api::Config::operator toml::table() const
{
    toml::table root;
    root.insert("port", this->port);
    root.insert("secrets", this->secrets);
    return root;
}
std::string ashoka::api::Config::name() const
{
    return "http";
}

// -------------------------
ashoka::api::Router::Router(const ashoka::api::Config &config)
    : secrets(config.secrets), routes(0)
{
    this->open("127.0.0.1", config.port);
}
ashoka::api::Router::Router(const std::string secrets, const std::string &host, const short int port)
    : secrets(secrets), routes(0)
{
    this->open(host, port);
}
ashoka::api::Router::~Router() {}

void ashoka::api::Router::open(const std::string host, const short int port)
{
    std::stringstream ss;
    ss << "http://" << host << ":" << port;
    listener = web::http::experimental::listener::http_listener(U(ss.str()));
    listener.support(std::bind(&ashoka::api::Router::handle, this, std::placeholders::_1));
}

void ashoka::api::Router::append(const ashoka::api::Method method, const std::string path, const std::shared_ptr<ashoka::api::Route> route)
{
    this->routes.push_back(std::tuple<ashoka::api::Method, std::string, std::shared_ptr<ashoka::api::Route>>(method, path, route));
}

void ashoka::api::Router::handle(web::http::http_request request)
{
    ashoka::api::Context ctx(request, this->secrets);
    const auto rqm = request.method();
    const auto path = ctx.path();

    BOOST_LOG_TRIVIAL(info) << ctx.request.method() << " " << ctx.uri();
    for (const auto &it : this->routes)
    {
        {
            const auto rtm = std::get<0>(it);
            if ((rqm == web::http::methods::GET && rtm != ashoka::api::Method::GET) ||
                (rqm == web::http::methods::POST && rtm != ashoka::api::Method::POST) ||
                (rqm == web::http::methods::PUT && rtm != ashoka::api::Method::PUT) ||
                (rqm == web::http::methods::PATCH && rtm != ashoka::api::Method::PATCH) ||
                (rqm == web::http::methods::DEL && rtm != ashoka::api::Method::DELETE))
            {
                continue;
            }
        }
        {
            const auto pattern = std::get<1>(it);
            const std::regex regex(pattern);
            std::smatch result;
            if (std::regex_match(path.cbegin(), path.cend(), result, regex))
            {
                BOOST_LOG_TRIVIAL(info) << "MATCH: " << pattern;
                const auto handler = std::get<2>(it);
                handler->execute(ctx);
                return;
            }
        }
    }
    request.reply(web::http::status_codes::NotFound);
}

void ashoka::api::Router::start()
{
    listener.open().wait();
    BOOST_LOG_TRIVIAL(info) << "wait(input quit/exit to exit)..." << std::endl;
    std::string line;
    for (;;)
    {
        std::getline(std::cin, line);
        if (line == "quit" || line == "exit")
        {
            break;
        }
    }
    listener.close().wait();
}