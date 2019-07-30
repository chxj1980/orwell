source load_variables.sh

TARGET="aarch64-linux-android"
CC_FLAGS="-target aarch64-none-linux-android -mfpu=neon -mfloat-abi=soft"
PLATFORM_ARCH="arm64"
CONFIGURATION="$CONFIGURATION --disable-pthreads"
TOOLCHAIN_FOLDER=$TARGET

source load_variables2.sh
ARCH="arm64-v8a"
PREFIX="android/$ARCH"
echo "cd /home/sources/ffmpeg && ./configure --prefix="/home/deps/build/ffmpeg/arm64" \
$CONFIGURATION \
--ar=$AR --strip=$STRIP --ld=$LD --cc=$CC --cxx=$CXX --as=$AS \
--target-os=android \
--extra-cflags="$CC_FLAGS -I$SYSROOT/usr/include/$TARGET $CFLAGS" \
--extra-ldflags="-L$NDK/toolchains/$TOOLCHAIN_FOLDER-4.9/prebuilt/$HOST/lib/gcc/$TARGET/4.9.x -L$NDK/platforms/android-$LEVEL/arch-$PLATFORM_ARCH/usr/lib $LDFLAGS" \
--sysroot=$SYSROOT && make -j4 && make install"

cd /home/sources/ffmpeg && ./configure \
--prefix="/home/deps/build/ffmpeg/arm64" \
--cross-prefix=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android- \
--sysroot=$SYSROOT \
--disable-static \
--disable-doc \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-symver \
--enable-shared \
--enable-protocol=concat \
--enable-protocol=file \
--enable-muxer=mp4 \
--enable-demuxer=mpegts \
--target-os=android \
--enable-decoder=h264 \
--enable-cross-compile \
--arch=aarch64 \
--toolchain=clang-usan \
--extra-cflags="$CC_FLAGS -I$SYSROOT/usr/include/$TARGET $CFLAGS" \
--extra-ldflags="-L$NDK/toolchains/$TOOLCHAIN_FOLDER-4.9/prebuilt/$HOST/lib/gcc/$TARGET/4.9.x -L$NDK/platforms/android-$LEVEL/arch-$PLATFORM_ARCH/usr/lib $LDFLAGS" \
--ar=$AR --strip=$STRIP --ld=$LD --cc=$CC --cxx=$CXX --as=$AS

cat /home/sources/ffmpeg/ffbuild/config.log

#
#cd deps/ffmpeg && ./configure --prefix="/home/deps/build/ffmpeg/arm64" \
#--sysroot="${NDK}/sysroot" \
#--target-os=linux \
#--arch=arm \
#--enable-cross-compile \
#--cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
#--enable-pic \
#--enable-shared \
#--pkg-config=$(which pkg-config) \
#--extra-cflags="-fpic -marm" && make -j4 && make install