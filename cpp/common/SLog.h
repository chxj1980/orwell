#ifndef SLog_H
#define SLog_H
#include <iostream>
#include <mutex>
#include <sstream>
#include <queue>
template <typename T>
class ThreadSafeQueue
{
public:
    /* 
        Returns the front element and removes it from the collection
        No exception is ever returned as we garanty that the deque is not empty
        before trying to return data.
        This is useful in our while loop renderer, because it just waits if there
        are no members to be popped.
    */
    T pop(void) noexcept
    {
        std::unique_lock<std::mutex> lock{_mutex};

        while (_collection.empty())
        {
            _condNewData.wait(lock);
        }
        auto elem = std::move(_collection.front());
        _collection.pop();
        return elem;
    }
    template <typename... Args>
    void emplace(Args &&... args)
    {
        addDataProtected([&] {
            _collection.emplace(std::forward<Args>(args)...);
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

private:
    std::queue<T> _collection;            // Concrete, not thread safe, storage.
    std::mutex _mutex;                    // Mutex protecting the concrete storage
    std::condition_variable _condNewData; // Condition used to notify that new data are available.
};
class LoggerThread
{
public:
    LoggerThread(std::unique_ptr<ThreadSafeQueue<std::string>> logMessages) : logMessages(std::move(logMessages))
    {
        thread = std::thread(&LoggerThread::run, this);
    }
    void run()
    {
        /* 
            Blocks until a message is added to queue, so no CPU time is wasted 
            in thread loop
        */
        std::cout << "thread started " << std::endl;
        auto message = logMessages->pop();
        std::cout << "printing message " << std::endl;
        std::cout << message << std::flush;
    }

private:
    std::unique_ptr<ThreadSafeQueue<std::string>> logMessages;
    std::thread thread;
    std::condition_variable condNewData;
};
/*
    Simple logging class designed to accomodate android logging too.
    No high performance, only simple logging.
*/
class SLog
{
public:
    enum CONFIG
    {
        NO_NEW_LINE,
        TO_FILE,
        NO_CONSOLE
    } config;
    SLog()
    {
    }

    template <typename T, typename... Args>
    SLog(T t, Args... args)
    {
        applyConfiguration(t);
        SLog(args...);
    }

    template <typename T>
    void applyConfiguration(T t)
    {
        //std::cout << "applying configuration: " << t << std::endl;
        switch (t)
        {
        case NO_NEW_LINE:
            newLine = false;
            break;
        case TO_FILE:
            toFile = true;
            break;
        case NO_CONSOLE:
            noConsole = true;
            break;
        default:
            break;
        }
    }
    template <typename T>
    void accumulateMessage(T message)
    {
        //std::cout << message;
    }
    void queue(std::stringstream &stringStream)
    {
        //std::cout << 'queueing ' << stringStream.str() << std::endl;
        logMessages->emplace(stringStream.str());
    }
    template <typename T>
    SLog &&operator()(T t)
    {
        applyConfiguration(t);
        return std::forward<SLog>(*this);
    }
    template <typename T, typename... Args>
    SLog &&operator()(T t, Args... args)
    {
        SLog &&a = operator()(t);
        operator()(args...);
        return std::forward<SLog>(a);
    }

private:
    std::unique_ptr<ThreadSafeQueue<std::string>> logMessages;
    LoggerThread loggerThread{std::move(logMessages)};

public:
    bool newLine = true;
    bool toFile = false;
    bool noConsole = false;
};

struct SLogBuffer
{
public:
    std::stringstream ss;
    SLog *sLog;
    SLogBuffer() = default;
    SLogBuffer(const SLogBuffer &) = delete;
    SLogBuffer &operator=(const SLogBuffer &) = delete;
    SLogBuffer &operator=(SLogBuffer &&) = delete;
    SLogBuffer(SLogBuffer &&buf) : ss(std::move(buf.ss))
    {
    }
    template <typename T>
    SLogBuffer &operator<<(T &&message)
    {
        ss << std::forward<T>(message);
        return *this;
    }

    ~SLogBuffer()
    {
        if (sLog->newLine)
            sLog->queue(ss);
        else
        {
            ss << "\n";
            sLog->queue(ss);
        }
    }
};
/* 
template <typename T>
SLog &&operator<<(SLog &&sLog, T message)
{
    sLog.accumulateMessage(message);
    return std::forward<SLog>(sLog);
}//SLogBuffer
*/
template <typename T>
SLogBuffer operator<<(SLog &&sLog, T message)
{
    SLogBuffer buffer;
    buffer.sLog = &sLog;
    buffer.ss << std::forward<T>(message);
    return buffer;
}

#endif //SLog_H