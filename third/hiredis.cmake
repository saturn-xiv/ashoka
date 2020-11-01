include(FetchContent)

FetchContent_Declare(
    hiredis
    GIT_REPOSITORY      "https://github.com/redis/hiredis.git"
    GIT_TAG             "e3f88eb"
)
FetchContent_MakeAvailable(hiredis)
