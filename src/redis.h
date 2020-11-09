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
            ~Connection()
            {
                BOOST_LOG_TRIVIAL(debug) << "close redis connection";
                if (NULL != context)
                {
                    redisFree(this->context);
                    context = NULL;
                }
            };

            void clear();

        private:
            redisContext *context;
            std::string prefix;
            friend class Factory;
        };

        class Factory : public ashoka::pool::Factory
        {
        public:
            Factory(const std::string host, const unsigned short int port, const unsigned short int db, const std::string prefix) : host(host), port(port), db(db), prefix(prefix)
            {
                BOOST_LOG_TRIVIAL(info) << "init redis connection factory";
            };

            std::shared_ptr<ashoka::pool::Connection> create()
            {
                BOOST_LOG_TRIVIAL(debug) << "open redis " << name();
                redisContext *context = redisConnect(host.c_str(), port);
                if (context == NULL)
                {
                    throw std::invalid_argument("can't allocate redis context");
                }

                {
                    redisReply *reply = (redisReply *)redisCommand(context, "SELECT %i", db);
                    if (reply->type == REDIS_REPLY_ERROR)
                    {
                        throw std::invalid_argument(reply->str);
                    }
                    freeReplyObject(reply);
                }

                std::shared_ptr<Connection> con(new Connection());
                con->prefix = this->prefix;
                con->context = context;

                return std::static_pointer_cast<ashoka::pool::Connection>(con);
            };

            std::string name() const
            {
                std::ostringstream ss;
                ss << "tcp://" << host << ":" << port << "/" << db << "/" << prefix;
                std::string it(ss.str());
                return it;
            }

        private:
            const std::string host;
            const unsigned short int port;
            const unsigned short int db;
            const std::string prefix;
        };

        class Config : public ashoka::env::Config
        {
        public:
            Config() : host("127.0.0.1"), port(6379), db(0), pool_size(16) {}
            Config(const toml::table &root)
            {
                std::optional<std::string> host = root["host"].value<std::string>();
                this->host = host.value_or("127.0.0.1");
                std::optional<unsigned short> port = root["port"].value<unsigned short>();
                this->port = port.value_or(6379);
                std::optional<unsigned short> db = root["db"].value<unsigned short>();
                this->db = db.value_or(0);
                std::optional<std::string> prefix = root["prefix"].value<std::string>();
                if (prefix)
                {
                    this->prefix = prefix.value();
                }
                std::optional<size_t> pool_size = root["pool-size"].value<size_t>();
                this->pool_size = pool_size.value_or(20);
            }

            operator toml::table() const
            {
                toml::table root;
                root.insert("host", this->host);
                root.insert("port", this->port);
                root.insert("prefix", this->prefix);
                root.insert("db", this->db);
                root.insert("pool-size", (long)this->pool_size);
                return root;
            };
            std::string name() const
            {
                return "redis";
            };

            std::shared_ptr<ashoka::pool::Pool<Connection>> open();

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