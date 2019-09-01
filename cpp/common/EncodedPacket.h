#ifndef EncodedPacket_H
#define EncodedPacket_H
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>

class EncodedPacket
{
public:
    EncodedPacket(const EncodedPacket &decodedFrame) = delete;
    EncodedPacket &operator=(const EncodedPacket &) = delete;
    EncodedPacket(EncodedPacket &&) = default;
    EncodedPacket &operator=(EncodedPacket &&) = default;
    /*
        Empty constructor is just for when we do things like this:
        EncodedPacket encodedPacket = std::move(otherEncodedPacket);
        No need to allocate data if it's just being used as temporary object 
    */
    EncodedPacket(){}
    EncodedPacket(size_t bufferSize) : frameSize(bufferSize),
                                    frameBuffer(new uint8_t[bufferSize])
    {
    }
    //Actual frameSize. Must be less than bufferSize obviously
    size_t frameSize = 0;
    std::unique_ptr<uint8_t> frameBuffer;
};
#endif //EncodedPacket_H