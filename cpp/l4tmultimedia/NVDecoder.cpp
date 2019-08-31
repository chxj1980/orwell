#include "NVDecoder.h"
#include "SLog.h"
SLOG_CATEGORY("NVDecoder");
#define TEST_ERROR(condition, message, errorCode)             \
    if (condition)                                            \
    {                                                         \
        throw NVDecoderCreationException(message, errorCode); \
    }
//TODO: do I really need to overload this function?
/*
void NVDecoder::run()
	{
		if (!encodedFramesFifo)
		{
			std::cerr << "NVDecoder.cpp: no encodedFramesFifo setted, nowhere to pull data from" << std::endl;
			return;
		}
		while (shouldContinue())
		{
			
				Pops an encoded frame from encodedFramesFifo. If there are none, it blocks, so
				no CPU time is wasted.
			
			//TODO: certify that the operation below is MOVING the frame to here, not copying it
			EncodedFrame encodedFrame = std::move(encodedFramesFifo->pop_front());
			/
				Since the frame is gone from the fifo, it only exists here. 
				decodeFrame() access its pointers and is blocking. When decodeFrame 
				finishes, `encodedFrame` is gone and its contents are automatically deleted.
				DecodedFrame is sent to the decodedFramesFifo
			
			decodeFrame(encodedFrame);
		}
	}
*/
NVDecoder::NVDecoder(Format format, Codec codec) : format(format), codec(codec)
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
    //TODO: what's 10?
    if (outputPlaneMemType == V4L2_MEMORY_MMAP)
        ret = nvVideoDecoder->output_plane.setupPlane(V4L2_MEMORY_MMAP, 10, true, false);
    else if (outputPlaneMemType == V4L2_MEMORY_USERPTR)
        ret = nvVideoDecoder->output_plane.setupPlane(V4L2_MEMORY_USERPTR, 10, false, true);
    TEST_ERROR(ret < 0, "Error while setting up output plane", ret);

    ret = nvVideoDecoder->output_plane.setStreamStatus(true);
    TEST_ERROR(ret < 0, "Error in output plane stream on", ret);
}
void NVDecoder::planeQueuer()
{
    struct v4l2_buffer v4l2_buf;
    struct v4l2_plane planes[MAX_PLANES];
    NvBuffer *buffer;

    memset(&v4l2_buf, 0, sizeof(v4l2_buf));
    memset(planes, 0, sizeof(planes));

    buffer = nvVideoDecoder->output_plane.getNthBuffer(0);
    if ((codec == H264) ||
        (codec == H265) ||
        (codec == MPEG2) ||
        (codec == MPEG4))
    {
        if (format == NALU)
        {
            read_decoder_input_nalu(ctx.in_file[current_file], buffer, nalu_parse_buffer,
                                    CHUNK_SIZE, &ctx);
        }
        else
        {
            read_decoder_input_chunk(ctx.in_file[current_file], buffer);
        }
    }
    if (codec == VP9)
    {
        ret = read_vp9_decoder_input_chunk(&ctx, buffer);
        if (ret != 0)
            cerr << "Couldn't read VP9 chunk" << endl;
    }
    if (codec == V4L2_PIX_FMT_VP8)
    {
        ret = read_vp8_decoder_input_chunk(&ctx, buffer);
        if (ret != 0)
            cerr << "Couldn't read VP8 chunk" << endl;
    }
}