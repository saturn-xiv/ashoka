include(ExternalProject)

set(HIREDIS_ROOT      ${CMAKE_BINARY_DIR}/3rd/hiredis)
set(HIREDIS_INSTALL   cd ${HIREDIS_ROOT}/src/HIREDIS-build && make -C ../HIREDIS PREFIX=${THIRD_ROOT} install) 

ExternalProject_Add(HIREDIS
    PREFIX            ${HIREDIS_ROOT}
    GIT_REPOSITORY    "https://github.com/redis/hiredis.git"
    GIT_TAG           "v1.0.0"
    INSTALL_COMMAND   ${HIREDIS_INSTALL}
)