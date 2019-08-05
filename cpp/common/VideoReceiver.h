#ifndef VideoReceiver_H
#define VideoReceiver_H
#include "Frame.h"
class VideoReceiver
{
    public:
        virtual int receiveVideo(Frame* frame) = 0;
};

#endif  //VideoReceiver_H