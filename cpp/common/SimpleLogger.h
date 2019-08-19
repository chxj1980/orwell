#ifndef SimpleLogger_H
#define SimpleLogger_H
#include <iostream>
#include <mutex>
class SimpleLogger
{
public:
    template <typename T>
    void log(T message)
    {
        mutex.lock();
        std::cout << message;
        mutex.unlock();
    }
private:
    std::mutex mutex;
};

template <typename T>
SimpleLogger &operator<<(SimpleLogger &simpleLogger, T message)
{
    simpleLogger.log(message);
    return simpleLogger;
}
#endif //SimpleLogger_H