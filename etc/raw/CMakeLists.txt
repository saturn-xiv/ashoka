cmake_minimum_required(VERSION 3.18)
include(ExternalProject)

project(ashoka VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


aux_source_directory(src DIR_SRCS)

execute_process(COMMAND git describe --tags --always --dirty
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET
)
string(STRIP "${GIT_REV}" GIT_REV)

configure_file(src/config.h.in config.h)


set(THIRD_ROOT ${CMAKE_BINARY_DIR}/local)
include_directories(${THIRD_ROOT}/include)
include(third/jwtpp.cmake)
include(third/hiredis.cmake)
include(third/poco.cmake)
include(third/boost.cmake)

add_executable(ashoka ${DIR_SRCS})
target_include_directories(ashoka 
    PUBLIC 
    "${PROJECT_BINARY_DIR}"
)
add_dependencies(ashoka JWTPP HIREDIS POCO BOOST)

set_target_properties(ashoka PROPERTIES LINK_FLAGS_RELEASE -s)
target_link_libraries(ashoka 
    ${CMAKE_THREAD_LIBS_INIT}    
    # ${Boost_LIBRARIES}    
    # ${OPENSSL_SSL_LIBRARIES}
    # ${CONAN_LIBS}
    ${THIRD_ROOT}/lib/libjwtpp.a
    ${THIRD_ROOT}/lib/libhiredis.a

    ${THIRD_ROOT}/lib/libPocoCrypto.a
    ${THIRD_ROOT}/lib/libPocoData.a
    ${THIRD_ROOT}/lib/libPocoDataMySQL.a
    ${THIRD_ROOT}/lib/libPocoDataODBC.a
    ${THIRD_ROOT}/lib/libPocoDataPostgreSQL.a
    ${THIRD_ROOT}/lib/libPocoDataSQLite.a
    ${THIRD_ROOT}/lib/libPocoEncodings.a
    ${THIRD_ROOT}/lib/libPocoFoundation.a
    ${THIRD_ROOT}/lib/libPocoJSON.a
    ${THIRD_ROOT}/lib/libPocoJWT.a
    ${THIRD_ROOT}/lib/libPocoMongoDB.a
    ${THIRD_ROOT}/lib/libPocoNet.a
    ${THIRD_ROOT}/lib/libPocoNetSSL.a
    ${THIRD_ROOT}/lib/libPocoRedis.a
    ${THIRD_ROOT}/lib/libPocoUtil.a
    ${THIRD_ROOT}/lib/libPocoXML.a
    ${THIRD_ROOT}/lib/libPocoZip.a

)