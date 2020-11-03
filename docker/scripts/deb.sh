#!/bin/sh

set -e

if [ $# -ne 2 ] ; then
    echo 'Please specify your arch(x64, linaro) AND project name'
    exit 1
fi

export WORKSPACE=$PWD

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

    sudo apt -y install libpq-dev:armhf postgresql-server-dev-all:armhf libsqlite3-dev:armhf \
        libssl-dev:armhf libsodium-dev:armhf libboost-all-dev:armhf libjsoncpp-dev:armhf \
        libczmq-dev:armhf libfltk1.3-dev:armhf
else
    # https://github.com/microsoft/cpprestsdk/pull/1462
    sudo apt -y install g++-9 \
        libpq-dev postgresql-server-dev-all libsqlite3-dev \
        libssl-dev libsodium-dev libboost-all-dev libjsoncpp-dev \
        libczmq-dev libfltk1.3-dev libgit2-dev libssh2-1-dev
    export CC=gcc-9
    export CXX=g++-9
    export CMAKE_C_COMPILER=gcc-9
    export CMAKE_CXX_COMPILER=g++-9
fi


mkdir -pv $WORKSPACE/build/$1
cd $WORKSPACE/build/$1
cmake ../..
make

rm -rfv $TARGET/usr
mkdir -pv $TARGET/usr/bin
cp -av bin/ashoka $TARGET/usr/bin/

rm -rfv $TARGET/etc
mkdir -pv $TARGET/etc/ashoka
cp $WORKSPACE/LICENSE $WORKSPACE/config.ini $TARGET/etc/ashoka/

cd $WORKSPACE
dpkg -b ubuntu $2-$1-$(lsb_release -cs)-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0