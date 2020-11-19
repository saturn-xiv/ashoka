#ifndef ASHOKA_SERVER_H_
#define ASHOKA_SERVER_H_

#include "common.h"

namespace ashoka
{
  namespace api
  {
    class Server
    {
    public:
      Server(unsigned short int port);
      ~Server();

      void listen();

    private:
      const unsigned short int port;
    };
  } // namespace api

} // namespace ashoka

#endif