#ifndef Stoppable_H
#define Stoppable_H
#include <atomic>

/*
    To be subclassed by our thread class so we can stop simply
    by calling class.stop();
*/
class Stoppable
{
    std::atomic<bool> shouldContinue;

public:
    Stoppable() {
        shouldContinue.store(true);
    }
   
    virtual void run() = 0;
    void operator()()
    {
        run();
    }
    bool shouldContinue()
    {
        return shouldContinue.load();
    }
    void stop()
    {
        shouldContinue.store(true);
    }
};
#endif //Stoppable_H