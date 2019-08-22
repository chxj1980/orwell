#ifndef SLog_H
#define SLog_H
#include <iostream>
#include <mutex>
#include <sstream>
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
    void accumulateMessage(T message)
    {
        std::cout << message;
    }
    void queue(std::stringstream& stringStream) {
        std::cout << stringStream.str() << std::endl;;
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
    std::mutex mutex;
    bool newLine = true;
    bool toFile = false;
    bool noConsole = false;
};
struct SLogBuffer
{
    public:
    std::stringstream ss;
    SLog sLog;
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
        sLog.queue(ss);
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
SLogBuffer &&operator<<(SLog &&sLog, T message)
{
    SLogBuffer buffer;
    buffer.sLog = std::move(sLog);
    buffer.ss << std::forward<T>(message);
    return buffer;
}

#endif //SLog_H