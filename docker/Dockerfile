FROM ubuntu:latest
LABEL maintainer="Jeremy Zheng"

ENV DEBIAN_FRONTEND noninteractive

RUN apt update
RUN apt -y upgrade
RUN apt -y install software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
RUN apt update
RUN apt -y install zsh git locales rsync openssh-client \
    vim sudo tzdata pwgen curl zip unzip wget yasm \
    build-essential pkg-config libtool automake autoconf binutils cmake clang \
    binutils-multiarch \
    g++-arm-linux-gnueabihf pkg-config-arm-linux-gnueabihf binutils-arm-linux-gnueabihf \
    mingw-w64 mingw-w64-tools binutils-mingw-w64 \
    python3 python3-pip python3-distutils python3-dev    

# https://wiki.ubuntu.com/ToolChain
# RUN dpkg --add-architecture armhf
# RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $(lsb_release -cs) main restricted universe multiverse" > /etc/apt/sources.list
# RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $(lsb_release -cs)-updates main restricted universe multiverse" >> /etc/apt/sources.list
# RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu/ $(lsb_release -cs)-security main restricted universe multiverse" >> /etc/apt/sources.list
# RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $(lsb_release -cs) main restricted universe multiverse" >> /etc/apt/sources.list
# RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $(lsb_release -cs)-security main restricted universe multiverse" >> /etc/apt/sources.list
# RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ $(lsb_release -cs)-updates main restricted universe multiverse" >> /etc/apt/sources.list

RUN apt update
RUN apt -y autoremove
RUN apt -y clean

RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
RUN locale-gen
RUN update-locale LANG=en_US.UTF-8
RUN update-alternatives --set editor /usr/bin/vim.basic

# https://www.linaro.org/downloads/
ADD packages /opt/packages
RUN tar xf /opt/packages/linaro/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar -C /opt
RUN tar xf /opt/packages/linaro/sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf.tar -C /opt
RUN tar xf /opt/packages/linaro/runtime-gcc-linaro-7.5.0-2019.12-arm-linux-gnueabihf.tar -C /opt

# deploy
RUN useradd -m deploy -s /bin/zsh
RUN passwd -l deploy
RUN echo 'deploy ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/101-deploy

USER deploy
# https://github.com/ohmyzsh/ohmyzsh
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

RUN pip3 install --user conan

# https://github.com/nvm-sh/nvm
RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.37.2/install.sh | sh
RUN sh -c ". $HOME/.profile \
    && nvm install node"

ENV ANDROID_SDK_VERSION 6858069
RUN wget -P $HOME/downloads https://dl.google.com/android/repository/commandlinetools-linux-${ANDROID_SDK_VERSION}_latest.zip
RUN mkdir -pv $HOME/local/android-sdk
RUN unzip $HOME/downloads/commandlinetools-linux-${ANDROID_SDK_VERSION}_latest.zip -d $HOME/local/android-sdk
RUN yes | $HOME/local/android-sdk/tools/bin/sdkmanager --licenses

RUN curl -s "https://get.sdkman.io" | sh
RUN sed -i -e 's/sdkman_auto_answer=false/sdkman_auto_answer=true/g' $HOME/.sdkman/etc/config
RUN zsh -c "source $HOME/.zshrc \
    && sdk install java 15.0.1-open \
    && sdk install maven \
    && sdk install gradle"

RUN echo 'source $HOME/.profile' >> $HOME/.zshrc

RUN  sh -c ". $HOME/.profile \
    && git clone -b poco https://github.com/saturn-xiv/ashoka.git $HOME/ashoka \
    && mkdir -pv $HOME/ashoka/build/gcc9 \
    && cd $HOME/ashoka/build/gcc9 \
    && conan install ../.. --profile=../../docker/conan/profiles/gcc9 --build=missing \
    && mkdir -pv $HOME/ashoka/build/linaro \
    && cd $HOME/ashoka/build/linaro \
    && conan install ../.. --profile=../../docker/conan/profiles/linaro --build=missing"

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
