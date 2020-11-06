FROM alpine:latest
LABEL maintainer="Jeremy Zheng"

RUN apk update
RUN apk add git curl vim zsh pwgen sudo build-base \
    postgresql-dev boost-dev \
    python3 py3-pip cmake pkgconf linux-headers

# deploy user
RUN adduser -s /bin/zsh -D deploy
RUN echo 'deploy ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/101-deploy
USER deploy

# https://github.com/ohmyzsh/ohmyzsh
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

RUN pip install --user conan
RUN echo 'export PATH=$HOME/.local/bin:$PATH' >> $HOME/.profile

# https://github.com/nvm-sh/nvm
# RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.35.3/install.sh | sh
# RUN sh -c ". $HOME/.profile \
#     && nvm install node"
# RUN sh -c ". $HOME/.nvm/nvm.sh \
#     && npm install -g yarn"

RUN echo 'source $HOME/.profile' >> $HOME/.zshrc

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
