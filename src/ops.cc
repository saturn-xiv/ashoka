#include "ops.h"

void ashoka::ops::deploy::Recipe::execute()
{
    BOOST_LOG_TRIVIAL(info) << "run recipe [" << this->name << "]";
    // std::map<std::string, std::string>
    std::map<ashoka::ops::deploy::Client, std::vector<std::string>> tasks;
    // for (auto job : this->jobs)
    // {
    //     for (auto it : job->)
    //     {
    //     }
    // }
}
std::map<std::string, std::vector<std::string>> ashoka::ops::deploy::Recipe::orders()
{
    std::map<std::string, std::vector<std::string>> items;
    return items;
}

void ashoka::ops::deploy::Client::execute(const std::string task, std::map<std::string, std::string>)
{
    this->log(task);
}

void ashoka::ops::deploy::Client::log(const std::string &message)
{
    BOOST_LOG_TRIVIAL(debug) << this << " " << message << std::endl;
    std::time_t now = std::time(nullptr);
    std::filesystem::path tmp("tmp");
    std::filesystem::create_directory("tmp");

    std::ofstream fs;
    fs.open(tmp / this->host, std::ios::out | std::ios::app);
    fs << std::asctime(std::localtime(&now)) << " ";
    if (this->user)
    {
        fs << this->user.value() << " ";
    }
    fs << message << std::endl;
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