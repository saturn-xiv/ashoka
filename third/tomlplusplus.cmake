include(ExternalProject)

set(TOMLPLUSPLUS_ROOT      ${CMAKE_BINARY_DIR}/3rd/tomlplusplus)
set(TOMLPLUSPLUS_INSTALL   install -m 644 -D ${TOMLPLUSPLUS_ROOT}/src/TOMLPLUSPLUS/toml.hpp ${THIRD_ROOT}/include/) 

ExternalProject_Add(TOMLPLUSPLUS
    PREFIX            ${TOMLPLUSPLUS_ROOT}
    GIT_REPOSITORY    "https://github.com/marzer/tomlplusplus.git"
    GIT_TAG           "v2.2.0"
    BUILD_COMMAND     ""
    CONFIGURE_COMMAND ""
    INSTALL_COMMAND   ${TOMLPLUSPLUS_INSTALL}
)