#ifndef ASHOKA_DEPLOYMENT_H_
#define ASHOKA_DEPLOYMENT_H_

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include <boost/log/trivial.hpp>
#include <inja.hpp>
#include <nlohmann/json.hpp>

namespace ashoka
{
    namespace ops
    {
        namespace deploy
        {
            class Client
            {
            public:
                Client(){};
                ~Client() {}
                friend void to_json(nlohmann::json &j, const Client &p)
                {
                    j = nlohmann::json{{"host", p.host}};
                    if (p.port)
                    {
                        j["port"] = p.port.value();
                    }
                    if (p.user)
                    {
                        j["user"] = p.user.value();
                    }
                    if (p.password)
                    {
                        j["password"] = p.password.value();
                    }
                    if (p.key)
                    {
                        j["key"] = p.key.value();
                    }
                }
                friend void from_json(const nlohmann::json &j, Client &p)
                {
                    j.at("host").get_to(p.host);
                    if (j.contains("port"))
                    {
                        p.port = j.at("port").get<unsigned short int>();
                    }
                    if (j.contains("user"))
                    {
                        p.user = j.at("user").get<std::string>();
                    }
                    if (j.contains("password"))
                    {
                        p.password = j.at("password").get<std::string>();
                    }
                    if (j.contains("key"))
                    {
                        p.key = j.at("key").get<std::string>();
                    }
                }

                void execute(const std::string task, std::map<std::string, std::string> env);
                friend std::ostream &operator<<(std::ostream &out, const Client &self)
                {
                    if (self.user)
                    {

                        out << self.user.value() << '@';
                    }
                    out << self.host;
                    if (self.port)
                    {

                        out << ':' << self.port.value();
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

                std::string host;
                std::optional<unsigned int> port;
                std::optional<std::string> user;
                std::optional<std::string> password;
                std::optional<std::string> key;
            };

            class Job
            {
            public:
                Job() {}
                ~Job() {}

                friend void to_json(nlohmann::json &j, const Job &p)
                {
                    j = nlohmann::json{{"name", p.name}, {"hosts", p.hosts}, {"tasks", p.tasks}};
                }
                friend void from_json(const nlohmann::json &j, Job &p)
                {
                    p.name = j.at("name").get<std::string>();
                    p.hosts = j.at("hosts").get<std::vector<std::string>>();
                    p.tasks = j.at("tasks").get<std::vector<std::string>>();
                }

                friend std::ostream &operator<<(std::ostream &out, const Job &self)
                {
                    out << "Name: " << self.name << std::endl
                        << "Hosts: ";
                    for (auto it : self.hosts)
                    {
                        out << it << ' ';
                    }
                    out << std::endl
                        << "Tasks: ";
                    for (auto it : self.tasks)
                    {
                        out << it << ' ';
                    }
                    return out;
                }

            private:
                std::string name;
                std::vector<std::string> hosts;
                std::vector<std::string> tasks;
            };

            class Recipe
            {
            public:
                Recipe() {}
                ~Recipe() {}
                void execute();

                friend void to_json(nlohmann::json &j, const Recipe &p)
                {

                    j = nlohmann::json{
                        {"name", p.name},
                        {"clients", p.clients},
                        {"jobs", p.jobs},
                        {"env", p.env}};
                }
                friend void from_json(const nlohmann::json &j, Recipe &p)
                {
                    p.name = j.at("name").get<std::string>();
                    p.clients = j.at("clients").get<std::vector<Client>>();
                    p.jobs = j.at("jobs").get<std::vector<Job>>();
                    p.env = j.at("env").get<std::map<std::string, std::string>>();
                }

                friend std::ostream &operator<<(std::ostream &out, const Recipe &self)
                {
                    out << "Name: " << self.name << std::endl
                        << "===\tCLIENTS\t===" << std::endl;
                    for (auto it : self.clients)
                    {
                        out << it << std::endl;
                    }
                    out << "===\tJOBS\t===" << std::endl;
                    for (auto it : self.jobs)
                    {
                        out << it << std::endl;
                    }
                    out << "===\tENV\t===" << std::endl;
                    for (auto const &it : self.env)
                    {
                        out << it.first << '=' << it.second << std::endl;
                    }
                    return out;
                }

            private:
                std::string name;
                std::vector<Client> clients;
                std::vector<Job> jobs;
                std::map<std::string, std::string> env;
            };

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