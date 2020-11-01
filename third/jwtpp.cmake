include(FetchContent)

set(OPENSSL_ROOT_DIR ${THIRD_ROOT})
FetchContent_Declare(
    jwtpp
    GIT_REPOSITORY      "https://github.com/troian/jwtpp.git"
    GIT_TAG             "v2.0.3"
)
FetchContent_MakeAvailable(jwtpp)