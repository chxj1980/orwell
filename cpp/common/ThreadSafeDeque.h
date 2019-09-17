#ifndef ThreadSafeDeque_H
#define ThreadSafeDeque_H
//based on https://www.xtof.info/blog/?p=593
#include <mutex>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <memory>

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
    //Thread safe because it's called from ThreadSafeDeque when mutex is locked
    virtual void afterEmplaceFront()
    {
    }
    //Thread safe because it's called from ThreadSafeDeque when mutex is locked
    virtual void afterEmplaceBack()
    {
    }
    //Thread safe because it's called from ThreadSafeDeque when mutex is locked
    virtual void afterPopFront()
    {
    }
    //Thread safe because it's called from ThreadSafeDeque when mutex is locked
    virtual void afterPopBack()
    {
    }

    std::shared_ptr<std::deque<T>> collection;
    //Attention, derived classes that have other functions must lock the mutex on these functions
    std::shared_ptr<std::mutex> mutex;
};

template <typename T>
class ThreadSafeDeque
{
public:
    
    template <typename... Args>
    void emplace_front(Args &&... args)
    {
        addDataProtected([&] {
            collection->emplace_front(std::forward<Args>(args)...);
            if (threadSafeDequePolicy)
                threadSafeDequePolicy->afterEmplaceFront();
        });
    }

    template <typename... Args>
    void emplace_back(Args &&... args)
    {
        addDataProtected([&] {
            collection->emplace_back(std::forward<Args>(args)...);
            if (threadSafeDequePolicy)
                threadSafeDequePolicy->afterEmplaceBack();
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
        std::unique_lock<std::mutex> lock{*mutex};

        while (collection->empty())
        {
            condNewData.wait(lock);
        }
        auto elem = std::move(collection->front());
        collection->pop_front();
        if (threadSafeDequePolicy)
            threadSafeDequePolicy->afterPopFront();
        return elem;
    }

    T pop_back(void) noexcept
    {
        std::unique_lock<std::mutex> lock{*mutex};
        while (collection->empty())
        {
            condNewData.wait(lock);
        }
        auto elem = std::move(collection->back());
        collection->pop_back();
        if (threadSafeDequePolicy)
            threadSafeDequePolicy->afterPopBack();
        return elem;
    }

    typename std::deque<T>::size_type size() const noexcept
    {
        return lockAndDo([&] {
            return collection->size();
        });
    }

    void setPolicy(std::shared_ptr<ThreadSafeDequePolicy<T>> threadSafeDequePolicy)
    {
        lockAndDo([&] {
            this->threadSafeDequePolicy = threadSafeDequePolicy;
            this->threadSafeDequePolicy->mutex = this->mutex;
            this->threadSafeDequePolicy->collection = this->collection;
        });
    }

    std::shared_ptr<ThreadSafeDequePolicy<T>> getPolicy()
    {
        lockAndDo([&] {
            return threadSafeDequePolicy;
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
        std::unique_lock<std::mutex> lock{*mutex};
        return fct();
    }

    template <class F>
    void addDataProtected(F &&fct)
    {
        lockAndDo(std::forward<F>(fct));
        condNewData.notify_one();
    }

    std::shared_ptr<std::deque<T>> collection{std::make_shared<std::deque<T>>()};
    std::shared_ptr<std::mutex> mutex{std::make_shared<std::mutex>()};
    std::shared_ptr<ThreadSafeDequePolicy<T>> threadSafeDequePolicy;
    std::condition_variable condNewData;
};
#endif //ThreadSafeDeque_H