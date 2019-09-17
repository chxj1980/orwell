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
        LOG << "FileWriter: file "<< getCurrentPath() << " not open, not going to record for camera " << cameraAlias;
    }
    else
    {
        while (shouldContinue())
        {
            auto encodedPacket = onAcquireNewPacket();
            //TODO: is it ok?????????????
            file.write(reinterpret_cast<const char *>(encodedPacket.get()->getFramePointer()), encodedPacket.get()->getSize());
        }
    }
}

std::string FileWriter::getCurrentPath() {
    if (cameraAlias.empty() || currentFileName.empty())
        throw std::runtime_error("filename malformed!");
    return path + "/" + cameraAlias + "/" + currentFileName;
}

bool FileWriter::setPath(std::string path)
{
    std::unique_lock<std::mutex> lock{pathMutex};
    this->path = path;
    lock.unlock();

    file = std::ofstream(getCurrentPath(), std::ios::in | std::ios::binary | std::ios::app | std::ios::ate);
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