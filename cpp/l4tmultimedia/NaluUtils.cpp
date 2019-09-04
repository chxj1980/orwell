#include "NaluUtils.h"
#define IS_NAL_UNIT_START(buffer_ptr) (!buffer_ptr[0] && !buffer_ptr[1] && !buffer_ptr[2] && (buffer_ptr[3] == 1))
#define IS_NAL_UNIT_START1(buffer_ptr) (!buffer_ptr[0] && !buffer_ptr[1] && (buffer_ptr[2] == 1))
#include "SLog.h"
//TODO: take this log off, this header is supposed to be free of types from outside
SLOG_CATEGORY("NaluUtils");


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
bool transferNalu(uint8_t *const currentEncodedPacketBegginingPtr,
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
    //printf("currentEncodedPacketSearchPtr: %p\n", (void*)currentEncodedPacketSearchPtr);
    /*
        We're gonna find the first NALU in the packet
        If currentEncodedPacketSearchPtr is not setted, it means we should start at the beggining
        of the currentEncodedPacket. Otherwise, we're continuing the work of the previous transferNalu
        call, in which it found a NALU in the middle of the currentEncodedPacket and returned true
    */
    if (!currentEncodedPacketSearchPtr)
        currentEncodedPacketSearchPtr = currentEncodedPacketBegginingPtr;
    /*
    printf("currentEncodedPacketBegginingPtr: %p\n", (void*)currentEncodedPacketBegginingPtr);
    printf("currentEncodedPacketSearchPtr: %p\n", (void*)currentEncodedPacketSearchPtr);
    printf("planeBufferPtr: %p\n", (void*)planeBufferPtr);
    printf("currentEncodedPacketSize: %p\n", (void*)currentEncodedPacketSize);

    std::cout << "r: " << (currentEncodedPacketSearchPtr - currentEncodedPacketBegginingPtr) << " r: " << (currentEncodedPacketSize - 3) << std::endl;
    abort();

    */
    while ((currentEncodedPacketSearchPtr - currentEncodedPacketBegginingPtr) < (currentEncodedPacketSize - 3))
    {
        nalu_found = IS_NAL_UNIT_START(currentEncodedPacketSearchPtr) || IS_NAL_UNIT_START1(currentEncodedPacketSearchPtr);
        if (nalu_found) {
            LOG << "Found NALU start at " << (void*)currentEncodedPacketSearchPtr;
            break;
        }
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
    //LOG << "found NALU at " << (void*)currentEncodedPacketSearchPtr << " gonna copy it now";
    std::memcpy(planeBufferPtr, currentEncodedPacketSearchPtr, 4);
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
    while ((currentEncodedPacketSearchPtr - currentEncodedPacketBegginingPtr) < (currentEncodedPacketSize - 3))
    {
        if (IS_NAL_UNIT_START(currentEncodedPacketSearchPtr) || IS_NAL_UNIT_START1(currentEncodedPacketSearchPtr))
        {
            //nalu_end_found = true;
            LOG << "Found NALU end at " << (void*)currentEncodedPacketSearchPtr << ", returning...";
            return false;
        }
        *planeBufferPtr = *currentEncodedPacketSearchPtr;
        planeBufferPtr++;
        currentEncodedPacketSearchPtr++;
        bytesWritten++;
    }
    //TODO:??
    LOG << "did not find NALU end, gonna return true to query another packet ";
    currentEncodedPacketSearchPtr = NULL;
    return true;
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
