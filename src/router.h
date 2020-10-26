#ifndef ASHOKA_ROUTER_H_
#define ASHOKA_ROUTER_H_

#include "common.h"
#include "redis.h"

namespace ashoka
{

    class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
    {
    public:
        RequestHandlerFactory(Redis *redis) : redis(redis)
        {
        }

        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request);

    private:
        Redis *redis;
    };

} // namespace ashoka

#endif