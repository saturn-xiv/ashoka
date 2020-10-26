#!/bin/bash

set -e


declare -a protocols=(
    hardware
)

for i in "${protocols[@]}"
do
    echo "gernerate $i"
    protoc -I protos --grpc_out=src --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` protos/$i.proto
    protoc -I protos --cpp_out=src protos/$i.proto   
done

rm -rfv build
mkdir -pv build
cd build

conan install --build=missing .. --profile ../profiles/arch
CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Debug ..

ln -s ../config.properties ashoka.properties

echo 'done.'

exit 0
