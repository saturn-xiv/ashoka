#define BOOST_TEST_MODULE Redis Test

#include <boost/test/included/unit_test.hpp>

#include "redis.h"

BOOST_AUTO_TEST_CASE(redis_ping_test)
{
    // std::shared_ptr<ashoka::pool::Pool<ashoka::redis::Connection>> redis = ashoka::redis::open(&cfg);
    int i = 1;
    BOOST_TEST(i == 1);
}
