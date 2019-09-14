#ifndef Renderer_H
#define Renderer_H
#include "Profiler.h"
#include "ThreadSafeDeque.h"
#include "Stoppable.h"
#include <memory>
class Renderer: public Stoppable
{
public:
    void setDecodedFramesFifo(std::shared_ptr<ThreadSafeDeque<std::shared_ptr<DecodedFrame>>> decodedFramesFifo)
    {
        this->decodedFramesFifo = decodedFramesFifo;
    }
    std::shared_ptr<ProfilerVariable<int>> fps = std::make_shared<ProfilerVariable<int>>(1000);
    virtual void run() = 0;
    virtual void startThreadMode() {
		runThread = std::thread(&Renderer::run, this);
	}
private:
    std::shared_ptr<ThreadSafeDeque<std::shared_ptr<DecodedFrame>>> decodedFramesFifo;
    std::thread runThread;
};
#endif //Renderer_H