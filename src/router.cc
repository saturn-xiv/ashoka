#include "router.h"
#include "cbeta.h"

Poco::Net::HTTPRequestHandler *ashoka::RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
{

    auto method = request.getMethod();
    auto uri = request.getURI();
    auto client = request.clientAddress();

    logger.information() << client << " " << request.getVersion() << " " << method << " " << uri << std::endl;
    if (uri == "/cbeta")
    {
        return new ashoka::cbeta::HomeRequestHandler();
    }
    return NULL;
}
