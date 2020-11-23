#ifndef ASHOKA_UTILS_H_
#define ASHOKA_UTILS_H_

#include <fcntl.h>
#include <linux/watchdog.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/local_time/local_time.hpp>

namespace ashoka
{

    namespace utils
    {
        void init_logging(bool daemon, bool debug);
        void watchdog(int dur);

        boost::posix_time::ptime str2time(const std::string &time, const std::string &format);
        boost::local_time::local_date_time str2time(const std::string &time, const std::string &format, const std::string &timezone);
        unsigned long str2ul(const std::string &s);

    } // namespace utils

} // namespace ashoka
#endif