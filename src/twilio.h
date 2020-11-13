#ifndef ASHOKA_TWILIO_H_
#define ASHOKA_TWILIO_H_

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>

#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

#include "env.h"

namespace ashoka
{
    namespace twilio
    {

        class Config : public ashoka::env::Config
        {
        public:
            Config(std::string from, std::string account_sid, std::string auth_token);
            Config(const toml::table &root);
            web::http::client::http_client connect() const;

            operator toml::table() const override;
            std::string name() const override;

            friend class Client;

        private:
            std::string account_sid;
            std::string auth_token;
            std::string from;
        };

        class Client
        {
        public:
            Client(const Config config);
            web::json::value sms(const std::string &to, const std::string &message) const;

        private:
            web::json::value get(const web::uri_builder &builder) const;
            web::json::value form(const web::uri_builder &builder, const utf8string &body) const;

            const Config config;
        };
    } // namespace twilio
} // namespace ashoka

#endif