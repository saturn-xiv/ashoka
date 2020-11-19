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
ashoka::api::Router::Router(const std::string secrets, const std::string &host, const short int port)
    : secrets(secrets), routes(0)
{

    std::stringstream ss;
    ss << "http://" << host << ":" << port;
    listener = web::http::experimental::listener::http_listener(U(ss.str()));
    listener.support(std::bind(&ashoka::api::Router::handle, this, std::placeholders::_1));
    // listener.support(web::http::methods::GET, std::bind(&ashoka::api::Router::handle_get, this, std::placeholders::_1));
    // listener.support(web::http::methods::PUT, std::bind(&ashoka::api::Router::handle_put, this, std::placeholders::_1));
    // listener.support(web::http::methods::POST, std::bind(&ashoka::api::Router::handle_post, this, std::placeholders::_1));
    // listener.support(web::http::methods::DEL, std::bind(&ashoka::api::Router::handle_delete, this, std::placeholders::_1));
}
ashoka::api::Router::~Router() {}

void ashoka::api::Router::append(const web::http::methods method, const std::string path, const std::shared_ptr<ashoka::api::Route> route)
{
    this->routes.push_back(std::tuple<web::http::methods, std::string, std::shared_ptr<ashoka::api::Route>>(method, path, route));
}

void ashoka::api::Router::handle(web::http::http_request request)
{
    ashoka::api::Context ctx(request, this->secrets);
    BOOST_LOG_TRIVIAL(info) << ctx.request.method() << " " << ctx.uri();
    for (const auto &it : this->routes)
    {
        // const web::http::method method = std::get<0>(it);
        const std::string regex = std::get<1>(it);
        const auto handler = std::get<2>(it);
        // if (method != ctx.request.method())
        // {
        //     continue;
        // }
        std::smatch match;
        const auto uri = ctx.path();
        // std::regex_match()
    }
    request.reply(web::http::status_codes::NotFound);
}
// void ashoka::api::Router::handle_get(web::http::http_request request)
// {
//     BOOST_LOG_TRIVIAL(debug) << "GET";
//     const auto uri = request.request_uri(); //web::http::uri::decode(request.relative_uri());
//     const std::vector<std::string> paths = web::http::uri::split_path(web::http::uri::decode(uri.path()));
//     const std::map<std::string, std::string> query = web::http::uri::split_query(web::http::uri::decode(uri.query()));
//     std::cout << "### "
//               << uri.to_string() << std::endl;

//     for (auto const &it : paths)
//     {
//         std::cout << "P " << it << std::endl;
//     }

//     for (auto const &it : query)
//     {
//         std::cout << "Q " << it.first << "=" << it.second << std::endl;
//     }

//     request.reply(web::http::status_codes::OK, "HI, GET");
// }
// void ashoka::api::Router::handle_put(web::http::http_request request)
// {
//     this->handle(ashoka::api::Method::PUT, request);
// }
// void ashoka::api::Router::handle_post(web::http::http_request request)
// {
//     this->handle(ashoka::api::Method::POST, request);
// }
// void ashoka::api::Router::handle_patch(web::http::http_request request)
// {
//     this->handle(ashoka::api::Method::PATCH, request);
// }
// void ashoka::api::Router::handle_delete(web::http::http_request request)
// {
//     this->handle(ashoka::api::Method::DELETE, request);
// }
// void ashoka::api::Router::handle_error(pplx::task<void> &t)
// {
//     try
//     {
//         t.get();
//     }
//     catch (...)
//     {
//         BOOST_LOG_TRIVIAL(debug) << "ERROR";
//     }
// }

pplx::task<void> ashoka::api::Router::open()
{
    return listener.open();
}
pplx::task<void> ashoka::api::Router::close()
{
    return listener.close();
}

// Poco::Net::HTTPRequestHandler
// *ashoka::RequestHandlerFactory::createRequestHandler(const
// Poco::Net::HTTPServerRequest &request)
// {

//     auto method = request.getMethod();
//     auto uri = request.getURI();
//     auto client = request.clientAddress();

//     BOOST_LOG_TRIVIAL(info) << client.toString() << " " <<
//     request.getVersion() << " " << method << " " << uri; if (uri == "/cbeta")
//     {
//         return new ashoka::cbeta::HomeRequestHandler();
//     }
//     return NULL;
// }
