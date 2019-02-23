FROM ubuntu:bionic
MAINTAINER Lucas Zanella (me@lucaszanella.com)
#https://askubuntu.com/questions/508503/whats-the-development-package-for-qt5-in-14-04
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    cmake build-essential \
    libavcodec-dev libavformat-dev libavdevice-dev libavfilter-dev libavutil-dev \
    qt5-default qml-module-qtquick-controls2 libqt5qml5 qml-module-qtquick2 qml-module-qtquick-window2

RUN apt-get update && apt install -y qtmultimedia5-dev qtbase5-dev libqt5multimedia5-plugins libqt5opengl5-dev

RUN apt-get install -y libboost-dev libboost-thread-dev

WORKDIR /home/orwell

ENTRYPOINT "/bin/bash"
