#ifndef Singleton_H
#define Singleton_H
#include <iostream>
#include <memory>
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "RTSPClient.h"
#include <thread>
#include <mutex>
#include <deque>
#include "DecodedFrame.h"
#include "Orwell.h"

/* 
struct SingletonObject
{
    std::shared_ptr<RTSPClient> rtspClient;
    std::shared_ptr<std::thread> mediaThread;
    std::shared_ptr<ThreadSafeDeque<EncodedFrame>> encodedFramesFifo;
    std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo;
    std::shared_ptr<Decoder> decoder;
    std::shared_ptr<std::thread> decoderThread;
    //std::shared_ptr<VideoReceiver> videoReceiver;
    std::shared_ptr<VideoRecorder> videoRecorder;
    //std::shared_ptr<MovementTracker> movementTracker
};
*/

//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/40337728#40337728
class Singleton
{
public:
    Singleton(Singleton const &) = delete;
    Singleton &operator=(Singleton const &) = delete;

    static void addStream(std::string id, Orwell orwell)
    {
        mutex.lock();
        orwellList[id] = orwell;
        mutex.unlock();
    }

    static Orwell getStream(std::string id)
    {
        mutex.lock();
        Orwell orwell = orwellList[id];
        mutex.unlock();
        return orwell;
    }

    static std::shared_ptr<Singleton> instance()
    {
        static std::shared_ptr<Singleton> s{new Singleton};
        return s;
    }

private:
    Singleton() {}
    static std::map<std::string, Orwell> orwellList;
    static std::mutex mutex;
};

#endif // Singleton_H
