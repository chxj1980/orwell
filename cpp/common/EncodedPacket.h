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
    EncodedPacket() {}
    EncodedPacket(const EncodedPacket &encodedPacket) = delete;
    EncodedPacket &operator=(const EncodedPacket &) = delete;
    EncodedPacket(EncodedPacket &&) = default;
    EncodedPacket &operator=(EncodedPacket &&) = default;

    virtual uint8_t* getFramePointer() = 0;
    virtual size_t* getSizePointer() = 0;
    virtual size_t getSize() = 0;
};
typedef std::shared_ptr<EncodedPacket> SharedEncodedPacket;
#endif //EncodedPacket_H