#!/bin/bash

set -e

echo "check package vcpkg"
export VCPKG_HOME=$HOME/local/vcpkg
if [ ! -d $VCPKG_HOME ]
then
    git clone https://github.com/microsoft/vcpkg $VCPKG_HOME
    $VCPKG_HOME/bootstrap-vcpkg.sh
fi

declare -a triplets=(
    arm-linux
    x64-linux
)

declare -a packages=(
    openssl
    libpq
    sqlite3
    hiredis
    librabbitmq
    poco
    boost 
)

for p in "${packages[@]}"
do
    for t in "${triplets[@]}"
    do
        echo "check package $p($t)"
        $VCPKG_HOME/vcpkg install --triplet=$t $p
    done
done

exit 0