#!/bin/bash

set -e

docker pull ubuntu:latest
docker pull ubuntu:bionic
docker pull ubuntu:xenial
docker pull debian:buster
docker pull alpine:latest

declare -a features=(    
    "focal"  
    "bionic"
    "xenial"
)

for i in "${features[@]}"
do
    n="ashoka-$i"
    echo "build image $n..."
    docker build -t $n . -f $i.Dockerfile
done

echo 'done.'

exit 0