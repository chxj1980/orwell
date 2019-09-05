#ifndef Orwell_H
#define Orwell_H
#include <iostream>
#include <memory>
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "RTSPClient.h"
#include "MyRTSPClient.h"
#include "FfmpegHardwareDecoder.h"
#include "FfmpegSoftwareDecoder.h"
#include "VideoReceiver.h"
#include <string>
#include <iostream>
//#include <GL/glew.h>
//#include <GL/glx.h>
#include <thread>
#include <mutex>
#include <thread>
#include <mutex>
#include <deque>
#include "DecodedFrame.h"
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif
class RTSPUrl
{
public:
    RTSPUrl(std::string url) : url(url) {}
    std::string url;
};
/*
    Represents an Orwell instance. It's a collection of media stream,
    decoder, FIFOs and threads that deals with one stream (one camera)
*/
class Orwell
{
public:
    explicit Orwell(RTSPUrl rtspUrl);
    explicit Orwell(RTSPUrl rtspUrl,  Decoder&& _decoder);
    //explicit Orwell(OnvifURL onvifURL);

    std::shared_ptr<RTSPClient> rtspClient;
    std::shared_ptr<std::thread> rtspClientThread;
    std::shared_ptr<ThreadSafeDeque<EncodedPacket>> encodedPacketsFifo;
    std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo;
    std::shared_ptr<Decoder> decoder;
    //std::shared_ptr<std::thread> decoderThread;
    std::shared_ptr<VideoRecorder> videoRecorder;
};

//C interface 
EXTERNC void* orwell_init_from_rtsp(char *rtspUrl);
//EXTERN C void* orwell_init_from_onvif(char *onvifUrl);
EXTERNC void orwell_destroy(void* mytype);
EXTERNC void orwell_doit(void* self, int param);

#undef EXTERNC
#endif //Orwell_H