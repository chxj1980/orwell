#ifndef Decoder_H
#define Decoder_H
#include <vector>

#include <iostream>
#include <memory>
#include "DecodedFrame.h"
#include "ThreadSafeDeque.h"

class Decoder
{
public:
	enum Codec
	{
		H264,
		H265
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
		while (true)
		{
			/*
				Pops an encoded frame from encodedFramesFifo. If there are none, it blocks, so
				no CPU time is wasted.
			*/
			//TODO: certify that the operation below is MOVING the frame to here, not copying it
			EncodedFrame frame = std::move(encodedFramesFifo->pop_front());
			/* 
				Since the frame is gone from the fifo, it only exists here. 
				decodeFrame() access its pointers and is blocking. When decodeFrame 
				finishes, `frame` is gone and its contents are automatically deleted
			*/
			decodeFrame(frame->frameBuffer.get(), frame.frameSize);
		}
	}
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
	virtual int decodeFrame(uint8_t *frameBuffer, int frameLength, DecodedFrame &frame) = 0;
	/*
		Decodes directly to the videoReceiver
	*/
	virtual int decodeFrame(uint8_t *frameBuffer, int frameLength) = 0;
	//Decoded data decoded through decodeFrame will be sent to videoReceiver

	/*
		Here go all the raw frames, as readed from network or file (Frame format is just a placeholder for now)
	*/
	void setUncodedFramesFifo(std::shared_ptr<ThreadSafeDeque<DecodedFrame>> encodedFramesFifo)
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
