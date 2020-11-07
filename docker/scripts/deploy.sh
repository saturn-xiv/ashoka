#!/bin/bash

set -e

curl https://bootstrap.pypa.io/get-pip.py -o /tmp/get-pip.py
python3.10 /tmp/get-pip.py

. $HOME/.profile
pip install --user cmake
pip install --user conan
. $HOME/.profile

# https://github.com/nvm-sh/nvm
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.36.0/install.sh | sh
. $HOME/.profile
nvm install node
. $HOME/.profile
npm install -g yarn

echo 'source $HOME/.profile' >> $HOME/.zshrc

git clone https://github.com/saturn-xiv/ashoka.git $HOME/workspace/ashoka
git checkout cpp

declare -a profiles=(    
    "gcc9"    
    "linaro"
)

for i in "${profiles[@]}"
do
    cd $HOME/workspace/ashoka/docker/conan/recipes
    ./build.sh $WORKSPACE/docker/conan/profiles/$i
    conan install $WORKSPACE --profile=$WORKSPACE/docker/conan/profiles/$i --build=missing
done


exit 0