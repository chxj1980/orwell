#ifndef FfmpegDecoder_H
#define FfmpegDecoder_H
#include "FrameUpdater.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavdevice/avdevice.h>
}
#include <iostream>


class FfmpegDecoder
{
public:
	FfmpegDecoder();
	~FfmpegDecoder();
	
	bool init();
	void decodeFrame(uint8_t* frameBuffer, int frameLength);
	void Record();
	void setPlayState(bool pause);
	void setRecordState(bool record);
	void setFrameUpdater(FrameUpdater * frameUpdater);

	//For debug purposes:
	std::string uri;

private:

	AVPicture        avPicture;
	AVCodec         *avCodec;
	AVCodecContext  *avCodecContext;
	AVFrame         *avFrame;
	SwsContext      *swsContext;
	AVStream        *avStream;
	AVFormatContext *avFormatContext;
	FrameUpdater    *frameUpdater;
	int m_videoWidth;
	int m_videoHeight;
};

#endif // FfmpegDecoder_H

