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
    libpq
    sqlite3
    hiredis
    librabbitmq
    boost 
    poco
)

for p in "${packages[@]}"
do
    echo "check package $p(x64)"
    $VCPKG_HOME/vcpkg install --triplet=x64-linux $p
done

# https://stackoverflow.com/questions/55204593/how-to-specify-cmake-version-used-by-vcpkg
# https://github.com/microsoft/vcpkg/issues/9570

export VCPKG_FORCE_SYSTEM_BINARIES=1
for p in "${packages[@]}"
do
    echo "check package $p(arm)"
    $VCPKG_HOME/vcpkg install --triplet=arm-linux $p
done

exit 0