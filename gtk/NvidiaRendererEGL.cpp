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
        DecodedFrame decodedFrame = std::move(decodedFramesFifo->pop_front());
        NVDecoderReusableBuffer *nVDecoderReusableBuffer = dynamic_cast<NVDecoderReusableBuffer *>(decodedFrame.reusableBuffer.get());
        //LOG << "v4l2Buffer.m.planes[0].m.fd: " << nVDecoderReusableBuffer->v4l2Buffer.m.planes[0].m.fd;
        LOG << "gonna set fps";
        renderer->setFPS(30);
        LOG << "did set fps";
        LOG << "gonna render " << nVDecoderReusableBuffer->nvBuffer->planes[0].fd;
        renderer->render(nVDecoderReusableBuffer->nvBuffer->planes[0].fd);
        LOG << "did render";
    }
}