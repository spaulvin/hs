FROM ubuntu:16.04

# Run a quick apt-get update/upgrade
RUN apt-get update && apt-get upgrade -y && apt-get autoremove -y --purge

# Install dependencies
RUN apt-get install -y wget

# Run as root
USER root

# Setup the default timezone
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Prepare Ivideon repository
RUN wget http://packages.ivideon.com/ubuntu/keys/ivideon.list -O /etc/apt/sources.list.d/ivideon.list && \
  wget -O - http://packages.ivideon.com/ubuntu/keys/ivideon.key | apt-key add - && \
  apt-get update

RUN apt-get install ivideon-server-headless -y

# Setup default environment variables for the server
ENV USER "root"

ENTRYPOINT ["/opt/ivideon/ivideon-server/videoserver"]