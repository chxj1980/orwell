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

class ReusableBuffer
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
    virtual int giveBack() = 0;
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
    ~DecodedFrame()
    {
        if (reusableBuffer)
        {
            int ret = reusableBuffer->giveBack();
            //TODO: make this use SLog
            if (ret < 0)
                std::cout << "reusableBuffer on ~DecodedFrame() gone wrong: " << ret << std::endl;
        }
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
    std::unique_ptr<ReusableBuffer> reusableBuffer;

public:
    int width;
    int height;
    int format;
    int linesize[AV_NUM_DATA_POINTERS];
    uint8_t *buffer[AV_NUM_DATA_POINTERS];
};

#endif // Frame_h
