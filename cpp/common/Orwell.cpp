#include "Orwell.h"
#include "Profiler.h"
#include "SLog.h"
#include "Singleton.h"
static SLog::SLog LOGP(SLog::Category("Profiler"));

//No more than x encoded packets
const int maxEncodedPacketFifoSize = 40;
//No more than x decoded video frames
const int maxDecodedFrameFifoSize = 15;
std::shared_ptr<ThreadSafeDequePolicy<std::shared_ptr<EncodedPacket>>> encodedPacketFifoSizePolicy = std::make_shared<SizePolicy<std::shared_ptr<EncodedPacket>>>(maxEncodedPacketFifoSize);
std::shared_ptr<ThreadSafeDequePolicy<std::shared_ptr<DecodedFrame>>> decodedFrameFifoSizePolicy = std::make_shared<SizePolicy<std::shared_ptr<DecodedFrame>>>(maxDecodedFrameFifoSize);
//std::shared_ptr<ProfilingThread> Orwell::profilingThread = std::make_shared<ProfilingThread>();

Orwell::Orwell(std::shared_ptr<RTSPClient> _rtspClient, std::shared_ptr<Decoder> _decoder, std::shared_ptr<Renderer> _renderer)
{
    rtspClient = _rtspClient;
    //FIFOs for encoded and decoder
    encodedPacketsFifo = std::make_shared<ThreadSafeDeque<std::shared_ptr<EncodedPacket>>>();
    encodedPacketsFifo->setPolicy(encodedPacketFifoSizePolicy);
    decodedFramesFifo = std::make_shared<ThreadSafeDeque<std::shared_ptr<DecodedFrame>>>();
    decodedFramesFifo->setPolicy(decodedFrameFifoSizePolicy);
    //Decoders
    decoder = _decoder;
    //decoder->setEncodedPacketsFifo(encodedPacketsFifo);
    //decoder->setDecodedFramesFifo(decodedFramesFifo);
    auto& encodedPacketsFifoReference = encodedPacketsFifo;
    auto& decodedFramesFifoReference = decodedFramesFifo;
    //Makes a COPY of the shared_ptrs below from its references
    //TODO: verify this
    decoder->setOnAcquireNewPacket([encodedPacketsFifo = encodedPacketsFifoReference]() -> std::shared_ptr<EncodedPacket> {
        return encodedPacketsFifo->pop_front();
    });
    decoder->setOnNewDecodedFrame([decodedFramesFifo = decodedFramesFifoReference](std::shared_ptr<DecodedFrame> decodedFrame){
        decodedFramesFifo->emplace_back(decodedFrame);
    });
    //Important, only start thread mode after inserting FIFOs like in above
    decoder->startThreadMode();
    //RTSP client
    //rtspClient->setEncodedPacketsFifo(encodedPacketsFifo);
    rtspClient->setOnNewPacket([encodedPacketsFifo = encodedPacketsFifoReference](std::shared_ptr<EncodedPacket> encodedPacket){
        encodedPacketsFifo->emplace_back(encodedPacket);
    });
    //Important, only start thread mode after inserting FIFOs like in above
    rtspClient->startThreadMode();
    renderer = _renderer;
    //renderer->setDecodedFramesFifo(decodedFramesFifo);
    renderer->setOnAcquireNewDecodedFrame([decodedFramesFifo = decodedFramesFifoReference]()->std::shared_ptr<DecodedFrame>{
        return decodedFramesFifo->pop_front();
    });
    renderer->startThreadMode();
    //profilingThread->addProfilerVariable(rtspClient->bytesPerSecond);
    //profilingThread->addProflingVariable(renderer->fps);

    //rtspClientThread = std::make_shared<std::thread>(&RTSPClient::run, rtspClient);
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
            //get weak ptr?
            LOGP << cam << ": " << orwell->renderer->fps->getSampleString() << " fps"
                 << ", "
                 << bytesToKbytes(orwell->rtspClient->bytesPerSecond->getSampleString()) << "kb/s"
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