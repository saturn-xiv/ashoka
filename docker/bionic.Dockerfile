FROM ubuntu:bionic
LABEL maintainer="Jeremy Zheng"

ENV DEBIAN_FRONTEND noninteractive

ADD scripts /ashoka
RUN /ashoka/ubuntu.sh

USER deploy
RUN sh -c "$( https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
RUN /ashoka/deploy.sh

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/zsh", "-l"]
