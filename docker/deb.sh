#!/bin/sh

set -e

if [ $# -ne 2 ] ; then
    echo 'Please specify your arch(x64, win, arm) AND project name'
    exit 1
fi

export WORKSPACE=$PWD

export BUILD_ROOT=$WORKSPACE/build/$2-$1
rm -rfv $BUILD_ROOT
mkdir -pv $BUILD_ROOT
cd $BUILD_ROOT

# sudo apt update g++-mingw-w64
# sudo apt -y install g++-9 
# apt --yes --force-yes -o Dpkg::Options::="--force-confnew" upgrade
if [[ $1 = "arm" ]]
then
    conan install $WORKSPACE --profile=$WORKSPACE/docker/conan/profiles/linaro --build=missing
    cmake -DCMAKE_TOOLCHAIN_FILE=$WORKSPACE/linaro.cmake -DCMAKE_BUILD_TYPE=Release $WORKSPACE
else if [[ $1 = "win" ]]
    conan install $WORKSPACE --profile=$WORKSPACE/docker/conan/profiles/mingw --build=missing
    cmake -DCMAKE_TOOLCHAIN_FILE=$WORKSPACE/mingw.cmake -DCMAKE_BUILD_TYPE=Release $WORKSPACE 
then
else if [[ $1 = "x64" ]]
then
    # https://github.com/microsoft/cpprestsdk/pull/1462
    conan install $WORKSPACE --profile=$WORKSPACE/docker/conan/profiles/gcc9 --build=missing
    cmake -DASHOKA_BUILD_STATIC=ON -DCMAKE_C_COMPILER=gcc-9 -DCMAKE_CXX_COMPILER=g++-9 -DCMAKE_BUILD_TYPE=Release $WORKSPACE
else
    echo "Unknown arch $1"
    exit 1
fi

make

export TARGET=$WORKSPACE/ubuntu
rm -rfv $TARGET/usr
mkdir -pv $TARGET/usr/bin 
cp -av bin/ashoka $TARGET/usr/bin/
strip -s $TARGET/usr/bin/ashoka
mkdir -pv $TARGET/usr/share/ashoka
cp -av $WORKSPACE/db $TARGET/usr/share/ashoka/

rm -rfv $TARGET/etc
mkdir -pv $TARGET/etc/ashoka
cp $WORKSPACE/LICENSE $WORKSPACE/config.toml $TARGET/etc/ashoka/

cd $WORKSPACE
# $(lsb_release -cs)-
dpkg -b ubuntu $2-$1-$(git describe --tags --always --dirty --first-parent).deb

echo 'done'

exit 0