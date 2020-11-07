SET(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# SET(CMAKE_SYSROOT /opt/sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf)
SET(CMAKE_SYSROOT /opt/nano-duo2/rootfs)
SET(CROSSTOOL_PREFIX /opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf)

SET(CMAKE_C_COMPILER   ${CROSSTOOL_PREFIX}-gcc)
SET(CMAKE_CXX_COMPILER ${CROSSTOOL_PREFIX}-g++)
SET(CMAKE_EXE_LINKER_FLAGS " -static")


# include_directories(SYSTEM /usr/include/arm-linux-gnueabihf)
# SET(ENV{PKG_CONFIG_ALLOW_CROSS} 1)
# SET(ENV{PKG_CONFIG_PATH} /usr/lib/arm-linux-gnueabihf/pkgconfig)
# SET(CMAKE_FIND_ROOT_PATH /opt/sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf)

# set(CMAKE_SYSROOT "/opt/nano-duo2/rootfs")
# set(ENV{PKG_CONFIG_DIR} "")
# set(ENV{PKG_CONFIG_LIBDIR} "${CMAKE_SYSROOT}/usr/lib/pkgconfig:${CMAKE_SYSROOT}/usr/share/pkgconfig")
# set(ENV{PKG_CONFIG_SYSROOT_DIR} ${CMAKE_SYSROOT})


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

