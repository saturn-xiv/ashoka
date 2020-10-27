#!/bin/sh

set -e

export ASHOKA=$PWD

git pull

echo "build for x86_64"
mkdir -p $ASHOKA/x86_64-build
cd $ASHOKA/x86_64-build
conan install --build=missing .. --profile ../profiles/focal-x86_64
CC=gcc-10 CXX=g++-10 cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

echo "build for armv7hf"
mkdir -p $ASHOKA/armv7hf-build
cd $ASHOKA/armv7hf-build
export USE_DEV_URANDOM=1
export CC=arm-linux-gnueabihf-gcc-10
export CXX=arm-linux-gnueabihf-g++-10
conan install --build=missing .. --profile ../profiles/focal-armv7hf
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

echo "done"

exit 0