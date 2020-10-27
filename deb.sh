#!/bin/sh

set -e

export WORKSPACE=$PWD
export TARGET=$WORKSPACE/ubuntu

rm -rfv $WORKSPACE/build
mkdir -pv $WORKSPACE/build
cd $WORKSPACE/build

CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release ..
make

rm -rfv $TARGET/usr
mkdir -pv $TARGET/usr/bin
cp -av bin/ashoka $TARGET/usr/bin/

rm -rfv $TARGET/etc
mkdir -pv $TARGET/etc/ashoka
cp $WORKSPACE/LICENSE $WORKSPACE/config.properties $TARGET/etc/ashoka/

cd $WORKSPACE
dpkg -b ubuntu ashoka-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0