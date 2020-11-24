#!/bin/sh

set -e

cargo build --release

if [ ! -d node_modules ]
then
    npm install
fi

cd dashboard
if [ ! -d node_modules ]
then
    yarn install
fi
yarn build
cd ..

mkdir -pv ubuntu/usr/bin
cp -v target/release/arete ubuntu/usr/bin/
strip -s ubuntu/usr/bin/arete

mkdir -pv ubuntu/var/lib/ashoka
cp -a node_modules package.json package-lock.json ubuntu/var/lib/ashoka/
cp -a dashboard/build ubuntu/var/lib/ashoka/dashboard

export VERSION=$(git describe --tags --always --dirty)
echo "$VERSION $(date -R)" > ubuntu/etc/ashoka/VERSION
cp config.toml README.md LICENCE ubuntu/etc/ashoka/

if [ $(lsb_release -is) = "Ubuntu" ]
then
    dpkg -b ubuntu ashoka-$VERSION.deb
fi

echo "Done! $TARGET"

exit 0