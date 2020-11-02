#include "ops.h"

void ashoka::ops::deploy::Recipe::execute()
{
    BOOST_LOG_TRIVIAL(info) << "run recipe [" << this->name << "]";
    // for (auto)
}

void ashoka::ops::deploy::Client::execute(const std::string task, std::map<std::string, std::string>)
{
    this->log(task);
}

void ashoka::ops::deploy::Client::log(const std::string &message)
{
    BOOST_LOG_TRIVIAL(debug) << this << " " << message << std::endl;

    // std::time_t now = std::time(nullptr);
    // std::filesystem::path tmp("tmp");
    // std::filesystem::create_directory("tmp");

    // std::ofstream fs(tmp / this->host);
    // fs << std::asctime(std::localtime(&now)) << " " << this->user << ": " << message << std::endl;
}