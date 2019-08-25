#ifndef Orwell_H
#define Orwell_H
#include <iostream>
#include <memory>
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "MediaStream.h"
#include "FfmpegHardwareDecoder.h"
#include "FfmpegSoftwareDecoder.h"
#include "Singleton.h"
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
class RTSPUrl
{
public:
    RTSPUrl(std::string url) : url(url) {}
    std::string url;
}
/*
    Represents an Orwell instance. It's a collection of media stream,
    decoder, FIFOs and threads that deals with one stream (one camera)
*/
class Orwell
{
public:
    Orwell(RTSPUrl rtspUrl);
    //Orwell(OnvifURL onvifURL);

    std::shared_ptr<MediaStream> mediaStream;
    std::shared_ptr<std::thread> mediaStreamThread;
    std::shared_ptr<ThreadSafeDeque<EncodedFrame>> encodedFramesFifo;
    std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo;
    std::shared_ptr<Decoder> decoder;
    std::shared_ptr<std::thread> decoderThread;
    //std::shared_ptr<VideoReceiver> videoReceiver;
    std::shared_ptr<VideoRecorder> videoRecorder;
    //std::shared_ptr<MovementTracker> movementTracker
};

#endif //Orwell_H