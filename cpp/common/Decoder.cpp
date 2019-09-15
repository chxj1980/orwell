#include "Decoder.h"
#include "SLog.h"
SLOG_CATEGORY("Decoder")

void Decoder::run()
	{
        LOG << "Starting Decoder.cpp run function";
		while (shouldContinue())
		{
			/*
				Pops an encoded frame from encodedPacketsFifo. If there are none, it blocks, so
				no CPU time is wasted.
			*/
			//TODO: certify that the operation below is MOVING the frame to here, not copying it
			std::shared_ptr<EncodedPacket> encodedPacket = onAcquireNewPacket();
			//std::shared_ptr<EncodedPacket> encodedPacket = encodedPacketsFifo->pop_front();
			/* 
				Since the frame is gone from the fifo, it only exists here. 
				sendPacket() access its pointers and is blocking. When sendPacket 
				finishes, `encodedPacket` is gone and its contents are automatically deleted.
				DecodedFrame is sent to the decodedFramesFifo
			*/
			sendPacket(encodedPacket);
		}
	}