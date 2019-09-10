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
	By inheriting from ReusableBuffer (virtual class), we don't need to put NVIDIA specific code
	inside DecodedFrame (where ReusableBuffer is defined). We use downcasting
	to treat ReusableBuffer as NVDecoderReusableBuffer when needed.
*/
class NVDecoderReusableBuffer : public ReusableBuffer
{
public:
	NVDecoderReusableBuffer(std::shared_ptr<NvVideoDecoder> nvVideoDecoder): nvVideoDecoder(nvVideoDecoder) {
		
	}
	NVDecoderReusableBuffer(std::shared_ptr<NvVideoDecoder> nvVideoDecoder,
							v4l2_buffer v4l2Buffer,
							NvBuffer* nvBuffer) : nvVideoDecoder(nvVideoDecoder),
												  v4l2Buffer(v4l2Buffer),
												  nvBuffer{nvBuffer}
	{
	}
	int giveBack()
	{
		v4l2Buffer.m.planes[0].m.fd = nvBuffer->planes[0].fd;
		return nvVideoDecoder->capture_plane.qBuffer(v4l2Buffer, NULL);
	}


public:
	//WHY??????????????????
	std::shared_ptr<NvVideoDecoder> nvVideoDecoder;
	//TODO: I think I can take off these two members
	struct v4l2_buffer v4l2Buffer;
	struct v4l2_plane planes[MAX_PLANES];
	NvBuffer* nvBuffer;
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
	int decodeFrame(EncodedPacket &encodedPacket);
	int decodeFrame(EncodedPacket &encodedPacket, DecodedFrame &decodedFrame);
	void respondToResolutionEvent(v4l2_format& format, v4l2_crop& crop);
	//Thread that reads NALUs or CHUNKs, parses it and sends to decoder
	void run();
	void captureLoop();
	void startThreadMode() {
		captureThread = std::thread(&NVDecoder::captureLoop, this);
		runThread = std::thread(&Decoder::run, this);
	}

protected:
	//void read_decoder_input_nalu(NvBuffer *nvBuffer, EncodedPackets& encodedPackets);
	//Creates our decoder in blocking mode
	std::shared_ptr<NvVideoDecoder> nvVideoDecoder{NvVideoDecoder::createVideoDecoder("dec0")};
	//NvVideoDecoder* nvVideoDecoder = NvVideoDecoder::createVideoDecoder("dec0");
	static NvApplicationProfiler& nvApplicationProfiler;// = NvApplicationProfiler::getProfilerInstance();
	Format format;
	Codec codec;
	enum v4l2_memory outputPlaneMemType = V4L2_MEMORY_USERPTR;
	enum v4l2_memory capturePlaneMemType = V4L2_MEMORY_DMABUF;
	int dmaBufferFileDescriptor[MAX_BUFFERS];
	int numberCaptureBuffers;
	uint32_t height;
	uint32_t width;
	std::thread captureThread;
	//NvEglRenderer *renderer;
};

#endif // NVDecoder_H
