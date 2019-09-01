#ifndef FfmpegSoftwareDecoder_H
#define FfmpegSoftwareDecoder_H
#include "FfmpegDecoder.h"

class FfmpegSoftwareDecoder: public FfmpegDecoder {
    public:
        FfmpegSoftwareDecoder(Codec codec);
        int decodeFrame(EncodedPacket& encodedPacket);
        int decodeFrame(EncodedPacket& encodedPacket, DecodedFrame& decodedFrame);
    private:
    	
};

#endif //FfmpegSoftwareDecoder_H