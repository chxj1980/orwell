# Orwell Network Video Recorder

Orwell is meant to be a modern Network Video Recorder built on top of React Native with support for Windows, Android, iOS, macOS and Linux.

Orwell implements the video decoder and (ffmpeg) and video player in C++ for each of these devices. 

# How to build

I don't expect you to successfully build it yet because you need access to my cameras but if you insist

For the little QT/QML visualizer (just for debugging):

```
cd qt
cmake .
make
./orwell
```

ps: you must have cameras accessible with the IPs and passwords of `qt/main.qml`.

Warning: it'll likely not work with your cameras for now, but it should work with vstarcam cameras

For react native desktop building:

```
cd orwellJS
#1st shell
npm start #starts bundler
#2nd shell
node node_modules/react-native/ubuntu-server.js #starts js server
#3rd shell
react-native run-desktop
```

ps: you must have cameras accessible with the IPs and passwords of `orwellJS/index.desktop.js` (or maybe other places for now)

Warning: it'll likely not work with your cameras for now, but it should work with vstarcam cameras
