#ifndef ASHOKA_TWILIO_H_
#define ASHOKA_TWILIO_H_

#include "common.h"

namespace ashoka
{
    namespace twilio
    {

        class SmsResponse
        {
        public:
            SmsResponse(const Poco::JSON::Object::Ptr root);
            friend std::ostream &operator<<(std::ostream &out, const SmsResponse &self)
            {
                out << "=== SMS Client Response ===" << std::endl;
                out << "account sid: " << self.account_sid << std::endl;
                out << "api version: " << self.api_version << std::endl;
                out << "body: " << self.body << std::endl;
                out << "date created: " << self.date_created << std::endl;
                out << "date sent: " << self.date_sent << std::endl;
                out << "date updated: " << self.date_updated << std::endl;
                out << "direction: " << self.direction << std::endl;
                out << "error code: " << self.error_code << std::endl;
                out << "error message: " << self.error_message << std::endl;
                out << "from: " << self.from << std::endl;
                out << "messaging service sid: " << self.messaging_service_sid << std::endl;
                out << "num media: " << self.num_media << std::endl;
                out << "num segments: " << self.num_segments << std::endl;
                out << "price: " << self.price << std::endl;
                out << "price unit: " << self.price_unit << std::endl;
                out << "sid: " << self.sid << std::endl;
                out << "status: " << self.status;

                for (auto const &[k, v] : self.subresource_uris)
                {
                    out << "subresources uris(" << k << "): " << v << std::endl;
                }

                out << ": " << self.to << std::endl;
                out << ": " << self.uri << std::endl;
                return out;
            }
            std::string account_sid;
            std::string api_version;
            std::string body;
            std::string date_created;
            std::string date_sent;
            std::string date_updated;
            std::string direction;
            int error_code;
            std::string error_message;
            std::string from;
            std::string messaging_service_sid;
            std::string num_media;
            std::string num_segments;
            std::string price;
            std::string price_unit;
            std::string sid;
            std::string status;
            std::map<std::string, std::string> subresource_uris;
            std::string to;
            std::string uri;
        };

        class Config : public ashoka::env::Config
        {
        public:
            Config(std::string from, std::string account_sid, std::string auth_token);
            Config(const toml::table &root);

            operator toml::table() const override;

            friend class Client;

        private:
            std::string account_sid;
            std::string auth_token;
            std::string from;
        };

        class Client
        {
        public:
            Client(const Config config) : config(config) {}
            ashoka::twilio::SmsResponse sms(const std::string &to, const std::string &body) const;

        private:
            const Config config;
        };

    } // namespace twilio
} // namespace ashoka
#endif