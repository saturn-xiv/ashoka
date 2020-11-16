#ifndef ASHOKA_SERVER_H_
#define ASHOKA_SERVER_H_

#include "common.h"

namespace ashoka
{
  class HttpServer
  {
  public:
    HttpServer(unsigned short int port);
    ~HttpServer();

    void listen();

  private:
    const unsigned short int port;
  };
} // namespace ashoka

#endif