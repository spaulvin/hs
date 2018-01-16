FROM ubuntu:16.04

# Run a quick apt-get update/upgrade
RUN apt-get update && apt-get upgrade -y && apt-get autoremove -y --purge

# Install dependencies
RUN apt-get install -y nut nut-server

# Run as root
USER root

# Setup the default timezone
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone