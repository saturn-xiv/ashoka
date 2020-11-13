#ifndef ASHOKA_APPLICATION_H_
#define ASHOKA_APPLICATION_H_

#include "common.h"
#include "postgresql.h"
#include "rabbitmq.h"
#include "redis.h"

namespace ashoka
{
    class Config
    {
    public:
        Config(const toml::table &root);
        friend class Application;

    private:
        operator toml::table() const;

        ashoka::rabbitmq::Config rabbitmq;
        ashoka::redis::Config redis;
        ashoka::postgresql::Config postgresql;
    };

    class Application
    {
    public:
        int run(int argc, char **argv);
    };
} // namespace ashoka

#endif