#ifndef ASHOKA_ANCIENT_H_
#define ASHOKA_ANCIENT_H_

#ifdef __cpp_lib_filesystem
#include <filesystem>
using fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem
#include <experimental/filesystem>
using fs = std::experimental::filesystem;
#else
#endif

#endif