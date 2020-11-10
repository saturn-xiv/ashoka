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
            Config(std::string from, std::string account_sid, std::string auth_token) : from(from), account_sid(account_sid), auth_token(auth_token) {}
            Config(const toml::table &root)
            {
                std::optional<std::string> form = root["from"].value<std::string>();
                if (form)
                {
                    this->from = form.value();
                }
                std::optional<std::string> account_sid = root["account-sid"].value<std::string>();
                if (account_sid)
                {
                    this->account_sid = account_sid.value();
                }
                std::optional<std::string> auth_token = root["auth-token"].value<std::string>();
                if (auth_token)
                {
                    this->auth_token = auth_token.value();
                }
            }
            web::http::client::http_client connect() const;
            friend class Client;

            operator toml::table() const
            {
                toml::table root;
                root.insert("account-sid", this->account_sid);
                root.insert("auth-token", this->auth_token);
                root.insert("from", this->from);
                return root;
            };
            std::string name() const
            {
                return "twilio";
            };

        private:
            std::string account_sid;
            std::string auth_token;
            std::string from;
        };

        class Client
        {
        public:
            Client(const Config config) : config(config) {}
            web::json::value sms(const std::string &to, const std::string &message) const;

        private:
            web::json::value get(const web::uri_builder &builder) const;
            web::json::value form(const web::uri_builder &builder, const utf8string &body) const;

            const Config config;
        };
    } // namespace twilio
} // namespace ashoka

#endif