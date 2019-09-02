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
    LOG << "NVDecoder::run started";
    LOG << "nvVideoDecoder->output_plane.getNumBuffers(): " << nvVideoDecoder->output_plane.getNumBuffers();
    //Counts the index of buffers we're going to fill first before dequeueing used ones
    int i = 0;
    int ret = 0;
    bool eos = false;
    /*
        We're gonna receive lots of EncodedPacket instances in the while loop below
        and extract its NALUs. Of course a NALU can end in the middle of an EncodedPacket,
        so currentEncodedPacket saves this EncodedPacket for work to continue in the offset
        currentEncodedPacketSearchPtr.
    */
    EncodedPacket currentEncodedPacket;
    //Must be null
    uint8_t *currentEncodedPacketSearchPtr = NULL;
    /*
        In this first loop we fill all the buffers, taking care to
        see if we reached EOS (in case of CHUNK format). After
        all the buffers have been setted, we start another while
        loop that blocks until a buffer can be dequeue
    */
    //TODO: what happens if shouldContinue() is setted to FALSE in the middle of the while?
    //How should I destroy things?
    while (shouldContinue())
    {
        TEST_ERROR(nvVideoDecoder->isInError(), "nvVideoDecoder->isInError() in while loop", 0)
        LOG << "Gonna grab currentEncodedPacket";

        currentEncodedPacket = std::move(encodedPacketsFifo->pop_front());
        /*
            This is a type of configuration file that tells how the buffer should be queued
        */
        struct v4l2_buffer v4l2Buffer;
        struct v4l2_plane planes[MAX_PLANES];
        NvBuffer *nvBuffer;

        memset(&v4l2Buffer, 0, sizeof(v4l2Buffer));
        if (i < nvVideoDecoder->output_plane.getNumBuffers())
        {
            memset(planes, 0, sizeof(planes));
        }
        /*
            The buffer to where a single NALU is written. 
            As I understood, each buffer should contain a single NALU and 
            then be queued.
            I guess NvBuffer is for decoded planes too, so that's why it supports
            lots of planes. But our data is encoded (has no planes) so we only use 
            the 0th plane
        */
        if (i < nvVideoDecoder->output_plane.getNumBuffers())
            nvBuffer = nvVideoDecoder->output_plane.getNthBuffer(i);
        else
        {
            ret = nvVideoDecoder->output_plane.dqBuffer(v4l2Buffer, &nvBuffer, NULL, -1);
            TEST_ERROR(ret < 0, "Error dequeueing buffer at output plane", 0)
        }
        //Number of bytes written to nvBuffer's data pointer planeBufferPtr
        uint32_t bytesWritten = 0;
        uint8_t *planeBufferPtr = static_cast<uint8_t *>(nvBuffer->planes[0].data);
        if ((codec == H264) ||
            (codec == H265) ||
            (codec == MPEG2) ||
            (codec == MPEG4))
        {
            if (format == NALU)
            {
                /*
                    Of course we're going to fill `planeBufferPtr` with an entire NALU.
                    transferNalu returns true if an entire NALU couldn't be found
                    inside `currentEncodedPacket`, so we need to query another unit from
                    our FIFO to continue writing our NALU to `planeBufferPtr`
                */
                while (transferNalu(currentEncodedPacket.frameBuffer.get(),
                                    currentEncodedPacketSearchPtr,
                                    currentEncodedPacket.frameSize,
                                    planeBufferPtr,
                                    bytesWritten))
                {
                    currentEncodedPacket = std::move(encodedPacketsFifo->pop_front());
                }
                nvBuffer->planes[0].bytesused = bytesWritten;
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
        if (i < nvVideoDecoder->output_plane.getNumBuffers())
        {
            v4l2Buffer.index = i;
            v4l2Buffer.m.planes = planes;
        }
        v4l2Buffer.m.planes[0].bytesused = nvBuffer->planes[0].bytesused;
        /*
            Finally queries the buffer. If v4l2Buffer.m.planes[0].bytesused == 0
            the decoder understands we reached EOS (I guess)
        */
        ret = nvVideoDecoder->output_plane.qBuffer(v4l2Buffer, NULL);
        TEST_ERROR(ret < 0, "Error queueing buffer at output plane in the first while loop of run", ret)
        if (i < nvVideoDecoder->output_plane.getNumBuffers())
            i++;

        if (v4l2Buffer.m.planes[0].bytesused == 0)
        {
            LOG << "End of stream";
            eos = true;
            break;
        }
    }
}
/*
    transferNalu transfers an entire NALU from currentEncodedPacketSearchPtr to planeBufferPtr.
    If the transaction did not read the entire currentEncodedPacket, it returns true, so the
    called knows it should call transferNalu again with the same 

    uint8_t* const currentEncodedPacketBegginingPtr: points to the begginng of the currentEncodedPacket
    uint8_t *currentEncodedPacketSearchPtr: poins to data inside the currentEncodedPacket, which 
    is the packed where we search for NALUs
    size_t currentEncodedPacketSize: size of currentEncodedPacket
    uint8_t * planeBufferPtr: buffer where the NALU is going to be written
    uint32_t &bytesWritten: tells the called how many bytes we wrote to planeBufferPtr
*/
static bool transferNalu(uint8_t *const currentEncodedPacketBegginingPtr,
                         uint8_t *currentEncodedPacketSearchPtr,
                         size_t currentEncodedPacketSize,
                         uint8_t *planeBufferPtr,
                         uint32_t &bytesWritten)
{
    int h265_nal_unit_type;
    bool nalu_found = false;
    //bool nalu_end_found = false;
    if (currentEncodedPacketSize == 0)
    {
        //Throw?
    }

    /*
        We're gonna find the first NALU in the packet
        If currentEncodedPacketSearchPtr is not setted, it means we should start at the beggining
        of the currentEncodedPacket. Otherwise, we're continuing the work of the previous transferNalu
        call, in which it found a NALU in the middle of the currentEncodedPacket and returned true
    */
    if (!currentEncodedPacketSearchPtr)
        currentEncodedPacketSearchPtr = currentEncodedPacketBegginingPtr;
    while ((currentEncodedPacketSearchPtr - planeBufferPtr) < (currentEncodedPacketSize - 3))
    {
        nalu_found = IS_NAL_UNIT_START(currentEncodedPacketSearchPtr) || IS_NAL_UNIT_START1(currentEncodedPacketSearchPtr);
        if (nalu_found)
            break;
        currentEncodedPacketSearchPtr++;
    }

    // Reached end of planeBuffer but could not find NAL unit
    if (!nalu_found)
    {
        /*
            If no NALU found, return true so the caller knows it should call 
            this function again with a new encodedPacket.
            Set currentEncodedPacketSearchPtr to null so in the next call of transferNalu, 
            it knows it should start from the beggining of the encodedPacket.
        */
        LOG << "did not find NALU start, gonna return true to query another packet ";
        currentEncodedPacketSearchPtr = NULL;
        return true;
    }
    /*
        Copies the first 4 bytes, which form the NALU header. If the header is in 3 byte form
        it doesn't matter, we just copied another extra byte
    */
    LOG << "found NALU at " << *currentEncodedPacketSearchPtr << " gonna copy it now";
    memcpy(planeBufferPtr, currentEncodedPacketSearchPtr, 4);
    planeBufferPtr += 4;
    currentEncodedPacketSearchPtr += 4;
    bytesWritten += 4;
    /*
    if (ctx->copy_timestamp)
    {
        if (ctx->decoder_pixfmt == V4L2_PIX_FMT_H264)
        {
            if ((IS_H264_NAL_CODED_SLICE(currentEncodedPacketSearchPtr)) ||
                (IS_H264_NAL_CODED_SLICE_IDR(currentEncodedPacketSearchPtr)))
                ctx->flag_copyts = true;
            else
                ctx->flag_copyts = false;
        }
        else if (ctx->decoder_pixfmt == V4L2_PIX_FMT_H265)
        {
            h265_nal_unit_type = GET_H265_NAL_UNIT_TYPE(currentEncodedPacketSearchPtr);
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
    while ((currentEncodedPacketSearchPtr - planeBufferPtr) < (currentEncodedPacketSize - 3))
    {
        if (IS_NAL_UNIT_START(currentEncodedPacketSearchPtr) || IS_NAL_UNIT_START1(currentEncodedPacketSearchPtr))
        {
            //nalu_end_found = true;
            LOG << "found NALU end at " << currentEncodedPacketSearchPtr << ", returning...";
            return false;
        }
        *planeBufferPtr = *currentEncodedPacketSearchPtr;
        planeBufferPtr++;
        currentEncodedPacketSearchPtr++;
        bytesWritten++;
    }
    /*
        If we're here it means we reached the end of the packet (because the second while loop finished)
        and so, if we found the nalu end, set currentEncodedPacketSearchPtr to NULL and return true, so caller 
        calls transferNalu again with new packet
    
    if (nalu_end_found)
    {
        currentEncodedPacketSearchPtr = NULL;
        return true;
    }
    */
}