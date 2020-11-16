#include "twilio.h"

web::http::client::http_client ashoka::twilio::Config::connect() const
{
  web::http::client::http_client_config cfg;
  web::credentials cred(U(this->account_sid), U(this->auth_token));
  cfg.set_credentials(cred);
  cfg.set_validate_certificates(false);

  web::http::client::http_client client(U("https://api.twilio.com"), cfg);
  return client;
}
ashoka::twilio::Config::Config(std::string from, std::string account_sid,
                               std::string auth_token)
    : from(from), account_sid(account_sid), auth_token(auth_token) {}
ashoka::twilio::Config::Config(const toml::table &root)
{
  std::optional<std::string> form = root["from"].value<std::string>();
  if (form)
  {
    this->from = form.value();
  }
  std::optional<std::string> account_sid =
      root["account-sid"].value<std::string>();
  if (account_sid)
  {
    this->account_sid = account_sid.value();
  }
  std::optional<std::string> auth_token =
      root["auth-token"].value<std::string>();
  if (auth_token)
  {
    this->auth_token = auth_token.value();
  }
}

ashoka::twilio::Config::operator toml::table() const
{
  toml::table root;
  root.insert("account-sid", this->account_sid);
  root.insert("auth-token", this->auth_token);
  root.insert("from", this->from);
  return root;
}
std::string ashoka::twilio::Config::name() const { return "twilio"; }

// ----------------------
ashoka::twilio::Client::Client(const Config config) : config(config) {}

web::json::value
ashoka::twilio::Client::get(const web::uri_builder &builder) const
{
  auto client = this->config.connect();
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

web::json::value ashoka::twilio::Client::form(const web::uri_builder &builder,
                                              const utf8string &body) const
{
  auto client = this->config.connect();
  web::json::value result;
  client
      .request(web::http::methods::POST, builder.to_string(), body,
               U("application/x-www-form-urlencoded"))
      .then([](const web::http::http_response &response) {
        return response.extract_json();
      })
      .then([&result](const pplx::task<web::json::value> &task) {
        result = task.get();
      })
      .wait();
  return result;
}

web::json::value ashoka::twilio::Client::sms(const std::string &to,
                                             const std::string &message) const
{
  web::uri_builder builder(
      U("/2010-04-01/Accounts/" + this->config.account_sid + "/Messages.json"));
  auto body = U("To=") + U(to) + U("&") + U("From=") + U(this->config.from) +
              U("&") + U("Body=") + U(message);

  return this->form(builder, body);
}