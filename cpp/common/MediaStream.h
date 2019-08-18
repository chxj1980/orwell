#ifndef MEDIASTREAM_H
#define MEDIASTREAM_H
#include <iostream>
#include "Decoder.h"
#include "rtspClient.h"
#include "FrameUpdater.h"
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "EncodedFrame.h"

class MediaStream
{
public:
	std::string uri;
	std::shared_ptr<Decoder> decoder;
	std::shared_ptr<VideoReceiver> videoReceiver;
	RtspClient rtspClient;

public:
	MediaStream(std::string uri);
	void setDecoder(std::shared_ptr<Decoder> decoder)
	{
		this->decoder = decoder;
	}
	void setEncodedFramesFifo(std::shared_ptr<ThreadSafeDeque<EncodedFrame>> encodedFramesFifo)
	{
		this->encodedFramesFifo = encodedFramesFifo;
	}
	int init();
	int receiveFrame();
	void run();

private:
	bool firstConnection = true;
	std::shared_ptr<ThreadSafeDeque<EncodedFrame>> encodedFramesFifo;
	//void ByeFromServerClbk();
};
#endif // MEDIASTREAM_H
