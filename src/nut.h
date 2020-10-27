#ifndef ASHOKA_NUT_H_
#define ASHOKA_NUT_H_

#include "common.h"

namespace ashoka
{
    namespace nut
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
    } // namespace nut
} // namespace ashoka

#endif