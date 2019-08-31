#ifndef Decoder_H
#define Decoder_H
#include <vector>

#include <iostream>
#include <memory>
#include "EncodedFrame.h"
#include "DecodedFrame.h"
#include "ThreadSafeDeque.h"
#include "Stoppable.h"

class Decoder: public Stoppable
{
public:
	enum Codec
	{
		H264,
		H265,
		MPEG2,
		MPEG4,
		VP9
	} codec;
	/*
		Thread loop that continuously pushes data from encodedFramesFifo and decoded it
	*/
	virtual void run()
	{
		if (!encodedFramesFifo)
		{
			std::cerr << "Decoder.h: no encodedFramesFifo setted, nowhere to pull data from" << std::endl;
			return;
		}
		while (shouldContinue())
		{
			/*
				Pops an encoded frame from encodedFramesFifo. If there are none, it blocks, so
				no CPU time is wasted.
			*/
			//TODO: certify that the operation below is MOVING the frame to here, not copying it
			EncodedFrame encodedFrame = std::move(encodedFramesFifo->pop_front());
			/* 
				Since the frame is gone from the fifo, it only exists here. 
				decodeFrame() access its pointers and is blocking. When decodeFrame 
				finishes, `encodedFrame` is gone and its contents are automatically deleted.
				DecodedFrame is sent to the decodedFramesFifo
			*/
			decodeFrame(encodedFrame);
		}
	}
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
	virtual int decodeFrame(EncodedFrame& encodedFrame, DecodedFrame &decodedFrame) = 0;
	/*
		Decodes directly to the decodedFramesFifo
	*/
	virtual int decodeFrame(EncodedFrame& encodedFrame) = 0;
	/*
		Here go all the raw frames, as readed from network or file
	*/
	void setEncodedFramesFifo(std::shared_ptr<ThreadSafeDeque<EncodedFrame>> encodedFramesFifo)
	{
		this->encodedFramesFifo = encodedFramesFifo;
	}
	/*
		Here go all the decoded frames
	*/
	void setDecodedFramesFifo(std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo)
	{
		this->decodedFramesFifo = decodedFramesFifo;
	}

protected:
	/*
		Here we store a pointer to our FIFOs, which are made of a simple thread-safe 
		deque implementation called ThreadSafeDeque
	*/
	std::shared_ptr<ThreadSafeDeque<EncodedFrame>> encodedFramesFifo;
	std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo;
};

#endif // Decoder_H
