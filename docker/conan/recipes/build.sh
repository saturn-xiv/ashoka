#!/bin/bash

set -e




if [ $# -ne 1 ]
then
    echo 'please specify your profile'
    exit 1
fi

if [ ! -f $1 ]
then
    echo "profile $1 doesn't exist"
    exit 1
fi

export WORKSPACE=$PWD

declare -a packages=(    
    # "hiredis"    
    # "libsodium"
    # "jwtpp"
    "paho_mqtt"
)

for i in "${packages[@]}"
do
    cd $WORKSPACE/$i
    conan create . null-gate/ashoka --profile $1
done

echo "done"

exit 0