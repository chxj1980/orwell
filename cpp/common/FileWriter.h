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
        file = std::ofstream(path + "file1.h264", std::ios::in | std::ios::binary | std::ios::app | std::ios::ate);
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
                std::cout << "." << std::endl;
                auto encodedPacket = onAcquireNewPacket();
                //TODO: is it ok?????????????
                file.write(reinterpret_cast<const char *>(encodedPacket.get()->getFramePointer()), encodedPacket.get()->getSize());
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
