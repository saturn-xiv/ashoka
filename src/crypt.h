#ifndef ASHOKA_CRYPT_H_
#define ASHOKA_CRYPT_H_

#include <iostream>
#include <sodium.h>

namespace ashoka
{
    namespace crypt
    {
        std::string random_base64_string(const size_t s);
    }

} // namespace  ashoka

#endif