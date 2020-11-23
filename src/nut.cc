#include "nut.h"

void ashoka::nut::HomeHandler::execute(ashoka::api::Context &context, const std::smatch &match) const
{
    nlohmann::json buf;
    context.html("/home.html", buf);
}

void ashoka::nut::RobotsTxtHandler::execute(ashoka::api::Context &context, const std::smatch &match) const
{
    nlohmann::json buf;
    context.html("/robots.txt", buf);
}

// --------------------
void ashoka::nut::Plugin::mount(ashoka::api::Router &router) const
{
    router.append(ashoka::api::Method::GET, "^/$", std::make_shared<ashoka::nut::HomeHandler>(ashoka::nut::HomeHandler()));
    router.append(ashoka::api::Method::GET, "^/robots\\.txt$", std::make_shared<ashoka::nut::RobotsTxtHandler>(ashoka::nut::RobotsTxtHandler()));
}