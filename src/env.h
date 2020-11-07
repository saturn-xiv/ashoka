#ifndef ASHOKA_ENV_H_
#define ASHOKA_ENV_H_

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace std_fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace std_fs = std::experimental::filesystem;
#endif

#endif