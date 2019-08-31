#include "NVDecoder.h"

NVDecoder::NVDecoder() {
    int ret;
    nvApplicationProfiler->start(NvApplicationProfiler::DefaultSamplingInterval);
    nvVideoDecoder->enableProfiling();
    ret = nvVideoDecoder->subscribeEvent(V4L2_EVENT_RESOLUTION_CHANGE, 0, 0);
    if (ret<0)
        std::cout << "Could not subscribe to V4L2_EVENT_RESOLUTION_CHANGE" << std::endl;
    
}