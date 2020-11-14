#ifndef ASHOKA_CRAWLER_H_
#define ASHOKA_CRAWLER_H_

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>

#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

namespace askhoka
{
    namespace crawler
    {

        class Connection
        {
        public:
            virtual ~Connection() = 0;
            virtual std::string name() const = 0;
            virtual std::string url() const = 0;
        };

        class Factory
        {
        public:
            virtual std::string name() const = 0;
            virtual std::string url() const = 0;
        };
    } // namespace crawler
} // namespace askhoka
#endif
