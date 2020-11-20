#include "nut.h"

void ashoka::nut::HomeHandler::execute(ashoka::api::Context &context) const
{
    nlohmann::json buf;
    context.html("/home.html", buf);
}