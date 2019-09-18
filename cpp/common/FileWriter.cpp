#include "FileWriter.h"
#include "SLog.h"
#include "stdexcept"
SLOG_CATEGORY("FileWriter");

FileWriter::FileWriter(std::string cameraAlias) : cameraAlias(cameraAlias)
{
    //file = std::ofstream(path + "file1.h264", std::ios::in | std::ios::binary | std::ios::app | std::ios::ate);
}

void FileWriter::run()
{
    if (!file)
    {
        LOG << "FileWriter: file " << getCurrentPath() << " not open, not going to record for camera " << cameraAlias;
    }
    else
    {
        while (shouldContinue())
        {
            if (onAcquireNewPacket)
            {
                auto encodedPacket = onAcquireNewPacket();
                //TODO: is this reinterpret_cast ok?????????????
                if (encodedPacket)
                    file.write(reinterpret_cast<const char *>(encodedPacket.get()->getFramePointer()), encodedPacket.get()->getSize());
                else
                    LOG << "NULL encodedPacket received";
            } else {
                LOG << "No lambda function passed to FileWriter";
                stop();
            }
        }
    }
}

std::string FileWriter::getCurrentPath()
{
    if (cameraAlias.empty() || currentFileName.empty())
        throw std::runtime_error("filename malformed!");
    return path + "/" + cameraAlias;
}

std::string FileWriter::getCurrentFilePath()
{
    return getCurrentPath() + "/" + currentFileName;
}

bool FileWriter::setPath(std::string path)
{
    std::unique_lock<std::mutex> lock{pathMutex};
    this->path = path;
    lock.unlock();
    //because of this we compile with -lstdc++fs
    namespace fs = std::experimental::filesystem; // In C++17 use std::filesystem.

    fs::create_directories(getCurrentPath());

    file = std::ofstream(getCurrentFilePath(), std::ios::in | std::ios::binary | std::ios::app | std::ios::ate);
    if (!file)
    {
        //Stops the thread
        this->stop();
        return false;
    }
    else
        return true;
}

void FileWriter::startThreadMode()
{
    runThread = std::thread(&FileWriter::run, this);
}

void FileWriter::setOnAcquireNewPacket(std::function<std::shared_ptr<EncodedPacket>()> onAcquireNewPacket)
{
    this->onAcquireNewPacket = onAcquireNewPacket;
}