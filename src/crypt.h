#ifndef ASHOKA_CRYPT_H_
#define ASHOKA_CRYPT_H_

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/log/trivial.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <sodium.h>

#define ASKOKA_TIMESTAMP_SIZE 14

namespace ashoka
{
    namespace crypt
    {
        void init();
        std::string uuid();
        std::string timestamp();
        std::string random_base64_string(const size_t s);
    } // namespace crypt

} // namespace  ashoka

#endif