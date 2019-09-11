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
    DecodedFrame(const std::shared_ptr<DecodedFrame> decodedFrame) = delete;
    DecodedFrame &operator=(const DecodedFrame &) = delete;
    DecodedFrame(DecodedFrame &&) = default;
    DecodedFrame &operator=(DecodedFrame &&) = default;
    //DecodedFrame &operator=(DecodedFrame &&) = delete;
    /*
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
    */
    enum
    {
        FFMPEG,
        MEDIA_CODEC,
        NVDECODER
    } decodedFrom;
    virtual uint8_t *getPointer(int plane) {};

    virtual int getLineSize(int plane) {};

    virtual int getWidth(int plane) {}

    virtual int getHeight(int plane) {}

    //Format number of the enum
    virtual int getFormat() {}

};

#endif // Frame_h
