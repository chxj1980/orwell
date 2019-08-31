#include "NVDecoder.h"
#include "SLog.h"
SLOG_CATEGORY("NVDecoder");
//#define NVException(E) throw NVDecoderCreationException(E);
//#define NVException(E,N) throw NVDecoderCreationException(E, N);
#define TEST_ERROR(condition, message, errorCode)             \
    if (condition)                                            \
    {                                                         \
        throw NVDecoderCreationException(message, errorCode); \
    }

NVDecoder::NVDecoder(Format format) : format(format)
{
    int ret;
    TEST_ERROR(!nvVideoDecoder, "Could not create decoder", 0)

    nvApplicationProfiler->start(NvApplicationProfiler::DefaultSamplingInterval);
    nvVideoDecoder->enableProfiling();

    ret = nvVideoDecoder->subscribeEvent(V4L2_EVENT_RESOLUTION_CHANGE, 0, 0);
    TEST_ERROR(ret < 0, "Could not subscribe to V4L2_EVENT_RESOLUTION_CHANGE", ret)

    ret = nvVideoDecoder->setOutputPlaneFormat(0, CHUNK_SIZE);
    TEST_ERROR(ret < 0, "Could not set output plane format", ret)

    if (format == NALU)
    {
        ret = nvVideoDecoder->setFrameInputMode(0);
        TEST_ERROR(ret < 0, "Error in decoder setFrameInputMode for NALU", ret)
    }
    else
    {
        ret = nvVideoDecoder->setFrameInputMode(1);
        TEST_ERROR(ret < 0, "Error in decoder setFrameInputMode for CHUNK", ret)

    }
}