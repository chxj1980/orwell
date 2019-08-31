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

class NVDecoder: public Decoder
{
public:
	NVDecoder();
	/* 
		
	*/
	virtual int decodeFrame(EncodedFrame& encodedFrameh) = 0;
	virtual int decodeFrame(EncodedFrame& encodedFrame, DecodedFrame &decodedFrame) = 0;

protected:
	//Creates our decoder in blocking mode
    std::unique_ptr<NvVideoDecoder> nvVideoDecoder = std::make_unique<NvVideoDecoder>(NvVideoDecoder::createVideoDecoder("dec0"));
	std::unique_ptr<NvApplicationProfiler> nvApplicationProfiler = std::make_unique<NvApplicationProfiler>(NvApplicationProfiler::getProfilerInstance());
};

#endif // NVDecoder_H
