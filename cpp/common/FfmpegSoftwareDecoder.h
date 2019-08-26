#ifndef FfmpegSoftwareDecoder_H
#define FfmpegSoftwareDecoder_H
#include "FfmpegDecoder.h"

class FfmpegSoftwareDecoder: public FfmpegDecoder {
    public:
        FfmpegSoftwareDecoder(Codec codec);
        int decodeFrame(uint8_t* frameBuffer, int frameLength);
        int decodeFrame(uint8_t* frameBuffer, int frameLength, DecodedFrame& frame);
    private:
    	
};

#endif //FfmpegSoftwareDecoder_H