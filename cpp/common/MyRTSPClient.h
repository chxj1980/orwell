#ifndef MyRTSPCLient_H
#define MyRTSPCLient_H
#include <iostream>
#include "Decoder.h"
#include "rtspClient.h"
#include "FrameUpdater.h"
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "EncodedPacket.h"
#include "Stoppable.h"
#include "RTSPClient.h"

class MyRTSPEncodedPacket : public EncodedPacket
{
public:
    MyRTSPEncodedPacket()
    {
    }
    MyRTSPEncodedPacket(size_t size) : size(size),
                                       frame(new uint8_t[size])
    {
    }

    /*
        Subclass these method to get the pointer to the data you need
    */
    uint8_t *getFramePointer()
    {
        return frame.get();
    }

    uint32_t getSize()
    {
        return size;
    }

    //Actual frameSize. Must be less than bufferSize obviously
    size_t size = 0;
    std::unique_ptr<uint8_t> frame;
};
/*
    RTSP client that uses MyRtspClient library
*/
class MyRTSPClient : public RTSPClient
{
public:
    RtspClient myRtspClient;
    using RTSPClient::RTSPClient;

    MyRTSPClient(std::string uri, Transport transport) : myRtspClient(uri), RTSPClient(uri, transport)
    {
    }
    MyRTSPClient(std::string uri, Transport transport, std::string user,
                 std::string password) : myRtspClient(uri), RTSPClient(uri, transport, user, password)
    {
    }

    int init();
    void run();
    int receivePacket();

private:
    bool firstConnection = true;
    //void ByeFromServerClbk();
};
#endif //MyRTSPCLient_H
