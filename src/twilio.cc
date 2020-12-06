#include "twilio.h"

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

ashoka::twilio::SmsResponse::SmsResponse(const Poco::JSON::Object::Ptr root)
{

    {
        auto it = root->get("account_sid");
        if (!it.isEmpty())
        {
            account_sid = it.toString();
        }
    }
    {
        auto it = root->get("api_version");
        if (!it.isEmpty())
        {
            api_version = it.toString();
        }
    }
    {
        auto it = root->get("body");
        if (!it.isEmpty())
        {
            body = it.toString();
        }
    }
    {
        auto it = root->get("date_created");
        if (!it.isEmpty())
        {
            date_created = it.toString();
        }
    }
    {
        auto it = root->get("date_sent");
        if (!it.isEmpty())
        {
            date_sent = it.toString();
        }
    }
    {
        auto it = root->get("date_updated");
        if (!it.isEmpty())
        {
            date_updated = it.toString();
        }
    }
    {
        auto it = root->get("direction");
        if (!it.isEmpty())
        {
            direction = it.toString();
        }
    }
    {
        auto it = root->get("error_code");
        if (!it.isEmpty())
        {
            error_code = Poco::NumberParser::parse(it.toString());
        }
    }
    {
        auto it = root->get("error_message");
        if (!it.isEmpty())
        {
            error_message = it.toString();
        }
    }
    {
        auto it = root->get("from");
        if (!it.isEmpty())
        {
            from = it.toString();
        }
    }
    {
        auto it = root->get("messaging_service_sid");
        if (!it.isEmpty())
        {
            messaging_service_sid = it.toString();
        }
    }
    {
        auto it = root->get("num_media");
        if (!it.isEmpty())
        {
            num_media = it.toString();
        }
    }
    {
        auto it = root->get("num_segments");
        if (!it.isEmpty())
        {
            num_segments = it.toString();
        }
    }
    {
        auto it = root->get("price");
        if (!it.isEmpty())
        {
            price = it.toString();
        }
    }
    {
        auto it = root->get("price_unit");
        if (!it.isEmpty())
        {
            price_unit = it.toString();
        }
    }
    {
        auto it = root->get("sid");
        if (!it.isEmpty())
        {
            sid = it.toString();
        }
    }
    {
        auto it = root->get("status");
        if (!it.isEmpty())
        {
            status = it.toString();
        }
    }

    {
        auto item = root->getObject("subresource_uris");
        for (auto it = item->begin(); it != item->end(); it++)
        {
            subresource_uris.insert(std::make_pair(it->first, it->second.toString()));
        }
        // if (!it.isEmpty())
        // {
        //     status = it.toString();
        // }
    }
    {
        auto it = root->get("to");
        if (!it.isEmpty())
        {
            to = it.toString();
        }
    }
    {
        auto it = root->get("uri");
        if (!it.isEmpty())
        {
            uri = it.toString();
        }
    }
}

ashoka::twilio::SmsResponse ashoka::twilio::Client::sms(const std::string &to, const std::string &body) const
{

    std::stringstream ss;
    ss << "https://api.twilio.com/2010-04-01/Accounts/" << this->config.account_sid << "/Messages.json";
    Poco::URI uri(ss.str());
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());

    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
    {
        Poco::Net::HTTPBasicCredentials cred(this->config.account_sid, this->config.auth_token);
        cred.authenticate(request);
    }
    request.setContentType("application/x-www-form-urlencoded");

    {
        Poco::Net::HTMLForm form;
        form.set("Body", body);
        form.set("From", this->config.from);
        form.set("To", to);

        form.prepareSubmit(request);
        std::ostream &os = session.sendRequest(request);
        form.write(os);
    }

    Poco::Net::HTTPResponse response;
    std::istream &is = session.receiveResponse(response);

    Poco::JSON::Parser parser;
    Poco::JSON::Object::Ptr result = parser.parse(is).extract<Poco::JSON::Object::Ptr>();

    {
        std::stringstream ss;
        result->stringify(ss, 2);
        if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_CREATED)
        {
            BOOST_LOG_TRIVIAL(debug) << ss.str();
        }
        else
        {
            BOOST_LOG_TRIVIAL(error) << response.getStatus() << "\n"
                                     << ss.str();
            throw std::invalid_argument("send sms message by twilio");
        }
    }

    auto it = ashoka::twilio::SmsResponse(result);
    return it;
}