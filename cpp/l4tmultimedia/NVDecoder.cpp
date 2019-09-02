#include "NVDecoder.h"
#include "SLog.h"
#include "EncodedPackets.h"

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
		if (!encodedPacketsFifo)
		{
			std::cerr << "NVDecoder.cpp: no encodedPacketsFifo setted, nowhere to pull data from" << std::endl;
			return;
		}
		while (shouldContinue())
		{
			
				Pops an encoded frame from encodedPacketsFifo. If there are none, it blocks, so
				no CPU time is wasted.
			
			//TODO: certify that the operation below is MOVING the frame to here, not copying it
			EncodedPacket encodedPacket = std::move(encodedPacketsFifo->pop_front());
			/
				Since the frame is gone from the fifo, it only exists here. 
				decodeFrame() access its pointers and is blocking. When decodeFrame 
				finishes, `encodedPacket` is gone and its contents are automatically deleted.
				DecodedFrame is sent to the decodedFramesFifo
			
			decodeFrame(encodedPacket);
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
        We're gonna receive lots of EncodedPacket instances in the while loop below
        and extract its NALUs. Of course a NALU can end in the middle of an EncodedPacket,
        so currentEncodedPacket saves this EncodedPacket for work to continue in the offset
        currentEncodedPacketPtr.
    */
    EncodedPacket currentEncodedPacket = std::move(encodedPacketsFifo->pop_front());
    uint8_t *currentEncodedPacketPtr = currentEncodedPacket.frameBuffer.get();
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
        struct v4l2_buffer v4l2Buffer;
        struct v4l2_plane planes[MAX_PLANES];
        NvBuffer *nvBuffer;

        memset(&v4l2Buffer, 0, sizeof(v4l2Buffer));
        memset(planes, 0, sizeof(planes));
        /*
            The buffer to where a single NALU is written. 
            As I understood, each buffer should contain a single NALU and 
            then be queued.
            I guess NvBuffer is for decoded planes too, so that's why it supports
            lots of planes. But our data is encoded (has no planes) so we only use 
            the 0th plane
        */
        nvBuffer = nvVideoDecoder->output_plane.getNthBuffer(i);
        uint8_t *planeBufferPtr = static_cast<uint8_t *>(nvBuffer->planes[0].data);
        if ((codec == H264) ||
            (codec == H265) ||
            (codec == MPEG2) ||
            (codec == MPEG4))
        {
            uint32_t bytesWritten;
            if (format == NALU)
            {
                /*
                    Of course we're going to fill `planeBufferPtr` with an entire NALU.
                    transferNalu returns true if an entire NALU couldn't be found
                    inside `currentEncodedPacket`, so we need to query another unit from
                    our FIFO to continue writing our NALU to `planeBufferPtr`
                */
                while (transferNalu(currentEncodedPacketPtr, currentEncodedPacket.frameSize, planeBufferPtr, bytesWritten))
                {
                    nvBuffer->planes[0].bytesused = bytesWritten;
                    currentEncodedPacket = std::move(encodedPacketsFifo->pop_front());
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
        v4l2Buffer.index = i;
        v4l2Buffer.m.planes = planes;
        v4l2Buffer.m.planes[0].bytesused = nvBuffer->planes[0].bytesused;

        /*
            It is necessary to queue an empty buffer to signal EOS to the decoder
            i.e. set v4l2_buf.m.planes[0].bytesused = 0 and queue the buffer. That means
            that if v4l2_buf.m.planes[0].bytesused = 0 from the previous calls, we reached 
            EOS and the decoder will know about it because we'll queue this v4l2_buf to it.
        */
        ret = nvVideoDecoder->output_plane.qBuffer(v4l2Buffer, NULL);
        TEST_ERROR(ret < 0, "Error queueing buffer at output plane in the first while loop of run", ret)
        if (v4l2Buffer.m.planes[0].bytesused == 0)
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

static bool transferNalu(uint8_t *currentEncodedPacketPtr, size_t currentEncodedPacketSize, uint8_t * const planeBufferPtr, uint32_t& bytesWritten)
{
    int h265_nal_unit_type;
    /*
        Pointer that traverses the bufferPlane (pointed by planeBufferPtr) in
        order to find NALUs
    */
    uint8_t* writtenBufferPtr = planeBufferPtr;
    bool nalu_found = false;

    if (currentEncodedPacketSize == 0)
    {
        //Throw?
    }

    /*
        We're gonna find the first NALU in the packet
        If currentEncodedPacketPtr is not setted, it means we should start at the beggining
        of the encodedPacket. Otherwise, we're continuing the work of the previous transferNalu
        call, in which it found a NALU in the middle of the encodedPacket and returned true
    */
    if (!currentEncodedPacketPtr)
        currentEncodedPacketPtr = planeBufferPtr;
    while ((currentEncodedPacketPtr - planeBufferPtr) < (currentEncodedPacketSize - 3))
    {
        nalu_found = IS_NAL_UNIT_START(currentEncodedPacketPtr) || IS_NAL_UNIT_START1(currentEncodedPacketPtr);
        if (nalu_found)
            break;
        currentEncodedPacketPtr++;
    }

    // Reached end of planeBuffer but could not find NAL unit
    if (!nalu_found)
    {
        /*
            If no NALU found, return true so the caller knows it should call 
            this function again with a new encodedPacket.
            Set currentEncodedPacketPtr to null so in the next call of transferNalu, 
            it knows it should start from the beggining of the encodedPacket.
        */
        currentEncodedPacketPtr = NULL;
        return true;
    }

    memcpy(planeBufferPtr, currentEncodedPacketPtr, 4);
    writtenBufferPtr += 4;
    currentEncodedPacketPtr += 4;
    bytesWritten += 4;
    /*
    if (ctx->copy_timestamp)
    {
        if (ctx->decoder_pixfmt == V4L2_PIX_FMT_H264)
        {
            if ((IS_H264_NAL_CODED_SLICE(currentEncodedPacketPtr)) ||
                (IS_H264_NAL_CODED_SLICE_IDR(currentEncodedPacketPtr)))
                ctx->flag_copyts = true;
            else
                ctx->flag_copyts = false;
        }
        else if (ctx->decoder_pixfmt == V4L2_PIX_FMT_H265)
        {
            h265_nal_unit_type = GET_H265_NAL_UNIT_TYPE(currentEncodedPacketPtr);
            if ((h265_nal_unit_type >= HEVC_NUT_TRAIL_N && h265_nal_unit_type <= HEVC_NUT_RASL_R) ||
                (h265_nal_unit_type >= HEVC_NUT_BLA_W_LP && h265_nal_unit_type <= HEVC_NUT_CRA_NUT))
                ctx->flag_copyts = true;
            else
                ctx->flag_copyts = false;
        }
    }
    */
    /*
        Copy bytes until the next NALU is found. Since
        NALUs aren't aware of its size, the only way to know that
        a NALU ended is to find a new NALU
    */
    while ((currentEncodedPacketPtr - planeBufferPtr) < (currentEncodedPacketSize - 3))
    {
        if (IS_NAL_UNIT_START(currentEncodedPacketPtr) || IS_NAL_UNIT_START1(currentEncodedPacketPtr))
        {
            std::streamsize seekto = stream_initial_pos +
                                     (currentEncodedPacketPtr - planeBufferPtr);
            if (stream->eof())
            {
                stream->clear();
            }
            stream->seekg(seekto, stream->beg);
            return 0;
        }
        *planeBufferPtr = *currentEncodedPacketPtr;
        writtenBufferPtr++;
        currentEncodedPacketPtr++;
        bytesWritten++;
    }
    /*
        If reached end, set currentEncodedPacketPtr to NULL so
    */
    if ((currentEncodedPacketPtr - planeBufferPtr) == (currentEncodedPacketSize - 3)) {
        currentEncodedPacketPtr = NULL;
    }
}