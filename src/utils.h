#ifndef ASHOKA_UTILS_H_
#define ASHOKA_UTILS_H_

#include <exception>
#include <fcntl.h>
#include <iostream>
#include <linux/watchdog.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

#include <boost/date_time/posix_time/posix_time.hpp>
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
#include <boost/smart_ptr/shared_ptr.hpp>

namespace ashoka
{

    namespace utils
    {
        void init_logging(bool daemon, bool debug);
        void watchdog(int dur);

    } // namespace utils

} // namespace ashoka
#endif