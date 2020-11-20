#ifndef ASHOKA_NUT_H_
#define ASHOKA_NUT_H_

#include "common.h"
#include "router.h"

namespace ashoka
{
    namespace nut
    {

        class HomeHandler : public ashoka::api::Handler
        {
        public:
            void execute(ashoka::api::Context &context) const override;
        };

        class RobotsTxtHandler : public ashoka::api::Handler
        {
        public:
            void execute(ashoka::api::Context &context) const override;
        };

        class Plugin : public ashoka::api::Plugin
        {
        public:
            void mount(ashoka::api::Router &router) const override;
        };

    } // namespace nut
} // namespace ashoka

#endif