#ifndef Frame_H
#define Frame_H
#include <iostream>
//Even if part of the project does not use ffmpeg, this particular header should be included
//as it is very useful
#include <libavutil/pixfmt.h>
#include <unordered_set>
#include <libavutil/frame.h>
#include "FfmpegDecoder.h"
class Frame
{
public:
    /*
        In this container we store a flag when some object consumes this object.
        Then, some process somewhere (yet to be defined) will delete this frame
        from the decoded frames FIFO only if all flags have been set. 
        This prevents the frame to go away before all consumers consume its data. 
    */
    std::unordered_set<std::string> consumedBy;
    enum
    {
        FFMPEG,
        MEDIA_CODEC
    } decodedFrom;
    std::unique_ptr<AVFrame, AVFrameDeleter> avFrame;
    ~Frame()
    {
        /*
            for (int i=0; i<=FRAME_CHANNELS_SIZE; i++) {
                //if (buffer[i]) 
                    delete buffer[i];
            }
            */
    }
};

#endif // Frame_h
