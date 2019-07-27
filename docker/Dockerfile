FROM ubuntu:bionic
MAINTAINER Lucas Zanella (me@lucaszanella.com)
#https://askubuntu.com/questions/508503/whats-the-development-package-for-qt5-in-14-04

#TODO: make download secure
ARG QT_SOURCE_LINK=http://download.qt.io/official_releases/qt/5.11/5.11.3/single/qt-everywhere-src-5.11.3.zip
ARG QT_INSTALLER_LINK=http://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run

#TODO: add the things below to compile ffmpeg with hardware support
#dpkg --add-architecture i386
#apt-get update
#apt-get install build-essential git yasm unzip wget sysstat nasm libc6:i386 \
#libavcodec-dev libavformat-dev libavutil-dev pkgconf g++ freeglut3-dev \
#libx11-dev libxmu-dev libxi-dev libglu1-mesa libglu1-mesa-dev
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    cmake build-essential ca-certificates yasm\
    libavcodec-dev libavformat-dev libavdevice-dev libavfilter-dev libavutil-dev libboost-dev libboost-thread-dev \
    libgl-dev perl python git wget unzip 

#CMAKE INSTALLATION. TODO: make it download a stable version, don't use git clone
RUN git clone https://github.com/Kitware/CMake \
    && cd CMake && ./bootstrap && make && make install \
    && cd .. && rm -rf CMake

#RTSP library needs a user on linux, it's also the place where QT gets installed, don't change it
RUN useradd -ms /bin/bash dev

#Installs modern qt, ubuntu's one is old 
RUN wget --progress=bar:force -O qt.run $QT_INSTALLER_LINK
ADD qt-installer-noninteractive.qs /home/qt-installer-noninteractive.qs
RUN chmod +x qt.run && ./qt.run -platform minimal --verbose --script /home/qt-installer-noninteractive.qs 

WORKDIR /home/dev/orwell
USER dev

ENTRYPOINT "/bin/bash"


# For when Ubuntu gets sufficiently modern QT:
#RUN apt-get update && apt install -y qtmultimedia5-dev qtbase5-dev libqt5multimedia5-plugins libqt5opengl5-dev qt5-default qml-module-qtquick-controls2 libqt5qml5 qml-module-qtquick2 qml-module-qtquick-window2
# TODO  && echo "Unzipping..." && unzip -qq qt.zip && ls


# Download qt source and build it (too much building, so we're not using it)
#RUN wget --progress=bar:force -O qt.tar.xz $make && mkdir qt
#RUN echo "Unzipping..." && tar xf qt.tar.xz -C qt --strip-components=1 \
#    && rm qt.tar.xz
#RUN cd qt && ls && ./configure \
#    -confirm-license -opensource \
#    -nomake examples -nomake tests -no-compile-examples \
#    -no-xcb \
#    -prefix "/usr/local/Qt" \
#    && make && make install \
#    && cd .. \
#    && rm -rf qt