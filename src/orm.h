#ifndef ASHOKA_ORM_H_
#define ASHOKA_ORM_H_

#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <ostream>
#include <optional>
#include <algorithm>
#include <functional>
#include <mutex>
#include <fstream>
#include <streambuf>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>

#include "env.h"
#include "crypt.h"

namespace ashoka
{
    namespace orm
    {
        class Migration
        {
        public:
            Migration(const std::string version,
                      const std::string name,
                      const std::string up,
                      const std::string down,
                      const std::optional<boost::posix_time::ptime> run_at);
            friend std::ostream &operator<<(std::ostream &out, const Migration &self)
            {
                out << "### " << self.version << " " << self.name;
                if (self.run_at)
                {
                    out << " " << boost::posix_time::to_iso_extended_string(self.run_at.value());
                }
                out << " ###" << std::endl
                    << "--- UP ---" << std::endl
                    << self.up
                    << "--- DOWN ---" << std::endl
                    << self.down;

                return out;
            }
            bool operator<(const Migration &self) const;

            friend class SchemaDao;

            std::string version;
            std::string name;
            std::string up;
            std::string down;
            std::optional<boost::posix_time::ptime> run_at;
        };

        class SchemaDao
        {
        public:
            void generate(const std::string &name) const;
            void load();
            void migrate();
            void rollback();
            friend std::ostream &operator<<(std::ostream &out, const SchemaDao &self)
            {
                const std::string fmt = "%1$-15s %2$-24s ";

                out << (boost::format(fmt) % "VERSION" % "NAME") << "RUN AT" << std::endl;
                for (auto it = self.migrations.begin(); it != self.migrations.end(); it++)
                {
                    out << (boost::format(fmt) % it->version % it->name);
                    if (it->run_at)
                    {
                        out << boost::posix_time::to_iso_extended_string(it->run_at.value());
                    }
                    else
                    {
                        out << "N/A";
                    };
                    out << std::endl;
                }
                return out;
            }

        protected:
            virtual void execute(const std::string &script) const = 0;
            virtual void delete_(const std::string &version) const = 0;
            virtual void insert(const std::string &version) const = 0;
            virtual std::optional<boost::posix_time::ptime> run_at(const std::string &version) const = 0;

            std::vector<Migration> migrations;
            std::mutex locker;
            std_fs::path root;
        };
    } // namespace orm
} // namespace ashoka
#endif