include(FetchContent)

set(JsonCPP_FOUND TRUE)
set(JsonCPP_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS_JSONCPP})
set(JsonCPP_LIBRARY_DIRS ${CONAN_LIB_DIRS_JSONCPP})

FetchContent_Declare(
    jwtpp
    GIT_REPOSITORY      "https://github.com/troian/jwtpp.git"
    GIT_TAG             "v2.0.3"
)
FetchContent_MakeAvailable(jwtpp)