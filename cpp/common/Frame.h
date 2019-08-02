#ifndef Frame_H
#define Frame_H
#include <iostream>
//Even if part of the project does not use ffmpeg, this particular header should be included
//as it is very useful
#include <libavutil/pixfmt.h>
#define FRAME_CHANNELS_SIZE 8
//TODO: add audio stuff too. See https://libav.org/documentation/doxygen/master/structAVFrame.html
struct Frame
{
    //frameBuffer is an array of pointers just as in AvFrame from ffmpeg
    uint8_t* frameBuffer[FRAME_CHANNELS_SIZE];
    int linesize [FRAME_CHANNELS_SIZE];
    unsigned int linesize;
    unsigned int frameLength;
    AVPixelFormat format;
    unsigned int width;
    unsigned int height;
};

#endif // Frame_h
