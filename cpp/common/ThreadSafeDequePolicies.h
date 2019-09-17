#ifndef ThreadSafeDequePolicies_H
#define ThreadSafeDequePolicies_H
#include <mutex>
#include <deque>
#include "EncodedPacket.h"

/*
    Simplest policy possible. Ensures our deque doesn't get bigger than
    maxSize. This is useful so our RAM doesn't flood in case some thread
    stop consuming the deque for any reason.
*/
template <typename T>
class SizePolicy : public ThreadSafeDequePolicy<T>
{
public:
    SizePolicy(int maxSize) : maxSize(maxSize)
    {
        if (maxSize <= 0)
            throw std::invalid_argument("SizePolicy received invalid value");
    }
    //Not thread-safe, we must lock the mutex
    void setSize(int maxSize)
    {
        std::unique_lock<std::mutex> lock{*mutex};
        if (maxSize <= 0)
            throw std::invalid_argument("SizePolicy::setSize received invalid value");
        this->maxSize = maxSize;
    }

    void afterEmplaceFront()
    {
        if (collection->size() >= maxSize)
            collection->pop_back();
    }
    void afterEmplaceBack()
    {
        if (collection->size() >= maxSize)
            collection->pop_front();
    }

    int maxSize;
};
/*
    Policy that puts a limit in the amount of RAM used by the deque. Actually
    only the costly part of the object is counted, the rest is ignored, so it's not actual RAM, but it's 
    fine
*/
//template <typename T>
class RamSizePolicy : public ThreadSafeDequePolicy<std::shared_ptr<EncodedPacket>>
{
public:
    //TODO: ensure deque size 0 when this object is addd otherwise wrong RAM measurements
    RamSizePolicy(int maxRamSize) : maxRamSize(maxRamSize)
    {
        if (maxRamSize <= 0)
            throw std::invalid_argument("RamSizePolicy received invalid value");
        currentRamSize.store(0);
    }
    void setMaximumRamSize(int maxRamSize)
    {
        std::unique_lock<std::mutex> lock{*mutex};
        if (maxRamSize <= 0)
            throw std::invalid_argument("RamSizePolicy::setSize received invalid value");
        this->maxRamSize = maxRamSize;
    }
    size_t getRamSize() {
        return currentRamSize.load();
    }
    void afterEmplaceFront()
    {
        currentRamSize += collection->front()->getSize();

        if (collection->size() >= maxRamSize)
        {
            currentRamSize -= collection->back()->getSize();
            collection->pop_back();
        }
    }
    void afterEmplaceBack()
    {
        currentRamSize += collection->back()->getSize();

        if (collection->size() >= maxRamSize)
        {
            currentRamSize -= collection->front()->getSize();
            collection->pop_front();
        }
    }
    //In bytes
    int maxRamSize;
    std::atomic<size_t> currentRamSize;
    std::mutex currentRamSizeMutex;
};
#endif //ThreadSafeDequePolicies_H