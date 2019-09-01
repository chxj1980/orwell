#include "NVDecoder.h"
#include "SLog.h"
#include "EncodedUnits.h"

SLOG_CATEGORY("NVDecoder");

#define TEST_ERROR(condition, message, errorCode)             \
    if (condition)                                            \
    {                                                         \
        throw NVDecoderCreationException(message, errorCode); \
    }
#define IS_NAL_UNIT_START(buffer_ptr) (!buffer_ptr[0] && !buffer_ptr[1] && !buffer_ptr[2] && (buffer_ptr[3] == 1))
#define IS_NAL_UNIT_START1(buffer_ptr) (!buffer_ptr[0] && !buffer_ptr[1] && (buffer_ptr[2] == 1))

//TODO: do I really need to overload this function?
/*
void NVDecoder::run()
	{
		if (!encodedUnitsFifo)
		{
			std::cerr << "NVDecoder.cpp: no encodedUnitsFifo setted, nowhere to pull data from" << std::endl;
			return;
		}
		while (shouldContinue())
		{
			
				Pops an encoded frame from encodedUnitsFifo. If there are none, it blocks, so
				no CPU time is wasted.
			
			//TODO: certify that the operation below is MOVING the frame to here, not copying it
			EncodedUnit encodedUnit = std::move(encodedUnitsFifo->pop_front());
			/
				Since the frame is gone from the fifo, it only exists here. 
				decodeFrame() access its pointers and is blocking. When decodeFrame 
				finishes, `encodedUnit` is gone and its contents are automatically deleted.
				DecodedFrame is sent to the decodedFramesFifo
			
			decodeFrame(encodedUnit);
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
    //1 = NV12, 2 = I420
    ret = nvVideoDecoder->setOutputPlaneFormat(1, CHUNK_SIZE);
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

void NVDecoder::run()
{
    //while (!eos && !ctx.got_error && !ctx.dec->isInError() &&
    //       i < nvVideoDecoder->output_plane.getNumBuffers())
    int i = 0;
    int ret = 0;
    bool eos = false;
    /*
        We're gonna receive lots of EncodedUnit instances in the while loop below
        and extract its NALUs. Of course a NALU can end in the middle of an EncodedUnit,
        so currentEncodedUnit saves this EncodedUnit for work to continue in the offset
        currentEncodedUnitOffset.
    */
    EncodedUnit currentEncodedUnit;
    std::streamsize currentEncodedUnitOffset = 0;
    /*
        In this first loop we fill all the buffers, taking care to
        see if we reached EOS (in case of CHUNK format). After
        all the buffers have been setted, we start another while
        loop that blocks until a buffer can be dequeue
    */
    //TODO: what happens if shouldContinue() is setted to FALSE in the middle of the while?
    //How should I destroy things?
    while (i < nvVideoDecoder->output_plane.getNumBuffers() && shouldContinue())
    {
        TEST_ERROR(nvVideoDecoder->isInError(), "nvVideoDecoder->isInError() in first while loop", 0)
        /*
            This is a type of configuration file that tells how the buffer should be queued
        */
        struct v4l2_buffer v4l2_buf;
        struct v4l2_plane planes[MAX_PLANES];
        NvBuffer *buffer;

        memset(&v4l2_buf, 0, sizeof(v4l2_buf));
        memset(planes, 0, sizeof(planes));
        /*
            The buffer to where a single NALU is written. 
            As I understood, each buffer should contain a single NALU and 
            then be queued
        */
        buffer = nvVideoDecoder->output_plane.getNthBuffer(i);
        if ((codec == H264) ||
            (codec == H265) ||
            (codec == MPEG2) ||
            (codec == MPEG4))
        {
            if (format == NALU)
            {
                while (shouldContinue())
                {
                    transferNalu(buffer, encodedUnits);
                }
            }
            else
            {
                //read_decoder_input_chunk(ctx.in_file[current_file], buffer);
            }
        }
        /*
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
        */
        v4l2_buf.index = i;
        v4l2_buf.m.planes = planes;
        v4l2_buf.m.planes[0].bytesused = buffer->planes[0].bytesused;

        /*
            It is necessary to queue an empty buffer to signal EOS to the decoder
            i.e. set v4l2_buf.m.planes[0].bytesused = 0 and queue the buffer. That means
            that if v4l2_buf.m.planes[0].bytesused = 0 from the previous calls, we reached 
            EOS and the decoder will know about it because we'll queue this v4l2_buf to it.
        */
        ret = nvVideoDecoder->output_plane.qBuffer(v4l2_buf, NULL);
        TEST_ERROR(ret < 0, "Error queueing buffer at output plane in the first while loop of run", ret)
        if (v4l2_buf.m.planes[0].bytesused == 0)
        {
            LOG << "End of stream";
            eos = true;
            break;
        }
        i++;
    }
    /*
        Our while loop ended, which means all output buffers have been filled.
        Now we need to create a new while loop that blocks until we can deque a
        new buffer to fill with data and queue again.
    */
}

static void transferNalu(NvBuffer *nvBuffer, EncodedUnits &encodedUnits)
{
    // Length is the size of the buffer in bytes
    char *nvBufferPlane = (char *)nvBuffer->planes[0].data;
    int h265_nal_unit_type;
    char *stream_ptr;
    bool nalu_found = false;
    //Blocks until a new frame arrives
    EncodedUnit encodedUnit = std::move(encodedUnitsFifo->pop_front());

    //std::streamsize bytes_read;
    //std::streamsize stream_initial_pos = stream->tellg();

    stream->read(parse_buffer, parse_buffer_size);
    bytes_read = stream->gcount();

    if (bytes_read == 0)
    {
        return buffer->planes[0].bytesused = 0;
    }

    // Find the first NAL unit in the buffer
    stream_ptr = parse_buffer;
    while ((stream_ptr - parse_buffer) < (bytes_read - 3))
    {
        nalu_found = IS_NAL_UNIT_START(stream_ptr) ||
                     IS_NAL_UNIT_START1(stream_ptr);
        if (nalu_found)
        {
            break;
        }
        stream_ptr++;
    }

    // Reached end of buffer but could not find NAL unit
    if (!nalu_found)
    {
        cerr << "Could not read nal unit from file. EOF or file corrupted"
             << endl;
        return -1;
    }

    memcpy(nvBufferPlane, stream_ptr, 4);
    nvBufferPlane += 4;
    nvBuffer->planes[0].bytesused = 4;
    stream_ptr += 4;
    /*
    if (ctx->copy_timestamp)
    {
        if (ctx->decoder_pixfmt == V4L2_PIX_FMT_H264)
        {
            if ((IS_H264_NAL_CODED_SLICE(stream_ptr)) ||
                (IS_H264_NAL_CODED_SLICE_IDR(stream_ptr)))
                ctx->flag_copyts = true;
            else
                ctx->flag_copyts = false;
        }
        else if (ctx->decoder_pixfmt == V4L2_PIX_FMT_H265)
        {
            h265_nal_unit_type = GET_H265_NAL_UNIT_TYPE(stream_ptr);
            if ((h265_nal_unit_type >= HEVC_NUT_TRAIL_N && h265_nal_unit_type <= HEVC_NUT_RASL_R) ||
                (h265_nal_unit_type >= HEVC_NUT_BLA_W_LP && h265_nal_unit_type <= HEVC_NUT_CRA_NUT))
                ctx->flag_copyts = true;
            else
                ctx->flag_copyts = false;
        }
    }
    */
    // Copy bytes till the next NAL unit is found
    while ((stream_ptr - parse_buffer) < (bytes_read - 3))
    {
        if (IS_NAL_UNIT_START(stream_ptr) || IS_NAL_UNIT_START1(stream_ptr))
        {
            std::streamsize seekto = stream_initial_pos +
                                     (stream_ptr - parse_buffer);
            if (stream->eof())
            {
                stream->clear();
            }
            stream->seekg(seekto, stream->beg);
            return 0;
        }
        *nvBufferPlane = *stream_ptr;
        nvBufferPlane++;
        stream_ptr++;
        nvBuffer->planes[0].bytesused++;
    }

    // Reached end of buffer but could not find NAL unit
    cerr << "Could not read nal unit from file. EOF or file corrupted"
         << endl;
    return -1;
}