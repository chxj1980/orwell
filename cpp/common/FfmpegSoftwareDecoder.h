#ifndef FfmpegSoftwareDecoder_H
#define FfmpegSoftwareDecoder_H
#include "FfmpegDecoder.h"

class FfmpegSoftwareDecoder: public FfmpegDecoder {
    public:
        FfmpegSoftwareDecoder(Codec codec);
        int sendPacket(std::shared_ptr<EncodedPacket> encodedPacket);
        int sendPacket(std::shared_ptr<EncodedPacket> encodedPacket, std::shared_ptr<DecodedFrame> decodedFrame);
    private:
    	
};

#endif //FfmpegSoftwareDecoder_H