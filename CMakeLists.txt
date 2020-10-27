cmake_minimum_required(VERSION 3.16)
include(ExternalProject)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED system filesystem regex timer date_time chrono random log program_options)
# find_package(cpprestsdk REQUIRED)
find_package(Protobuf REQUIRED)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protos/nut.proto protos/hardware.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
message(${PROTO_SRCS})


set(THIRD_ROOT ${CMAKE_BINARY_DIR}/local)
include_directories(${THIRD_ROOT}/include)
include(third/tomlplusplus.cmake)
include(third/jwtpp.cmake)

aux_source_directory(src DIR_SRCS)
execute_process(COMMAND git describe --tags --always --dirty
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET
)
string(STRIP "${GIT_REV}" GIT_REV)
configure_file(src/config.h.in config.h)
add_executable(ashoka ${DIR_SRCS} ${PROTO_SRCS} ${PROTO_HDRS})
add_dependencies(ashoka TOMLPLUSPLUS JWTPP)

set_target_properties(ashoka PROPERTIES LINK_FLAGS_RELEASE -s)
target_link_libraries(ashoka 
    ${CMAKE_THREAD_LIBS_INIT}    
    ${Boost_LIBRARIES}    
    ${OPENSSL_SSL_LIBRARIES}
    ${Protobuf_LIBRARIES}
    sodium
    pq sqlite3 hiredis
    rabbitmq zmq
    ${THIRD_ROOT}/lib/libjwtpp.a
    # cpprestsdk::cpprest
)