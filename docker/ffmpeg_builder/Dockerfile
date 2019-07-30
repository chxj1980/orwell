# Lucas Zanella - lucaszanella.com
# FFmpeg builder with hardware support and support for cross compiling for Android (and iOS in the future)
#
#https://arstech.net/compile-ffmpeg-with-nvenc-h264/ #didn't work, I used
#https://developer.nvidia.com/ffmpeg
#https://gist.github.com/Brainiarc7/3f7695ac2a0905b05c5b #TODO: dockerize this
#https://gist.github.com/Brainiarc7/eb45d2e22afec7534f4a117d15fe6d89 #or this
#https://maniaclander.blogspot.com/2017/08/ffmpeg-with-pi-hardware-acceleration.html

#Do not change the ubuntu version as software installed here depends on that
FROM ubuntu:bionic

ENV DEBIAN_FRONTEND noninteractive

RUN echo "APT::Acquire::Retries \"20\";" > /etc/apt/apt.conf.d/80-retries \
    && dpkg --add-architecture i386 && apt-get update \
    && apt-get install -y build-essential git yasm unzip wget sysstat nasm libc6:i386 \
                       libavcodec-dev libavformat-dev libavutil-dev pkgconf g++ freeglut3-dev \
                       libx11-dev libxmu-dev libxi-dev libglu1-mesa libglu1-mesa-dev \
                       cmake autoconf automake build-essential libass-dev libfreetype6-dev libgpac-dev \
                       libsdl1.2-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev \
                       libxcb-xfixes0-dev texi2html zlib1g-dev
 #Install pkg-config? It breaks dependencies with pkgconf : Breaks: pkg-config (>= 0.29-1) \ Breaks: pkg-config:i386 (>= 0.29-1)

WORKDIR /home/sources

ADD deps/cuda-repo-ubuntu1804_10.1.168-1_amd64.deb cuda.deb

#--------------- Cuda installation

#Method 1:
#RUN dpkg -i cuda.deb \
#    && apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/7fa2af80.pub\
#    && apt-get update && apt-get install -y cuda #&& nvidia-smi #output driver information

#Method 2:
ARG CUDA_BIG_DEB_FILE=https://developer.nvidia.com/compute/cuda/10.1/Prod/local_installers/cuda-repo-ubuntu1804-10-1-local-10.1.168-418.67_1.0-1_amd64.deb
#TODO: check hash
RUN wget --progress=bar:force -O cuda.deb $CUDA_BIG_DEB_FILE \
     && dpkg -i cuda.deb \
     && apt-key add /var/cuda-repo-*/7fa2af80.pub \
     && apt-get update \
     && apt-get install -y cuda

#PATH=/usr/local/cuda-10.1/bin${PATH:+:${PATH}} 
#LD_LIBRARY_PATH=/usr/local/cuda-10.1/lib {LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

ENV PATH="/usr/local/cuda-10.1/bin:${PATH}"
ENV LD_LIBRARY_PATH="/usr/local/cuda-10.1/lib:${LD_LIBRARY_PATH}"

#--------------- NVIDIA video codec installation
#Method 1: 
#Download youself?

#Method 2:
ADD deps/Video_Codec_SDK_9.0.20.zip video_codec.zip
RUN unzip video_codec.zip && cp Video_Codec_SDK_9.0.20/Samples/common.mk /usr/local/include/

#--------------- NVIDIA Sdk installation
#Method 1:
#RUN git clone https://git.videolan.org/git/ffmpeg/nv-codec-headers.git \
#    cd nv-codec-headers && make && make install

#Method 2:
ADD deps/nv-codec-headers nv-codec-headers/
RUN cd nv-codec-headers && make && make install

#RUN git clone https://code.videolan.org/videolan/x264.git \ 
ADD deps/x264 x264
RUN cd x264/ \
    && ./configure --disable-cli --enable-static --enable-shared --enable-strip\
    && make && make install && ldconfig

#--------------- NASM installation
#Method 1:
#RUN wget http://www.nasm.us/pub/nasm/releasebuilds/2.14rc0/nasm-2.14rc0.tar.gz \
#    && tar xzvf nasm-2.14rc0.tar.gz \
#    && cd nasm-2.14rc0 \
#    && ./configure --prefix="/home/ffmpeg" --bindir="$HOME/bin" \
#    && make -j$(nproc) \
#    && make -j$(nproc) install \
#    && make -j$(nproc) distclean 

#Method 2
#ADD nasm-2.14rc0.tar.gz nasm.tar.gz
#RUN tar xzvf nasm.tar.gz \
#    && cd nasm-* \
#    && ./configure --prefix="/home/ffmpeg" --bindir="$HOME/bin" \
#    && make -j$(nproc) \
#    && make -j$(nproc) install \
#    && make -j$(nproc) distclean 

#--------------- Android NDK installation
ARG ANDROID_NDK=https://dl.google.com/android/repository/android-ndk-r20-linux-x86_64.zip
RUN wget --progress=bar:force -O android-ndk.zip $ANDROID_NDK \
    && unzip android-ndk.zip && rm android-ndk.zip && cd android-ndk-r20

ENV NDK=/home/sources/android-ndk-r20

COPY deps/ffmpeg-4.1.4.tar.bz2 ffmpeg-4.1.4.tar.bz2
RUN tar jxf ffmpeg-4.1.4.tar.bz2 \
    && rm ffmpeg-4.1.4.tar.bz2 \
    && mv ffmpeg-4.1.4 ffmpeg

#ADD docker/ffmpeg_builder/scripts/build_all.sh /home/build_all.sh
ADD docker/ffmpeg_builder/scripts/video_decoders_list.txt /home/video_decoders_list.txt
ADD docker/ffmpeg_builder/scripts/ffmpeg_build.sh /home/ffmpeg_build.sh
#ADD docker/ffmpeg_builder/scripts/load_variables.sh /home/load_variables.sh
#ADD docker/ffmpeg_builder/scripts/load_variables2.sh /home/load_variables2.sh
#ADD docker/ffmpeg_builder/scripts/build_linux.sh /home/build_linux.sh 
#ADD docker/ffmpeg_builder/scripts/build_android_arm64.sh /home/build_android_arm64.sh 
#ADD docker/ffmpeg_builder/scripts/build_android_armhf32.sh /home/build_android_armhf32.sh 
#ADD docker/ffmpeg_builder/scripts/build_android_x86_64.sh /home/build_android_x86_64.sh 
#ADD docker/ffmpeg_builder/scripts/build_windows.sh /home/build_windows.sh 

WORKDIR /home/

#You should run this inside:
#./configure --prefix="/home/ffmpeg/binaries"  --enable-cuda-sdk --enable-cuvid --enable-nvenc --enable-nonfree --enable-libnpp --extra-cflags=-I/usr/local/cuda/include --extra-ldflags=-L/usr/local/cuda/lib64
#TODO: add --enable-opengl