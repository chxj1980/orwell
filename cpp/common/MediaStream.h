#ifndef MEDIASTREAM_H
#define MEDIASTREAM_H
#include <iostream>
#include "FfmpegDecoder.h"
#include "rtspClient.h"
#include "FrameUpdater.h"
#include "VideoReceiver.h"
#include "VideoRecorder.h"

class MediaStream {
	std::string   uri;
	FfmpegDecoder* ffmpegDecoder;
	FrameUpdater* frameUpdater;
	RtspClient    rtspClient;
	//bool ByeFromServerFlag = false;
  public:
    MediaStream (std::string uri);
		//TODO: create a decoder class so we can have other types of decoders
		void setDecoder(FfmpegDecoder* ffmpegDecoder) {this->ffmpegDecoder = ffmpegDecoder;}
		//void setFrameUpdater(FrameUpdater* frameUpdater) {this->frameUpdater = frameUpdater;}
		int init();
		int receiveFrame();
		void run();
	private:
		bool firstConnection = true;

		//void ByeFromServerClbk();
};
#endif // MEDIASTREAM_H
