#TODO: verify if --enable-cuda-sdk is working
cd /home/sources/ffmpeg && ./configure --prefix="/home/deps/build/ffmpeg/linux" \
--enable-cuvid \
--enable-nvenc \
--enable-nonfree \
--enable-libnpp \
--enable-cuda-sdk \
--extra-cflags=-I/usr/local/cuda/include \
--extra-ldflags=-L/usr/local/cuda/lib64 && make -j4 && make install

#TODO: add opengl??