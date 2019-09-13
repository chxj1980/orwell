#ifndef Profiler_H
#define Profiler_H
#include <chrono>
#include <stdexcept>

const int MAX = 10;
class Profiler
{
public:
    /*
        If interval since last target reset is >= intervalInMilliseconds, 
        reset target to 0 and record the time it was reseted in before[index].
    */
    void profile(int index, int intervalInMilliseconds, 
                 std::function<void()> const& increaseFunction, std::function<void()> const& resetFunction)
    {
        if (index < MAX)
        {
            auto now = std::chrono::system_clock::now();
            auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(now - before[index]).count();
            if (difference >= intervalInMilliseconds)
            {
                resetFunction();
                before[index] = std::chrono::system_clock::now();
            }
            increaseFunction();
        }
        else
        {
            throw std::runtime_error("profiler index is too big");
        }
    }

private:
    std::chrono::system_clock::time_point before[MAX] = {};
};

#endif //Profiler_H