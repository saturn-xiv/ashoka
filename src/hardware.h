#ifndef ASHOKA_HARDWARE_H_
#define ASHOKA_HARDWARE_H_

#include <string>

#include <boost/log/trivial.hpp>

#include "ancient.h"

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
            void play(const fs::path &file);
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
            Queue();
            ~Queue();

        private:
        };
    } // namespace hardware

} // namespace ashoka

#endif