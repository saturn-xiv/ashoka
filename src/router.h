#ifndef ASHOKA_ROUTER_H_
#define ASHOKA_ROUTER_H_

#include "common.h"
#include "redis.h"

namespace ashoka
{

    class HttpRouter
    {
    public:
        HttpRouter(Redis *redis) : redis(redis)
        {
        }
        ~HttpRouter();

    private:
        Redis *redis;
    };

} // namespace ashoka

#endif