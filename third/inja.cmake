include(FetchContent)

FetchContent_Declare(
    inja
    GIT_REPOSITORY      "https://github.com/pantor/inja.git"
    GIT_TAG             "v3.1.0"
)
FetchContent_MakeAvailable(inja)