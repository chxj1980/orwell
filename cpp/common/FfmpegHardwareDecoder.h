#ifndef HardwareFfmpegDecoder_H
#define HardwareFfmpegDecoder_H
#include "FfmpegDecoder.h"
//Extern C is absolutely needed!
extern "C"
{
#include <libavutil/hwcontext.h>
#include <libavutil/imgutils.h>
}

class FfmpegHardwareDecoder : public FfmpegDecoder
{
public:
    enum Device
    {
        SOFTWARE,
        HARDWARE
    } device;
    static std::vector<std::string> getSupportedDevices();
    FfmpegHardwareDecoder(Codec codec, Device device, std::string hardwareType);

    //Decodes to GPU memory but not get it back to CPU memory
    int hardwareDecode(EncodedUnit& encodedUnit);
    /* 
            Calls hardwareDecode() then gets video from GPU memory to CPU memory and 
            sends to VideoReceiver instance.
        */
    int decodeFrame(EncodedUnit& encodedUnit);
    int decodeFrame(EncodedUnit& encodedUnit, DecodedFrame &decodedFrame);
    //
    AVPixelFormat print_avaliable_pixel_formats_for_hardware(struct AVCodecContext *avctx, const AVPixelFormat *fmt);
    AVPixelFormat get_format(struct AVCodecContext *s, const AVPixelFormat *fmt);


private:
    //AVBufferRef *avBufferRef;
    std::unique_ptr<AVBufferRef, AVBufferRefDeleter> avBufferRef;

    //AVFrame *decodedAvFrame = NULL;
    std::unique_ptr<AVFrame, AVFrameDeleter> decodedAvFrame;
    std::unique_ptr<AVFrame, AVFrameDeleter> fromGPUAvFrame;

    //AVFrame         *fromGPUAvFrame = NULL;
    //AVFrame         *tmp_frame = NULL;
    //AVPacket avPacket;
    std::unique_ptr<AVPacket, AVPacketDeleter> avPacket {new AVPacket()};

    //Image from GPU memory gets stored here
    uint8_t *buffer = NULL;
    AVPixelFormat avPixelFormat;
    std::string hardwareType;
};

#endif //HardwareFfmpegDecoder_H