#ifndef FfmpegDecoder_H
#define FfmpegDecoder_H
#include "FrameUpdater.h"
#include <vector>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavdevice/avdevice.h>
#include <libavutil/hwcontext.h>
}
#include <iostream>
#include "VideoReceiver.h"
#include "VideoRecorder.h"


class FfmpegDecoder
{
public:
	enum Codec{H264, H265} codec;

	//FfmpegDecoder(Codec codec, Device device):codec(codec), device(device){};
	~FfmpegDecoder(){
		av_frame_free(&avFrame);
	}

	bool init();
	virtual void decodeFrame(uint8_t* frameBuffer, int frameLength);//Decodes to CPU memory
	void setVideoReceiver(VideoReceiver * videoReceiver) {
		this->videoReceiver = videoReceiver;
	}
	void disableVideoReceiver() {
		this->videoReceiver = nullptr;
	}

protected:

	AVPicture        avPicture;
	AVCodec         *avCodec;
	AVCodecContext  *avCodecContext;
	AVFrame         *avFrame;
	SwsContext      *swsContext;
	AVStream        *avStream;
	AVFormatContext *avFormatContext;
	VideoReceiver   *videoReceiver = nullptr;

};

#endif // FfmpegDecoder_H

