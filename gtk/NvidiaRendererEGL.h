#ifndef NvidiaRendererEGL_H
#define NvidiaRendererEGL_H
#include "NvEglRenderer.h"
#include "DecodedFrame.h"
#include "ThreadSafeDeque.h"
#include "Stoppable.h"
#include <memory>

class NvidiaRendererEGL: public Stoppable {
public:
    NvidiaRendererEGL();
    NvidiaRendererEGL(int width, int height, int x, int y);
    void setDecodedFramesFifo(std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo)
	{
		this->decodedFramesFifo = decodedFramesFifo;
	}
    void run();
    //std::unique_ptr<NvEglRenderer> renderer;
    //TODO: change back to smart pointer
    NvEglRenderer* renderer;
    static int count;
protected:
    std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo;
};
#endif //NvidiaRendererEGL_H