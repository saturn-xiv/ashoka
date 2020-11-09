#include "ops.h"
#include "crypt.h"
#include "env.h"

void load_env(toml::node_view<toml::v2::node> table, std::map<std::string, std::string> &map)
{
    if (table.is_table())
    {
        auto items = table.as_table();
        for (auto [k, v] : *items)
        {
            std::optional<std::string> val = v.value<std::string>();
            if (val)
            {
                map.insert(std::pair<std::string, std::string>(k, val.value()));
            }
        }
    }
}

void ashoka::ops::deploy::Recipe::load(toml::table &root)
{
    this->timestamp = ashoka::crypt::timestamp();
    load_env(root["env"], this->env);
    {
        auto items = root["clients"];

        if (items.is_table())
        {
            auto clients = items.as_table();
            for (auto [k, v] : *clients)
            {
                Client it(k, *(v.as_table()));
                this->clients.push_back(it);
            }
        }
    }
    {
        auto items = root["groups"];

        if (items.is_table())
        {
            auto groups = items.as_table();
            for (auto [k, v] : *groups)
            {
                Group it(k, *(v.as_table()));
                this->groups.push_back(it);
            }
        }
    }
}

ashoka::ops::deploy::Recipe::Recipe(std::string name, toml::table &root) : name(name)
{
    this->load(root);
}

ashoka::ops::deploy::Recipe::Recipe(std::string name)
{
    const auto file = name + "/config.toml";
    BOOST_LOG_TRIVIAL(info) << "load recipe from " << file;
    toml::table root = toml::parse_file(file);
    this->name = name;
    this->load(root);
}

void ashoka::ops::deploy::Recipe::execute()
{
    // TODO
}

void ashoka::ops::deploy::execute(
    std::string host,
    std::optional<unsigned short> port,
    std::optional<std::string> user,
    std::optional<std::string> key,
    std::map<std::string, std::string> env)
{
    // TODO
}

ashoka::ops::deploy::Client::Client(std::string name, toml::table &root) : name(name)
{
    std::optional<std::string> host = root["host"].value<std::string>();
    if (host)
    {
        this->host = host.value();
    }
    std::optional<unsigned short> port = root["port"].value<unsigned short>();
    if (port)
    {
        this->port = port.value();
    }
    std::optional<std::string> user = root["user"].value<std::string>();
    if (user)
    {
        this->user = user.value();
    }
    std::optional<std::string> key = root["key"].value<std::string>();
    if (key)
    {
        this->key = key.value();
    }
    load_env(root["env"], this->env);
}
void ashoka::ops::deploy::Client::ping()
{
    // TODO
}

bool ashoka::ops::deploy::Client::is_local()
{
    for (auto it : {"localhost", "127.0.0.1", "::1"})
    {
        if (this->host == it)
        {
            return true;
        }
    }
    return false;
}

void ashoka::ops::deploy::Client::log(const std::string &message)
{
    BOOST_LOG_TRIVIAL(debug) << this << " " << message << std::endl;
    std::time_t now = std::time(nullptr);
    std_fs::path tmp("tmp");
    std_fs::create_directory("tmp");

    std::ofstream fs;
    fs.open(tmp / this->host, std::ios::out | std::ios::app);
    fs << std::asctime(std::localtime(&now)) << " ";
    if (this->user)
    {
        fs << this->user.value() << " ";
    }
    fs << message << std::endl;
}

// void ashoka::ops::deploy::Recipe::execute()
// {
//     BOOST_LOG_TRIVIAL(info) << "run recipe [" << this->name << "]";
//     // std::map<std::string, std::string>
//     std::map<ashoka::ops::deploy::Client, std::vector<std::string>> tasks;
//     // for (auto job : this->jobs)
//     // {
//     //     for (auto it : job->)
//     //     {
//     //     }
//     // }
// }
// std::map<std::string, std::vector<std::string>> ashoka::ops::deploy::Recipe::orders()
// {
//     std::map<std::string, std::vector<std::string>> items;
//     return items;
// }

// void ashoka::ops::deploy::Client::execute(const std::string task, std::map<std::string, std::string>)
// {
//     this->log(task);
// }

ashoka::ops::deploy::Group::Group(std::string name, toml::table &root) : name(name)
{
    {
        auto items = root["hosts"];
        if (items.is_array())
        {
            auto hosts = items.as_array();
            for (toml::node &it : *hosts)
            {
                std::optional<std::string> val = it.value<std::string>();
                if (val)
                {
                    this->hosts.push_back(val.value());
                }
            }
        }
    }
    {
        auto items = root["tasks"];
        if (items.is_array())
        {
            auto tasks = items.as_array();
            for (toml::node &it : *tasks)
            {
                std::optional<std::string> val = it.value<std::string>();
                if (val)
                {
                    this->tasks.push_back(val.value());
                }
            }
        }
    }

    load_env(root["env"], this->env);
}