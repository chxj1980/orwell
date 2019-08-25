#include "Orwell.h"

Orwell::Orwell(RTSPUrl rtspUrl)
{
    mediaStream = std::make_shared<MediaStream>(rtspUrl.url);
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
    mediaStream->setDecoder(decoder);
    mediaStream->setEncodedFramesFifo(encodedFramesFifo);
    mediaStreamThread = std::make_shared<std::thread>(&MediaStream::run, mediaStream);
}