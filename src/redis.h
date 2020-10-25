#ifndef ASHOKA_REDIS_H_
#define ASHOKA_REDIS_H_

#include <string>

#include <Poco/Exception.h>
#include <Poco/LogStream.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <hiredis/hiredis.h>

namespace ashoka
{
    class Redis
    {
    public:
        Redis(const Poco::Util::LayeredConfiguration &config, Poco::LogStream &logger);
        ~Redis();

        void clear();

    private:
        redisContext *context;
        std::string prefix;
    };
} // namespace ashoka

#endif