#ifndef MediaCodecDecoder_H
#define MediaCodecDecoder_H


#include <iostream>
#include <memory>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaExtractor.h>
#include "VideoRecorder.h"
#include "Decoder.h"
#include "FfmpegDeleters.h"
#include "SLog.h"

SLOG_CATEGORY("MediaCodecDecoder");


class MediaCodecDecoder: public Decoder
{
public:
    MediaCodecDecoder(Codec codec);
    int sendPacket(std::shared_ptr<EncodedPacket> encodedPacketh) = 0;
    int sendPacket(std::shared_ptr<EncodedPacket> encodedPacket, std::shared_ptr<DecodedFrame> decodedFrame) = 0;

protected:
    //std::unique_ptr<AMediaExtractor> aMediaExtractor = std::make_unique<AMediaExtractor>(AMediaExtractor_new());
    std::unique_ptr<AMediaCodec> aMediaCodec;// = std::make_unique<AMediaCodec>();
};

#endif // MediaCodecDecoder_H
