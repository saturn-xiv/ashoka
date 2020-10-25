#include "postgresql.h"

ashoka::postgresql::Connection::Connection(const Poco::Util::LayeredConfiguration &config, Poco::LogStream &logger)
{
    std::string url(config.getString("postgresql.url"));
    // connection = new pqxx::connection(url);
}

ashoka::postgresql::Connection::~Connection()
{
    // delete connection;
}

// template <typename T>
// T ashoka::postgresql::Connection::call(ashoka::postgresql::Callback<T> *cb)
// {
//     pqxx::work tx{*connection};
//     T it = cb->execute(&tx);
//     tx.commit();
//     return it;
// }

// class Heartbeat : public ashoka::postgresql::Callback<std::string>
// {
// public:
//     std::string execute(pqxx::work *tx) const
//     {
//         std::string now = tx->query_value<std::string>("SELECT CURRENT_TIMESTAMP()");

//         return now;
//     }
// };

// void ashoka::postgresql::Connection::ping()
// {
//     call(new Heartbeat);
// }