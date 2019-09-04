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
#define CHUNK_SIZE 4000000
#define MAX_BUFFERS 32

#include <exception>
#include <sstream>

class NVDecoderQueueableBuffer : public QueueableBuffer
{
public:
	NVDecoderQueueableBuffer(std::shared_ptr<NvVideoDecoder> nvVideoDecoder,
							 v4l2_buffer v4l2Buffer,
							 NVBuffer* nvBuffer) : nvVideoDecoder(nvVideoDecoder),
												   v4l2Buffer(v4l2Buffer),
												   nvBuffer(std::make_unique<NVBuffer>(nvBuffer))
	{
	}
	int queue()
	{
		return nvVideoDecoder->capture_plane.qBuffer(v4l2Buffer, NULL);
	}

private:
	std::shared_ptr<NvVideoDecoder> nvVideoDecoder;
	struct v4l2_buffer v4l2Buffer;
	std::unique_ptr<NVBuffer> nvBuffer;
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
	void prepareDecoder();
	void captureLoop();
	//Thread that reads NALUs or CHUNKs, parses it and sends to decoder
	void run();

protected:
	//void read_decoder_input_nalu(NvBuffer *nvBuffer, EncodedPackets& encodedPackets);
	//Creates our decoder in blocking mode
	//SHOULD DESTROY???
	std::shared_ptr<NvVideoDecoder> nvVideoDecoder = std::make_shared<NvVideoDecoder>(NvVideoDecoder::createVideoDecoder("dec0"));
	//NvVideoDecoder* nvVideoDecoder = NvVideoDecoder::createVideoDecoder("dec0");
	//static NvApplicationProfiler& nvApplicationProfiler = NvApplicationProfiler::getProfilerInstance();
	Format format;
	Codec codec;
	enum v4l2_memory outputPlaneMemType = V4L2_MEMORY_USERPTR;
	enum v4l2_memory capturePlaneMemType = V4L2_MEMORY_DMABUF;
	int dmaBufferFileDescriptor[MAX_BUFFERS];
	int numberCaptureBuffers;
	uint32_t height;
	uint32_t width;
};

#endif // NVDecoder_H
