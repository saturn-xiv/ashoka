#ifndef ASHOKA_SERVER_H_
#define ASHOKA_SERVER_H_

#include "common.h"

namespace ashoka
{
    class Server
    {
    public:
        Server(unsigned short int port) : port(port) {}
        ~Server() {}

        void listen();

    private:
        const unsigned short int port;
    };
} // namespace ashoka

#endif