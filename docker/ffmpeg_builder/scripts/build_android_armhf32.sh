source load_variables.sh

TARGET="aarch64-linux-android"
CC_FLAGS="-target aarch64-none-linux-android -mfpu=neon -mfloat-abi=soft"
PLATFORM_ARCH="arm64"
CONFIGURATION="$CONFIGURATION --disable-pthreads"
TOOLCHAIN_FOLDER=$TARGET

source load_variables2.sh

PREFIX="android/$ARCH"
cd /home/sources/ffmpeg && ./configure --prefix=$PREFIX \
$CONFIGURATION \
--ar=$AR --strip=$STRIP --ld=$LD --cc=$CC --cxx=$CXX --as=$AS \
--target-os=android \
--extra-cflags="$CC_FLAGS -I$SYSROOT/usr/include/$TARGET $CFLAGS" \
--extra-ldflags="-L$NDK/toolchains/$TOOLCHAIN_FOLDER-4.9/prebuilt/$HOST/lib/gcc/$TARGET/4.9.x -L$NDK/platforms/android-$LEVEL/arch-$PLATFORM_ARCH/usr/lib $LDFLAGS" \
--sysroot=$SYSROOT && make -j4 && make install


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