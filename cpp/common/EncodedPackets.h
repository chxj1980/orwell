#ifndef EncodedPackets_H
#define EncodedPackets_H
#include <iostream>
#include <vector>
#include "EncodedPacket.h"
/*
    A class to hold together multiple EncodedPacket instances
    in order to do NALU finding. It also enables to copy the NALUs
    to a buffer.
    How it works?
    It stores lots of EncodedPacket instances, and treat them as a 
    large file, so given an index in this large file, it will end
    up accessing one of the internal elements. 
*/

class EncodedPackets
{
public:
    EncodedPackets(const EncodedPackets &decodedFrame) = delete;
    EncodedPackets &operator=(const EncodedPackets &) = delete;
    EncodedPackets(EncodedPackets &&) = default;
    EncodedPackets &operator=(EncodedPackets &&) = default;

    EncodedPackets()
    {
    }

    void addUnit(EncodedPacket &&encodedPacket)
    {
        size += encodedPacket.frameSize;
        encodedPackets.push_back(std::move(encodedPacket));
    }
    //Returns the EncodedPacket element that holds data for the index
    EncodedPacket &whichElement(std::streamsize index)
    {
        if (index > size)
        {
            //throw exception
        }
        std::streamsize accumulatedSize = 0;
        std::streamsize futureAccumulatedSize = 0;

        for (auto encodedPacket = encodedPackets.begin(); encodedPacket != encodedPackets.end();)
        {
            accumulatedSize += (*encodedPacket).frameSize;
            if (encodedPacket != encodedPackets.end())
                ++encodedPacket;
            futureAccumulatedSize = accumulatedSize + (*encodedPacket).frameSize;

        }
    }

private:
    std::streamsize size = 0;
    std::vector<EncodedPacket> encodedPackets;
};
#endif //EncodedPackets_H