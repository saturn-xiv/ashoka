include(FetchContent)

FetchContent_Declare(
    cpprestsdk
    GIT_REPOSITORY      "https://github.com/microsoft/cpprestsdk.git"
    GIT_TAG             "v2.10.16"
)
FetchContent_MakeAvailable(cpprestsdk)
