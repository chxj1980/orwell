#include "NvidiaRendererEGL.h"
#include "NVDecoder.h"
#include "SLog.h"
SLOG_CATEGORY("NvidiaRendererEGL");

int NvidiaRendererEGL::count = 0;

NvidiaRendererEGL::NvidiaRendererEGL()
{
    NvidiaRendererEGL(0, 0, 0, 0);
}
NvidiaRendererEGL::NvidiaRendererEGL(int width, int height, int x, int y)
{
    std::string s = std::string("renderer") + std::to_string(count);
    LOG << "creatind renderer with name " << s;
    renderer = NvEglRenderer::createEglRenderer(s.c_str(), width, height, x, y);
    //renderer = std::unique_ptr<NvEglRenderer>(NvEglRenderer::createEglRenderer(s.c_str(), width, height, x, y));
    count += 1;
}

void NvidiaRendererEGL::run()
{
    while (shouldContinue())
    {
        auto decodedNvFrame = std::dynamic_pointer_cast<DecodedNvFrame>(decodedFramesFifo->pop_front());
        //renderer->setFPS(30);
        renderer->render(decodedNvFrame->nvBuffer->planes[0].fd);
        fps.profile([](int& counter){counter++;}, [](int& counter){counter=0;});
    }
}