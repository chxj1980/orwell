FROM ubuntu:bionic
MAINTAINER Lucas Zanella (me@lucaszanella.com)

RUN dpkg --add-architecture arm64
#&& echo "deb http://ports.ubuntu.com/ubuntu-ports/ bionic main restricted\n\
#deb-src http://ports.ubuntu.com/ubuntu-ports/ bionic main restricted\n\
#deb http://ports.ubuntu.com/ubuntu-ports/ bionic-updates main restricted\n\
#deb-src http://ports.ubuntu.com/ubuntu-ports/ bionic-updates main restricted\n\
#deb http://ports.ubuntu.com/ubuntu-ports/ bionic universe\n\
#deb-src http://ports.ubuntu.com/ubuntu-ports/ bionic universe\n\
#deb http://ports.ubuntu.com/ubuntu-ports/ bionic-updates universe\n\
#deb-src http://ports.ubuntu.com/ubuntu-ports/ bionic-updates universe\n\
#deb http://ports.ubuntu.com/ubuntu-ports/ bionic multiverse\n\
#deb-src http://ports.ubuntu.com/ubuntu-ports/ bionic multiverse\n\
#deb http://ports.ubuntu.com/ubuntu-ports/ bionic-updates multiverse\n\
#deb-src http://ports.ubuntu.com/ubuntu-ports/ bionic-updates multiverse"\
#| tee -a /etc/apt/sources.list 

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    cmake build-essential ca-certificates git \
    gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf \
    libgtkmm-3.0-dev libgtkmm-3.0-dev:arm64

#CMAKE INSTALLATION. TODO: make it download a stable version, don't use git clone
#RUN git clone https://github.com/Kitware/CMake \
#    && cd CMake && ./bootstrap && make && make install \
#    && cd .. && rm -rf CMake

#RTSP library needs a user on linux
RUN useradd -ms /bin/bash dev
