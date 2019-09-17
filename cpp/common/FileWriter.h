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
#include <mutex>

class FileWriter : public Stoppable
{
public:
    FileWriter(std::string cameraAlias);

    virtual void run();

    bool setPath(std::string path);

    virtual void startThreadMode();

    virtual void setOnAcquireNewPacket(std::function<std::shared_ptr<EncodedPacket>()> onAcquireNewPacket);

    std::string getCurrentPath();

protected:
    std::thread runThread;
    std::function<std::shared_ptr<EncodedPacket>()> onAcquireNewPacket;
    std::mutex pathMutex;
    std::string path;
    std::ofstream file;
    std::string cameraAlias;
    std::string currentFileName = "file1.h264";
};

#endif // FileWriter_H
