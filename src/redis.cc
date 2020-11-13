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
ashoka::redis::Connection::~Connection()
{
    BOOST_LOG_TRIVIAL(debug) << "close redis connection";
    if (NULL != context)
    {
        redisFree(this->context);
        context = NULL;
    }
}
// ---------------------------
ashoka::redis::Factory::Factory(const std::string host, const unsigned short int port, const unsigned short int db, const std::string prefix)
    : host(host), port(port), db(db), prefix(prefix)
{
    BOOST_LOG_TRIVIAL(info) << "init redis connection factory";
}

std::string ashoka::redis::Factory::name() const
{
    std::ostringstream ss;
    ss << "tcp://" << host << ":" << port << "/" << db << "/" << prefix;
    return ss.str();
}

std::shared_ptr<ashoka::pool::Connection> ashoka::redis::Factory::create()
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
}

// ------------------------
ashoka::redis::Config::operator toml::table() const
{
    toml::table root;
    root.insert("host", this->host);
    root.insert("port", this->port);
    root.insert("prefix", this->prefix);
    root.insert("db", this->db);
    root.insert("pool-size", (long)this->pool_size);
    return root;
};

ashoka::redis::Config::Config() : host("127.0.0.1"), port(6379), db(0), pool_size(16)
{
}

ashoka::redis::Config::Config(const toml::table &root)
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

std::string ashoka::redis::Config::name() const
{
    return "redis";
}
// ---------------------------

std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> ashoka::redis::Config::open()
{
    std::shared_ptr<ashoka::redis::Factory> factory(new ashoka::redis::Factory(this->host, this->port, this->db, this->prefix));
    std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> pool(new ashoka::pool::Pool<ashoka::redis::Connection>(this->pool_size, factory));
    return pool;
}

// ---------------------------

std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> ashoka::redis::open(const std::string host, const unsigned short int port, const unsigned short int db, const std::string prefix, const size_t size)
{
    std::shared_ptr<ashoka::redis::Factory> factory(new ashoka::redis::Factory(host, port, db, prefix));
    std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> pool(new ashoka::pool::Pool<ashoka::redis::Connection>(size, factory));
    return pool;
}

std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> ashoka::redis::open(boost::property_tree::ptree *tree)
{
    return ashoka::redis::open(
        tree->get<std::string>("redis.host", "127.0.0.1"),
        tree->get<unsigned short int>("redis.port", 6379),
        tree->get<unsigned short int>("redis.db", 0),
        tree->get<std::string>("redis.prefix"),
        tree->get<size_t>("redis.pool-size", 32));
}
