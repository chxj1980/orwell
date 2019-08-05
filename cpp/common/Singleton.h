#ifndef Singleton_H
#define Singleton_H
#include <iostream>
#include <memory>
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "MediaStream.h"
#include <thread>
#include <mutex>


struct SingletonObject {
    std::shared_ptr<MediaStream> mediaStream;
    std::shared_ptr<std::thread> mediaThread;
    std::shared_ptr<VideoReceiver> videoReceiver;
    std::shared_ptr<VideoRecorder> videoRecorder;
    //std::shared_ptr<MovementTracker> movementTracker
};

//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/40337728#40337728
class Singleton
{
public:
    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;

    static void addStream(std::string id, SingletonObject singletonObject) {
        mutex.lock();
        streamList[id] = singletonObject;
        mutex.unlock();
    }

    static SingletonObject getStream(std::string id) {
        mutex.lock();
        SingletonObject singletonObject = streamList[id];
        mutex.unlock();
        return singletonObject;
    }

    //Very important. Must be called from React Native when view is being changed, 
    //so the Media Player gets destroyed and there's no place for ffmpegDecoder to send
    //video data. Otherwise it tries to send data do invalid object.
    static void DisableStreamReceiver(std::string id) {
        mutex.lock();
        SingletonObject singletonObject = streamList[id];
        singletonObject.mediaStream->decoder->disableVideoReceiver();
        mutex.unlock();
    }

    static std::shared_ptr<Singleton> instance()
    {
        static std::shared_ptr<Singleton> s{new Singleton};
        return s;
    }


private:
    Singleton() {}
    static std::map<std::string,SingletonObject> streamList;
    static std::mutex mutex;
};

#endif // Singleton_H
