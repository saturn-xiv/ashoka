#ifndef ASHOKA_DEPLOYMENT_H_
#define ASHOKA_DEPLOYMENT_H_

#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include <boost/log/trivial.hpp>
#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

#define TOML_EXCEPTIONS 1
#include <toml.hpp>

namespace ashoka
{
    namespace ops
    {
        namespace deploy
        {
            class Client
            {
            public:
                Client(std::string name, toml::table &table);
                ~Client() {}
                bool is_local();
                void ping();

                friend std::ostream &operator<<(std::ostream &out, const Client &self)
                {
                    out << "###### CLIENT " << self.name << " ######" << std::endl;
                    if (self.user)
                    {

                        out << self.user.value() << '@';
                    }
                    out << self.host;
                    if (self.port)
                    {

                        out << ':' << self.port.value();
                    }

                    out << std::endl
                        << "#-----\tENV\t-----#" << std::endl;
                    for (auto const &it : self.env)
                    {
                        out << it.first << " = " << it.second << std::endl;
                    }
                    return out;
                }
                bool operator==(const Client &self) const
                {
                    return this->host == self.host && this->port == self.port && this->user == self.user;
                }
                bool operator==(const std::string &self) const
                {
                    std::stringstream buf;
                    buf << this;
                    return buf.str() == self;
                }

            private:
                void log(const std::string &message);

                std::string name;
                std::string host;
                std::optional<unsigned short> port;
                std::optional<std::string> user;
                std::optional<std::string> key;
                std::map<std::string, std::string> env;
            };

            class Group
            {
            public:
                Group(std::string name, toml::table &table);
                ~Group() {}

                friend std::ostream &operator<<(std::ostream &out, const Group &self)
                {
                    out << "$$$$$$ GROUP " << self.name << " $$$$$$" << std::endl;
                    out << "$-----\tTASKS\t-----$" << std::endl;
                    for (auto const &it : self.tasks)
                    {
                        out << it << std::endl;
                    }
                    out << "$-----\tHOSTS\t-----$" << std::endl;
                    for (auto const &it : self.hosts)
                    {
                        out << it << std::endl;
                    }
                    out << "$-----\tENV\t-----$" << std::endl;
                    for (auto const &it : self.env)
                    {
                        out << it.first << " = " << it.second << std::endl;
                    }
                    return out;
                }

            private:
                std::string name;
                std::vector<std::string> hosts;
                std::vector<std::string> tasks;
                std::map<std::string, std::string> env;
            };

            class Recipe
            {
            public:
                Recipe(std::string file);
                Recipe(std::string name, toml::table &root);
                ~Recipe() {}
                void execute();

                friend std::ostream &operator<<(std::ostream &out, const Recipe &self)
                {
                    out << "====== RECIPE " << self.name << " ======" << std::endl;
                    out << "timestamp: " << self.timestamp << std::endl;
                    out << "=-----\tGROUPS\t-----=" << std::endl;
                    for (auto const &it : self.groups)
                    {
                        out << it << std::endl;
                    }
                    out << "=-----\tCLIENTS\t-----=" << std::endl;
                    for (auto const &it : self.clients)
                    {
                        out << it << std::endl;
                    }
                    out << "=-----\tENV\t-----=" << std::endl;
                    for (auto const &it : self.env)
                    {
                        out << it.first << " = " << it.second << std::endl;
                    }
                    return out;
                }

            private:
                void load(toml::table &root);

                std::string timestamp;
                std::string name;
                std::vector<Group> groups;
                std::vector<Client> clients;
                std::map<std::string, std::string> env;
            };

            void execute(
                std::string host,
                std::optional<unsigned short> port,
                std::optional<std::string> user,
                std::optional<std::string> key,
                std::map<std::string, std::string> env);

        } // namespace deploy

        namespace email
        {
            class Postfix
            {
            };
            class Dovecot
            {
            };
        } // namespace email
        namespace vpn
        {
            class OpenVPN
            {
            };
        } // namespace vpn
    }     // namespace ops
} // namespace ashoka
#endif