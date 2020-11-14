#include "crawler.h"

ashoka::crawler::Crawler::Crawler(const std::shared_ptr<pqxx::connection> connection, const toml::table &root)
    : connection(connection), sources(0)
{
    for (auto &&[k, v] : root)
    {
        std::optional<std::string> val = v.value<std::string>();
        if (val)
        {
            std::pair<std::string, std::string> it(k, val.value());
            this->sources.push_back(it);
        }
    }
}

void ashoka::crawler::Crawler::execute() const
{
    for (auto it : this->sources)
    {
        try
        {
            this->execute(it.first, it.second);
        }
        catch (...)
        {
            BOOST_LOG_TRIVIAL(error) << boost::current_exception_diagnostic_information();
        }
    }
}

void ashoka::crawler::Crawler::execute(const std::string &name) const
{
    for (auto it : this->sources)
    {
        if (it.first == name)
        {
            this->execute(it.first, it.second);
            return;
        }
    }
    BOOST_LOG_TRIVIAL(error) << "can't find crawler nameed by " << name;
}

std::optional<std::pair<std::string, boost::posix_time::ptime>> ashoka::crawler::Crawler::latest(const std::string &name) const
{
    pqxx::work tx(*(this->connection));
    pqxx::result rst = tx.exec_prepared("crawlers_logs.find-by-name", name);
    tx.commit();

    for (auto it = rst.begin(); it != rst.end(); it++)
    {
        const auto body = it["body"].as<std::string>();
        const auto created_at = ASHOKA_STR2TS(it["created_at"]);
        std::pair<std::string, boost::posix_time::ptime> ret(body, created_at);
        return ret;
    }

    return std::nullopt;
}

void ashoka::crawler::Crawler::execute(const std::string &name, const std::string &url) const
{
    BOOST_LOG_TRIVIAL(debug) << "fetch " << name << " from " << url;

    web::http::client::http_client_config cfg;
    cfg.set_validate_certificates(false);

    web::http::client::http_client client(U(url), cfg);
    std::string body;
    client.request(web::http::methods::GET)
        .then([](const web::http::http_response &response) {
            const auto code = response.status_code();
            if (code == web::http::status_codes::OK)
            {
                return response.extract_string();
            }
            std::stringstream ss;
            ss << "HTTP " << code;
            throw std::runtime_error(ss.str());
        })
        .then([&body](const pplx::task<std::string> &task) {
            body = task.get();
        })
        .wait();

    pqxx::work tx(*(this->connection));
    pqxx::result rst = tx.exec_prepared("crawlers_logs.insert", name, url, body);
    tx.commit();
}