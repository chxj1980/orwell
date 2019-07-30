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
#pkgconf
RUN echo "APT::Acquire::Retries \"20\";" > /etc/apt/apt.conf.d/80-retries \
    && dpkg --add-architecture i386 && apt-get update \
    && apt-get install -y build-essential git yasm unzip wget sysstat nasm libc6:i386 \
                       libavcodec-dev libavformat-dev libavutil-dev g++ freeglut3-dev \ 
                       libx11-dev libxmu-dev libxi-dev libglu1-mesa libglu1-mesa-dev \
                       cmake autoconf automake build-essential libass-dev libfreetype6-dev libgpac-dev \
                       libsdl1.2-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev \
                       libxcb-xfixes0-dev texi2html zlib1g-dev pkg-config \
                       libsdl2-dev libxcb1-dev libxcb-shm0-dev libxcb-xfixes0-dev texinfo \
                       ladspa-sdk ladspa-foo-plugins libbluray-dev libbluray-doc libbluray2 libbluray-bin
 #Install pkg-config? It breaks dependencies with pkgconf : Breaks: pkg-config (>= 0.29-1) \ Breaks: pkg-config:i386 (>= 0.29-1)

WORKDIR /home/sources

#--------------- Cuda installation
#Method 1:
ADD deps/cuda-repo-ubuntu1804_10.1.168-1_amd64.deb cuda.deb
RUN dpkg -i cuda.deb \
    && apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/7fa2af80.pub\
    && apt-get update && apt-get install -y cuda #&& nvidia-smi #output driver information

#Method 2:
#ARG CUDA_BIG_DEB_FILE=https://developer.nvidia.com/compute/cuda/10.1/Prod/local_installers/cuda-repo-ubuntu1804-10-1-local-10.1.168-418.67_1.0-1_amd64.deb
#TODO: check hash
#RUN wget --progress=bar:force -O cuda.deb $CUDA_BIG_DEB_FILE \
#     && dpkg -i cuda.deb \
#     && apt-key add /var/cuda-repo-*/7fa2af80.pub \
#     && apt-get update \
#     && apt-get install -y cuda

#PATH=/usr/local/cuda-10.1/bin${PATH:+:${PATH}} 
#LD_LIBRARY_PATH=/usr/local/cuda-10.1/lib {LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

ENV PATH="/usr/local/cuda-10.1/bin:${PATH}"
ENV LD_LIBRARY_PATH="/usr/local/cuda-10.1/lib:${LD_LIBRARY_PATH}"

#--------------- NVIDIA video codec installation
#Method 1: 
#Download youself? You have to register into nvidia website to download it

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

#--------------- MFX Dispatch installation
COPY deps/mfx_dispatch-1.25.tar.gz mfx_dispatch-1.25.tar.gz
RUN tar -xvzf mfx_dispatch-1.25.tar.gz \
    && cd mfx_dispatch-1.25 \
    && autoreconf -fiv \
    && ./configure \
    && make -j $(nproc) \
    && make install \
    && ldconfig

RUN apt-get update && apt-get install -y \
    ocl-icd-opencl-dev opencl-headers libva-dev vainfo \
    vdpau-driver-all mesa-vdpau-drivers vdpau-va-driver libvdpau-dev vdpauinfo

#--------------- X264 installation
#apt-get install -y cmake mercurial
#RUN git clone https://code.videolan.org/videolan/x264.git \ 
COPY deps/x264-snapshot-20190729-2245-stable.tar.bz2 x264-snapshot-20190729-2245-stable.tar.bz2
RUN tar -xf x264-snapshot-20190729-2245-stable.tar.bz2 \
    && cd x264-snapshot-20190729-2245-stable \
    && ./configure --disable-cli --enable-static --enable-shared --enable-strip \
    && make && make install && ldconfig

#--------------- XFDK-AAC installation
#RUN git clone https://github.com/mstorsjo/fdk-aac
COPY deps/fdk-aac-2.0.0.tar.gz fdk-aac-2.0.0.tar.gz
RUN tar xf fdk-aac-2.0.0.tar.gz \
    && cd fdk-aac-2.0.0 \
    && autoreconf -fiv \
    && ./configure \
    && make -j $(nproc) \
    && make install \
    && ldconfig

#--------------- LIBVPX installation
RUN apt-get install -y libmp3lame-dev libopus-dev
#RUN wget wget http://storage.googleapis.com/downloads.webmproject.org/releases/webm/libvpx-1.5.0.tar.bz2
COPY deps/libvpx-1.5.0.tar.bz2 libvpx-1.5.0.tar.bz2
RUN tar xjvf libvpx-1.5.0.tar.bz2 \
    && cd libvpx-1.5.0 \
    && ./configure --disable-examples --disable-unit-tests \
    && make -j $(nproc) \
    && make install \
    && ldconfig

#libssh-dbg
RUN apt-get install -y sox libsox-dev libsox-fmt-all libsox3 libsoxr-dev libsoxr-lsr0 \
    libssh-dev libssh2-1-dev libssh2-1-dbg libssl-dev openssl \
    libtheora-dev libwavpack-dev libwavpack1 libxvidcore-dev libxvidcore4 \
    libvorbis-dev libopenal-dev librtmp-dev rtmpdump
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