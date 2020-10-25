#ifndef ASHOKA_POSTGRESQL_H_
#define ASHOKA_POSTGRESQL_H_

#include <iostream>
#include <string>

#include <Poco/Exception.h>
#include <Poco/LogStream.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <pqxx/except>
#include <pqxx/pqxx>

namespace ashoka
{
    namespace postgresql
    {

        template <typename T>
        class Callback
        {
        public:
            virtual T execute(pqxx::work *tx) const = 0;
        };

        class Connection
        {
            Connection(const Poco::Util::LayeredConfiguration &config, Poco::LogStream &logger);
            ~Connection();

            void ping();

            template <typename T>
            T call(Callback<T> *cb);

        private:
            pqxx::connection *connection;
        };
    } // namespace postgresql
} // namespace ashoka
#endif
