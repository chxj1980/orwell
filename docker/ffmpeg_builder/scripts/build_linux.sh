#TODO: verify if --enable-cuda-sdk is working
cd deps/ffmpeg && ./configure --prefix="/home/deps/build/ffmpeg_binaries_linux" \
--enable-cuvid \
--enable-nvenc \
--enable-nonfree \
--enable-libnpp \
--enable-cuda-sdk \
--extra-cflags=-I/usr/local/cuda/include \
--extra-ldflags=-L/usr/local/cuda/lib64 && make -j4 && make install

#TODO: add opengl??