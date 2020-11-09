#define BOOST_TEST_MODULE Twilio Test

#include <boost/test/included/unit_test.hpp>

#include "twilio.h"

BOOST_AUTO_TEST_CASE(receive_test)
{
    int i = 1;
    BOOST_TEST(i == 1);
}

BOOST_AUTO_TEST_CASE(send_test)
{
    int i = 1;
    BOOST_TEST(i == 1);
}
