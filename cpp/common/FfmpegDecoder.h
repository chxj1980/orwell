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
#include "VideoReceiver.h"
#include "VideoRecorder.h"


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
	//void setFrameUpdater(FrameUpdater * frameUpdater);
	void setVideoReceiver(VideoReceiver * videoReceiver) {this->videoReceiver = videoReceiver;}
	void disableVideoReceiver() {this->videoReceiver = nullptr;}
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
	VideoReceiver   *videoReceiver = nullptr;

};

#endif // FfmpegDecoder_H

