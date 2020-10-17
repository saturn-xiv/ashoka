# FROM alpine:latest
# LABEL maintainer="Jeremy Zheng"

# RUN apk update
# RUN apk add git curl vim zsh pwgen sudo build-base openssl-dev

# # deploy user
# RUN adduser -s /bin/bash -D deploy
# RUN echo 'deploy ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/101-deploy
# USER deploy

# # https://github.com/ohmyzsh/ohmyzsh
# RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# # https://www.rust-lang.org/tools/install
# RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

# RUN echo 'source $HOME/.profile' >> $HOME/.zshrc

# VOLUME /workspace
# WORKDIR /workspace

# # https://github.com/rust-lang/cargo/issues/7563
# ENV RUSTFLAGS="-C target-feature=-crt-static"
# CMD ["/bin/zsh", "-l"]


FROM ubuntu:latest
LABEL maintainer="Jeremy Zheng"

ENV DEBIAN_FRONTEND noninteractive

RUN apt update
RUN apt -y upgrade
RUN apt -y install zsh build-essential git cmake locales pkg-config \
    vim sudo tzdata pwgen curl wget \
    libpq-dev libsqlite3-dev libudev-dev libssl-dev libsodium-dev
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

# https://www.rust-lang.org/tools/install
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

# https://github.com/nvm-sh/nvm
RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.36.0/install.sh | sh
RUN sh -c ". $HOME/.profile \
    && nvm install node"
RUN sh -c ". $HOME/.profile \
    && npm install -g yarn"
RUN echo 'source $HOME/.profile' >> $HOME/.zshrc

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
