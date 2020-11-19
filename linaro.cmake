SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_SYSROOT /opt/sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf)
SET(CROSSTOOL_PREFIX /opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf)

SET(CMAKE_C_COMPILER   ${CROSSTOOL_PREFIX}-gcc)
SET(CMAKE_CXX_COMPILER ${CROSSTOOL_PREFIX}-g++)

