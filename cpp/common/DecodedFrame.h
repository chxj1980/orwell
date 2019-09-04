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

class QueueableBuffer
{
    /*
        Subclass this to add your own custom properties, and 
        remember that DecodedFrame uses a pointer to this class
        for a very important reason: to avoid object slicing and
        to call queue() on the class that derives from this
    */
public:
    /*
        Some decoder generate buffers that are reusable. That is,
        once we consume them, we must queue them back to be filled
        again in the future. We call it on DecodedFrame's destructor
        so when DecodedFrame goes out of scope, the buffer is queued 
        back to its decoder.
    */
    virtual int queue() = 0;
};

class DecodedFrame
{
public:
    DecodedFrame() {}
    DecodedFrame(const DecodedFrame &decodedFrame) = delete;
    DecodedFrame &operator=(const DecodedFrame &) = delete;
    DecodedFrame(DecodedFrame &&) = default;
    DecodedFrame &operator=(DecodedFrame &&) = default;
    //DecodedFrame &operator=(DecodedFrame &&) = delete;
    ~DecodedFrame() {
        int ret = queueableBuffer->queue();
        //TODO: make this use SLog
        if (ret<0)
            std::cout << "queueableBuffer on ~DecodedFrame() gone wrong: " << ret << std::endl;
    }
    enum
    {
        FFMPEG,
        MEDIA_CODEC,
        NVDECODER
    } decodedFrom;
    /*
        In this container we store a flag when some object consumes this object.
        Then, some process somewhere (yet to be defined) will delete this frame
        from the decoded frames FIFO only if all flags have been set. 
        This prevents the frame to go away before all consumers consume its data. 
    */
    //std::unordered_set<std::string> consumedBy;
    std::unique_ptr<AVFrame, AVFrameDeleter> avFrame;
    std::unique_ptr<QueueableBuffer> queueableBuffer;
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
    //FFmpeg specific
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
