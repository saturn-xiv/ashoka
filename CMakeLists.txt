cmake_minimum_required(VERSION 3.16)

include(ExternalProject)
include(FetchContent)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED Crypto SSL)
find_package(Boost REQUIRED log_setup log thread system filesystem regex timer date_time chrono random  program_options)

include(third/jwtpp.cmake)
include(third/hiredis.cmake)
include(third/cpprestsdk.cmake)
include(third/nlohmann_json.cmake)
include(third/inja.cmake)
include(third/paho_mqtt.cmake)
include(third/libpqxx.cmake)

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
    ${inja_SOURCE_DIR}/single_include/inja
    ${inja_SOURCE_DIR}/third_party/include
    ${paho_mqtt_SOURCE_DIR}/src
    ${libpqxx_SOURCE_DIR}/include
)

target_link_libraries(ashoka PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    hiredis_static jwtpp-static nlohmann_json paho-mqtt3a-static pqxx cpprest    
    jsoncpp
    sodium pq sqlite3 zmq ssh2 git2 
    fltk fltk_images fltk_forms
)

include(CTest)
enable_testing()
add_subdirectory(test)

    