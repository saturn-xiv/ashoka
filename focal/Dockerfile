FROM ubuntu:latest
LABEL maintainer="Jeremy Zheng"

ENV DEBIAN_FRONTEND noninteractive

RUN apt update
RUN apt -y install software-properties-common
RUN apt update
RUN apt -y upgrade
RUN apt -y install zsh git locales pkg-config rsync openssh-client \
    vim sudo tzdata pwgen curl zip unzip wget \
    build-essential libtool automake autoconf binutils libtool automake autoconf cmake clang \
    python3 python3-distutils python3-pip
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

RUN curl https://bootstrap.pypa.io/get-pip.py -o /tmp/get-pip.py
RUN python3 /tmp/get-pip.py
RUN sh -c ". $HOME/.profile \
    && pip install --user cmake \
    && pip install --user conan"

# https://github.com/nvm-sh/nvm
RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.36.0/install.sh | sh
RUN sh -c ". $HOME/.profile \
    && nvm install node"
RUN sh -c ". $HOME/.profile \
    && npm install -g yarn"
RUN echo 'source $HOME/.profile' >> $HOME/.zshrc

RUN git clone https://github.com/saturn-xiv/ashoka.git $HOME/workspace/ashoka
RUN sh -c ". $HOME/.profile \
    && cd $HOME/workspace/ashoka \
    && git checkout cpp \
    && git pull \
    && cd $HOME/workspace/ashoka/focal/conan/recipes/hiredis && conan create . jeremy/local \
    && cd $HOME/workspace/ashoka/focal/conan/recipes/libsodium && conan create . jeremy/local \
    && cd $HOME/workspace/ashoka/focal/conan/recipes/jwtpp && conan create --build=missing . jeremy/local \
    && mkdir $HOME/workspace/ashoka/build && cd $HOME/workspace/ashoka/build && conan install --build=missing .. --profile ../focal/conan/profiles/x86_64"

ADD packages /opt/packages
RUN sudo mkdir -p /opt/toolchains && sudo chown -R deploy:deploy /opt/toolchains

RUN mkdir $HOME/downloads
# https://www.linaro.org/downloads/
ENV LINARO "linaro"
RUN mkdir -pv /opt/toolchains/${LINARO}
RUN tar xf /opt/packages/${LINARO}/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar -C /opt/toolchains/${LINARO}
RUN tar xf /opt/packages/${LINARO}/sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf.tar -C /opt/toolchains/${LINARO}

# http://wiki.friendlyarm.com/wiki/index.php/NanoPi_Duo2
ENV NANO_DUO2 "nano-duo2"
RUN mkdir -pv /opt/toolchains/${NANO_DUO2}
RUN tar xf /opt/packages/${NANO_DUO2}/gcc-linaro-arm.tar.xz -C /opt/toolchains/${NANO_DUO2}
RUN tar xf /opt/packages/${NANO_DUO2}/rootfs_friendlycore_4.14.tgz -C /opt/toolchains/${NANO_DUO2}
RUN tar xf /opt/packages/${NANO_DUO2}/arm-cortexa9-linux-gnueabihf-4.9.3-20160512.tar.xz -C /opt/toolchains/${NANO_DUO2}

ADD conan /opt/conan
VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
