FROM ubuntu:16.04

RUN apt-get update \
 && apt-get -y install ffmpeg vlc-nox mediatomb-daemon locales

RUN locale-gen en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

VOLUME ["/var/log/mediatomb"]
VOLUME ["/var/lib/mediatomb"]

RUN sed -i 's/geteuid/getppid/' /usr/bin/vlc