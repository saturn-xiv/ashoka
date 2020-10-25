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

RUN apt update
RUN apt -y upgrade
RUN apt -y install zsh build-essential git cmake clang locales pkg-config \
    vim sudo tzdata pwgen curl zip unzip wget
RUN apt-get -y autoremove
RUN apt-get -y clean

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

ADD vcpkg.sh /vcpkg.sh
# RUN bash /vcpkg.sh

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
