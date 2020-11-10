#include "twilio.h"

web::http::client::http_client ashoka::twilio::Client::connect() const
{
    web::http::client::http_client_config cfg;
    web::credentials cred(U(this->account_sid), U(this->auth_token));
    cfg.set_credentials(cred);
    cfg.set_validate_certificates(false);

    web::http::client::http_client client(U("https://api.twilio.com"), cfg);
    return client;
}

web::json::value ashoka::twilio::Client::sms(const std::string &phone, const std::string &message) const
{
    auto client = this->connect();

    web::uri_builder builder(U("/2010-04-01/Accounts/" + this->account_sid + "/Messages.json"));
    builder.append_query(U("To"), U(phone));
    builder.append_query(U("From"), U(this->phone));
    builder.append_query(U("Body"), U(message));

    web::json::value result;
    client.request(web::http::methods::POST, builder.to_string())
        .then([](const web::http::http_response &response) {
            return response.extract_json();
        })
        .then([&result](const pplx::task<web::json::value> &task) {
            result = task.get();
        })
        .wait();
    return result;
}