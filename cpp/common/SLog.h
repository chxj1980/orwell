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
        NO_CONSOLE
    } config;
    SLog()
    {
        init();
    }
    template <typename T>
    void SLog_(T t)
    {
        applyConfiguration(t);
    }
    template <typename T, typename... Args>
    SLog(T t, Args... args)
    {
        SLog_(t);
        SLog(args...);
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
        std::cout << "applying configuration: " << t << std::endl;
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
    void log(T message)
    {
        std::cout << message;
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
        SLog && a = operator()(t);
        operator()(args...);
        return std::forward<SLog>(a);
    }

private:
    std::mutex mutex;
    bool newLine = true;
    bool toFile = false;
    bool noConsole = false;
};

template <typename T>
SLog &&operator<<(SLog &&sLog, T message)
{
    sLog.log(message);
    return std::forward<SLog>(sLog);
}

#endif //SLog_H