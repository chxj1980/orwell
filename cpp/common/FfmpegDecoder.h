#ifndef FfmpegDecoder_H
#define FfmpegDecoder_H
#include <vector>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavutil/hwcontext.h>
}
#include <iostream>
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "Decoder.h"

struct AVPacketDeleter
{
	void operator()(AVPacket *avPacket) { av_free_packet(avPacket); }
};

struct AVFrameDeleter
{
	void operator()(AVFrame *avFrame) { av_frame_unref(avFrame); }
};

struct AVBufferRefDeleter
{
	void operator()(AVBufferRef *avBufferRef) { av_buffer_unref(&avBufferRef); }
};

struct AVCodecContextDeleter
{
	void operator()(AVCodecContext *avCodecContext) { avcodec_free_context(&avCodecContext); }
};

class FfmpegDecoder : public Decoder
{
public:
	//Initiates all the av things
	virtual int init() = 0;
	/* 
		Decodes to CPU memory. 
		If invoked in a FfmpegSoftwareDecoder instance, it'll simply do
		all the process in software.
		If invoked in a FfmpegHardwareDecoder, it'll decode in hardware,
		then copy from GPU memory to CPU memory.
		To just decode to GPU memory but not get it in CPU memory, use
		FfmpegHardwareDecoder::hardwareDecode().
	*/
	virtual int decodeFrame(uint8_t *frameBuffer, int frameLength) = 0;
	virtual int decodeFrame(uint8_t *frameBuffer, int frameLength, Frame &frame) = 0;
	/* 
	static int avFrameToFrame(AVFrame* avFrame, Frame &frame)
	{
		for (int i = 0; i <= FRAME_CHANNELS_SIZE; i++)
		{
			frame.buffer[i] = avFrame->data[i];
			frame.linesize[i] = avFrame->linesize[i];
		}
		frame.width = avFrame->width;
		frame.height = avFrame->height;
		//std::cout << "avFrame converter" <<  std::endl;
		//std::cout << "format of avFrame is " << avFrame->format << std::endl;
		frame.format = (AVPixelFormat) avFrame->format;
	}
	*/
	/*
	static int frameToAvFrame(AvFrame* avFrame, Frame* frame) {
		avFrame->data[0] = frame->frameBuffer[0];
		//...?
		avFrame->linesize = frame
		avFrame->width = frame->width;
		avFrame->height = frame->height;
	}
	*/
	~FfmpegDecoder()
	{
		//av_frame_free(&avFrame);
		//avcodec_free_context(&avCodecContext);
		//av_packet_unref(avPacket);
		//delete?
	}

protected:
	AVCodec *avCodec;
	std::unique_ptr<AVCodecContext, AVCodecContextDeleter> avCodecContext;
	std::unique_ptr<AVBufferRef, AVBufferRefDeleter> avBufferRef;

	//SwsContext *swsContext;
	//AVStream *avStream;
	//AVFormatContext *avFormatContext;
};

#endif // FfmpegDecoder_H
