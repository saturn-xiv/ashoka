include(ExternalProject)

set(BOOST_ROOT      ${CMAKE_BINARY_DIR}/3rd/boost)
set(BOOST_CONFIGURE cd ${BOOST_ROOT}/src/BOOST-build && ../BOOST/bootstrap.sh --prefix=${THIRD_ROOT} )
# set(BOOST_MAKE      cd ${BOOST_ROOT}/src/BOOST-build && make) 
set(BOOST_INSTALL   cd ${BOOST_ROOT}/src/BOOST-build && ./b2 install) 

ExternalProject_Add(BOOST
    PREFIX            ${BOOST_ROOT}
    GIT_REPOSITORY    "https://github.com/boostorg/boost.git"
    GIT_TAG           "boost-1.74.0"
    CONFIGURE_COMMAND ${BOOST_CONFIGURE}
    INSTALL_COMMAND   ${BOOST_INSTALL}
)