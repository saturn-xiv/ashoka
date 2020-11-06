#!/bin/sh

set -e

if [ $# -ne 2 ] ; then
    echo 'Please specify your arch(x64, linaro) AND project name'
    exit 1
fi

export WORKSPACE=$PWD

export BUILD_ROOT=$WORKSPACE/build/$2-$1
mkdir -pv $BUILD_ROOT
cd $BUILD_ROOT

# sudo apt update
if [ $1 = "linaro" ]
then
    # sudo apt -y install libpq-dev:armhf libsqlite3-dev:armhf \
    #     libssl-dev:armhf libsodium-dev:armhf libboost-all-dev:armhf libjsoncpp-dev:armhf \
    #     libczmq-dev:armhf libfltk1.3-dev:armhf \
    #     pkg-config-arm-linux-gnueabihf
    export PKG_CONFIG_ALLOW_CROSS=1
    export PKG_CONFIG_PATH=/usr/lib/arm-linux-gnueabihf/pkgconfig
    conan install ../.. --profile=../docker/conan/profiles/$1 --build=missing
    cmake -DCMAKE_TOOLCHAIN_FILE=$WORKSPACE/third/$1.cmake -DCMAKE_BUILD_TYPE=Release ../..
else
    # https://github.com/microsoft/cpprestsdk/pull/1462
    # sudo apt -y install g++-9 \
    #     libpq-dev postgresql-server-dev-all libsqlite3-dev \
    #     libssl-dev libsodium-dev libboost-all-dev libjsoncpp-dev \
    #     libczmq-dev libfltk1.3-dev libgit2-dev libssh2-1-dev
    sudo apt -y install g++-9
    conan install ../.. --profile=../../docker/conan/profiles/bionic --build=missing
    cmake -DCMAKE_C_COMPILER=gcc-9 -DCMAKE_CXX_COMPILER=g++-9 -DCMAKE_BUILD_TYPE=Release ../..
fi

make

export TARGET=$WORKSPACE/ubuntu
rm -rfv $TARGET/usr
mkdir -pv $TARGET/usr/bin
cp -av bin/ashoka $TARGET/usr/bin/
strip -s $TARGET/usr/bin/ashoka

rm -rfv $TARGET/etc
mkdir -pv $TARGET/etc/ashoka
cp $WORKSPACE/LICENSE $WORKSPACE/config.ini $TARGET/etc/ashoka/

cd $WORKSPACE
dpkg -b ubuntu $2-$1-$(lsb_release -cs)-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0