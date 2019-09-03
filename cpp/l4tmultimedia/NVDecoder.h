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

#define CHUNK_SIZE 4000000

#include <exception>
#include <sstream>
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

	//Thread that reads NALUs or CHUNKs, parses it and sends to decoder
	void run();

protected:
	//void read_decoder_input_nalu(NvBuffer *nvBuffer, EncodedPackets& encodedPackets);
	//Creates our decoder in blocking mode
	//SHOULD DESTROY???
	//std::unique_ptr<NvVideoDecoder> nvVideoDecoder = std::make_unique<nvVideoDecoder>(NvVideoDecoder::createVideoDecoder("dec0"));
	NvVideoDecoder* nvVideoDecoder = NvVideoDecoder::createVideoDecoder("dec0");
	static NvApplicationProfiler& nvApplicationProfiler = NvApplicationProfiler::getProfilerInstance();
	Format format;
	Codec codec;
	enum v4l2_memory outputPlaneMemType = V4L2_MEMORY_USERPTR;
	enum v4l2_memory outputPlaneMemType = V4L2_MEMORY_DMABUF;

};

#endif // NVDecoder_H
