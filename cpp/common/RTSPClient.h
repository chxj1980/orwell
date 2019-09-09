#ifndef RTSPCLient_H
#define RTSPCLient_H
#include <iostream>
#include "Decoder.h"
#include "rtspClient.h"
#include "FrameUpdater.h"
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "EncodedPacket.h"
#include "Stoppable.h"
/*
	Generic class to accomodate RTSP clients of any type
*/
class RTSPClient : public Stoppable
{
public:
	std::string uri;
	RTSPClient(std::string uri) : uri(uri) {}

	void setEncodedPacketsFifo(std::shared_ptr<ThreadSafeDeque<EncodedPacket>> encodedPacketsFifo)
	{
		this->encodedPacketsFifo = encodedPacketsFifo;
	}
	/*
		Initiates the client and connects to the URL
	*/
	virtual int init() = 0;
	/*
		Thread loop that pushes new encoded frames to fifo
	*/
	virtual void run() = 0;
	/*
		Receive an individual frame (or part of it). That is,
		receive an RTSP packet
	*/
	virtual int receivePacket() = 0;

	virtual void startThreadMode() {
		runThread = std::thread(&RTSPClient::run, this);
	}

protected:
	std::shared_ptr<ThreadSafeDeque<EncodedPacket>> encodedPacketsFifo;
	std::thread runThread;
};
#endif // RTSPCLient_H
