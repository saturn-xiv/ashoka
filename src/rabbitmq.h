#ifndef ASHOKA_RABBITMQ_H_
#define ASHOKA_RABBITMQ_H_

#include <amqp.h>
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>

#include "pool.h"

namespace ashoka
{
    // https://github.com/alanxz/rabbitmq-c/tree/master/examples
    namespace rabbitmq
    {
        class Config : public ashoka::env::Config
        {
        public:
            Config() : host("127.0.0.1"), port(5672), user("guest"), password("guest"), virtual_host("dev"), pool_size(16) {}
            Config(const toml::table &root)
            {
                std::optional<std::string> host = root["host"].value<std::string>();
                this->host = host.value_or("127.0.0.1");
                std::optional<unsigned short> port = root["port"].value<unsigned short>();
                this->port = port.value_or(5672);
                std::optional<std::string> user = root["user"].value<std::string>();
                this->user = user.value_or("guest");
                std::optional<std::string> password = root["password"].value<std::string>();
                this->password = password.value_or("guest");
                std::optional<std::string> virtual_host = root["virtual-host"].value<std::string>();
                if (virtual_host)
                {
                    this->virtual_host = virtual_host.value();
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
                root.insert("password", this->password);
                root.insert("virtual-host", this->virtual_host);
                root.insert("pool-size", (long)this->pool_size);
                return root;
            };
            std::string name() const
            {
                return "rabbitmq";
            };

        private:
            std::string host;
            unsigned short int port;
            std::string user;
            std::string password;
            std::string virtual_host;
            size_t pool_size;
        };
        class Connection
        {
        };
    } // namespace rabbitmq
} // namespace ashoka
#endif
