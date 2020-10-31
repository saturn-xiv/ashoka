#include "hardware.h"

ashoka::hardware ::Queue::Queue()
{
    BOOST_LOG_TRIVIAL(info) << "open serial port";
    BOOST_LOG_TRIVIAL(info) << "open audio device";
}

ashoka::hardware::Queue::~Queue()
{
    BOOST_LOG_TRIVIAL(info) << "close serial port";
    BOOST_LOG_TRIVIAL(info) << "close audio device";
}