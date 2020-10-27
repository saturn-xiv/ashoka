

#ifndef ASHOKA_JWT_H_
#define ASHOKA_JWT_H_

#include <string>

#include <Poco/Exception.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <boost/log/trivial.hpp>
#include <jwtpp/jwtpp.hh>

namespace ashoka
{
    class Jwt
    {
    public:
        Jwt(const Poco::Util::LayeredConfiguration &config);
        ~Jwt();

        std::string sum();
        bool verify(const std::string &token);

    private:
    };
} // namespace ashoka

#endif