#include "Orwell.h"

Orwell::Orwell(RTSPUrl rtspUrl)
{
    rtspClient = std::make_shared<MyRTSPClient>(rtspUrl.url);
    //FIFO encoded and decoder
    encodedFramesFifo = std::make_shared<ThreadSafeDeque<EncodedFrame>>();
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<DecodedFrame>>();
    //Decoders
    //auto ffmpegHardwareDecoder = std::make_shared<FfmpegHardwareDecoder>(Decoder::H264, FfmpegHardwareDecoder::HARDWARE, std::string("cuda"));
    auto ffmpegSoftwareDecoder = std::make_shared<FfmpegSoftwareDecoder>(Decoder::H264);
    //Decoder specific configuration
    decoder = ffmpegSoftwareDecoder;
    decoder->setEncodedFramesFifo(encodedFramesFifo);
    decoder->setDecodedFramesFifo(decodedFramesFifo);
    decoderThread = std::make_shared<std::thread>(&Decoder::run, decoder);
    //RTSP client
    rtspClient->setEncodedFramesFifo(encodedFramesFifo);
    rtspClientThread = std::make_shared<std::thread>(&RTSPClient::run, rtspClient);
}