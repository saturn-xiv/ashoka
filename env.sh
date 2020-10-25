#!/bin/bash

set -e

echo "check package vcpkg"
export VCPKG_HOME=$HOME/local/vcpkg
if [ ! -d $VCPKG_HOME ]
then
    git clone https://github.com/microsoft/vcpkg $VCPKG_HOME
    $VCPKG_HOME/bootstrap-vcpkg.sh
fi

declare -a packages=(
    openssl
    libpq
    sqlite3
    hiredis
    librabbitmq
    poco
    boost 
)
for i in "${packages[@]}"
do
    echo "check package $i"
    $VCPKG_HOME/vcpkg install $i
done

exit 0