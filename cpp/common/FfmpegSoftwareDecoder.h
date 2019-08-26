#ifndef FfmpegSoftwareDecoder_H
#define FfmpegSoftwareDecoder_H
#include "FfmpegDecoder.h"

class FfmpegSoftwareDecoder: public FfmpegDecoder {
    public:
        FfmpegSoftwareDecoder(Codec codec);
        int decodeFrame(EncodedFrame& encodedFrame);
        int decodeFrame(EncodedFrame& encodedFrame, DecodedFrame& decodedFrame);
    private:
    	
};

#endif //FfmpegSoftwareDecoder_H