#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>

class EncodedFrame
{
public:
    EncodedFrame() : frameBuffer(new uint8_t[bufferSize])
    {
    }
    EncodedFrame(size_t bufferSize) : bufferSize(bufferSize),
                                      frameBuffer(new uint8_t[bufferSize])
    {
    }
    //TODO: make it variable according to video's size?
    size_t bufferSize = 408304;
    //Actual frameSize. Must be less than bufferSize obviously
    size_t frameSize = 0;
    std::unique_ptr<uint8_t> frameBuffer;
};