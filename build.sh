#!/bin/sh

set -e

export WORKSPACE=$PWD
export VERSION=$(git describe --tags --always --dirty)

cargo build --release

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
rm -rf ubuntu/usr/bin
mkdir -pv ubuntu/usr/bin
cp -v target/release/ashoka ubuntu/usr/bin/
strip -s ubuntu/usr/bin/ashoka

rm -rf ubuntu/var/lib/ashoka
mkdir -pv ubuntu/var/lib/ashoka
cp -a node_modules package.json package-lock.json ubuntu/var/lib/ashoka/
cp -a dashboard/build ubuntu/var/lib/ashoka/dashboard

rm -rf ubuntu/etc/ashoka
mkdir -pv ubuntu/etc/ashoka
echo "$VERSION $(date -R)" > ubuntu/etc/ashoka/VERSION
cp -r config.toml db README.md LICENSE ubuntu/etc/ashoka/

if [ $(lsb_release -is) = "Ubuntu" ]
then
    dpkg -b ubuntu tmp/ashoka-$VERSION.deb
fi

echo "Done! $TARGET"

exit 0