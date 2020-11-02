include(FetchContent)

FetchContent_Declare(
    libpqxx
    GIT_REPOSITORY      "https://github.com/jtv/libpqxx.git"
    GIT_TAG             "7.2.1"
)
FetchContent_MakeAvailable(libpqxx)
