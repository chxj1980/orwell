HOST=linux-x86_64
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/$HOST
LLVM_TOOLCHAIN="$NDK/toolchains/llvm/prebuilt/$HOST/bin" # location of clang binary
SYSROOT="$NDK/sysroot" # sysroot for the cross-building
CFLAGS="-O3 -fPIC"
LDFLAGS="-lc"
LDFLAGS="" #Why?
CONFIGURATION="--disable-asm --enable-cross-compile --disable-static --disable-programs --disable-doc --enable-shared --enable-protocol=file --enable-pic --enable-small"
