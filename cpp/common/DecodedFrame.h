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
    DecodedFrame(const std::shared_ptr<DecodedFrame> decodedFrame) = delete;
    DecodedFrame &operator=(const DecodedFrame &) = delete;
    DecodedFrame(DecodedFrame &&) = default;
    DecodedFrame &operator=(DecodedFrame &&) = default;
    
    enum
    {
        FFMPEG,
        MEDIA_CODEC,
        NVDECODER
    } decodedFrom;

    virtual int getWidth() {}

    virtual int getHeight() {}

    //Format number of the enum
    virtual int getFormat() {}

};

#endif // Frame_h
