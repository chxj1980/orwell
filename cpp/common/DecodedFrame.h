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

class DecodedFrame
{
public:
    DecodedFrame() {}
    DecodedFrame(const DecodedFrame &decodedFrame) = delete;
    DecodedFrame &operator=(const DecodedFrame &) = delete;
    DecodedFrame(DecodedFrame &&) = default;
    DecodedFrame &operator=(DecodedFrame &&) = default;
    //DecodedFrame &operator=(DecodedFrame &&) = delete;
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
    int width()
    {
        if (decodedFrom == FFMPEG)
        {
            return avFrame->width;
        }
        else
        {
            return 0;
        }
    }
    int height()
    {
        if (decodedFrom == FFMPEG)
        {
            return avFrame->height;
        }
        else
        {
            return 0;
        }
    }
    int format()
    {
        if (decodedFrom == FFMPEG)
        {
            return avFrame->format;
        }
        else
        {
            return 0;
        }
    }
    int linesize(int i)
    {
        if (decodedFrom == FFMPEG)
        {
            return avFrame->linesize[i];
        }
        else
        {
            return 0;
        }
    }
    uint8_t *buffer(int i)
    {
        if (decodedFrom == FFMPEG)
        {
            return avFrame->data[i];
        }
        else
        {
            return NULL;
        }
    }
};

#endif // Frame_h
