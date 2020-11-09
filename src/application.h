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
        Config(const std::string &name);
        friend class Application;

    private:
        void load(const toml::table &root);

        operator toml::table() const
        {
            toml::table root;
            {
                toml::table node = this->postgresql;
                root.insert("postgresql", node);
            }
            {
                toml::table node = this->redis;
                root.insert("redis", node);
            }
            {
                toml::table node = this->rabbitmq;
                root.insert("rabbitmq", node);
            }

            return root;
        };

    private:
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