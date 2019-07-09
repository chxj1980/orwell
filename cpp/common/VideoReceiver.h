#ifndef VideoReceiver_H
#define VideoReceiver_H

class VideoReceiver
{
    public:
        virtual void receiveVideo(unsigned char**videoBuffer, int frameWidth, int frameHeight) = 0;
};

#endif  //VideoReceiver_H