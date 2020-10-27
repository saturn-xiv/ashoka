cmake_minimum_required(VERSION 3.16)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Poco REQUIRED Foundation Net NetSSL JSON Crypto XML Zip Data DataSQLite Redis)
find_package(Boost REQUIRED system filesystem regex timer date_time chrono random log)
find_package(Protobuf REQUIRED)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protos/nut.proto protos/hardware.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

aux_source_directory(src DIR_SRCS)
execute_process(COMMAND git describe --tags --always --dirty
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET
)
string(STRIP "${GIT_REV}" GIT_REV)
configure_file(src/config.h.in config.h)
add_executable(ashoka ${DIR_SRCS} ${PROTO_SRCS} ${PROTO_HDRS})
# target_include_directories(ashoka 
#     PUBLIC 
#     "${PROJECT_BINARY_DIR}"
# )
set_target_properties(ashoka PROPERTIES LINK_FLAGS_RELEASE -s)
target_link_libraries(ashoka 
    ${CMAKE_THREAD_LIBS_INIT}    
    ${Boost_LIBRARIES}    
    ${OPENSSL_SSL_LIBRARIES}
    ${Protobuf_LIBRARIES}
    Poco::Foundation Poco::Net Poco::NetSSL Poco::JSON Poco::Crypto Poco::XML Poco::Zip Poco::Data Poco::DataSQLite Poco::Redis 
    pq sqlite3 hiredis rabbitmq
)