#!/bin/sh

set -e

export WORKSPACE=$PWD
export TARGET=$WORKSPACE/ubuntu

rm -rfv $WORKSPACE/build
mkdir -pv $WORKSPACE/build
cd $WORKSPACE/build

conan install --build=missing .. --profile ../x64
CC=gcc-10 CXX=g++-10 cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
mkdir -pv $TARGET/usr/bin
cp -av bin/ashoka $TARGET/usr/bin/
mkdir -pv $TARGET/etc/ashoka

cp $WORKSPACE/LICENSE $WORKSPACE/config.properties $TARGET/etc/ashoka/

dpkg -b ubuntu ashoka-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0