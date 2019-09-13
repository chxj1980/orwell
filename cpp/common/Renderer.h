#ifndef Renderer_H
#define Renderer_H
#include "Profiler.h"
class Renderer: public Profiler
{
public:
    std::shared_ptr<ProfilerVariable<int>> fps;
};
#endif //Renderer_H