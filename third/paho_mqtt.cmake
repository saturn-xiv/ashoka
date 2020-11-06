include(FetchContent)

set(PAHO_BUILD_SHARED FALSE)
set(PAHO_BUILD_STATIC TRUE)

FetchContent_Declare(
    paho_mqtt
    GIT_REPOSITORY      "https://github.com/eclipse/paho.mqtt.c.git"
    GIT_TAG             "v1.3.6"
)
FetchContent_MakeAvailable(paho_mqtt)