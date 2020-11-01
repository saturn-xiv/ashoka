FROM ubuntu:bionic
LABEL maintainer="Jeremy Zheng"

ENV DEBIAN_FRONTEND noninteractive

ADD scripts /ashoka
RUN /ashoka/ubuntu.sh

USER deploy
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
RUN /ashoka/deploy.sh
ADD packages /opt/packages
RUN /ashoka/cross-chain.sh

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
