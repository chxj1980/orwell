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


class FfmpegDecoder: public Decoder
{
public:
	~FfmpegDecoder(){
		av_frame_free(&avFrame);
	}
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
	virtual void decodeFrame(uint8_t* frameBuffer, int frameLength)=0;

	static int avFrameToFrame(AVFrame* avFrame, Frame* frame) {
		for (int i=0; i<=FRAME_CHANNELS_SIZE; i++)
			frame->frameBuffer[i] = avFrame->data[i];

		frame->linesize = avFrame->linesize;
		frame->width    = avFrame->width;
		frame->height   = avFrame->height;
		frame->format   = (AVPixelFormat) avFrame->format;
	}
	/*
	static int frameToAvFrame(AvFrame* avFrame, Frame* frame) {
		avFrame->data[0] = frame->frameBuffer[0];
		//...?
		avFrame->linesize = frame
		avFrame->width = frame->width;
		avFrame->height = frame->height;
	}
	*/

protected:
	AVPicture        *avPicture;
	AVCodec          *avCodec;
	AVCodecContext   *avCodecContext;
	AVFrame          *avFrame;
	SwsContext       *swsContext;
	AVStream         *avStream;
	AVFormatContext  *avFormatContext;
};

#endif // FfmpegDecoder_H

