#ifndef ASHOKA_REDIS_H_
#define ASHOKA_REDIS_H_

#include <hiredis.h>

#include "pool.h"

namespace ashoka
{
    namespace redis
    {

        class Connection : public ashoka::pool::Connection
        {
        public:
            ~Connection() override;
            void clear();
            friend class Factory;

        private:
            redisContext *context;
            std::string prefix;
        };

        class Factory : public ashoka::pool::Factory
        {
        public:
            Factory(const std::string host, const unsigned short int port, const unsigned short int db, const std::string prefix);
            std::shared_ptr<ashoka::pool::Connection> create() override;
            std::string name() const override;

        private:
            const std::string host;
            const unsigned short int port;
            const unsigned short int db;
            const std::string prefix;
        };

        class Config : public ashoka::env::Config
        {
        public:
            Config();
            Config(const toml::table &root);
            std::shared_ptr<ashoka::pool::Pool<Connection>> open();
            operator toml::table() const override;
            std::string name() const override;

        private:
            std::string host;
            unsigned short int port;
            unsigned short int db;
            std::string prefix;
            size_t pool_size;
        };

        std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> open(const std::string host, const unsigned short int port, const unsigned short int db, const std::string prefix, const size_t size);
        std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> open(boost::property_tree::ptree *tree);

    } // namespace redis

} // namespace ashoka

#endif