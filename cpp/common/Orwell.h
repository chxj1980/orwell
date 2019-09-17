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
#include "ThreadSafeDequePolicies.h"
#include "Renderer.h"
#include "FileWriter.h"

//#include "Profiler.h"
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
    explicit Orwell(std::string cameraAlias, std::shared_ptr<RTSPClient> _rtspClient, 
               std::shared_ptr<Decoder> _decoder, std::shared_ptr<Renderer> _renderer);
    //explicit Orwell(OnvifURL onvifURL);
    std::shared_ptr<RTSPClient> rtspClient;
    std::shared_ptr<ThreadSafeDeque<std::shared_ptr<EncodedPacket>>> encodedPacketsFifo;
    std::shared_ptr<ThreadSafeDeque<std::shared_ptr<EncodedPacket>>> encodedPacketsCacheFifo;
    //std::shared_ptr<ThreadSafeDequePolicy<std::shared_ptr<EncodedPacket>>> encodedPacketsCacheFifoPolicy;
    std::shared_ptr<ThreadSafeDeque<std::shared_ptr<DecodedFrame>>> decodedFramesFifo;
    std::shared_ptr<FileWriter> fileWriter;
    std::shared_ptr<Decoder> decoder;
    std::shared_ptr<VideoRecorder> videoRecorder;
    std::shared_ptr<Renderer> renderer;
    //static std::shared_ptr<ProfilingThread> profilingThread;
};

class ProfilingThread : public Stoppable
{
public:
    ProfilingThread();
    void run();
    //void addProfilerVariable(std::shared_ptr<ProfilerVariableInterface> profilerVariable)
    //{
    //    std::unique_lock<std::mutex> lock{mutex};
    //    profilerVariables.push_back(profilerVariable);
    //}

private:
    std::thread thread;
    //std::mutex mutex;
    //std::list<std::shared_ptr<ProfilerVariableInterface>> profilerVariables;
};
/*
//C interface 
EXTERNC enum Decoder {
    FFMPEG,
    MEDIACODEC,
    NVDEC
};

EXTERNC enum RtspClient {
    MY_RTSP_CLIENT
};

EXTERNC struct Config{
    Decoder decoder;
    RtspClient rtspClient;
};

EXTERNC void* orwell_init_from_rtsp(Config* config, char* rtspUrl);
//EXTERN C void* orwell_init_from_onvif(char *onvifUrl);
EXTERNC void orwell_destroy(void* mytype);
EXTERNC void orwell_doit(void* self, int param);
*/
//#undef EXTERNC
#endif //Orwell_H