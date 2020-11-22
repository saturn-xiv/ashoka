#include "crontab.h"

ashoka::crontab::Job::Job(const std::string name,
                          const std::string user,
                          const std::string home,
                          const std::string command,
                          const std::string run_at)
    : name(name), user(user), home(home), command(command), run_at(run_at)
{
}

ashoka::crontab::Config::Config(const std::string name, std::vector<ashoka::crontab::Job> jobs)
    : name(name), jobs(jobs)
{
}
ashoka::crontab::Config::Config(const std::string name, const std::shared_ptr<pqxx::connection> connection)
    : name(name), jobs()
{
    pqxx::work tx(*connection);
    pqxx::result rst = tx.exec_prepared("cron_jobs.find.all");
    tx.commit();

    for (auto it = rst.begin(); it != rst.end(); it++)
    {

        Job job(it["name"].as<std::string>(),
                it["user"].as<std::string>(),
                it["home"].as<std::string>(),
                it["command"].as<std::string>(),
                it["run_at"].as<std::string>());
        this->jobs.push_back(job);
    }
}
std_fs::path ashoka::crontab::Config::filename() const
{
    return std_fs::path("/etc/cron.d") / this->name;
}