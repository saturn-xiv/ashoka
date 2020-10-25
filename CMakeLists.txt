cmake_minimum_required(VERSION 3.16)
include(ExternalProject)

SET(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/local/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(ashoka VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# set(Boost_USE_STATIC_LIBS TRUE)
# set(Boost_USE_STATIC_RUNTIME TRUE)
# set(OPENSSL_USE_STATIC_LIBS TRUE)
# set(OPENSSL_MSVC_STATIC_RT TRUE)

# include_directories("$ENV{HOME}/local/vcpkg/installed/x64-linux/include")
# link_directories("$ENV{HOME}/local/vcpkg/installed/x64-linux/lib")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Poco REQUIRED Foundation Net NetSSL JSON Crypto XML)
find_package(Boost REQUIRED system filesystem regex timer date_time chrono random log)

aux_source_directory(src DIR_SRCS)

execute_process(COMMAND git describe --tags --always --dirty
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET
)
string(STRIP "${GIT_REV}" GIT_REV)

configure_file(src/config.h.in config.h)

add_executable(ashoka ${DIR_SRCS})

target_include_directories(ashoka 
    PUBLIC 
    "${PROJECT_BINARY_DIR}"
)
set_target_properties(ashoka PROPERTIES LINK_FLAGS_RELEASE -s)
target_link_libraries(ashoka 
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}    
    ${OPENSSL_SSL_LIBRARIES}
    ${CONAN_LIBS}
    pq hiredis sqlite3 rabbitmq
    Poco::Foundation Poco::Net Poco::NetSSL Poco::JSON Poco::Crypto Poco::XML    
)
