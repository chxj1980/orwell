#ifndef ThreadSafeDeque_H
#define ThreadSafeDeque_H
//based on https://www.xtof.info/blog/?p=593
#include <mutex>
#include <condition_variable>
#include <deque>
#include <iostream>

/*
    Little class to ensure simple policies like size control
    after emplace, or any thing you might imagine.
    Calls to afterEmplaceFront, afterEmplaceBack, afterPopFront
    and afterPopBack are thread-safe because they are called by
    ThreadSafeDeque when the mutex is locked.
    ThreadSafeDequePolicy only shares a mutex with ThreadSafeDeque
    because when ThreadSafeDequePolicy gets its properties
    updated it MUST lock the mutex from ThreadSafeDeque.
*/
template <typename T>
class ThreadSafeDequePolicy
{
public:
    virtual void afterEmplaceFront()
    {
    }

    virtual void afterEmplaceBack()
    {
    }

    virtual void afterPopFront()
    {
    }

    virtual void afterPopBack()
    {
    }

    std::deque<T> &collection;
    std::mutex &mutex;
};

template <typename T>
class ThreadSafeDeque
{
public:
    template <typename... Args>
    void emplace_front(Args &&... args)
    {
        addDataProtected([&] {
            _collection.emplace_front(std::forward<Args>(args)...);
            if (threadSafeDequePolicy)
                threadSafeDequePolicy.afterEmplaceFront();
        });
    }

    template <typename... Args>
    void emplace_back(Args &&... args)
    {
        addDataProtected([&] {
            _collection.emplace_back(std::forward<Args>(args)...);
            if (threadSafeDequePolicy)
                threadSafeDequePolicy.afterEmplaceBack();
        });
    }

    /* 
        Returns the front element and removes it from the collection
        No exception is ever returned as we garanty that the deque is not empty
        before trying to return data.
        This is useful in our while loop renderer, because it just waits if there
        are no members to be popped.
    */
    T pop_front(void) noexcept
    {
        std::unique_lock<std::mutex> lock{_mutex};

        while (_collection.empty())
        {
            _condNewData.wait(lock);
        }
        auto elem = std::move(_collection.front());
        _collection.pop_front();
        if (threadSafeDequePolicy)
            threadSafeDequePolicy.afterPopFront();
        return elem;
    }

    T pop_back(void) noexcept
    {
        std::unique_lock<std::mutex> lock{_mutex};
        while (_collection.empty())
        {
            _condNewData.wait(lock);
        }
        auto elem = std::move(_collection.back());
        _collection.pop_back();
        if (threadSafeDequePolicy)
            threadSafeDequePolicy.afterPopBack();
        return elem;
    }

    typename std::deque<T>::size_type size() const noexcept
    {
        return lockAndDo([&] {
            return _collection.size();
        });
    }

    void setPolicy(std::shared_ptr<ThreadSafeDequePolicy> threadSafeDequePolicy)
    {
        lockAndDo([&] {
            this->threadSafeDequePolicy = threadSafeDequePolicy;
            this->threadSafeDequePolicy.mutex = this->_mutex;
            this->threadSafeDequePolicy.collection = this->_collection;
        });
    }

private:
    /*
        Locks the thread and do something with the deque. 
        Then unique_lock goes away and unlocks the thread
    */

    template <class F>
    decltype(auto) lockAndDo(F &&fct)
    {
        std::unique_lock<std::mutex> lock{_mutex};
        return fct();
    }

    template <class F>
    void addDataProtected(F &&fct)
    {
        lockAndDo(std::forward<F>(fct));
        _condNewData.notify_one();
    }

    std::deque<T> _collection;            // Concrete, not thread safe, storage.
    std::mutex _mutex;                    // Mutex protecting the concrete storage
    std::condition_variable _condNewData; // Condition used to notify that new data are available.
    std::shared_ptr<ThreadSafeDequePolicy> threadSafeDequePolicy;
};
#endif //ThreadSafeDeque_H