#TODO: see if this works on android emulator for x86_64, I don't have any idea
cd deps/ffmpeg && ./configure --prefix="/home/deps/build/ffmpeg/armx86_64_" && make -j4 && make install
#--sysroot="${NDK}/sysroot" \
#--target-os=linux \
#--arch=arm \
#--cpu=cortex-a8 \
#--enable-cross-compile \
#--enable-pic \
#--enable-shared \
#--extra-cflags="-fpic"
#TODO: add --enable-opengl

