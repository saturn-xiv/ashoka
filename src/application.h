#ifndef ASHOKA_APPLICATION_H_
#define ASHOKA_APPLICATION_H_

#include "common.h"

namespace ashoka
{
    namespace env
    {
        class Http
        {
        public:
            const unsigned short int port;

        private:
            const std::string secrets;
        };
        class Redis
        {
        public:
            const std::string host;
            const unsigned short int port;
            const unsigned short int db;
        };
        class Config
        {
        public:
            const Http http;
            const std::string postgresql;
            const std::string redis;
        };
    } // namespace env
    class Application
    {
    public:
        int run(int argc, char **argv);
    };
} // namespace ashoka

#endif