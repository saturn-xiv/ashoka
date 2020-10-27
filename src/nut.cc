#include "nut.h"

void ashoka::nut::HomeRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{

        response.setContentType(APPLICATION_JSON_UTF8);

        std::ostream &ostr = response.send();
        ostr << "nut handler";
}