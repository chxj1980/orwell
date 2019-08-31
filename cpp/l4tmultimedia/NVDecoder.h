#ifndef NVDecoder_H
#define NVDecoder_H

#include <iostream>
#include "VideoRecorder.h"
#include "Decoder.h"
#include "NvVideoDecoder.h"
#include "NvApplicationProfiler.h"
#include <memory>
#include "EncodedFrame.h"
#include "DecodedFrame.h"
#define CHUNK_SIZE 4000000

#include <iostream>
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
	int decodeFrame(EncodedFrame &encodedFrame) = 0;
	int decodeFrame(EncodedFrame &encodedFrame, DecodedFrame &decodedFrame) = 0;
	//void run();
	void planeQueuer();

protected:
	//Creates our decoder in blocking mode
	std::unique_ptr<NvVideoDecoder> nvVideoDecoder = std::make_unique<NvVideoDecoder>(NvVideoDecoder::createVideoDecoder("dec0"));
	std::unique_ptr<NvApplicationProfiler> nvApplicationProfiler = std::make_unique<NvApplicationProfiler>(NvApplicationProfiler::getProfilerInstance());
	Format format;
	Codec codec;
	int outputPlaneMemType = V4L2_MEMORY_USERPTR;
};

#endif // NVDecoder_H
