#!/bin/sh

set -e

rm -rfv build/$1
mkdir -pv build/$1
cd build

conan install .. --profile=../docker/conan/profiles/arch --build=missing
CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release ..

ln -s ../config.toml ../staging ../tasks ../db ../templates ./

echo 'done.'

exit 0
