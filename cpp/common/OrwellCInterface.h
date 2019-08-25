#ifndef OrwellCInterface_H
#define OrwellCInterface_H
#include "Orwell.h"
// *.h file
// ...
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

//typedef void Orwell;

EXTERNC void* orwell_init_from_rtsp(char *rtspUrl);
//EXTERN C void* orwell_init_from_onvif(char *onvifUrl);
EXTERNC void orwell_destroy(void* mytype);
EXTERNC void orwell_doit(void* self, int param);

#undef EXTERNC
#endif //OrwellCInterface_H