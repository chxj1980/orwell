#ifndef EncodedUnit_H
#define EncodedUnit_H
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>

class EncodedUnit
{
public:
    EncodedUnit(const EncodedUnit &decodedFrame) = delete;
    EncodedUnit &operator=(const EncodedUnit &) = delete;
    EncodedUnit(EncodedUnit &&) = default;
    EncodedUnit &operator=(EncodedUnit &&) = default;
    //TODO: take this off
    EncodedUnit() : frameBuffer(new uint8_t[bufferSize])
    {
    }
    EncodedUnit(size_t bufferSize) : bufferSize(bufferSize),
                                      frameBuffer(new uint8_t[bufferSize])
    {
    }
    //TODO: make it variable according to video's size?
    size_t bufferSize = 408304;
    //Actual frameSize. Must be less than bufferSize obviously
    size_t frameSize = 0;
    std::unique_ptr<uint8_t> frameBuffer;
};
#endif //EncodedUnit_H