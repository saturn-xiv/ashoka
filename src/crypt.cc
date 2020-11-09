#include "crypt.h"

void ashoka::crypt::init()
{
    if (sodium_init() < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "can't init sodium";
        throw new std::runtime_error("init libsodium");
    }
}

std::string ashoka::crypt::uuid()
{
    boost::uuids::uuid uid = boost::uuids::random_generator()();
    std::stringstream ss;
    ss << uid;
    return ss.str();
}

std::string ashoka::crypt::timestamp()
{
    std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::gmtime(&tt);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y%m%d%H%M%S");
    return ss.str();
}

std::string ashoka::crypt::random_base64_string(const size_t s)
{
    return std::string("");
}