#ifndef HardwareFfmpegDecoder_H
#define HardwareFfmpegDecoder_H
#include "FfmpegDecoder.h"
#include <libavutil/hwcontext.h>

class FfmpegHardwareDecoder: private FfmpegDecoder {
    public:
        enum Device{SOFTWARE, HARDWARE} device;
        static std::vector<std::string> getSupportedDevices();
        FfmpegHardwareDecoder(Codec codec, Device device):codec(codec), device(device){};
        bool init(std::string type);
        AVPixelFormat print_avaliable_pixel_formats_for_hardware(struct AVCodecContext *avctx, const AVPixelFormat *fmt);
        AVPixelFormat get_format(struct AVCodecContext *s, const AVPixelFormat *fmt);
    private:
    	AVBufferRef 	*avBufferRef;

};

#endif //HardwareFfmpegDecoder_H