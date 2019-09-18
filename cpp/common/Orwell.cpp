#include "Orwell.h"
#include "Profiler.h"
#include "SLog.h"
#include "Singleton.h"
static SLog::SLog LOGP(SLog::Category("Profiler"));

//No more than x encoded packets
const int maxEncodedPacketFifoSize = 40;
//No more than x decoded video frames
const int maxDecodedFrameFifoSize = 15;
//Default size in bytes
const int defaultMaxRamSizeCacheFifo = 500000;

Orwell::Orwell(std::string cameraAlias, std::shared_ptr<RTSPClient> _rtspClient, 
               std::shared_ptr<Decoder> _decoder, std::shared_ptr<Renderer> _renderer)
{
    encodedPacketsFifo = std::make_shared<ThreadSafeDeque<std::shared_ptr<EncodedPacket>>>();
    encodedPacketsFifo->setPolicy(std::make_shared<SizePolicy<std::shared_ptr<EncodedPacket>>>(maxEncodedPacketFifoSize));
    encodedPacketsCacheFifo = std::make_shared<ThreadSafeDeque<std::shared_ptr<EncodedPacket>>>();
    encodedPacketsCacheFifo->setPolicy(std::make_shared<RamSizePolicy>(defaultMaxRamSizeCacheFifo));
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<std::shared_ptr<DecodedFrame>>>();
    decodedFramesFifo->setPolicy(std::make_shared<SizePolicy<std::shared_ptr<DecodedFrame>>>(maxDecodedFrameFifoSize));
    //These references help we pass by copy inside the lambda functions
    auto &encodedPacketsFifoReference = encodedPacketsFifo;
    auto &decodedFramesFifoReference = decodedFramesFifo;
    auto &encodedPacketsCacheFifoReference = encodedPacketsCacheFifo;
    rtspClient = _rtspClient;
    rtspClient->setOnNewPacket([encodedPacketsFifo = encodedPacketsFifoReference, encodedPacketsCacheFifo = encodedPacketsCacheFifoReference](std::shared_ptr<EncodedPacket> encodedPacket) {
        encodedPacketsFifo->emplace_back(encodedPacket);
        encodedPacketsCacheFifo->emplace_back(encodedPacket);
    });

    fileWriter = std::make_shared<FileWriter>(cameraAlias);
    fileWriter->setOnAcquireNewPacket([encodedPacketsCacheFifo = encodedPacketsCacheFifoReference]() -> std::shared_ptr<EncodedPacket> {
        return encodedPacketsCacheFifo->pop_front();
    });

    decoder = _decoder;
    //TODO: verify this
    decoder->setOnAcquireNewPacket([encodedPacketsFifo = encodedPacketsFifoReference]() -> std::shared_ptr<EncodedPacket> {
        return encodedPacketsFifo->pop_front();
    });
    decoder->setOnNewDecodedFrame([decodedFramesFifo = decodedFramesFifoReference](std::shared_ptr<DecodedFrame> decodedFrame) {
        decodedFramesFifo->emplace_back(decodedFrame);
    });
    
    renderer = _renderer;
    renderer->setOnAcquireNewDecodedFrame([decodedFramesFifo = decodedFramesFifoReference]() -> std::shared_ptr<DecodedFrame> {
        return decodedFramesFifo->pop_front();
    });

    rtspClient->startThreadMode();
    //fileWriter->startThreadMode();
    decoder->startThreadMode();
    renderer->startThreadMode();
}

//Interval in milliseconds
const int profilingInterval = 250;

ProfilingThread::ProfilingThread()
{
    LOGP.applyConfiguration(SLog::Filename("profiler.log"));
    LOGP.applyConfiguration(SLog::TO_FILE);
    //LOG.enableCategories("Profiler");
    thread = std::thread(&ProfilingThread::run, this);
}
static int bytesToKbytes(std::string s)
{
    auto f = std::stod(s) / 1000;
    return f;
}
void ProfilingThread::run()
{
    while (shouldContinue())
    {
        //std::unique_lock<std::mutex> lock{mutex};
        /*
        for (auto profilerVariable : profilerVariables)
        {
            LOG << profilerVariable.getSample();
        }
        lock.unlock();
        */
        for (auto cam : Singleton::instance()->getStreamKeys())
        {
            auto orwell = Singleton::instance()->getStream(cam);
            auto encodedPacketsCacheFifoRamSizePolicy = std::dynamic_pointer_cast<RamSizePolicy>(orwell->encodedPacketsCacheFifo->getPolicy());
            //get weak ptr?
            LOGP << cam << ": " << orwell->renderer->fps->getSampleString() << " fps"
                 << ", " << bytesToKbytes(orwell->rtspClient->bytesPerSecond->getSampleString()) << "kb/s"
                 //<< "encodedPacketsFifo: " << orwell->encodedPacketsFifo->size()
                 //<< "decodedFramesFifo: " << orwell->decodedFramesFifo->size()
                 << ", " << "cacheFifo RAM size: " << bytesToKbytes(std::to_string(encodedPacketsCacheFifoRamSizePolicy->getRamSize())) << " kb"
                 << ", ";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(profilingInterval));
    }
}
/*
//C interface
void *orwell_init_from_rtsp(char *rtspUrl)
{
    //RTSPUrl rtspUrlObject(rtspUrl);
    //return new Orwell(rtspUrlObject);
}

void* orwell_init_from_onvif(char *onvifUrl)
{
    RTSPUrl onvifUrlObject(onvifUrl);
    return new Orwell(onvifUrlObject);
}


void orwell_destroy(void *untyped_ptr)
{
    Orwell *typed_ptr = static_cast<Orwell *>(untyped_ptr);
    delete typed_ptr;
}

void orwell_doit(void *untyped_self, int param)
{
    Orwell *typed_self = static_cast<Orwell *>(untyped_self);
    //typed_self->doIt(param);
}
*/