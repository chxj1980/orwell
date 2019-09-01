#ifndef RTSPCLient_H
#define RTSPCLient_H
#include <iostream>
#include "Decoder.h"
#include "rtspClient.h"
#include "FrameUpdater.h"
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "EncodedUnit.h"
#include "Stoppable.h"
/*
	Generic class to accomodate RTSP clients of any type
*/
class RTSPClient : public Stoppable
{
public:
	std::string uri;
	RTSPClient(std::string uri) : uri(uri) {}

	void setEncodedUnitsFifo(std::shared_ptr<ThreadSafeDeque<EncodedUnit>> encodedUnitsFifo)
	{
		this->encodedUnitsFifo = encodedUnitsFifo;
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

protected:
	std::shared_ptr<ThreadSafeDeque<EncodedUnit>> encodedUnitsFifo;
};
#endif // RTSPCLient_H
