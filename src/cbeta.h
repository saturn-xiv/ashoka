#ifndef ASHOKA_MINI_WEB_H_
#define ASHOKA_MINI_WEB_H_

#include "common.h"

namespace ashoka
{
    namespace cbeta
    {
        class HomeRequestHandler : public Poco::Net::HTTPRequestHandler
        {
        public:
            HomeRequestHandler()
            {
            }
            void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

        private:
        };
    } // namespace cbeta
} // namespace ashoka

#endif