cmake_minimum_required(VERSION 3.16)
# include(ExternalProject)
include(FetchContent)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED system filesystem regex timer date_time chrono random log program_options)

# set(BUILD_TESTS OFF)
# set(DISABLE_TESTS ON)
# set(BUILD_EXAMPLES OFF)
# set(BUILD_SHARED_LIBS OFF)
# set(BUILD_STATIC_LIBS ON)
# ---------------------------------------------------------
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protos/nut.proto protos/hardware.proto)
# ${PROTO_SRCS} ${PROTO_HDRS}
# ---------------------------------------------------------

FetchContent_Declare(
    hiredis
    GIT_REPOSITORY https://github.com/redis/hiredis.git
    GIT_TAG        e3f88eb 
)
FetchContent_MakeAvailable(hiredis)

# ---------------------------------------------------------

FetchContent_Declare(
    jwtpp
    GIT_REPOSITORY https://github.com/troian/jwtpp.git
    GIT_TAG        v2.0.3
)
FetchContent_MakeAvailable(jwtpp)

# ---------------------------------------------------------

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
    ${jwtpp_SOURCE_DIR}/include/export
    ${hiredis_SOURCE_DIR}
)
target_link_libraries(ashoka 
    ${CMAKE_THREAD_LIBS_INIT}    
    ${CONAN_LIBS}
    ${CMAKE_CURRENT_BINARY_DIR}/lib/libjwtpp.a
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(ashoka libhiredis_static.a)
else()
    target_link_libraries(ashoka ${CMAKE_CURRENT_BINARY_DIR}/lib/libhiredis_staticd.a)
endif()