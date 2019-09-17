#ifndef FileWriter_H
#define FileWriter_H
#include <vector>
#include <thread>
#include <iostream>
#include <memory>
#include "EncodedPacket.h"
#include "DecodedFrame.h"
#include "ThreadSafeDeque.h"
#include "Stoppable.h"
#include <functional>
#include <fstream>

class FileWriter : public Stoppable
{
public:
    FileWriter(std::string path) : path(path)
    {
        file = std::ofstream(path, std::ios::in | std::ios::binary | std::ios::app);
    }

    virtual void run()
    {
        if (!file)
        {
            std::cout << "Cannot open file!" << std::endl;
        }
        else
        {
            while (shouldContinue())
            {
                auto encodedPacket = onAcquireNewPacket();
                //file.write(static_cast<const char *>(encodedPacket.get()->getFramePointer()), encodedPacket.get()->getSize());
            }
        }
    }

    virtual void startThreadMode()
    {
        runThread = std::thread(&FileWriter::run, this);
    }

    virtual void setOnAcquireNewPacket(std::function<std::shared_ptr<EncodedPacket>()> onAcquireNewPacket)
    {
        this->onAcquireNewPacket = onAcquireNewPacket;
    }

protected:
    std::thread runThread;
    std::function<std::shared_ptr<EncodedPacket>()> onAcquireNewPacket;
    std::string path;
    std::ofstream file;
};

#endif // FileWriter_H
