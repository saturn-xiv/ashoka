#ifndef ASHOKA_REDIS_H_
#define ASHOKA_REDIS_H_

#include <string>

#include <boost/log/trivial.hpp>
#include <hiredis/hiredis.h>

namespace ashoka
{
    class Redis
    {
    public:
        Redis();
        ~Redis();

        void clear();

    private:
        redisContext *context;
        std::string prefix;
    };
} // namespace ashoka

#endif