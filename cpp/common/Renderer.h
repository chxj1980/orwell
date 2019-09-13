#ifndef Renderer_H
#define Renderer_H
#include "Profiler.h"
class Renderer
{
public:
    std::shared_ptr<ProfilerVariable<int>> fps = std::make_shared<ProfilerVariable<int>>(1000);
};
#endif //Renderer_H