#FFmpeg builder with hardware support
#https://arstech.net/compile-ffmpeg-with-nvenc-h264/
#https://maniaclander.blogspot.com/2017/08/ffmpeg-with-pi-hardware-acceleration.html
#Do not change the ubuntu version as software installed here depends on that
FROM ubuntu:bionic
#ARG CUDA_BIG_DEB_FILE=https://developer.nvidia.com/compute/cuda/10.1/Prod/local_installers/cuda-repo-ubuntu1804-10-1-local-10.1.168-418.67_1.0-1_amd64.deb
#Number of retries for atp
RUN echo "APT::Acquire::Retries \"20\";" > /etc/apt/apt.conf.d/80-retries 

RUN dpkg --add-architecture i386 && apt-get update \
    && apt-get install -y build-essential git yasm unzip wget sysstat nasm libc6:i386 \
                       libavcodec-dev libavformat-dev libavutil-dev pkgconf g++ freeglut3-dev \
                       libx11-dev libxmu-dev libxi-dev libglu1-mesa libglu1-mesa-dev \
                       cmake

WORKDIR /home/tmp_build

ADD cuda-repo-ubuntu1804_10.1.168-1_amd64.deb cuda.deb

ENV DEBIAN_FRONTEND noninteractive

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
     && apt-get update
RUN  apt-get install -y cuda

#ENV PATH=/usr/local/cuda-10.1/bin${PATH:+:${PATH}} 
#ENV LD_LIBRARY_PATH=/usr/local/cuda-10.1/lib {LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

ENV PATH="/usr/local/cuda-10.1/bin:${PATH}"
ENV LD_LIBRARY_PATH="/usr/local/cuda-10.1/lib:${LD_LIBRARY_PATH}"

RUN echo $PATH && echo $LD_LIBRARY_PATH 

#--------------- NVIDIA video codec installation
#Method 1:
#Download youself?

#Method 2:
ADD Video_Codec_SDK_9.0.20.zip video_codec.zip
RUN unzip video_codec.zip && cp Video_Codec_SDK_9.0.20/Samples/common.mk /usr/local/include/

#--------------- NVIDIA Sdk installation
#Method 1:
#RUN git clone https://git.videolan.org/git/ffmpeg/nv-codec-headers.git \
#    cd nv-codec-headers && make && make install

#Method 2:
ADD nv-codec-headers nv-codec-headers/
RUN cd nv-codec-headers && make && make install

#RUN git clone https://code.videolan.org/videolan/x264.git \ 
ADD x264 x264
RUN cd x264/ \
    && ./configure --disable-cli --enable-static --enable-shared --enable-strip\
    && make && make install && ldconfig

