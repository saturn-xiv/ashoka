include(FetchContent)

FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY  "https://github.com/nlohmann/json.git"
    GIT_TAG         "v3.9.1"
)
FetchContent_GetProperties(nlohmann_json)