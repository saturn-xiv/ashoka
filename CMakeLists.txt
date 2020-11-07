cmake_minimum_required(VERSION 3.16)

# include(ExternalProject)
# include(FetchContent)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED Crypto SSL)
find_package(Boost REQUIRED log_setup log thread system filesystem regex timer date_time chrono random  program_options)

aux_source_directory(src DIR_SRCS)
execute_process(COMMAND git describe --tags --always --dirty
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET
)
string(STRIP "${GIT_REV}" GIT_REV)
configure_file(src/config.h.in config.h)
add_executable(ashoka ${DIR_SRCS})

set_target_properties(ashoka PROPERTIES LINK_FLAGS_RELEASE -s)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

if(CMAKE_CROSSCOMPILING)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    SET(CMAKE_EXE_LINKER_FLAGS "-static")
    SET_TARGET_PROPERTIES(ashoka PROPERTIES LINK_SEARCH_END_STATIC 1)
    target_link_libraries(ashoka PRIVATE
        stdc++fs ${CONAN_LIBS}
    )
else(CMAKE_CROSSCOMPILING)
    target_link_libraries(ashoka PRIVATE
        # ${CMAKE_THREAD_LIBS_INIT}
        ${CONAN_LIBS}
    )
endif(CMAKE_CROSSCOMPILING)

include(CTest)
enable_testing()
add_subdirectory(test)

    