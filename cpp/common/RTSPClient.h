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
#include "Profiler.h"
/*
	Generic class to accomodate RTSP clients of any type
*/
class RTSPClient : public Stoppable, public Profiler
{
public:
	enum Transport
	{
		RTP_OVER_UDP,
		RTP_OVER_TCP,
		RTP_OVER_HTTP,
		RTP_OVER_UDP_MULTICAST
	};
	RTSPClient(std::string uri, Transport transport) : RTSPClient(uri, transport, "", "")
	{
	}
	RTSPClient(std::string uri, Transport transport,
			   std::string username, std::string password): transport(transport), uri(uri),
															username(username), password(password)
	{
	}
	/*
	RTSPClient(std::string uri, Transport transport,
			   std::string user, std::string password, int port) : transport(transport), uri(uri),
																   username(username), password(password),
																   port(port)
	{
	}
	*/
	/*
	void setUsername(std::string username) {
		this->username = username;
	}
	void setPassword(std::string username) {
		this->password = password;
	}
	void setPort(int port) {
		this->port = port;
	}
	*/
	void setEncodedPacketsFifo(std::shared_ptr<ThreadSafeDeque<std::shared_ptr<EncodedPacket>>> encodedPacketsFifo)
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
	/*
		To be used when there's a FIFO to write to
	*/
	virtual void startThreadMode()
	{
		runThread = std::thread(&RTSPClient::run, this);
	}
	ProfilerVariable<int> bytesPerSecond;


protected:
	std::shared_ptr<ThreadSafeDeque<std::shared_ptr<EncodedPacket>>> encodedPacketsFifo;
	std::thread runThread;
	Transport transport;
	std::string uri;
	std::string username;
	std::string password;
	int port;
};
#endif // RTSPCLient_H
