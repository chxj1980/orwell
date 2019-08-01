#ifndef FfmpegDecoder_H
#define FfmpegDecoder_H
#include "FrameUpdater.h"
#include <vector>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavutil/hwcontext.h>
}
#include <iostream>
#include "VideoReceiver.h"
#include "VideoRecorder.h"


class FfmpegDecoder
{
public:
	enum Codec{H264, H265} codec;
	~FfmpegDecoder(){
		av_frame_free(&avFrame);
	}
	//Initiates all the av things
	virtual bool init() = 0;
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
	//Decoded data decoded through decodeFrame will be sent to videoReceiver
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

