#ifndef Profiler_H
#define Profiler_H
#include <chrono>
#include <stdexcept>
#include<functional>

/*
    A class to safely update the sample variable at the end of a 
*/
template <typename T>
class ProfilerVariable
{
public:
    //To be executed at the end of the profiling period
    void profile() {
        std::unique_lock<std::mutex> lock{mutex};
        sample = counter;
    }
    //TODO: return is safe????
    T getSample() {
        std::unique_lock<std::mutex> lock{mutex};
        return sample;
    }
    //Value to be increased at every profile call
    T counter;
    //Value to store the result of the accumulated counter at the end of profile period
private:
    T sample;
    std::mutex mutex;
};

const int MAX = 10;
class Profiler
{
public:
    /*
        If interval since last target reset is >= intervalInMilliseconds, 
        reset target to 0 and record the time it was reseted in before[index].
    */
    template <typename T>
    void profile(int index, int intervalInMilliseconds, ProfilerVariable<T>& profilerVariable,
                 std::function<void(ProfilerVariable<T>&)> const &increaseFunction, 
                 std::function<void(ProfilerVariable<T>&)> const &resetFunction)
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

#endif //Profiler_H