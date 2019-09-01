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
    /*
        Empty constructor is just for when we do things like this:
        EncodedUnit encodedUnit = std::move(otherEncodedUnit);
        No need to allocate data if it's just being used as temporary object 
    */
    EncodedUnit(){}
    EncodedUnit(size_t bufferSize) : frameSize(bufferSize),
                                    frameBuffer(new uint8_t[bufferSize])
    {
    }
    //Actual frameSize. Must be less than bufferSize obviously
    size_t frameSize = 0;
    std::unique_ptr<uint8_t> frameBuffer;
};
#endif //EncodedUnit_H