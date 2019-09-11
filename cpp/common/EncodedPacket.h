#ifndef EncodedPacket_H
#define EncodedPacket_H
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <cstdint>

class EncodedPacket
{
public:
    EncodedPacket(const EncodedPacket &decodedFrame) = delete;
    EncodedPacket &operator=(const EncodedPacket &) = delete;
    EncodedPacket(EncodedPacket &&) = default;
    EncodedPacket &operator=(EncodedPacket &&) = default;

    EncodedPacket()
    {
    }
    EncodedPacket(size_t size) : size(size),
                                 frame(new uint8_t[size])
    {
    }

    /*
        Subclass these method to get the pointer to the data you need
    */
    virtual uint8_t *getFramePointer()
    {
        return frame.get();
    }

    virtual uint32_t getSize()
    {
        return size;
    }

    //Actual frameSize. Must be less than bufferSize obviously
    size_t size = 0;
    std::unique_ptr<uint8_t> frame;
};
#endif //EncodedPacket_H