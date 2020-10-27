#ifndef ASHOKA_HARDWARE_H_
#define ASHOKA_HARDWARE_H_

#include <filesystem>
#include <string>

#include <Poco/Exception.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <boost/log/trivial.hpp>

#include "hardware.pb.h"

namespace ashoka
{
    namespace hardware
    {
        class Tty
        {
        public:
            void send(const std::string &buf);
            void receive(const std::string &buf);
        };

        class Audio
        {
        public:
            void play(const std::filesystem::path &file);
            void stop();
        };

        class Tts
        {
        public:
            void send(const std::string &buf);
        };

        class Video
        {
        };

        class Queue
        {
        public:
            Queue(const Poco::Util::LayeredConfiguration &config);
            ~Queue();

        private:
        };
    } // namespace hardware

} // namespace ashoka

#endif