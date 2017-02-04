FROM ubuntu:16.04

RUN locale-gen en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

RUN apt-get update \
 && apt-get -y install transmission-daemon

CMD /usr/bin/transmission-daemon  --foreground -g /etc/transmission-daemon
