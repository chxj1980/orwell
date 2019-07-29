#TODO: see if this works on android emulator for x86_64, I don't have any idea
./configure --prefix="/home/ffmpeg/binaries_android_x86_64" 
#--sysroot="${ANDROID_NDK_PATH}/sysroot" \
#--target-os=linux \
#--arch=arm \
#--cpu=cortex-a8 \
#--enable-cross-compile \
#--enable-pic \
#--enable-shared \
#--extra-cflags="-fpic"
#TODO: add --enable-opengl

make -j4