FROM ubuntu:xenial
LABEL maintainer="Jeremy Zheng"

ENV DEBIAN_FRONTEND noninteractive
ENV UBUNTU xenial

RUN apt update
RUN apt -y install software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
RUN add-apt-repository ppa:deadsnakes/ppa -y
RUN apt update
RUN apt -y upgrade
# https://wiki.ubuntu.com/ToolChain
RUN apt -y install zsh git locales pkg-config rsync openssh-client \
    vim sudo tzdata pwgen curl zip unzip wget \
    python3.10 python3.10-distutils\
    g++-9 \
    build-essential libtool automake autoconf binutils-multiarch cmake clang \
    linux-libc-dev-armel-cross g++-arm-linux-gnueabihf pkg-config-arm-linux-gnueabihf binutils-arm-linux-gnueabihf

RUN dpkg --add-architecture armhf
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $UBUNTU main restricted universe multiverse" > /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $UBUNTU-updates main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $UBUNTU-security main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $UBUNTU main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $UBUNTU-security main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $UBUNTU-updates main restricted universe multiverse" >> /etc/apt/sources.list

RUN apt update
RUN apt -y install libssl-dev:armhf libpoco-dev:armhf libboost-all-dev:armhf
RUN apt -y autoremove
RUN apt -y clean

RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
RUN locale-gen
RUN update-locale LANG=en_US.UTF-8
RUN update-alternatives --set editor /usr/bin/vim.basic

# deploy
RUN useradd -m deploy -s /bin/zsh
RUN passwd -l deploy
RUN echo 'deploy ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/101-deploy
USER deploy

# https://github.com/ohmyzsh/ohmyzsh
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# https://github.com/nvm-sh/nvm
RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.36.0/install.sh | sh
RUN sh -c ". $HOME/.profile \
    && nvm install node"
RUN sh -c ". $HOME/.profile \
    && npm install -g yarn"
RUN echo 'source $HOME/.profile' >> $HOME/.zshrc

RUN curl https://bootstrap.pypa.io/get-pip.py -o /tmp/get-pip.py
RUN python3.10 /tmp/get-pip.py
RUN echo 'export PATH=$HOME/.local/bin:$PATH' >> $HOME/.zshrc
RUN sh -c ". $HOME/.profile \
    && pip install --user cmake"

RUN git clone https://github.com/saturn-xiv/ashoka.git $HOME/workspace/ashoka

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
