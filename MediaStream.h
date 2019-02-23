#ifndef MEDIASTREAM_H
#define MEDIASTREAM_H
#include <iostream>
#include "FfmpegDecoder.h"
#include "VideoConsumer.h"
#include "rtspClient.h"
#include "FrameUpdater.h"

class MediaStream {
	std::string   uri;
	FfmpegDecoder ffmpegDecoder;
	VideoConsumer videoConsumer;
	FrameUpdater* frameUpdater;
	RtspClient    rtspClient;
	//bool ByeFromServerFlag = false;
  public:
    MediaStream (std::string uri);
		void setFrameUpdater(FrameUpdater* frameUpdater);
		int init();
		int receiveFrame();
		void run();

		//void ByeFromServerClbk();
};
#endif // MEDIASTREAM_H
