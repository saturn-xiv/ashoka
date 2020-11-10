#!/bin/sh

set -e

if [ $# -ne 2 ] ; then
    echo 'Please specify your arch(x64, linaro) AND project name'
    exit 1
fi

export WORKSPACE=$PWD

export BUILD_ROOT=$WORKSPACE/build/$2-$1
rm -rfv $BUILD_ROOT
mkdir -pv $BUILD_ROOT
cd $BUILD_ROOT

# sudo apt update
if [ $1 = "linaro" ]
then
    # apt --yes --force-yes -o Dpkg::Options::="--force-confnew" upgrade
    conan install $WORKSPACE --profile=$WORKSPACE/docker/conan/profiles/$1 --build=missing
    cmake -DCMAKE_TOOLCHAIN_FILE=$WORKSPACE/$1.cmake -DCMAKE_BUILD_TYPE=Release $WORKSPACE
else
    # https://github.com/microsoft/cpprestsdk/pull/1462
    sudo apt -y install g++-9
    conan install $WORKSPACE --profile=$WORKSPACE/docker/conan/profiles/gcc9 --build=missing
    cmake -DCMAKE_C_COMPILER=gcc-9 -DCMAKE_CXX_COMPILER=g++-9 -DCMAKE_BUILD_TYPE=Release $WORKSPACE
fi

make

export TARGET=$WORKSPACE/ubuntu
rm -rfv $TARGET/usr
mkdir -pv $TARGET/usr/bin
cp -av bin/ashoka $TARGET/usr/bin/
strip -s $TARGET/usr/bin/ashoka

rm -rfv $TARGET/etc
mkdir -pv $TARGET/etc/ashoka
cp $WORKSPACE/LICENSE $WORKSPACE/config.toml $TARGET/etc/ashoka/

cd $WORKSPACE
dpkg -b ubuntu $2-$1-$(lsb_release -cs)-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0