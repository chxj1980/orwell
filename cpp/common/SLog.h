#ifndef SLog_H
#define SLog_H
#include <iostream>
#include <mutex>
class SLog
{
public:
    static const bool NO_NEW_LINE = false;
    SLog()
    {
        init();
    }
    SLog(bool newLine) : newLine(newLine)
    {
        init();
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
    void log(T message)
    {
        std::cout << message;
    }

private:
    /* 
    Very important, only one global mutex, otherwise
    we couldn't call like this:
    SLog() << "hello " << "world";
    and expect it to be thread safe between consicutive 
    << operator calls
    */
    static std::mutex mutex;
    bool newLine = true;
};

template <typename T>
SLog &&operator<<(SLog &&sLog, T message)
{
    sLog.log(message);
    return std::forward<SLog>(sLog);
}
#endif //SLog_H