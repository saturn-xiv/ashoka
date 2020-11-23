#ifndef ASHOKA_CRONTAB_H_
#define ASHOKA_CRONTAB_H_

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <functional>
#include <mutex>
#include <optional>
#include <ostream>
#include <streambuf>
#include <string>
#include <vector>

#include <boost/log/trivial.hpp>

#include "env.h"
#include "postgresql.h"

namespace ashoka
{
    namespace crontab
    {
        class Job
        {
        public:
            Job(const std::string name,
                const std::string user,
                const std::string home,
                const std::string command,
                const std::string run_at);
            friend class Config;
            friend std::ostream &operator<<(std::ostream &out, const Job &self)
            {
                out << self.run_at << "\t" << self.user << "\t"
                    << "cd " << self.home << " && " << self.command;
                return out;
            }

            const std::string name;
            const std::string user;
            const std::string home;
            const std::string command;
            const std::string run_at;
        };
        class Config
        {
        public:
            Config(const std::string name, const std::shared_ptr<pqxx::connection> connection);
            Config(const std::string name, std::vector<Job> jobs);
            std_fs::path filename() const;
            friend std::ostream &operator<<(std::ostream &out, const Config &self)
            {
                out << "# m h dom mon dow user  command" << std::endl;
                for (const auto &it : self.jobs)
                {
                    out << "# " << it.name << std::endl
                        << it << std::endl;
                }
                return out;
            }

        private:
            const std::string name;
            std::vector<Job> jobs;
        };
    } // namespace crontab
} // namespace ashoka

#endif