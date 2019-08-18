#ifndef ThreadSafeDeque_H
#define ThreadSafeDeque_H
//based on https://www.xtof.info/blog/?p=593
#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T>
class ThreadSafeDeque
{
    //using const_iterator = typename std::deque<T>::const_iterator;

public:
    template <typename... Args>
    void emplace_front(Args &&... args)
    {
        addDataProtected([&] {
            _collection.emplace_front(std::forward<Args>(args)...);
        });
    }

    template <typename... Args>
    void emplace_back(Args &&... args)
    {
        addDataProtected([&] {
            _collection.emplace_back(std::forward<Args>(args)...);
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
        return elem;
    }

    typename std::deque<T>::size_type size() const noexcept
    {
        return lockAndDoAndReturn([&] {
            return _collection.size();
        });
    }

private:
    /*
        Locks the thread and do something with the deque. 
        Then unique_lock goes away and unlocks the thread
    */
    template <class F>
    F lockAndDoAndReturn(F &&fct)
    {
        std::unique_lock<std::mutex> lock{_mutex};
        return fct();
        //lock.unlock(); //Not needed as unique_lock does this when it goes away
    }
    template <class F>
    void lockAndDo(F &&fct)
    {
        std::unique_lock<std::mutex> lock{_mutex};
        fct();
        //lock.unlock(); //Not needed as unique_lock does this when it goes away
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
};
#endif //ThreadSafeDeque_H