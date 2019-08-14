#ifndef Frame_H
#define Frame_H
#include <iostream>
//Even if part of the project does not use ffmpeg, this particular header should be included
//as it is very useful
#include <libavutil/pixfmt.h>
#include <unordered_set>
//Defined as AV_NUM_DATA_POINTERS in ffmpeg
#define FRAME_CHANNELS_SIZE 8
//TODO: add audio stuff too. See https://libav.org/documentation/doxygen/master/structAVFrame.html
class Frame
{
    public:
        ~Frame() {
            for (int i=0; i<=FRAME_CHANNELS_SIZE; i++) {
                //if (buffer[i]) 
                    delete buffer[i];
            }
        }
        /*
            In this container we store a flag when some object consumes this object.
            Then, some process somewhere (yet to be defined) will delete this frame
            from the decoded frames FIFO only if all flags have been set. 
            This prevents the frame to go away before all consumers consume its data. 
        */
        std::unordered_set<std::string> consumedBy;
        /* 
            This class supports holding frames decoded from ffmpeg and android's media codec.
            In each case, different class properties are used.
        */
        enum {FFMPEG, MEDIA_CODEC} decodedFrom;
        //frameBuffer is an array of pointers just as in AvFrame from ffmpeg
        uint8_t* buffer[FRAME_CHANNELS_SIZE]; //ffmpeg-specific //https://stackoverflow.com/questions/29297809/determining-size-of-data0-in-avframe-of-ffmpeg
        int linesize [FRAME_CHANNELS_SIZE]; //ffmpeg-specific
        unsigned int lenght; //ffmpeg-specific
        AVPixelFormat format; //ffmpeg-specific
        unsigned int width; //ffmpeg-specific
        unsigned int height; //ffmpeg-specific
        
};

#endif // Frame_h
