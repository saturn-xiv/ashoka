#ifndef ASHOKA_TWILIO_H_
#define ASHOKA_TWILIO_H_

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>

#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

namespace ashoka
{
    namespace twilio
    {
        class Client
        {
        public:
            Client(const std::string phone, const std::string account_sid, const std::string auth_token) : phone(phone), account_sid(account_sid), auth_token(auth_token) {}
            web::json::value sms(const std::string &phone, const std::string &message) const;

        private:
            web::http::client::http_client connect() const;
            web::json::value get(const web::uri_builder &builder) const;
            web::json::value form(const web::uri_builder &builder, const utf8string &body) const;

            const std::string phone;
            const std::string account_sid;
            const std::string auth_token;
        };
    } // namespace twilio
} // namespace ashoka

#endif