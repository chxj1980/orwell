#ifndef FfmpegSoftwareDecoder_H
#define FfmpegSoftwareDecoder_H
#include "FfmpegDecoder.h"

class FfmpegSoftwareDecoder: public FfmpegDecoder {
    public:
        FfmpegSoftwareDecoder(Codec codec) {
            this->codec = codec;
        }
        bool init();
        void decodeFrame(uint8_t* frameBuffer, int frameLength);
    private:
    	
};

#endif //FfmpegSoftwareDecoder_H