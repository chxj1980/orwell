#ifndef ThreadSafeDequePolicies_H
#define ThreadSafeDequePolicies_H
#include <mutex>
#include <deque>

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
        if (mutex)
            std::unique_lock<std::mutex> lock{*mutex};
        if (maxSize <= 0)
            throw std::invalid_argument("SizePolicy::setSize received invalid value");
        this->maxSize = maxSize;
    }
    //Thread safe because it's called from ThreadSafeDeque when mutex is locked
    void afterEmplaceFront()
    {
        if (collection->size() >= maxSize)
            collection->pop_back();
    }
    //Thread safe because it's called from ThreadSafeDeque when mutex is locked
    void afterEmplaceBack()
    {
        if (collection->size() >= maxSize)
            collection->pop_front();
    }

    int maxSize;
    std::shared_ptr<std::deque<T>> collection;
    std::shared_ptr<std::mutex> mutex;
};
#endif //ThreadSafeDequePolicies_H