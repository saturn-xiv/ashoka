include(ExternalProject)

set(JWTPP_ROOT      ${CMAKE_BINARY_DIR}/3rd/jwtpp)
set(JWTPP_CONFIGURE cd ${JWTPP_ROOT}/src/jwtpp && cmake -D CMAKE_INSTALL_PREFIX=${THIRD_ROOT} .)
set(JWTPP_MAKE      cd ${JWTPP_ROOT}/src/jwtpp && make -j) 
set(JWTPP_INSTALL   cd ${JWTPP_ROOT}/src/jwtpp && make install) 

ExternalProject_Add(JWTPP
    PREFIX            ${JWTPP_ROOT}
    GIT_REPOSITORY    "https://github.com/troian/jwtpp.git"
    GIT_TAG           "v2.0.3"
    CONFIGURE_COMMAND ${JWTPP_CONFIGURE}
    BUILD_COMMAND     ${JWTPP_MAKE}
    INSTALL_COMMAND   ${JWTPP_INSTALL}
)