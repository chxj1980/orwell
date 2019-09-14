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

//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/40337728#40337728
class Singleton
{
public:
    Singleton(Singleton const &) = delete;
    Singleton &operator=(Singleton const &) = delete;

    static void addStream(std::string id, std::shared_ptr<Orwell> orwell)
    {
        std::unique_lock<std::mutex> lock{mutex};
        orwellMap.insert(std::pair<std::string, std::shared_ptr<Orwell>>(id, orwell));
    }

    static std::shared_ptr<Orwell> getStream(std::string id)
    {
        std::unique_lock<std::mutex> lock{mutex};
        return orwellMap.at(id);
    }

    static std::list<std::string> getStreamKeys()
    {
        std::unique_lock<std::mutex> lock{mutex};
        std::list<std::string> returnValue;
        for (auto const &element : orwellMap)
        {
            returnValue.push_front(element.first);
        }
        return returnValue;
    }

    static std::shared_ptr<Singleton> instance()
    {
        static std::shared_ptr<Singleton> singleton{new Singleton};
        return singleton;
    }

private:
    Singleton() {}
    static std::map<std::string, std::shared_ptr<Orwell>> orwellMap;
    static std::mutex mutex;
};

#endif // Singleton_H
