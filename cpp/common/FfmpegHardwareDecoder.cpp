#include "FfmpegHardwareDecoder.h"
struct avCodecContextOpaque
{
    AVPixelFormat *avPixelFormat;
};
//Callback of avCodecContext which selects the correct pixel format. TODO: write a better definition for this
//TODO: study ifetime of AVPixelFormat
enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts)
{
    const enum AVPixelFormat *p;

    for (p = pix_fmts; *p != -1; p++)
    {
        if (*p == *(((avCodecContextOpaque *)(ctx->opaque))->avPixelFormat))
            return *p;
    }

    fprintf(stderr, "Failed to get HW surface format.\n");
    return AV_PIX_FMT_NONE;
}
/* 
AVPixelFormat FfmpegHardwareDecoder::get_format(struct AVCodecContext *s, const AVPixelFormat *fmt)
{
	//printf("chosen format: ");
	//printf((char *)*fmt);
	return AVPixelFormat::AV_PIX_FMT_0BGR;
}
*/

std::vector<std::string> FfmpegHardwareDecoder::getSupportedDevices()
{
    //Begin with any type since we're gonna iterate through all until we get back to AV_HWDEVICE_TYPE_NONE
    AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE;
    std::vector<std::string> result;
    while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
    {
        result.push_back(std::string(av_hwdevice_get_type_name(type)));
    }
    return result;
}
FfmpegHardwareDecoder::FfmpegHardwareDecoder(Codec codec, Device device, std::string hardwareType) : device(device), hardwareType(hardwareType)
{
    this->codec = codec;
    //create context here

    AVHWDeviceType aVHWDeviceType = av_hwdevice_find_type_by_name(hardwareType.c_str());
    if (aVHWDeviceType == AV_HWDEVICE_TYPE_NONE)
    {
        std::cout << "ERROR: Device " << hardwareType << "not supported. " << std::endl;
        std::cout << "List of supported devices: " << std::endl;
        for (auto i : FfmpegHardwareDecoder::getSupportedDevices())
            std::cout << i << ", ";
        std::cout << std::endl;
        std::cout << "end of list." << std::endl;
        //return 1;
    }

    switch (codec)
    {
    case H264:
        avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
        break;
    case H265:
        //avCodec = avcodec_find_decoder(AV_CODEC_ID_H265);
        std::cout << "H265 not yet supported" << std::endl;
        //return 2;
        break;
    }

    avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    avCodecContext.reset(avcodec_alloc_context3(avCodec));
    if (!(avCodecContext.get()))
    {
        std::cout << "avCodecContext error:" << AVERROR(ENOMEM) << std::endl;
        //return 3;
    }

    for (int i = 0;; i++)
    {
        const AVCodecHWConfig *config = avcodec_get_hw_config(avCodec, i);
        if (!config)
        {
            fprintf(stderr, "Decoder %s does not support device type %s.\n",
                    avCodec->name, av_hwdevice_get_type_name(aVHWDeviceType));
            //return -1;
        }
        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
            config->device_type == aVHWDeviceType)
        {
            avPixelFormat = config->pix_fmt;
            break;
        }
    }

    avCodecContextOpaque o;
    o.avPixelFormat = &avPixelFormat;
    /* 
        Uses the opaqe field of AVCodecContext to store our pixelformat so the callback 
        function get_hw_format
    */
    avCodecContext->opaque = &o;
    //This callback selects the right AvPixelFormat for the hardware decoder
    avCodecContext->get_format = get_hw_format;

    AVBufferRef *avBufferRef_ptr;
    int err = 0;
    err = av_hwdevice_ctx_create(&avBufferRef_ptr, aVHWDeviceType, NULL, NULL, 0);
    /*
        Important, immediately add our pointer to our unique_ptr container, so we don't forget
        to add later. We just didn't add it at its constrcutor because we need to pass a reference
        to it.
    */
    avBufferRef.reset(avBufferRef_ptr);
    if (err < 0)
    {
        std::cout << "av_hwdevice_ctx_create failed to create hardware device context." << std::endl;
        //return 4;
    }

    avCodecContext->hw_device_ctx = av_buffer_ref(avBufferRef.get());

    int ret = 0;
    if ((ret = avcodec_open2(avCodecContext.get(), avCodec, NULL)) < 0)
    {
        std::cout << "avcodec_open2 problem" << std::endl;
        //return 5;
    }

};

int FfmpegHardwareDecoder::hardwareDecode(EncodedPacket& encodedPacket)
{
    avPacket.get()->size = encodedPacket.frameSize;
    avPacket.get()->data = encodedPacket.frameBuffer.get();
    int ret = avcodec_send_packet(avCodecContext.get(), avPacket.get());
    if (ret < 0)
    {
        std::cout << "avcodec_send_packet error " << ret << std::endl;
        return false;
    }
    decodedAvFrame.reset(av_frame_alloc());
    fromGPUAvFrame.reset(av_frame_alloc());

    if (!(decodedAvFrame.get()) || !(fromGPUAvFrame.get()))
    {
        std::cout << "Can not alloc frame " << ret << std::endl;
        //ret = AVERROR(ENOMEM);
        //goto fail;
    }
    ret = avcodec_receive_frame(avCodecContext.get(), decodedAvFrame.get());
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
    {
        //av_frame_free(&decodedAvFrame);
        //av_frame_free(&fromGPUAvFrame);
        return 0;
    }
    else if (ret < 0)
    {
        std::cout << "avcodec_receive_frame error " << ret << std::endl;
        //goto fail;
    }
    //Decoded data is now in GPU!
    return true;
}

int FfmpegHardwareDecoder::decodeFrame(EncodedPacket& encodedPacket)
{
    DecodedFrame frame;
    frame.decodedFrom = DecodedFrame::FFMPEG;
    //Decodes video into `frame`.
    int r = decodeFrame(encodedPacket, frame);
    if (!decodedFramesFifo)
    {
        std::cerr << "No decodedFramesFifo setted in FfmpegHardwareDecoder" << std::endl;
    }
    //Adds the frame to the end of the FIFO.
    if (r != 0)
        this->decodedFramesFifo->emplace_back(std::move(frame));
    return r;
}

int FfmpegHardwareDecoder::decodeFrame(EncodedPacket& encodedPacket, DecodedFrame &decodedFrame)
{
    bool r = hardwareDecode(encodedPacket);

    if (r != 0)
        return -1;
    //Create a pointer to avFrame and put into unique_ptr immediately so we don't forget to delete it
    //AVFrame* av;
    //av = av_frame_alloc();
    std::unique_ptr<AVFrame, AVFrameDeleter> avFrameUniquePtr(av_frame_alloc());

    if (decodedAvFrame->format == avPixelFormat)
    {
        /* retrieve data from GPU to CPU */
        int ret = 0;
        if ((ret = av_hwframe_transfer_data(avFrameUniquePtr.get(), decodedAvFrame.get(), 0)) < 0)
        {
            std::cout << "av_hwframe_transfer_data error, could not transfer video from GPU memory " << ret << std::endl;
            return ret;
            //goto fail;
        }
        else
        {
            decodedFrame.avFrame = std::move(avFrameUniquePtr);
        }
        /*
            Now the decoded frame from hardware is in CPU memory, in frame struct
        */

        //FfmpegDecoder::avFrameToFrame(fromGPUAvFrame, frame);
        return 0;
        //tmp_frame = fromGPUAvFrame;
    }
    else
        std::cout << "FfmpegHardwareDecoder::decodeFrame: something is wrong, decodedAvFrame->format != avPixelFormat" << std::endl;
    return -2;
    //tmp_frame = decodedAvFrame;
    /*
    //This code is for transforming into a buffer and saving into disk
    int size = av_image_get_buffer_size(static_cast<AVPixelFormat>(tmp_frame->format), 
                                        tmp_frame->width,
                                        tmp_frame->height, 1);
    //TODO: is this casting ok??????
    buffer = (uint8_t *)av_malloc(size);
    if (!buffer) {
        fprintf(stderr, "Can not alloc buffer\n");
        //ret = AVERROR(ENOMEM);
        //goto fail;
    }
    int ret = av_image_copy_to_buffer(buffer, size,
                                 (const uint8_t * const *)tmp_frame->data,
                                 (const int *)tmp_frame->linesize, 
                                 static_cast<AVPixelFormat>(tmp_frame->format),
                                 tmp_frame->width, tmp_frame->height, 1);
    if (ret < 0) {
        fprintf(stderr, "Can not copy image to buffer\n");
        //goto fail;
    }
    */
    /*
        Most important code line. If we ended here, it means avFrame has the decoded frame.
        We just need to convert our avFrame to a generic Frame object. 
        Now caller has a video frame and can render it.
	*/

    //av_frame_unref(fromGPUAvFrame);
}

/*
    if (!(decoder_ctx = avcodec_alloc_context3(decoder)))
        return AVERROR(ENOMEM);

    video = input_ctx->streams[video_stream];
    if (avcodec_parameters_to_context(decoder_ctx, video->codecpar) < 0)
        return -1;

    decoder_ctx->get_format  = get_hw_format;

    if (hw_decoder_init(decoder_ctx, type) < 0)
        return -1;

    if ((ret = avcodec_open2(decoder_ctx, decoder, NULL)) < 0) {
        fprintf(stderr, "Failed to open codec for stream #%u\n", video_stream);
        return -1;
    }

    output_file = fopen(argv[3], "w+");

    while (ret >= 0) {
        if ((ret = av_read_frame(input_ctx, &packet)) < 0)
            break;

        if (video_stream == packet.stream_index)
            ret = decode_write(decoder_ctx, &packet);

        av_packet_unref(&packet);
    }

    packet.data = NULL;
    packet.size = 0;
    ret = decode_write(decoder_ctx, &packet);
    av_packet_unref(&packet);

    if (output_file)
        fclose(output_file);
    avcodec_free_context(&decoder_ctx);
    avformat_close_input(&input_ctx);
    av_buffer_unref(&hw_device_ctx);

    return 0;
    */

AVPixelFormat FfmpegHardwareDecoder::print_avaliable_pixel_formats_for_hardware(struct AVCodecContext *avctx, const AVPixelFormat *fmt)
{
    //const AVPixFmtDescriptor *desc;
    //const AVCodecHWConfig *config;
    int i, n;
    /* 
     // If a device was supplied when the codec was opened, assume that the
     // user wants to use it.
     if (avctx->hw_device_ctx && avctx->codec->hw_configs) {
         AVHWDeviceContext *device_ctx =
             (AVHWDeviceContext*)avctx->hw_device_ctx->data;
         for (i = 0;; i++) {
             config = &avctx->codec->hw_configs[i]->public;
             if (!config)
                 break;
             if (!(config->methods &
                   AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX))
                 continue;
             if (device_ctx->type != config->device_type)
                 continue;
             for (n = 0; fmt[n] != AV_PIX_FMT_NONE; n++) {
                 if (config->pix_fmt == fmt[n])
                     return fmt[n];
             }
         }
     }
	*/
    // No device or other setup, so we have to choose from things which
    // don't any other external information.

    // Finally, traverse the list in order and choose the first entry
    // with no external dependencies (if there is no hardware configuration
    // information available then this just picks the first entry).
    printf("supported formats are \n");
    for (n = 0; fmt[n] != AV_PIX_FMT_NONE; n++)
    {

        /* 
         for (i = 0;; i++) {
			 
             config = avcodec_get_hw_config(avctx->codec, i);
             if (!config)
                 break;
             if (config->pix_fmt == fmt[n])
                 break;
         }
		 */
        /* 
         if (!config) {
             // No specific config available, so the decoder must be able
             // to handle this format without any additional setup.
             return fmt[n];
         }
         if (config->methods & AV_CODEC_HW_CONFIG_METHOD_INTERNAL) {
             // Usable with only internal setup.
             return fmt[n];
         }
		 */
    }

    // Nothing is usable, give up.
    return AV_PIX_FMT_NONE;
}