#!/bin/sh

set -e

echo "check repo"
export ASHOKA=$HOME/workspace/ashoka

if [ -d $ASHOKA ]
then
    cd $ASHOKA
    git pull
else
    git clone https://github.com/saturn-xiv/ashoka.git $ASHOKA/
    git checkout cpp
fi

mkdir -p $ASHOKA/build
cd $ASHOKA/build

echo "build amd64"
CC=gcc-10 CXX=g++10 conan install --build=missing .. --profile ../x64

echo "build arm7hf"
CC=arm-linux-gnueabihf-gcc-10 CXX=arm-linux-gnueabihf-g++-10 conan install --build=missing .. --profile ../arm

echo "done"

exit 0