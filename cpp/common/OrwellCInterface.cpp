#include "OrwellCInterface.h"
// *.cpp file
void* orwell_init_from_rtsp(char *rtspUrl)
{
    RTSPUrl rtspUrlObject(rtspUrl);
    return new Orwell(rtspUrlObject);
}
/* 
void* orwell_init_from_onvif(char *onvifUrl)
{
    RTSPUrl onvifUrlObject(onvifUrl);
    return new Orwell(onvifUrlObject);
}
*/

void orwell_destroy(void* untyped_ptr)
{
    Orwell *typed_ptr = static_cast<Orwell *>(untyped_ptr);
    delete typed_ptr;
}

void orwell_doit(void* untyped_self, int param)
{
    Orwell *typed_self = static_cast<Orwell *>(untyped_self);
    typed_self->doIt(param);
}