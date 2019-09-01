#include "Orwell.h"

Orwell::Orwell(RTSPUrl rtspUrl)
{
    rtspClient = std::make_shared<MyRTSPClient>(rtspUrl.url);
    //FIFO encoded and decoder
    encodedUnitsFifo = std::make_shared<ThreadSafeDeque<EncodedUnit>>();
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<DecodedFrame>>();
    //Decoders
    //auto ffmpegHardwareDecoder = std::make_shared<FfmpegHardwareDecoder>(Decoder::H264, FfmpegHardwareDecoder::HARDWARE, std::string("cuda"));
    auto ffmpegSoftwareDecoder = std::make_shared<FfmpegSoftwareDecoder>(Decoder::H264);
    //Decoder specific configuration
    decoder = ffmpegSoftwareDecoder;
    decoder->setEncodedUnitsFifo(encodedUnitsFifo);
    decoder->setDecodedFramesFifo(decodedFramesFifo);
    //Important, only start decoderThread after inserting FIFOs like in above
    decoderThread = std::make_shared<std::thread>(&Decoder::run, decoder);
    //RTSP client
    rtspClient->setEncodedUnitsFifo(encodedUnitsFifo);
    rtspClientThread = std::make_shared<std::thread>(&RTSPClient::run, rtspClient);
}

//C interface
void* orwell_init_from_rtsp(char *rtspUrl)
{
    RTSPUrl rtspUrlObject(rtspUrl);
    return new Orwell(rtspUrlObject);
}
/* 
void* orwell_init_from_onvif(char *onvifUrl)
{
    RTSPUrl onvifUrlObject(onvifUrl);
    return new Orwell(onvifUrlObject);
}
*/

void orwell_destroy(void* untyped_ptr)
{
    Orwell *typed_ptr = static_cast<Orwell *>(untyped_ptr);
    delete typed_ptr;
}

void orwell_doit(void* untyped_self, int param)
{
    Orwell *typed_self = static_cast<Orwell *>(untyped_self);
    //typed_self->doIt(param);
}