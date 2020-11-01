#include "crypt.h"

void ashoka::crypt::init()
{
    if (sodium_init() < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "can't init sodium";
        throw new std::runtime_error("init libsodium");
    }
}

std::string ashoka::crypt::random_base64_string(const size_t s)
{
    return std::string("");
}