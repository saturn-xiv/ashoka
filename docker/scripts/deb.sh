#!/bin/sh

set -e

if [ $# -ne 1 ] ; then
    echo 'Please specify your arch: x86_64, nano-pi-duo2'
    exit 1
fi

export WORKSPACE=$PWD
export TARGET=$WORKSPACE/ubuntu

# if [ $1 = 'x86_64' ]
# then
#     export $CONAN_PROFILE=$1
# elif [ $1 = 'nano-pi-duo2' ]
# then
#     export 
# else
#     echo "Unknown arch $1"
#     exit 1
# fi

if [ ! -f $WORKSPACE/focal/conan/profiles/$1 ]
then
    echo "can't find profile $1"
    exit 1
fi

rm -rfv $WORKSPACE/build/$1
mkdir -pv $WORKSPACE/build/$1
cd $WORKSPACE/build/$1

conan install --build=missing ../.. --profile ../../focal/conan/profiles/$1


if [ $1 = 'nano-pi-duo2' ]
then
else
    cmake -DCMAKE_BUILD_TYPE=Release ../..
fi


make -j

rm -rfv $TARGET/usr
mkdir -pv $TARGET/usr/bin
cp -av bin/ashoka $TARGET/usr/bin/

rm -rfv $TARGET/etc
mkdir -pv $TARGET/etc/ashoka
cp $WORKSPACE/LICENSE $WORKSPACE/config.ini $TARGET/etc/ashoka/

cd $WORKSPACE
dpkg -b ubuntu ashoka-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0