include(FetchContent)

set(OPENSSL_ROOT_DIR ${THIRD_ROOT})
FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY      "https://github.com/open-source-parsers/jsoncpp.git"
    GIT_TAG             "1.9.4"
)
FetchContent_MakeAvailable(jsoncpp)