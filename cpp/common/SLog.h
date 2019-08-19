#ifndef SLog_H
#define SLog_H
#include <iostream>
#include <mutex>
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
    } config;
    SLog()
    {
        init();
    }
    template <typename T>
    SLog(T t) :
    {
        init();
    }
    template <typename T, typename... Args>
    SLog(T t, Args... args) : SLog(args...)
    {
        //init();
    }
    void init()
    {
        mutex.lock();
    }
    ~SLog()
    {
        if (newLine)
            std::cout << std::endl;
        mutex.unlock();
    }
    template <typename T>
    void applyConfiguration(T t)
    {
    }
    template <typename T>
    void log(T message)
    {
        std::cout << message;
    }
    template <typename T>
    SLog &&operator()(T t)
    {
        applyConfiguration(t);
        return std::forward<SLog>(this);
    }
    template <typename T, typename... Args>
    SLog &&operator()(T t, Args... args)
    {
        operator()(t);
        operator()(args...);
        return std::forward<SLog>(this);
    }

private:
    std::mutex mutex;
    bool newLine = true;
};

template <typename T>
SLog &&operator<<(SLog &&sLog, T message)
{
    sLog.log(message);
    return std::forward<SLog>(sLog);
}

#endif //SLog_H