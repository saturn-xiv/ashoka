#!/bin/sh

set -e

if [ $# -ne 2 ] ; then
    echo 'Please specify your arch(x64, linaro) AND project name'
    exit 1
fi

export CMAKE_BUILD_TYPE=Release
if [ $1 = "linaro" ]
then
    export LINARO_HOME=/opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf
    export HOME=$LINARO_HOME/bin:$HOME
    export TARGET_HOST=arm-linux-gnueabihf
    export AR=$TARGET_HOST-ar
    export AS=$TARGET_HOST-as
    export RANLIB=$TARGET_HOST-ranlib
    export STRIP=$TARGET_HOST-strip
    export LD=$TARGET_HOST-ld
    export CC=$TARGET_HOST-gcc
    export CXX=$TARGET_HOST-g++

    export CMAKE_SYSTEM_NAME=Linux
    export CMAKE_SYSTEM_PROCESSOR=arm
    export CMAKE_C_COMPILER=$LINARO_HOME/bin/$CC
    export CMAKE_CXX_COMPILER=$LINARO_HOME/bin/$CXX
fi


mkdir -pv $WORKSPACE/build/$1
cd $WORKSPACE/build/$1
cmake ../..
make -j

rm -rfv $TARGET/usr
mkdir -pv $TARGET/usr/bin
cp -av bin/ashoka $TARGET/usr/bin/

rm -rfv $TARGET/etc
mkdir -pv $TARGET/etc/ashoka
cp $WORKSPACE/LICENSE $WORKSPACE/config.ini $TARGET/etc/ashoka/

cd $WORKSPACE
dpkg -b ubuntu ashoka-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0