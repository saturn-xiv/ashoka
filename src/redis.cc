#include "redis.h"

void ashoka::redis::Connection::clear()
{
    redisReply *reply = (redisReply *)redisCommand(context, "FLUSHDB");
    if (reply->type == REDIS_REPLY_ERROR)
    {
        throw std::runtime_error(reply->str);
    }
    freeReplyObject(reply);
}

std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> ashoka::redis::open(const std::string host, const unsigned short int port, const unsigned short int db, const std::string prefix, const size_t size)
{
    std::shared_ptr<ashoka::redis::Factory> factory(new ashoka::redis::Factory(host, port, db, prefix));
    std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> pool(new ashoka::pool::Pool<ashoka::redis::Connection>(size, factory));
    return pool;
}