#include "FfmpegHardwareDecoder.h"


AVPixelFormat FfmpegHardwareDecoder::get_format(struct AVCodecContext *s, const AVPixelFormat *fmt)
{
	printf("chosen format: ");
	printf((char *)*fmt);
	return AVPixelFormat::AV_PIX_FMT_0BGR;
}

std::vector<std::string> FfmpegHardwareDecoder::getSupportedDevices()
{
	//Begin with any type since we're gonna iterate through all until we get back to AV_HWDEVICE_TYPE_NONE
	enum AVHWDeviceType type = AV_HWDEVICE_TYPE_VDPAU ;
	std::vector<std::string> result;
	while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
	{
		//printf(av_hwdevice_get_type_name(type));
		result.push_back(std::string(av_hwdevice_get_type_name(type)));
	}
	return result;
}

bool FfmpegHardwareDecoder::init(std::string type) {
    //create context here
    int err = 0;

    if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
                                      NULL, NULL, 0)) < 0) {
        fprintf(stderr, "Failed to create specified HW device.\n");
        return err;
    }
    ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

    return err;

    //-------------------------------------------

    AVHWDeviceType ahwDeviceType = av_hwdevice_find_type_by_name(type.c_str());
    if (ahwDeviceType == AV_HWDEVICE_TYPE_NONE) {
        fprintf(stderr, "Device type %s is not supported.\n", type.c_str());
        std::cout << "Device " << type << "not supported" << std::endl;
        std::cout << "Avaliable devices:" << std::endl;
        for (auto i: getSupportedDevices())
  		    std::cout << i << std::endl;
        return -1;
    }
 
    //avCodecContext is for our decoder
    for (int i = 0;; i++) {
        const AVCodecHWConfig *config = avcodec_get_hw_config(avCodecContext, i);
        if (!config) {
            fprintf(stderr, "Decoder %s does not support device type %s.\n",
                    decoder->name, av_hwdevice_get_type_name(type));
            return -1;
        }
        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
            config->device_type == type) {
            hw_pix_fmt = config->pix_fmt;
            break;
        }
    }

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

    /* open the file to dump raw data */
    output_file = fopen(argv[3], "w+");

    /* actual decoding and dump the raw data */
    while (ret >= 0) {
        if ((ret = av_read_frame(input_ctx, &packet)) < 0)
            break;

        if (video_stream == packet.stream_index)
            ret = decode_write(decoder_ctx, &packet);

        av_packet_unref(&packet);
    }

    /* flush the decoder */
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
}





AVPixelFormat FfmpegHardwareDecoder::print_avaliable_pixel_formats_for_hardware(struct AVCodecContext *avctx,
														 const AVPixelFormat *fmt)
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