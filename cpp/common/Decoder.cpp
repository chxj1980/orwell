#include "Decoder.h"
#include "SLog.h"
SLOG_CATEGORY("Decoder")

void Decoder::run()
	{
        LOG << "Starting Decoder.cpp run function";
		if (!encodedPacketsFifo)
		{
			std::cerr << "Decoder.h: no encodedPacketsFifo setted, nowhere to pull data from" << std::endl;
			return;
		}
		while (shouldContinue())
		{
			/*
				Pops an encoded frame from encodedPacketsFifo. If there are none, it blocks, so
				no CPU time is wasted.
			*/
			//TODO: certify that the operation below is MOVING the frame to here, not copying it
			EncodedPacket encodedPacket = std::move(encodedPacketsFifo->pop_front());
			/* 
				Since the frame is gone from the fifo, it only exists here. 
				decodeFrame() access its pointers and is blocking. When decodeFrame 
				finishes, `encodedPacket` is gone and its contents are automatically deleted.
				DecodedFrame is sent to the decodedFramesFifo
			*/
			decodeFrame(encodedPacket);
		}
	}