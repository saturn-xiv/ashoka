#!/bin/bash

set -e

declare -a packages=(    
    # https://www.linaro.org/downloads/
    "linaro/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar"  
    "linaro/sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf.tar"
    "linaro/runtime-gcc-linaro-7.5.0-2019.12-arm-linux-gnueabihf.tar"
)

for i in "${packages[@]}"
do
    sudo tar xf /opt/packages/$i -C /opt
done


exit 0