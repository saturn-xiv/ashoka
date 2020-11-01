#ifndef ASHOKA_COMMON_H_
#define ASHOKA_COMMON_H_

#include <cassert>
#include <cstdlib>
#include <deque>
#include <exception>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <linux/watchdog.h>
#include <mutex>
#include <regex>
#include <set>
#include <signal.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/make_shared.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "config.h"
// #if BOOST_ARCH_X86_64
// #define BOOST_LOG_DYN_LINK 1

#define APPLICATION_JSON_UTF8 "application/json; charset=UTF-8"
#define TEXT_HTML_UTF8 "text/html; charset=UTF-8"

// #ifdef __cpp_lib_filesystem
// #include <filesystem>
// using fs = std::filesystem;
// #elif __cpp_lib_experimental_filesystem
// #include <experimental/filesystem>
// using fs = std::experimental::filesystem;
// #else
// #endif

#endif