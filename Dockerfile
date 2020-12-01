FROM ubuntu:xenial
LABEL maintainer="Jeremy Zheng"

RUN apt update
# RUN apt -y install software-properties-common
# RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
# RUN apt update
RUN apt -y upgrade
RUN apt -y install zsh git locales rsync openssh-client \
    lsb-release vim sudo tzdata pwgen curl zip unzip wget yasm \
    build-essential pkg-config libtool automake autoconf binutils cmake \
    # python3-distutils
    python3 python3-pip \ 
    binutils-multiarch \
    g++-arm-linux-gnueabihf pkg-config-arm-linux-gnueabihf binutils-arm-linux-gnueabihf

# https://wiki.ubuntu.com/ToolChain
RUN dpkg --add-architecture armhf
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $(lsb_release -cs) main restricted universe multiverse" > /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $(lsb_release -cs)-updates main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $(lsb_release -cs)-security main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $(lsb_release -cs) main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $(lsb_release -cs)-security main restricted universe multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $(lsb_release -cs)-updates main restricted universe multiverse" >> /etc/apt/sources.list

RUN apt update
RUN apt -y autoremove
RUN apt -y clean

RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
RUN locale-gen
RUN update-locale LANG=en_US.UTF-8
RUN update-alternatives --set editor /usr/bin/vim.basic

# https://www.linaro.org/downloads/
# ADD tmp/packages /opt/packages
# RUN tar xf /opt/packages/linaro/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar -C /opt
# RUN tar xf /opt/packages/linaro/sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf.tar -C /opt
# RUN tar xf /opt/packages/linaro/runtime-gcc-linaro-7.5.0-2019.12-arm-linux-gnueabihf.tar -C /opt

# deploy user
RUN useradd -m deploy -s /bin/zsh
RUN passwd -l deploy
RUN echo 'deploy ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/101-deploy
USER deploy

# https://github.com/ohmyzsh/ohmyzsh
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# https://github.com/nvm-sh/nvm
RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.37.2/install.sh | sh
RUN sh -c ". $HOME/.profile \
    && nvm install node"
RUN sh -c ". $HOME/.profile \
    && npm install -g yarn"

# https://www.rust-lang.org/tools/install
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

RUN sh -c ". $HOME/.cargo/env \
    && rustup target add armv7-unknown-linux-gnueabihf"

RUN echo 'source $HOME/.profile' >> $HOME/.zshrc

RUN sh -c "git clone -b cpp https://github.com/saturn-xiv/ashoka.git $HOME/ashoka"

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]


