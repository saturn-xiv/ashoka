#ifndef ASHOKA_POSTGRESQL_H_
#define ASHOKA_POSTGRESQL_H_

#include <libpq-fe.h>

#include "pool.h"

namespace ashoka
{
    namespace postgresql
    {
        class Config : public ashoka::env::Config
        {
        public:
            Config() : host("127.0.0.1"), port(5432), user("postgres"), password(std::nullopt), db("dev"), pool_size(16) {}
            Config(const toml::table &root)
            {
                std::optional<std::string> host = root["host"].value<std::string>();
                this->host = host.value_or("127.0.0.1");
                std::optional<unsigned short> port = root["port"].value<unsigned short>();
                this->port = port.value_or(5432);
                std::optional<std::string> user = root["user"].value<std::string>();
                this->user = user.value_or("postgres");
                std::optional<std::string> password = root["password"].value<std::string>();
                if (password)
                {
                    this->password = std::optional<std::string>{password.value()};
                }
                std::optional<std::string> db = root["db"].value<std::string>();
                if (db)
                {
                    this->db = db.value();
                }
                std::optional<size_t> pool_size = root["pool-size"].value<size_t>();
                this->pool_size = pool_size.value_or(20);
            };

            operator toml::table() const
            {
                toml::table root;
                root.insert("host", this->host);
                root.insert("port", this->port);
                root.insert("user", this->user);
                if (this->password)
                {
                    root.insert("password", this->password.value());
                }
                root.insert("db", this->db);
                root.insert("pool-size", (long)this->pool_size);
                return root;
            };
            std::string name() const
            {
                return "postgresql";
            };

        private:
            std::string host;
            unsigned short int port;
            std::string user;
            std::optional<std::string> password;
            std::string db;
            size_t pool_size;
        };

        // template <typename T>
        // class Callback
        // {
        // public:
        //     virtual T execute(pqxx::work *tx) const = 0;
        // };

        class Connection
        {
            Connection();
            ~Connection();

            //     void ping();

            //     template <typename T>
            //     T call(Callback<T> *cb);

            // private:
            //     pqxx::connection *connection;
        };
    } // namespace postgresql
} // namespace ashoka
#endif
