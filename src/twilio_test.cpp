#define BOOST_TEST_MODULE Twilio Test

#include <boost/test/included/unit_test.hpp>

#include "twilio.h"

BOOST_AUTO_TEST_CASE(sms_test)
{
    const std::string from(std::getenv("TWILIO_FROM"));
    const std::string to(std::getenv("TWILIO_TO"));
    const std::string account_sid(std::getenv("TWILIO_ACCOUNT_SID"));
    const std::string auth_token(std::getenv("TWILIO_AUTH_TOKEN"));
    ashoka::twilio::Client client(from, account_sid, auth_token);
    auto response = client.sms(to, "Hi, boost unit test!");
    std::cout << response << std::endl;
}
