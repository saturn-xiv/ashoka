#define BOOST_TEST_MODULE TTY Test

#include <boost/test/included/unit_test.hpp>

#include "tty.h"
#include "utils.h"

// https://gist.github.com/yoggy/3323808
class SerialPort : public ashoka::tty::SerialPort
{
protected:
    void on_receive(const std::string &response)
    {
        std::cout << "$\t" << response;
    }
    std::optional<std::string> match(const std::string &buf)
    {
        if (boost::algorithm::ends_with(buf, "]"))
        {
            return {buf};
        }
        return std::nullopt;
    }
};

BOOST_AUTO_TEST_CASE(tty_rs232_test)
{
    ashoka::utils::init_logging(false, true);
    SerialPort tty;
    tty.open("/dev/ttyUSB0");
    tty.write("aaa\r\n");
    tty.write("bbb\r\n");
    std::this_thread::sleep_for(std::chrono::minutes(2));
    tty.close();
}
