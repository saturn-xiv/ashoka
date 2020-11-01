#ifndef ASHOKA_HARDWARE_H_
#define ASHOKA_HARDWARE_H_

#include <string>

#include <boost/log/trivial.hpp>

// #include "ancient.h"

// #ifdef __cpp_lib_filesystem
#include <filesystem>
// using fs = std::filesystem;
// #elif __cpp_lib_experimental_filesystem
// #include <experimental/filesystem>
// using fs = std::experimental::filesystem;
// #else
// #endif

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
            Queue();
            ~Queue();

        private:
        };
    } // namespace hardware

} // namespace ashoka

#endif