#include "Orwell.h"
//No more than x encoded packets
const int maxEncodedPacketFifoSize = 40;
//No more than x decoded video frames
const int maxDecodedFrameFifoSize = 15;
std::shared_ptr<ThreadSafeDequePolicy<std::shared_ptr<EncodedPacket>>> encodedPacketFifoSizePolicy = std::make_shared<SizePolicy<std::shared_ptr<EncodedPacket>>>(maxEncodedPacketFifoSize);
std::shared_ptr<ThreadSafeDequePolicy<std::shared_ptr<DecodedFrame>>> decodedFrameFifoSizePolicy = std::make_shared<SizePolicy<std::shared_ptr<DecodedFrame>>>(maxDecodedFrameFifoSize);
std::shared_ptr<ProfilingThread> Orwell::profilingThread = std::make_shared<ProfilingThread>();

Orwell::Orwell(std::shared_ptr<RTSPClient> _rtspClient, std::shared_ptr<Decoder> _decoder)
{
    rtspClient = _rtspClient;
    //FIFOs for encoded and decoder
    encodedPacketsFifo = std::make_shared<ThreadSafeDeque<std::shared_ptr<EncodedPacket>>>();
    encodedPacketsFifo->setPolicy(encodedPacketFifoSizePolicy);
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<std::shared_ptr<DecodedFrame>>>();
    decodedFramesFifo->setPolicy(decodedFrameFifoSizePolicy);
    //Decoders
    decoder = _decoder;
    decoder->setEncodedPacketsFifo(encodedPacketsFifo);
    decoder->setDecodedFramesFifo(decodedFramesFifo);
    //Important, only start thread mode after inserting FIFOs like in above
    decoder->startThreadMode();
    //RTSP client
    rtspClient->setEncodedPacketsFifo(encodedPacketsFifo);
    //Important, only start thread mode after inserting FIFOs like in above
    rtspClient->startThreadMode();
    //profilingThread->addProfilerVariable(rtspClient->bytesPerSecond);
    //profilingThread->addProflingVariable(renderer->fps);

    //rtspClientThread = std::make_shared<std::thread>(&RTSPClient::run, rtspClient);
}
/*
//C interface
void *orwell_init_from_rtsp(char *rtspUrl)
{
    //RTSPUrl rtspUrlObject(rtspUrl);
    //return new Orwell(rtspUrlObject);
}

void* orwell_init_from_onvif(char *onvifUrl)
{
    RTSPUrl onvifUrlObject(onvifUrl);
    return new Orwell(onvifUrlObject);
}


void orwell_destroy(void *untyped_ptr)
{
    Orwell *typed_ptr = static_cast<Orwell *>(untyped_ptr);
    delete typed_ptr;
}

void orwell_doit(void *untyped_self, int param)
{
    Orwell *typed_self = static_cast<Orwell *>(untyped_self);
    //typed_self->doIt(param);
}
*/