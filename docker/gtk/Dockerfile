# Lucas Zanella - lucaszanella.com
# 

FROM ffmpeg_builder

RUN apt-get update && apt-get install -y \
    gcc make gcc-arm-linux-gnueabi binutils-arm-linux-gnueabi\
    gcc-arm-linux-gnueabihf gcc-aarch64-linux-gnu\
    libglew-dev libgtkmm-3.0-dev gdb

#RTSP(JRTPLIB) library needs a user on linux
RUN useradd -ms /bin/bash dev
USER dev

WORKDIR /home/dev/orwell/gtk/build/desktoplinux

ENTRYPOINT "/bin/bash"