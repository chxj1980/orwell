#FFmpeg builder with hardware support

FROM ubuntu:bionic

RUN dpkg --add-architecture i386 && apt-get update \
    && apt-get install build-essential git yasm unzip wget sysstat nasm libc6:i386 \
                       libavcodec-dev libavformat-dev libavutil-dev pkgconf g++ freeglut3-dev \
                       libx11-dev libxmu-dev libxi-dev libglu1-mesa libglu1-mesa-dev \
                       cmake

ADD ../deps/cuda-repo-ubuntu1804_10.1.168-1_amd64.deb cuda.deb

RUN dpkg -i cuda.deb \
    && apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/7fa2af80.pub\
    && apt-get update && apt-get install cuda && nvidia-smi #output driver information

RUN export PATH=/usr/local/cuda-10.1/bin${PATH:+:${PATH}} \
    && export LD_LIBRARY_PATH=/usr/local/cuda-10.1/lib {LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

ADD ../deps/Video_Codec_SDK_9.0.20.zip video_codec.zip
RUN unzip video_codec.zip && cp Video_Codec_SDK_9.0.20/Samples/common.mk /usr/local/include/

RUN git clone https://git.videolan.org/git/ffmpeg/nv-codec-headers.git \
    && cd nv-codec-headers && make && make install

RUN git clone https://code.videolan.org/videolan/x264.git \ 
    && cd x264/ \
    && ./configure --disable-cli --enable-static --enable-shared --enable-strip\
    && make && make install && ldconfig

