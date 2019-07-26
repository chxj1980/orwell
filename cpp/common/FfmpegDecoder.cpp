#include "FfmpegDecoder.h"
//TODO: put those below on .h
#include <iostream>
#include <cstdint>
#include <stdint.h>
#include <time.h>

/*
void FfmpegDecoder::setFrameUpdater(FrameUpdater * frameUpdater) {
	this->frameUpdater = frameUpdater;
}
*/
/* 
AVPixelFormat print_avaliable_pixel_formats_for_hardware(struct AVCodecContext *avctx,
														 const AVPixelFormat *fmt)
{
	//const AVPixFmtDescriptor *desc;
	//const AVCodecHWConfig *config;
	int i, n;
	
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
	
	// No device or other setup, so we have to choose from things which
	// don't any other external information.

	// Finally, traverse the list in order and choose the first entry
	// with no external dependencies (if there is no hardware configuration
	// information available then this just picks the first entry).
	printf("supported formats are \n");
	for (n = 0; fmt[n] != AV_PIX_FMT_NONE; n++)
	{

		
         for (i = 0;; i++) {
			 
             config = avcodec_get_hw_config(avctx->codec, i);
             if (!config)
                 break;
             if (config->pix_fmt == fmt[n])
                 break;
         }
		
		
         if (!config) {
             // No specific config available, so the decoder must be able
             // to handle this format without any additional setup.
             return fmt[n];
         }
         if (config->methods & AV_CODEC_HW_CONFIG_METHOD_INTERNAL) {
             // Usable with only internal setup.
             return fmt[n];
         }
		 
	}

	// Nothing is usable, give up.
	return AV_PIX_FMT_NONE;
}

AVPixelFormat get_format(struct AVCodecContext *s, const AVPixelFormat *fmt)
{
	printf("chosen format: ");
	printf((char *)*fmt);
	return AVPixelFormat::AV_PIX_FMT_0BGR;
}

std::vector<std::string> FfmpegDecoder::getSupportedDevices()
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
*/
/* 
bool FfmpegDecoder::init()
{
	avcodec_register_all();

	avFrame = av_frame_alloc();

	switch (codec)
	{
	case H264:
		avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
		break;
	case H265:
		//avCodec = avcodec_find_decoder(AV_CODEC_ID_H265);
		std::cout << "ERROR, CODEC NOT YET SUPPORTED" << std::endl;
		break;
	default:
		break;
	}

	if (!avCodec)
	{
		std::cout << "avcodec_find_decoder problem" << std::endl;
		return false;
	}

	avCodecContext = avcodec_alloc_context3(avCodec);
	switch (device)
	{
	case SOFTWARE:
		
		break;
	case HARDWARE:

		break;
	default:
		std::cout << "avcodec_open2 problem" << std::endl;
	}
	//printf("gonna insert avCodecContext \n");
	//Insert pixel format callback to define our decoding pixel format https://stackoverflow.com/questions/9652760/how-to-set-decode-pixel-format-in-libavcodec
	//avCodecContext->get_format = &print_avaliable_pixel_formats_for_hardware;

	if (avcodec_open2(avCodecContext, avCodec, NULL) < 0)
	{
		std::cout << "avcodec_open2 problem" << std::endl;
		return false;
	}

	return true;
}

//https://stackoverflow.com/questions/30784549/best-simplest-way-to-display-ffmpeg-frames-in-qt5
void FfmpegDecoder::decodeFrame(uint8_t *frameBuffer, int frameLength)
{
	if (frameLength <= 0)
		return;

	int frameFinished = 0;

	AVPacket *avPacket = av_packet_alloc();
	if (!avPacket)
		std::cout << "av packet error" << std::endl;
	//AVRational tb = avCodecContext->time_base; //For fixed-fps content, timebase should be 1/framerate and timestamp increments should be identically 1. This often, but not always is the inverse of the frame rate or field rate for video. 1/time_base is not the average frame rate if the frame rate is not constant.
	//AVRational frameRate = av_guess_frame_rate(avFormatContext, avStream, NULL); //Guess the frame rate, based on both the container and codec information.

	avPacket->size = frameLength;
	avPacket->data = frameBuffer;

	//https://github.com/saki4510t/pupilClient/blob/0e9f7bdcfe9f5fcb197b1c2408a6fffb90345f8d/src/media/h264_decoder.cpp#L119
	//Disable ffmpeg annoying output
	av_log_set_level(AV_LOG_QUIET);

	int sendPacketResult = avcodec_send_packet(avCodecContext, avPacket);

	if (!sendPacketResult)
	{
		int receiveFrameResult = avcodec_receive_frame(avCodecContext, avFrame);
		if (!receiveFrameResult)
		{
			if (this->videoReceiver != nullptr)
			{
				this->videoReceiver->receiveVideo(avFrame->data, avFrame->width, avFrame->height);
			}
		}
		else if ((receiveFrameResult < 0) && (receiveFrameResult != AVERROR(EAGAIN)) && (receiveFrameResult != AVERROR_EOF))
		{
			std::cout << "avcodec_receive_frame returned error " << receiveFrameResult << std::endl;
		}
		else
		{
			switch (receiveFrameResult)
			{
			//Not exactly an error, we just have to wait for more data
			case AVERROR(EAGAIN):
				break;
			//To be done: what does this error mean? I think it's literally the end of an mp4 file
			case AVERROR_EOF:
				std::cout << "avcodec_receive_frame AVERROR_EOF" << std::endl;
				break;
			//To be done: describe what error is this in std cout before stopping
			default:
				std::cout << "avcodec_receive_frame returned error, stopping..." << receiveFrameResult  << std::endl;
				break;
				//Error happened, should break anyway
				break;
			}
		}
	}
	else
	{
		switch (sendPacketResult)
		{
		case AVERROR(EAGAIN):
			std::cout << "avcodec_send_packet EAGAIN" << std::endl;
			break;
		case AVERROR_EOF:
			std::cout << "avcodec_send_packet AVERROR_EOF" << std::endl;
			break;
		//To be done: debug which erros are these. Last time I checked it was a bunch of -1094995529 errors
		default:
			//std::cout << "ffmpeg default unknown error number " << result << " for " << this->uri << std::endl;
			break;
		}
	}
}
*/
//if (frameFinished) {
/*
		double dpts = NAN;

		if (avFrame->pts != AV_NOPTS_VALUE)
			dpts = av_q2d(avStream->time_base) * avFrame->pts;

		avFrame->sample_aspect_ratio = av_guess_sample_aspect_ratio(avFormatContext, avStream, avFrame);

		AVRational avRational;
		avRational.num = frameRate.den;
		avRational.den = frameRate.num;
		duration = (frameRate.num && frameRate.den ? av_q2d(avRational) : 0);
		pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
		*/

//int width = avFrame->width;
//int height= avFrame->height;
/*
		AvPicture* picture;
		picture = avcodec_alloc_frame();
		int ret = avpicture_alloc(picture, AV_PIX_FMT_YUV420P, width, height);
		//mVideoRender->setSize(width,height);
		//mVideoRender->start();
		//if(ret==0)
		//	picInit=true;

		
		
		//avpicture_fill(&picture, video_buf, AV_PIX_FMT_YUV420P,width, height);

		//Context for scaling
		convertContext = sws_getCachedContext(convertContext, width, height, avCodecContext->pix_fmt,
												width,height,AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR,
												NULL, NULL, NULL);

		if (!convertContext) qDebug("Cannot initialize the conversion context\n");

		//Lets scale our frame
		int rr = sws_scale(convertContext, avFrame->data, avFrame->linesize,
						   0, frame->height, picture->data, picture->linesize);
		*/
/*
		if(dpts > mClock->getClockTime())
		{
			QThread::msleep((dpts-mClock->getClockTime())*1000);
		}
		*/

//av_frame_unref(avFrame);
//}
