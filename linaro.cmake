SET(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_SYSROOT /opt/sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf)
SET(CROSSTOOL_PREFIX /opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf)


SET(CMAKE_C_COMPILER   ${CROSSTOOL_PREFIX}-gcc)
SET(CMAKE_CXX_COMPILER ${CROSSTOOL_PREFIX}-g++)

# include_directories(SYSTEM /usr/include/arm-linux-gnueabihf)
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

