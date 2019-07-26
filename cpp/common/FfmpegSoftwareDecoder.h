#ifndef SoftwareFfmpegDecoder_H
#define SoftwareFfmpegDecoder_H
#include "FfmpegDecoder.h"

class SoftwareFfmpegDecoder: public FfmpegDecoder {
    public:
        SoftwareFfmpegDecoder();
        bool init();
        void decodeFrame(uint8_t* frameBuffer, int frameLength);
    private:
    	
};

#endif //SoftwareFfmpegDecoder_H