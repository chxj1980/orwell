#ifndef Renderer_H
#define Renderer_H
#include "Profiler.h"
class Renderer: public Profiler
{
public:
    ProfilerVariable<int> fps;
};
#endif //Renderer_H