#!/bin/sh

# https://yarnpkg.com/getting-started/install
# https://create-react-app.dev/docs/adding-typescript/

set -e

# export RUSTFLAGS="-C target-feature=+crt-static"
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

export TARGET=tmp/$(git describe --tags --always --dirty)
cp target/release/arete $TARGET/
strip -s $TARGET/arete
cp -r dashboard/build $TARGET/dashboard

echo "Done! $TARGET"

exit 0