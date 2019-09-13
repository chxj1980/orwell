#ifndef Profiler_H
#define Profiler_H
#include <chrono>
#include <stdexcept>
#include <functional>
#include "Stoppable.h"
#include <thread>
#include <mutex>

//Just so we can treat ProfilerVariable<T> equally
class ProfilerVariableInterface
{
public:
    virtual std::string getSampleString(){};
};

/*
    A class to safely update the sample variable at the end of a 
*/
template <typename T>
class ProfilerVariable : public ProfilerVariableInterface
{
public:
    ProfilerVariable(int interval) : interval(interval)
    {
    }
    void profile(std::function<void(T &)> const &increaseFunction,
                 std::function<void(T &)> const &resetFunction)
    {
        auto now = std::chrono::system_clock::now();
        auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(now - before).count();
        if (difference >= interval)
        {
            snapshot();
            resetFunction(counter);
            before = std::chrono::system_clock::now();
        }
        increaseFunction(counter);
    }

    //To be executed at the end of the profiling period, takes a snapshot of counter
    void snapshot()
    {
        std::unique_lock<std::mutex> lock{mutex};
        sample = counter;
    }
    //TODO: return is safe????
    T getSample()
    {
        std::unique_lock<std::mutex> lock{mutex};
        return sample;
    }
    //To be shown as string, converts the generic T type to string
    std::string getSampleString() {
        return std::to_string(sample);
    }

    //Value to be increased at every profile call. `counter` can only be modified from one thread
    T counter;

private:
    //Value to store the result of the accumulated counter at the end of profile period
    T sample;
    //Mutex for setting and reading sample.
    std::mutex mutex;
    //Using array (not vector) to be fast. Only drawback is MAX ammount of profiler variables
    std::chrono::system_clock::time_point before;
    //Interval in milliseconds
    int interval;
};

/*
    Classes should subclass this class to have profiling capabilities. 
    They can simply call profile(index,...) using a different index for each 
    variable, up to MAX of them
*/

class ProfilingThread : public Stoppable
{
public:
    ProfilingThread();
    void run();
    //void addProfilerVariable(std::shared_ptr<ProfilerVariableInterface> profilerVariable)
    //{
    //    std::unique_lock<std::mutex> lock{mutex};
    //    profilerVariables.push_back(profilerVariable);
    //}

private:
    std::thread thread;
    std::mutex mutex;
    //std::list<std::shared_ptr<ProfilerVariableInterface>> profilerVariables;
};

#endif //Profiler_H