#define BOOST_TEST_MODULE MQTT Test

#include <boost/test/included/unit_test.hpp>

#include "mqtt.h"

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