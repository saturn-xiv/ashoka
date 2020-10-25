#include "redis.h"

ashoka::Redis::Redis(const Poco::Util::LayeredConfiguration &config, Poco::LogStream &logger)
{

    std::string host(config.getString("redis.host", "localhost"));
    unsigned short port = static_cast<unsigned short>(config.getInt("redis.port", 6379));
    unsigned short db = static_cast<unsigned short>(config.getInt("redis.db", 0));
    prefix = config.getString("redis.prefix");

    logger.information() << "open redis tcp://" << host << ":" << port << "/" << db << "/" << prefix << std::endl;
    context = redisConnect(host.c_str(), port);
    if (context == NULL)
    {
        throw Poco::ApplicationException("can't allocate redis context");
    }
    redisReply *reply = (redisReply *)redisCommand(context, "SELECT %i", db);
    if (reply->type == REDIS_REPLY_ERROR)
    {
        throw Poco::ApplicationException(reply->str);
    }
    freeReplyObject(reply);
}

ashoka::Redis::~Redis()
{
    redisFree(context);
}

void ashoka::Redis::clear()
{
    // TOCO clear by prefix
    redisReply *reply = (redisReply *)redisCommand(context, "FLUSHDB");
    if (reply->type == REDIS_REPLY_ERROR)
    {
        throw Poco::ApplicationException(reply->str);
    }
    freeReplyObject(reply);
}