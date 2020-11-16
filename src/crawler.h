#ifndef ASHOKA_CRAWLER_H_
#define ASHOKA_CRAWLER_H_

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/log/trivial.hpp>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <pqxx/pqxx>

#include "env.h"

namespace ashoka {
namespace crawler {

class Crawler {
public:
  Crawler(const std::shared_ptr<pqxx::connection> connection,
          const toml::table &root);
  void execute() const;
  void execute(const std::string &name) const;
  std::optional<std::pair<std::string, boost::posix_time::ptime>>
  latest(const std::string &name) const;

private:
  void execute(const std::string &name, const std::string &url) const;

  const std::shared_ptr<pqxx::connection> connection;
  std::vector<std::pair<std::string, std::string>> sources;
};

} // namespace crawler
} // namespace ashoka
#endif
