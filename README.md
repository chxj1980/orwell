# Orwell Network Video Recorder

Orwell is meant to be a modern Network Video Recorder built on top of React Native with support for Windows, Android, iOS, macOS and Linux.

Orwell implements the video decoder and (ffmpeg) and video player in C++ for each of these devices. 

# How to build

First of all, see the dockerfile in `/docker` to understand how your system might look like for the installation to work 

I don't expect you to successfully build it yet because you need access to my cameras but if you insist

For the little QT/QML visualizer (just for debugging):

```
cd qt
cmake .
make
./orwell
```

Or if you want everything manually (this is how I do because ubuntu's cmake that comes with package system has a bug n the -DCMAKE_PREFIX_PATH variable):

```
cd qt
/home/lz/cmake-3.14.5-Linux-x86_64/bin/cmake -DCMAKE_PREFIX_PATH=/home/lz/Qt/5.12.3/gcc_64 -DCMAKE_BUILD_TYPE=Debug .
make
./orwell
```

ps: you must have cameras accessible with the IPs and passwords of `qt/main.qml`.

ps: if you're me, use `/home/lz/cmake-3.14.5-Linux-x86_64/bin/cmake -DCMAKE_PREFIX_PATH=/home/lz/Qt2/5.11.2/gcc_64 -DCMAKE_BUILD_TYPE=Debug .`

Warning: it'll likely not work with your cameras for now, but it should work with vstarcam cameras

For react native desktop building:

```
cd orwellJS
#1st shell
npm install
npm start #starts bundler
#2nd shell
node node_modules/react-native/ubuntu-server.js #starts js server
#3rd shell
react-native run-desktop
```

ps: you must have cameras accessible with the IPs and passwords of `orwellJS/index.desktop.js` (or maybe other places for now)

Warning: it'll likely not work with your cameras for now, but it should work with vstarcam cameras
