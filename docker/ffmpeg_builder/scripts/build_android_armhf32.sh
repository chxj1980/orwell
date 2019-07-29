cd deps/ffmpeg && ./configure --prefix="/home/deps/build/ffmpeg_binaries_arm32" \
--sysroot="${ANDROID_NDK_PATH}/sysroot" \
--target-os=linux \
--arch=arm \
--cpu=cortex-a8 \
--enable-cross-compile \
--enable-pic \
--enable-shared \
--extra-cflags="-fpic" && make -j4 && make install