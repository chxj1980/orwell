#!/bin/bash
echo "Building Desktop Ffmpeg binaries..."
sh /home/build_linux.sh
echo "Building Android ARM 64 Ffmpeg binaries..."
sh /home/build_android_arm64.sh
echo "Building Android ARMHF 32 Ffmpeg binaries..."
sh /home/build_android_armhf32.sh
echo "Building Android X86_64 Ffmpeg binaries..."
sh /home/build_android_armhf32.sh
echo "Building Windows Ffmpeg binaries..."
sh /home/build_windows.sh