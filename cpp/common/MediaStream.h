#ifndef MEDIASTREAM_H
#define MEDIASTREAM_H
#include <iostream>
#include "FfmpegDecoder.h"
#include "rtspClient.h"
#include "FrameUpdater.h"
#include "VideoReceiver.h"
#include "VideoRecorder.h"

class MediaStream {
	public:
	std::string   uri;
	std::shared_ptr<Decoder> decoder;
	std::shared_ptr<VideoReceiver> videoReceiver;
	RtspClient    rtspClient;
  public:
    MediaStream (std::string uri);
	void setDecoder(std::shared_ptr<Decoder> decoder) {
		this->decoder = decoder;
	}
	int init();
	int receiveFrame();
	void run();
	private:
		bool firstConnection = true;
		//void ByeFromServerClbk();
};
#endif // MEDIASTREAM_H
