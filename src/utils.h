#ifndef ASHOKA_UTILS_H_
#define ASHOKA_UTILS_H_

#include <fcntl.h>
#include <linux/watchdog.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <boost/log/trivial.hpp>

namespace ashoka
{

    namespace utils
    {
        void watchdog(int dur);

    } // namespace utils

} // namespace ashoka
#endif