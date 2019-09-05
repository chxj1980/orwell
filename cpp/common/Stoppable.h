#ifndef Stoppable_H
#define Stoppable_H
#include <atomic>

/*
    To be subclassed by our thread class so we can stop simply
    by calling class.stop();
*/
class Stoppable
{
private:
    std::atomic<bool> _shouldContinue;

public:
    Stoppable()
    {
        _shouldContinue.store(true);
    }

    Stoppable(const Stoppable&& other)
    {
        _shouldContinue.store(other._shouldContinue.load());
    }

    virtual void run() = 0;
    void operator()()
    {
        run();
    }
    bool shouldContinue()
    {
        return _shouldContinue.load();
    }
    void stop()
    {
        _shouldContinue.store(false);
    }
};
#endif //Stoppable_H