#ifndef ASHOKA_ENV_H_
#define ASHOKA_ENV_H_

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace std_fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace std_fs = std::experimental::filesystem;
#endif

#define TOML_EXCEPTIONS 1
#include <toml.hpp>

namespace ashoka
{
    namespace env
    {
        class Config
        {
        public:
            virtual operator toml::table() const = 0;
            virtual std::string name() const = 0;
        };
    } // namespace env
} // namespace ashoka

#endif