#!/bin/sh

set -e

rm -rfv build
mkdir -pv build
cd build

conan install --build=missing ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
ln -s ../config.properties ashoka.properties

echo 'done.'

exit 0
