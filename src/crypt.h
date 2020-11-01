#ifndef ASHOKA_CRYPT_H_
#define ASHOKA_CRYPT_H_

#include <iostream>
#include <stdexcept>

#include <boost/log/trivial.hpp>
#include <sodium.h>

namespace ashoka
{
    namespace crypt
    {
        void init();
        std::string random_base64_string(const size_t s);
    } // namespace crypt

} // namespace  ashoka

#endif