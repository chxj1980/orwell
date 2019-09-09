#include "Orwell.h"
const int maxEncodedPacketFifoSize = 40;
const int maxDecodedFrameFifoSize = 15;
static std::shared_ptr<SizePolicy> encodedPacketFifoSizePolicy = std::make_shared<SizePolicy>(maxEncodedPacketFifoSize);
static std::shared_ptr<SizePolicy> decodedFrameFifoSizePolicy = std::make_shared<SizePolicy>(maxDecodedFrameFifoSize);

Orwell::Orwell(RTSPClient _rtspClient, std::shared_ptr<Decoder> _decoder)
{
    rtspClient = _rtspClient;
    //FIFOs for encoded and decoder
    encodedPacketsFifo = std::make_shared<ThreadSafeDeque<EncodedPacket>>();
    encodedPacketsFifo.setPolicy(encodedPacketFifoSizePolicy);
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<DecodedFrame>>();
    decodedFramesFifo.setPolicy(decodedFrameFifoSizePolicy);
    //Decoders
    decoder = _decoder;
    decoder->setEncodedPacketsFifo(encodedPacketsFifo);
    decoder->setDecodedFramesFifo(decodedFramesFifo);
    //Important, only start decoderThread after inserting FIFOs like in above
    decoder->startThreadMode();
    //RTSP client
    rtspClient->setEncodedPacketsFifo(encodedPacketsFifo);
    rtspClientThread = std::make_shared<std::thread>(&RTSPClient::run, rtspClient);
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