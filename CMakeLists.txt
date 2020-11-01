cmake_minimum_required(VERSION 3.16)

include(ExternalProject)
include(FetchContent)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED Crypto SSL)
find_package(Boost REQUIRED log_setup log thread system filesystem regex timer date_time chrono random  program_options)

include(third/jwtpp.cmake)
include(third/hiredis.cmake)
include(third/cpprestsdk.cmake)

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
    ${hiredis_SOURCE_DIR}
    ${jwtpp_SOURCE_DIR}/include/export
    ${cpprestsdk_SOURCE_DIR}/Release/include
)

target_link_libraries(ashoka PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    hiredis_static jwtpp-static
    cpprest
    pq sqlite3
)


    