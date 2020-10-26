#include "utils.h"

void ashoka::utils::watchdog(int dur)
{
    int fd = open("/dev/watchdog", O_WRONLY);
    // ioctl(fd, WDIOC_SETTIMEOUT, 60);
    // ioctl(fd, WDIOC_SETPRETIMEOUT, 10);
    if (fd == -1)
    {
        BOOST_LOG_TRIVIAL(fatal) << "can't open watchdog";
        return;
    }

    int to, pt;
    ioctl(fd, WDIOC_GETTIMEOUT, &to);
    ioctl(fd, WDIOC_GETPRETIMEOUT, &pt);
    BOOST_LOG_TRIVIAL(info) << "watchdog: (" << to << ',' << pt << ") " << dur;
    while (true)
    {
#ifndef NDEBUG
        int left;
        ioctl(fd, WDIOC_GETTIMELEFT, &left);
        BOOST_LOG_TRIVIAL(debug) << "the timeout was is " << left << "seconds";
#endif
        ioctl(fd, WDIOC_KEEPALIVE, 0);
        sleep(dur);
    }
}