include(ExternalProject)

set(POCO_ROOT      ${CMAKE_BINARY_DIR}/3rd/poco)
set(POCO_CONFIGURE cd ${POCO_ROOT}/src/POCO-build && cmake -D BUILD_SHARED_LIBS=OFF -D CMAKE_INSTALL_PREFIX=${THIRD_ROOT} ../POCO)
set(POCO_MAKE      cd ${POCO_ROOT}/src/POCO-build && make) 
set(POCO_INSTALL   cd ${POCO_ROOT}/src/POCO-build && make install) 

ExternalProject_Add(POCO
    PREFIX            ${POCO_ROOT}
    GIT_REPOSITORY    "https://github.com/pocoproject/poco.git"
    GIT_TAG           "poco-1.10.1-release"
    CONFIGURE_COMMAND ${POCO_CONFIGURE}
    BUILD_COMMAND     ${POCO_MAKE}
    INSTALL_COMMAND   ${POCO_INSTALL}
)