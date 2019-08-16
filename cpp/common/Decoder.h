#ifndef Decoder_H
#define Decoder_H
#include <vector>

#include <iostream>
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include <memory>
#include "Frame.h"
#include "ThreadSafeDeque.h"

class Decoder
{
public:
	enum Codec
	{
		H264,
		H265
	} codec;

	//Initiates the decoder contexts
	virtual int init() = 0;
	/* 
		Decodes to CPU memory. 
		If invoked in a FfmpegSoftwareDecoder instance, it'll simply do
		all the process in software.
		If invoked in a FfmpegHardwareDecoder, it'll decode in hardware,
		then copy from GPU memory to CPU memory.
		To just decode to GPU memory but not get it in CPU memory, use
		FfmpegHardwareDecoder::hardwareDecode().
		
		- frameBuffer contains RAW undecoded video
		- frameLength contains size of frameBuffer
		- frame is the pointer to the decoded frame data
	*/
	virtual int decodeFrame(uint8_t *frameBuffer, int frameLength, Frame &frame) = 0;
	/*
		Decodes directly to the videoReceiver
	*/
	virtual int decodeFrame(uint8_t *frameBuffer, int frameLength) = 0;
	//Decoded data decoded through decodeFrame will be sent to videoReceiver
	//TODO: study the living time of VideoReceiver
	void setVideoReceiver(VideoReceiver *videoReceiver)
	{
		this->videoReceiver = videoReceiver;
	}

	void setUncodedFramesFifo(std::shared_ptr<ThreadSafeDeque<Frame>> uncodedFramesFifo)
	{
		this->uncodedFramesFifo = uncodedFramesFifo;
	}

	void setDecodedFramesFifo(std::shared_ptr<ThreadSafeDeque<Frame>> decodedFramesFifo)
	{
		this->decodedFramesFifo = decodedFramesFifo;
	}

	void disableVideoReceiver()
	{
		this->videoReceiver = nullptr;
	}

protected:
	VideoReceiver *videoReceiver;
	/*
		Here we store a pointer to our FIFO, which is a simple thread safe deque implementation

	*/
	std::shared_ptr<ThreadSafeDeque<Frame>> uncodedFramesFifo;
	std::shared_ptr<ThreadSafeDeque<Frame>> decodedFramesFifo;

	//Temporary frame used for decoding
	//Frame frame;
};

#endif // Decoder_H
