#ifndef ASHOKA_UTILS_H_
#define ASHOKA_UTILS_H_

#include "common.h"

namespace ashoka
{

    namespace utils
    {
        void init_logging(bool daemon, bool debug);
        void watchdog(int dur);

    } // namespace utils

} // namespace ashoka
#endif