cmake_minimum_required(VERSION 3.16)

# include(ExternalProject)
# include(FetchContent)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# find_package(OpenSSL REQUIRED Crypto SSL)
# find_package(Boost REQUIRED log_setup log thread system filesystem regex timer date_time chrono random  program_options unit_test_framework)
# find_package(Poco REQUIRED Crypto Data DataSQLite DataMySQL DataPostgreSQL Redis MongoDB JSON JWT Net Util XML Zip)

option(ASHOKA_BUILD_STATIC "build all static binary" OFF)

file(GLOB ASHOKA_HEADERS src/*.h)
file(GLOB ASHOKA_SOURCES src/*.cc)
execute_process(COMMAND git describe --tags --always --dirty
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET
)
string(STRIP "${GIT_REV}" GIT_REV)
configure_file(src/config.h.in config.h)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(ashoka src/main.cpp ${ASHOKA_HEADERS} ${ASHOKA_SOURCES})
set_target_properties(ashoka PROPERTIES LINK_FLAGS_RELEASE -s)

if(CMAKE_CROSSCOMPILING OR ASHOKA_BUILD_STATIC)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    SET(CMAKE_EXE_LINKER_FLAGS "-static")
    SET_TARGET_PROPERTIES(ashoka PROPERTIES LINK_SEARCH_END_STATIC 1)
endif(CMAKE_CROSSCOMPILING OR ASHOKA_BUILD_STATIC)
 
target_link_libraries(ashoka 
    ${CONAN_LIBS}
    stdc++fs 
)

# --------------------------------------

include(CTest)
enable_testing()

add_executable(queue_test src/queue_test.cpp ${ASHOKA_HEADERS} ${ASHOKA_SOURCES})
target_link_libraries(queue_test ${CONAN_LIBS} stdc++fs)

add_executable(audio_test src/audio_test.cpp ${ASHOKA_HEADERS} ${ASHOKA_SOURCES})
target_link_libraries(audio_test ${CONAN_LIBS} stdc++fs)

add_executable(mqtt_test src/mqtt_test.cpp ${ASHOKA_HEADERS} ${ASHOKA_SOURCES})
target_link_libraries(mqtt_test ${CONAN_LIBS} stdc++fs)

add_executable(tty_test src/tty_test.cpp ${ASHOKA_HEADERS} ${ASHOKA_SOURCES})
target_link_libraries(tty_test ${CONAN_LIBS} stdc++fs)

add_executable(twilio_test src/twilio_test.cpp ${ASHOKA_HEADERS} ${ASHOKA_SOURCES})
target_link_libraries(twilio_test ${CONAN_LIBS} stdc++fs)
