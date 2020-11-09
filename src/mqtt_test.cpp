#define BOOST_TEST_MODULE My Test

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(mqtt_pub_test)
{
    int i = 1;
    BOOST_TEST(i == 1);
}

BOOST_AUTO_TEST_CASE(mqtt_sub_test)
{
    int i = 2;
    BOOST_TEST(i == 2);
}