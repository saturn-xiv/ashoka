#!/bin/sh

set -e

if [ $# -ne 1 ] ; then
    echo 'Please specify your arch: x86_64, armv7hf'
    exit 1
fi

export WORKSPACE=$PWD
export TARGET=$WORKSPACE/ubuntu

rm -rfv $WORKSPACE/build
mkdir -pv $WORKSPACE/build
cd $WORKSPACE/build

if [ $1 = 'x86_64' ]
then
    export CC=clang
    export CXX=clang++ 
elif [ $1 = 'armv7hf' ]
then
    export TARGET_HOST=arm-linux-gnueabihf
    export CC=$TARGET_HOST-gcc
    export CXX=$TARGET_HOST-g++
else
    echo "Unknown arch $1"
    exit 1
fi

cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

rm -rfv $TARGET/usr
mkdir -pv $TARGET/usr/bin
cp -av ashoka $TARGET/usr/bin/

rm -rfv $TARGET/etc
mkdir -pv $TARGET/etc/ashoka
cp $WORKSPACE/LICENSE $WORKSPACE/config.properties $TARGET/etc/ashoka/

cd $WORKSPACE
dpkg -b ubuntu ashoka-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0