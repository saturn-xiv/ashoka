#!/bin/sh

set -e

rm -rfv build/$1
mkdir -pv build/$1
cd build

CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release ..

ln -s ../config.ini ../staging.json ../staging ../tasks ../jobs ./

echo 'done.'

exit 0
