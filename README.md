# Orwell Network Video Recorder

Orwell is meant to be a modern Network Video Recorder built on top of React Native with support for Windows, Android, iOS, macOS and Linux.

Orwell is intended to implement the video receiver and decoder in C++ so it is the same in all devices. Only the displaying of the video is specific for each device. In case of desktop, it's QT based, in Android it's Java based, and in iOS it's C++/Objective-C based. The user interface will be coded in react native so the same code will work on all devices, with the difference only being in the mobile/desktop layouts.

# What works rigth now?

I'm testing multiple implementations on various plataforms so nothing works rigth now. It kinda works, but I don't have the time to explain rigth now. Wait a few days.

# How to build

See the dockerfile in `docker` folder to understand how your system might look like for the installation to work, or follow below:

In Ubuntu, you should install these:

```
sudo apt update && sudo apt install -y cmake build-essential ca-certificates \
    libavcodec-dev libavformat-dev libavdevice-dev libavfilter-dev libavutil-dev libboost-dev libboost-thread-dev \
    libgl-dev perl python git wget unzip 
```

and also have a modern QT installtion on `/home/$USER`. Explained below.

## QT little app (debugging only)

This little app is used for debugging. It runs the QT module that is injected into React Native Desktop (which is based on QT).

#### If you have an updated QT installed on your machine (like Qt 5.11 or later)

For the little QT/QML visualizer (just for debugging):

```
cd qt
cmake .
make
./orwell
```
#### If you have locally installed cmake or want to point the locally installed QT directly:

If you don't have qt already, download here: https://www.qt.io/download-qt-installer

Download a modern cmake, the one that comes with ubuntu, for me, won't set `DCMAKE_PREFIX_PATH`: https://cmake.org/download/ (don't forget to `make install`)

So, if you want to pass QT folder manually (change QT folder to tour version):

```
cd qt
cmake -DCMAKE_PREFIX_PATH=/home/$USER/Qt/5.13.0/gcc_64 -DCMAKE_BUILD_TYPE=Debug .
make
./orwell
```

ps: you must have cameras accessible with the IPs and passwords of `qt/main.qml`.

Warning: it'll likely not work with your cameras for now, but it should work with vstarcam cameras

## React Native Desktop app

```
cd orwellJS
#1st shell
npm install
react-native desktop #important
npm start #starts bundler. 
#2nd shell
node ubuntu-server.js #starts js server
#3rd shell
react-native run-desktop
```

ps: you must have cameras accessible with the IPs and passwords of `orwellJS/index.desktop.js` (or maybe other places for now)

Warning: it'll likely not work with your cameras for now, but it should work with vstarcam cameras

## React Native Android app

To be done.

## React Native iOS app

To be done.
