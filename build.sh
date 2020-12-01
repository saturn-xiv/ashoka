#!/bin/bash

set -e

export WORKSPACE=$PWD
export VERSION=$(git describe --tags --always --dirty)

cd $WORKSPACE

if [ $# -ne 1 ] ; then
    echo "Please specify your arch(x86_64, armv7)"
    exit 1
fi

rm -rf ubuntu/usr/bin
mkdir -pv ubuntu/usr/bin

# https://doc.rust-lang.org/rustc/codegen-options/index.html#link-arg
# export RUSTFLAGS="-C target-feature=+crt-static -C link-arg=-static"
# export OPENSSL_STATIC=1
# export PQ_LIB_STATIC=1
# export SQLITE3_STATIC=1
# export PKG_CONFIG_ALL_STATIC=1

if [ $1 == "armv7" ]
then
    sudo apt -y install libssl-dev:armhf libudev-dev:armhf \
        libsqlite3-dev:armhf libpq-dev:armhf libmysqlclient-dev:armhf
    PKG_CONFIG_ALLOW_CROSS=1
    PKG_CONFIG_DIR=
    PKG_CONFIG_LIBDIR=/usr/lib/arm-linux-gnueabihf/pkgconfig
    export PKG_CONFIG_ALLOW_CROSS PKG_CONFIG_DIR PKG_CONFIG_LIBDIR
    cargo build --target armv7-unknown-linux-gnueabihf --release
    cp -v target/armv7-unknown-linux-gnueabihf/release/ashoka ubuntu/usr/bin/
    arm-linux-gnueabihf-strip -s ubuntu/usr/bin/ashoka
elif [ $1 == "x86_64" ]
then
    sudo apt -y install libssl-dev \
        libsqlite3-dev libpq-dev libmysqlclient-dev 
    cargo build --release
    cp -v target/release/ashoka ubuntu/usr/bin/
    strip -s ubuntu/usr/bin/ashoka
else
    echo "Unknown arch $1"
    exit 1
fi

if [ ! -d node_modules ]
then
    npm install
fi

cd $WORKSPACE/dashboard
if [ ! -d node_modules ]
then
    npm install
fi
npm run build

cd $WORKSPACE
rm -rf ubuntu/usr/share/ashoka
mkdir -pv ubuntu/usr/share/ashoka
cp -a views node_modules package.json package-lock.json ubuntu/usr/share/ashoka/
cp -a dashboard/build ubuntu/usr/share/ashoka/dashboard

rm -rf ubuntu/etc/ashoka
mkdir -pv ubuntu/etc/ashoka
echo "$VERSION $(date -R)" > ubuntu/etc/ashoka/VERSION
cp -r db README.md LICENSE ubuntu/etc/ashoka/
target/release/ashoka generate config -n ubuntu/etc/ashoka/config.toml

rm -rf ubuntu/var/lib/ashoka
mkdir -p ubuntu/var/lib/ashoka

if [ $(lsb_release -is) = "Ubuntu" ]
then
    dpkg -b ubuntu tmp/ashoka-$VERSION-$1.deb
fi

echo "Done! $TARGET"

exit 0