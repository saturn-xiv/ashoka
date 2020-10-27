cmake_minimum_required(VERSION 3.16)
include(ExternalProject)
include(FetchContent)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED system filesystem regex timer date_time chrono random log program_options)

# ---------------------------------------------------------

FetchContent_Declare(
    pistache
    GIT_REPOSITORY https://github.com/pistacheio/pistache.git
    GIT_TAG        c04166c
)
FetchContent_MakeAvailable(pistache)

# ---------------------------------------------------------

FetchContent_Declare(
    jwtpp
    GIT_REPOSITORY https://github.com/troian/jwtpp.git
    GIT_TAG        v2.0.3
)
FetchContent_MakeAvailable(jwtpp)

# ---------------------------------------------------------

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v2.2.0
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(tomlplusplus)

# ---------------------------------------------------------

FetchContent_Declare(
    rapidjson
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG        v1.1.0
)
set(RAPIDJSON_BUILD_DOC OFF)
set(RAPIDJSON_BUILD_TESTS OFF)
set(RAPIDJSON_BUILD_EXAMPLES OFF)
FetchContent_MakeAvailable(rapidjson)

# ---------------------------------------------------------

FetchContent_Declare(
    protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG        v3.13.0
    SOURCE_SUBDIR  cmake
)
set(protobuf_BUILD_TESTS OFF)
FetchContent_MakeAvailable(protobuf)

# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protos/nut.proto protos/hardware.proto)
# ---------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${rapidjson_SOURCE_DIR}/include
    ${tomlplusplus_SOURCE_DIR}
    ${jwtpp_SOURCE_DIR}/include/export
    ${pistache_SOURCE_DIR}/include
)


aux_source_directory(src DIR_SRCS)
execute_process(COMMAND git describe --tags --always --dirty
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET
)
string(STRIP "${GIT_REV}" GIT_REV)
configure_file(src/config.h.in config.h)
add_executable(ashoka ${DIR_SRCS} ${PROTO_SRCS} ${PROTO_HDRS})

set_target_properties(ashoka PROPERTIES LINK_FLAGS_RELEASE -s)
target_link_libraries(ashoka 
    ${CMAKE_THREAD_LIBS_INIT}    
    ${Boost_LIBRARIES}    
    ${OPENSSL_SSL_LIBRARIES}
        
    ${protobuf_BINARY_DIR}/libprotobufd.a
    ${jwtpp_BINARY_DIR}/libjwtpp.a
    ${pistache_BINARY_DIR}/src/libpistache.a

    sodium
    pq sqlite3 hiredis
    rabbitmq zmq 
    # cpprestsdk::cpprest
)