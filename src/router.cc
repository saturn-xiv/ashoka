#include "router.h"
#include "cbeta.h"

ashoka::HttpRouter::HttpRouter() {}

// Poco::Net::HTTPRequestHandler *ashoka::RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
// {

//     auto method = request.getMethod();
//     auto uri = request.getURI();
//     auto client = request.clientAddress();

//     BOOST_LOG_TRIVIAL(info) << client.toString() << " " << request.getVersion() << " " << method << " " << uri;
//     if (uri == "/cbeta")
//     {
//         return new ashoka::cbeta::HomeRequestHandler();
//     }
//     return NULL;
// }
