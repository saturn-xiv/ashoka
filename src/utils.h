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

namespace ashoka
{

    namespace utils
    {
        void init_logging(bool daemon, bool debug);
        void watchdog(int dur);

    } // namespace utils

} // namespace ashoka
#endif