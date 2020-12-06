#ifndef ASHOKA_COMMON_H_
#define ASHOKA_COMMON_H_

#include <cassert>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <regex>
#include <set>
#include <signal.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/log/trivial.hpp>
#include <boost/make_shared.hpp>
#include <boost/program_options.hpp>

#include "config.h"

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace std_fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace std_fs = std::experimental::filesystem;
#endif

#define TOML_EXCEPTIONS 1
#include <toml.hpp>

#define ASHOKA_STR2TS(x) \
    boost::posix_time::time_from_string(x.as<std::string>())
#define ASHOKA_APPLICATION_JSON_UTF8 "application/json; charset=UTF-8"
#define ASHOKA_TEXT_HTML_UTF8 "text/html; charset=UTF-8"

namespace ashoka
{
} // namespace ashoka

#endif