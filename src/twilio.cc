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

web::json::value ashoka::twilio::Client::get(const web::uri_builder &builder) const
{
    auto client = this->connect();
    web::json::value result;
    client.request(web::http::methods::GET, builder.to_string())
        .then([](const web::http::http_response &response) {
            return response.extract_json();
        })
        .then([&result](const pplx::task<web::json::value> &task) {
            result = task.get();
        })
        .wait();
    return result;
}

web::json::value ashoka::twilio::Client::form(const web::uri_builder &builder, const utf8string &body) const
{
    auto client = this->connect();
    web::json::value result;
    client.request(web::http::methods::POST, builder.to_string(), body, U("application/x-www-form-urlencoded"))
        .then([](const web::http::http_response &response) {
            return response.extract_json();
        })
        .then([&result](const pplx::task<web::json::value> &task) {
            result = task.get();
        })
        .wait();
    return result;
}

web::json::value ashoka::twilio::Client::sms(const std::string &phone, const std::string &message) const
{
    auto client = this->connect();

    web::uri_builder builder(U("/2010-04-01/Accounts/" + this->account_sid + "/Messages.json"));
    auto body = U("To=") + U(phone) + U("&") + U("From=") + U(this->phone) + U("&") + U("Body=") + U(message);

    return this->form(builder, body);
}