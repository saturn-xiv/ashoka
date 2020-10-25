#ifndef ASHOKA_ROUTER_H_
#define ASHOKA_ROUTER_H_

#include "common.h"
#include "redis.h"

namespace ashoka
{

    class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
    {
    public:
        RequestHandlerFactory(Redis *redis,
                              Poco::LogStream &logger)
            : redis(redis), logger(logger)
        {
        }

        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request);

    private:
        Poco::LogStream &logger;
        Redis *redis;
    };
} // namespace ashoka

} // namespace ashoka
#endif