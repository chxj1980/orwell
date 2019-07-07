#include "FfmpegDecoder.h"
//TODO: put those below on .h
#include <iostream>
#include <cstdint>
#include <stdint.h>
#include <time.h>
#include <QDebug>
#include <QDateTime>
#include <QPainter>
#include <QFont>


FfmpegDecoder::FfmpegDecoder()
{
	m_videoStreamIndex = -1;
	m_isRecord = -1;
	swsContext = nullptr;
	
	//av_register_all(); DEPRECATED, no need to use anymore
	avcodec_register_all();
	avFrame = av_frame_alloc();
	//std::cout << "ffmpegdecoder created" << std::endl;
}

FfmpegDecoder::~FfmpegDecoder()
{
	av_frame_free(&avFrame);
}

void FfmpegDecoder::setFrameUpdater(FrameUpdater * frameUpdater) {
	//std::cout << "openglwidget setted" << std::endl;
	this->frameUpdater = frameUpdater;
}

bool FfmpegDecoder::init()
{
	//std::cout << "FfmpegDecoder init!"  << std::endl;

	avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);

	if (!avCodec) {return false;}
	//std::cout << "avCodec worked!"  << std::endl;

	avCodecContext = avcodec_alloc_context3(avCodec);

	if (avcodec_open2(avCodecContext, avCodec, NULL) < 0)  return false;
	//std::cout << "avcodec_open2 worked" << std::endl;

	return true;
}

//https://stackoverflow.com/questions/30784549/best-simplest-way-to-display-ffmpeg-frames-in-qt5
void  FfmpegDecoder::decodeFrame(uint8_t* frameBuffer, int frameLength)
{
	if (frameLength <= 0) return;

	int frameFinished = 0;
	if (this->uri =="rtsp://admin:19929394@192.168.0.102:10554/tcp/av0_0")
			std::cout << " allocatin packet for " << this->uri << std::endl;
	AVPacket* avPacket = av_packet_alloc();
	if (!avPacket) std::cout << "av packet error" << std::endl;
	//AVRational tb = avCodecContext->time_base; //For fixed-fps content, timebase should be 1/framerate and timestamp increments should be identically 1. This often, but not always is the inverse of the frame rate or field rate for video. 1/time_base is not the average frame rate if the frame rate is not constant.
    //AVRational frameRate = av_guess_frame_rate(avFormatContext, avStream, NULL); //Guess the frame rate, based on both the container and codec information. 

	avPacket->size = frameLength;
	avPacket->data = frameBuffer;

    //https://github.com/saki4510t/pupilClient/blob/0e9f7bdcfe9f5fcb197b1c2408a6fffb90345f8d/src/media/h264_decoder.cpp#L119
	//no output from ffmpeg
	av_log_set_level(AV_LOG_QUIET); 
	bool stop = false;
	int result = avcodec_send_packet(avCodecContext, avPacket);
	if (!result) {
		while (!result) {
			result = avcodec_receive_frame(avCodecContext, avFrame);
			if (!result) {
				this->frameUpdater->updateData(avFrame->data, avFrame->width, avFrame->height);
				break;
			} else if ((result < 0) && (result != AVERROR(EAGAIN)) && (result != AVERROR_EOF)) {
				//LOGE("avcodec_receive_frame returned error %d:%s", result, av_error(result).c_str());
				std::cout << "avcodec_receive_frame returned error " << result /*<< *av_err2str(result).c_str()*/ << std::endl;
				break;
			} else {
				switch (result) {
					//Not exactly an error, we just have to wait for more data
					case AVERROR(EAGAIN):
						result = 0;
						break;
					//To be done: what does this error mean? I think it's literally the end of an mp4 file
					case AVERROR_EOF:
						// buffer flushed
						std::cout << "avcodec_receive_frame AVERROR_EOF" << std::endl;
						result = 0;
						break;
					//To be done: describe what error is this in std cout before stopping
					default:
						std::cout << "avcodec_receive_frame returned error, stopping..." << result /*<< av_err2str(result).c_str()*/ << std::endl;
						break;
					//Error happened, should break anyway
					break;
				}
				break;
			}
		}
	} else {
		switch (result) {
			case AVERROR(EAGAIN):
				//LOGE("avcodec_send_packet EAGAIN");
				std::cout << "avcodec_send_packet EAGAIN" << std::endl;

				break;
			case AVERROR_EOF:
				//LOGE("avcodec_send_packet AVERROR_EOF");
				std::cout << "avcodec_send_packet AVERROR_EOF" << std::endl;

				result = 0;
				break;
			default:
				//LOGE("avcodec_send_packet returned error %d:%s", result, av_error(result).c_str());
				//std::cout << "avcodec_send_packet returned error" << result /*av_error(result).c_str()*/ << std::endl;
				std::cout << "ffmpeg default unknown error for " << this->uri << std::endl;
				break;
		}
	}
	if (frameFinished) {
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

		int width = avFrame->width;
		int height= avFrame->height;
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
		
		av_frame_unref(avFrame);
	}
}
