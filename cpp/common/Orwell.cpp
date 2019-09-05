#include "Orwell.h"

Orwell::Orwell(RTSPUrl rtspUrl)
{
    rtspClient = std::make_shared<MyRTSPClient>(rtspUrl.url);
    //FIFO encoded and decoder
    encodedPacketsFifo = std::make_shared<ThreadSafeDeque<EncodedPacket>>();
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<DecodedFrame>>();
    //Decoders
    //auto ffmpegHardwareDecoder = std::make_shared<FfmpegHardwareDecoder>(Decoder::H264, FfmpegHardwareDecoder::HARDWARE, std::string("cuda"));
    auto ffmpegSoftwareDecoder = std::make_shared<FfmpegSoftwareDecoder>(Decoder::H264);
    //auto nvDecoder = std::make_shared<NVDecoder>(NVDecoder::NALU,Decoder::H264);
    //Decoder specific configuration
    decoder = ffmpegSoftwareDecoder;
    decoder->setEncodedPacketsFifo(encodedPacketsFifo);
    decoder->setDecodedFramesFifo(decodedFramesFifo);
    //Important, only start decoderThread after inserting FIFOs like in above
    decoder->startThreadMode();
    //    decoderThread = std::make_shared<std::thread>(&Decoder::run, decoder);
    //RTSP client
    rtspClient->setEncodedPacketsFifo(encodedPacketsFifo);
    rtspClientThread = std::make_shared<std::thread>(&RTSPClient::run, rtspClient);
}

Orwell::Orwell(RTSPUrl rtspUrl, std::shared_ptr<Decoder> _decoder)
{
    rtspClient = std::make_shared<MyRTSPClient>(rtspUrl.url);
    //FIFO encoded and decoder
    encodedPacketsFifo = std::make_shared<ThreadSafeDeque<EncodedPacket>>();
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<DecodedFrame>>();
    //Decoders
    //Decoder specific configuration
    decoder = std::make_shared<Decoder>(_decoder);
    decoder->setEncodedPacketsFifo(encodedPacketsFifo);
    decoder->setDecodedFramesFifo(decodedFramesFifo);
    //Important, only start decoderThread after inserting FIFOs like in above
    decoder->startThreadMode();
    //decoderThread = std::make_shared<std::thread>(&Decoder::run, decoder);
    //RTSP client
    rtspClient->setEncodedPacketsFifo(encodedPacketsFifo);
    rtspClientThread = std::make_shared<std::thread>(&RTSPClient::run, rtspClient);
}

//C interface
void *orwell_init_from_rtsp(char *rtspUrl)
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