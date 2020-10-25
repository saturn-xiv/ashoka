# FROM alpine:latest
# LABEL maintainer="Jeremy Zheng"

# RUN apk update
# RUN apk add vim bash zsh pwgen sudo openssl git tar curl zip unzip \
#     build-base cmake clang

# # deploy user
# RUN adduser -s /bin/bash -D deploy
# RUN echo 'deploy ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/101-deploy
# USER deploy

# # https://github.com/ohmyzsh/ohmyzsh
# RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# ADD vcpkg.sh /vcpkg.sh
# RUN bash /vcpkg.sh

# VOLUME /workspace
# WORKDIR /workspace

# CMD ["/bin/zsh", "-l"]


FROM ubuntu:latest
LABEL maintainer="Jeremy Zheng"

ENV DEBIAN_FRONTEND noninteractive
ENV UBUNTU focal

RUN apt update
RUN apt -y install software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
RUN apt update
RUN apt -y upgrade
# https://wiki.ubuntu.com/ToolChain
RUN apt -y install zsh git locales pkg-config rsync openssh-client \
    vim sudo tzdata pwgen curl zip unzip wget \
    python3 python3-pip \
    g++-10 \
    build-essential  binutils-multiarch cmake clang \
    linux-libc-dev-armel-cross g++-10-arm-linux-gnueabihf pkg-config-arm-linux-gnueabihf binutils-arm-linux-gnueabihf

RUN dpkg --add-architecture armhf
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $UBUNTU main restricted universe multiverse" > /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $UBUNTU-updates main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $UBUNTU-security main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $UBUNTU main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $UBUNTU-security main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $UBUNTU-updates main restricted universe multiverse" >> /etc/apt/sources.list

RUN apt update
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

RUN pip3 install --user --upgrade pip
RUN pip3 install --user conan
RUN echo 'export PATH=$HOME/.local/bin:$PATH' >> $HOME/.zshrc

RUN git clone https://github.com/saturn-xiv/ashoka.git $HOME/workspace/ashoka
RUN sh -c ". $HOME/.profile \
    && cd $HOME/workspace/ashoka \
    && git checkout cpp \
    && ./cocan.sh"

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
