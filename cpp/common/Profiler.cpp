#include "Profiler.h"
#include "SLog.h"
SLOG_CATEGORY("Profiler")
//Interval in milliseconds
const int profilingInterval = 500;

ProfilingThread::ProfilingThread()
{
    SLOG_FILENAME("profiler.log")
    //LOG.enableCategories("Profiler");
    thread = std::thread(&ProfilingThread::run, this);
}

void ProfilingThread::run()
{
    while (shouldContinue())
    {
        std::unique_lock<std::mutex> lock{mutex};
        for (auto profilerVariable : profilerVariables)
        {
            LOG << profilerVariable.getSample();
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(profilingInterval));
    }
}