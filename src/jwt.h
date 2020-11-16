

#ifndef ASHOKA_JWT_H_
#define ASHOKA_JWT_H_

#include <chrono>
#include <string>

#include <boost/log/trivial.hpp>
#include <jwtpp/jwtpp.hh>

namespace ashoka
{
  class Jwt
  {
  public:
    Jwt();
    ~Jwt();

    std::string sum();
    bool verify(const std::string &token);

  private:
  };
} // namespace ashoka

#endif