#ifndef NVDecoder_H
#define NVDecoder_H

#include <iostream>
#include "VideoRecorder.h"
#include "Decoder.h"
#include "NvVideoDecoder.h"
#include "NvApplicationProfiler.h"
#include <memory>
#include "EncodedPacket.h"
#include "DecodedFrame.h"
#include <linux/videodev2.h>
#include <nvbuf_utils.h>
#include <chrono>
#include "NvEglRenderer.h"
#define CHUNK_SIZE 4000000
#define MAX_RTP_SIZE 0
//TODO: why 32??????? Wouldn't the index get out of range????
#define MAX_BUFFERS 32
#define MAXIMUM_OUTPUT_PLANES 10

#include <exception>
#include <sstream>

/*
	
*/
class DecodedNvFrame : public DecodedFrame
{
public:
	DecodedNvFrame(std::shared_ptr<NvVideoDecoder> nvVideoDecoder) : nvVideoDecoder(nvVideoDecoder)
	{
		memset(&v4l2Buffer, 0, sizeof(v4l2Buffer));
		memset(planes, 0, sizeof(planes));
		v4l2Buffer.m.planes = planes;
	}
	/*
		On destruction we queue the buffer back to nvVideoDecoder so the decoder can fill again
	*/
	~DecodedNvFrame()
	{
		//TODO: checking if nvbuffer is not NULL is not a good way to do things, I guess
		if(nvBuffer) {
			v4l2Buffer.m.planes[0].m.fd = nvBuffer->planes[0].fd;
			int ret = nvVideoDecoder->capture_plane.qBuffer(v4l2Buffer, NULL);
			if (ret < 0)
				printf("DecodedNvFrame: Could not quebe buffer back");
		}
			//throw std::runtime_error("DecodedNvFrame: Could not quebe buffer back");
		
	}

	int getFormat()
	{
		return format;
	}

	int getWidth()
	{
		return width;
	}

	int getHeight()
	{
		return height;
	}

	std::shared_ptr<NvVideoDecoder> nvVideoDecoder;
	struct v4l2_buffer v4l2Buffer;
	struct v4l2_plane planes[MAX_PLANES];
	NvBuffer *nvBuffer = NULL;
	int width;
	int height;
	int format;
};

struct NVDecoderCreationException : public std::exception
{
public:
	NVDecoderCreationException(std::string error, int errorCode) : error(error), errorCode(errorCode)
	{
	}
	const char *what() const throw()
	{
		if (errorCode)
			return (error + ": " + std::to_string(errorCode)).c_str();
		else
			return error.c_str();
	}

private:
	std::string error;
	int errorCode;
};

class NVDecoder : public Decoder
{
public:
	enum Format
	{
		NALU,
		CHUNK
	};
	NVDecoder(Format format, Codec codec);
	/* 
		
	*/
	int sendPacket(std::shared_ptr<EncodedPacket> encodedPacket);
	int sendPacket(std::shared_ptr<EncodedPacket> encodedPacket, std::shared_ptr<DecodedFrame> decodedFrame);
	void respondToResolutionEvent(v4l2_format &format, v4l2_crop &crop);
	//Thread that reads NALUs or CHUNKs, parses it and sends to decoder
	void run();
	void captureLoop();
	void startThreadMode()
	{
		runThread = std::thread(&Decoder::run, this);
		captureThread = std::thread(&NVDecoder::captureLoop, this);
	}

protected:
	static int counter;
	//Creates our decoder in blocking mode
	std::shared_ptr<NvVideoDecoder> nvVideoDecoder{NvVideoDecoder::createVideoDecoder("dec0")};
	//NvVideoDecoder* nvVideoDecoder = NvVideoDecoder::createVideoDecoder("dec0");
	static NvApplicationProfiler &nvApplicationProfiler; // = NvApplicationProfiler::getProfilerInstance();
	Format format;
	Codec codec;
	enum v4l2_memory outputPlaneMemType = V4L2_MEMORY_USERPTR;
	enum v4l2_memory capturePlaneMemType = V4L2_MEMORY_DMABUF;
	int dmaBufferFileDescriptor[MAX_BUFFERS] = {0};
	//TODO!!!!!!!!!!!! what? why numberCaptureBuffers=MAX_BUFFERS?
	//I only know that this undefined thing was causing problems
	int numberCaptureBuffers = MAX_BUFFERS;
	uint32_t height;
	uint32_t width;
	std::thread captureThread;
	//NvEglRenderer *renderer;
};

#endif // NVDecoder_H
