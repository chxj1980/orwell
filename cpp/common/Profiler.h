#ifndef Profiler_H
#define Profiler_H
#include <chrono>
#include <stdexcept>
#include <functional>
#include "Stoppable.h"
#include <thread>
/*
    A class to safely update the sample variable at the end of a 
*/
template <typename T>
class ProfilerVariable
{
public:
    //To be executed at the end of the profiling period
    void profile()
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
    //Value to be increased at every profile call. `counter` can only be modified from one thread
    T counter;

private:
    //Value to store the result of the accumulated counter at the end of profile period
    T sample;
    //Mutex for setting and reading sample.
    std::mutex mutex;
};

const int MAX = 10;
/*
    Classes should subclass this class to have profiling capabilities. 
    They can simply call profile(index,...) using a different index for each 
    variable, up to MAX of them
*/
class Profiler
{
public:
    /*
        If interval since last target reset is >= intervalInMilliseconds, 
        reset target to 0 and record the time it was reseted in before[index].
    */
    template <typename T>
    void profile(int index, int intervalInMilliseconds, ProfilerVariable<T> &profilerVariable,
                 std::function<void(ProfilerVariable<T> &)> const &increaseFunction,
                 std::function<void(ProfilerVariable<T> &)> const &resetFunction)
    {
        if (index < MAX)
        {
            auto now = std::chrono::system_clock::now();
            auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(now - before[index]).count();
            if (difference >= intervalInMilliseconds)
            {
                profilerVariable.profile();
                resetFunction(profilerVariable);
                before[index] = std::chrono::system_clock::now();
            }
            increaseFunction(profilerVariable);
        }
        else
        {
            throw std::runtime_error("profiler index is too big");
        }
    }

private:
    //Using array (not vector) to be fast. Only drawback is MAX ammount of profiler variables
    std::chrono::system_clock::time_point before[MAX] = {};
};

class ProfilingThread: public Stoppable
{
public:
    ProfilingThread();
    void run();
    void addProflingVariable(std::shared_ptr<ProfilerVariable> profilerVariable) {
        std::unique_lock<std::mutex> lock{mutex};
        profilerVariables.insert(profilerVariable);
    }

private:
    std::thread thread;
    std::mutex mutex;
    std::vector<std::shared_ptr<ProfilerVariable>> profilerVariables;
};

#endif //Profiler_H