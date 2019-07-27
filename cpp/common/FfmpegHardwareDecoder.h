#ifndef HardwareFfmpegDecoder_H
#define HardwareFfmpegDecoder_H
#include "FfmpegDecoder.h"
#include <libavutil/hwcontext.h>

class FfmpegHardwareDecoder: private FfmpegDecoder {
    public:
        enum Device{SOFTWARE, HARDWARE} device;
        static std::vector<std::string> getSupportedDevices();
        FfmpegHardwareDecoder(Codec codec, Device device):device(device){
            this->codec = codec;
        };
        bool init(std::string type);
        AVPixelFormat print_avaliable_pixel_formats_for_hardware(struct AVCodecContext *avctx, const AVPixelFormat *fmt);
        AVPixelFormat get_format(struct AVCodecContext *s, const AVPixelFormat *fmt);
        bool hardwareDecode(uint8_t* frameBuffer, int frameLength);
    private:
    	AVBufferRef 	*avBufferRef;
        AVPacket         avPacket;
        enum AVPixelFormat avPixelFormat;
        //Callback of avCodecContext which selects the correct pixel format. TODO: write a better definition for this
        enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts)
        {
            const enum AVPixelFormat *p;

            for (p = pix_fmts; *p != -1; p++) {
                if (*p == avPixelFormat)
                    return *p;
            }

            fprintf(stderr, "Failed to get HW surface format.\n");
            return AV_PIX_FMT_NONE;
        }

};

#endif //HardwareFfmpegDecoder_H