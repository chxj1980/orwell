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
        std::unique_lock<std::mutex> lock{*this->mutex};
        if (maxSize <= 0)
            throw std::invalid_argument("SizePolicy::setSize received invalid value");
        this->maxSize = maxSize;
    }

    void afterEmplaceFront()
    {
        if (this->collection->size() >= maxSize)
            this->collection->pop_back();
    }
    void afterEmplaceBack()
    {
        if (this->collection->size() >= maxSize)
            this->collection->pop_front();
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
        std::unique_lock<std::mutex> lock{*this->mutex};
        if (maxRamSize <= 0)
            throw std::invalid_argument("RamSizePolicy::setSize received invalid value");
        this->maxRamSize = maxRamSize;
    }
    size_t getRamSize() {
        return currentRamSize.load();
    }
    //Going to eliminate element in the back, so measure it and take it off of currentRamSize
    void beforePopBack() {
            currentRamSize -= this->collection->back()->getSize();
    }
    //Going to eliminate element in the front, so measure it and take it off of currentRamSize
    void beforePopFront() {
            currentRamSize -= this->collection->front()->getSize();
    }
    //Going to add element in the front, so measure it and add to currentRamSize
    //Also, eliminate elements from back if RAM is high
    void afterEmplaceFront()
    {
        currentRamSize += this->collection->front()->getSize();

        while (currentRamSize >= maxRamSize)
        {
            //TODO: add warning that cache fifo got full and we're losing packets to write
            currentRamSize -= this->collection->back()->getSize();
            this->collection->pop_back();
        }
    }
    //Going to add element in the back, so measure it and add to currentRamSize
    //Also, eliminate elements from front if RAM is high
    void afterEmplaceBack()
    {
        currentRamSize += this->collection->back()->getSize();

        while (currentRamSize >= maxRamSize)
        {
            //TODO: add warning that cache fifo got full and we're losing packets to write
            currentRamSize -= this->collection->front()->getSize();
            this->collection->pop_front();
        }
    }
    //private:
    //In bytes
    int maxRamSize;
    std::atomic<size_t> currentRamSize;
    std::mutex currentRamSizeMutex;
};
#endif //ThreadSafeDequePolicies_H