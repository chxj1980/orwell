#ifndef HardwareFfmpegDecoder_H
#define HardwareFfmpegDecoder_H
#include "FfmpegDecoder.h"
#include <libavutil/hwcontext.h>
#include <libavutil/imgutils.h>

class FfmpegHardwareDecoder: public FfmpegDecoder {
    public:
        enum Device{SOFTWARE, HARDWARE} device;
        static std::vector<std::string> getSupportedDevices();
        FfmpegHardwareDecoder(Codec codec, Device device):device(device){
            this->codec = codec;
        };
        ~FfmpegHardwareDecoder(){
            av_frame_free(&decodedAvFrame);
            av_frame_free(&fromGPUAvFrame);
            //av_freep(&buffer);
        }
        bool init(std::string type);
        //Decodes to GPU memory but not get it back to CPU memory
        bool hardwareDecode(uint8_t* frameBuffer, int frameLength);
        /* 
            Calls hardwareDecode() then gets video from GPU memory to CPU memory and 
            sends to VideoReceiver instance.
        */
        void decodeFrame(uint8_t* frameBuffer, int frameLength);
        //
        AVPixelFormat print_avaliable_pixel_formats_for_hardware(struct AVCodecContext *avctx, const AVPixelFormat *fmt);
        AVPixelFormat get_format(struct AVCodecContext *s, const AVPixelFormat *fmt);
    private:
    	AVBufferRef 	*avBufferRef;
        AVFrame         *decodedAvFrame = NULL;
        AVFrame         *fromGPUAvFrame = NULL;
        AVFrame         *tmp_frame = NULL;
        AVPacket         avPacket;
        //Image from GPU memory gets stored here
        uint8_t         *buffer = NULL;
        AVPixelFormat    avPixelFormat;
};

#endif //HardwareFfmpegDecoder_H