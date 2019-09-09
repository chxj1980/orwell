#include "Orwell.h"
const int maxEncodedPacketFifoSize = 40;
const int maxDecodedFrameFifoSize = 15;
static std::shared_ptr<ThreadSafeDequePolicy<EncodedPacket>> encodedPacketFifoSizePolicy = std::make_shared<SizePolicy<EncodedPacket>>(maxEncodedPacketFifoSize);
static std::shared_ptr<ThreadSafeDequePolicy<DecodedFrame>> decodedFrameFifoSizePolicy = std::make_shared<SizePolicy<DecodedFrame>>(maxDecodedFrameFifoSize);

Orwell::Orwell(std::shared_ptr<RTSPClient> _rtspClient, std::shared_ptr<Decoder> _decoder)
{
    rtspClient = _rtspClient;
    //FIFOs for encoded and decoder
    encodedPacketsFifo = std::make_shared<ThreadSafeDeque<EncodedPacket>>();
    encodedPacketsFifo->setPolicy(encodedPacketFifoSizePolicy);
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<DecodedFrame>>();
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