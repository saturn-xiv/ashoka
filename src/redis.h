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

        std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> open(const std::string host, const unsigned short int port, const unsigned short int db, const std::string prefix, const size_t size);
        std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> open(boost::property_tree::ptree *tree);

    } // namespace redis

} // namespace ashoka

#endif