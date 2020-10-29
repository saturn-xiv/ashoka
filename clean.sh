#!/bin/sh

set -e

rm -rfv build
mkdir -pv build
cd build

conan install --build=missing .. --profile ../arch
CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Debug ..

ln -s ../config.ini ./

echo 'done.'

exit 0
