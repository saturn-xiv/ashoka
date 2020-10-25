#!/bin/sh

set -e

export ASHOKA=$PWD

git pull

echo "build for x64"
mkdir -p $ASHOKA/x64-build
cd $ASHOKA/x64-build
export CC=gcc-10 
export CXX=g++-10 
conan install --build=missing .. --profile ../x64
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

echo "build for arm"
mkdir -p $ASHOKA/arm-build
cd $ASHOKA/arm-build
export USE_DEV_URANDOM=1
export CC=arm-linux-gnueabihf-gcc-10
export CXX=arm-linux-gnueabihf-g++-10
conan install --build=missing .. --profile ../arm
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

echo "done"

exit 0