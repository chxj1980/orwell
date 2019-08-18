#ifndef Frame_H
#define Frame_H
#include <iostream>
#include <memory>
#include <unordered_set>
//Even if part of the project does not use ffmpeg, this particular header should be included
//as it is very useful
extern "C"
{
#include <libavutil/pixfmt.h>
#include <libavutil/frame.h>
}
#include "FfmpegDeleters.h"

class Frame
{
public:
    enum
    {
        FFMPEG,
        MEDIA_CODEC
    } decodedFrom;
    /*
        In this container we store a flag when some object consumes this object.
        Then, some process somewhere (yet to be defined) will delete this frame
        from the decoded frames FIFO only if all flags have been set. 
        This prevents the frame to go away before all consumers consume its data. 
    */
    std::unordered_set<std::string> consumedBy;
    std::unique_ptr<AVFrame, AVFrameDeleter> avFrame;
    int width() {
        if (decodedFrom==FFMPEG) {
            return avFrame->width;
        }
    }
    int height() {
        if (decodedFrom==FFMPEG) {
            return avFrame->height;
        }
    }
    int format() {
        if (decodedFrom==FFMPEG) {
            return avFrame->format;
        }
    }
    int linesize(int i) {
        if (decodedFrom==FFMPEG) {
            return avFrame->linesize[i];
        }
    }
    uint8_t* buffer(int i) {
        if (decodedFrom==FFMPEG) {
            return avFrame->data[i];
        }
    }
    
};

#endif // Frame_h
