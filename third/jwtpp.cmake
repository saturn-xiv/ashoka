include(FetchContent)

FetchContent_Declare(
    jwtpp
    GIT_REPOSITORY      "https://github.com/troian/jwtpp.git"
    GIT_TAG             "v2.0.3"
)
FetchContent_MakeAvailable(jwtpp)