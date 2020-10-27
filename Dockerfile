FROM ubuntu:latest
LABEL maintainer="Jeremy Zheng"

ENV DEBIAN_FRONTEND noninteractive

RUN apt update
RUN apt -y install software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
RUN apt update
RUN apt -y upgrade
# https://wiki.ubuntu.com/ToolChain
RUN apt -y install zsh git locales pkg-config rsync openssh-client \
    vim sudo tzdata pwgen curl zip unzip wget \
    build-essential libtool automake autoconf binutils-multiarch cmake clang \
    libpq
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

RUN git clone https://github.com/saturn-xiv/ashoka.git $HOME/workspace/ashoka

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
