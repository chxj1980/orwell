#include "NaluUtils.h"
//(buffer_ptr) so it works when we pass pointer to pointer too
#define IS_NAL_UNIT_HEADER(buffer_ptr) (!(buffer_ptr)[0] && !(buffer_ptr)[1] && !(buffer_ptr)[2] && ((buffer_ptr)[3] == 1))
#define IS_NAL_UNIT_HEADER1(buffer_ptr) (!(buffer_ptr)[0] && !(buffer_ptr)[1] && ((buffer_ptr)[2] == 1))
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
    uint32_t &bytesWritten: tells the called how many bytes we wrote to planeBufferPt
    NaluSearchState &naluSearchState: holds the state of the transferNalu function
*/
bool transferNalu(uint8_t *const currentEncodedPacketBegginingPtr,
                  uint8_t **currentEncodedPacketSearchPtr,
                  size_t currentEncodedPacketSize,
                  uint8_t *planeBufferPtr,
                  uint32_t &bytesWritten,
                  NaluSearchState &naluSearchState)
{
    int h265_nal_unit_type;
    bool nalu_found = false;

    if (currentEncodedPacketSize <= 4)
    {
        //Throw?
    }
    /*
        We're gonna find the first NALU in the packet
        If currentEncodedPacketSearchPtr is not setted, it means we should start at the beggining
        of the currentEncodedPacket. Otherwise, we're continuing the work of the previous transferNalu
        call, in which it found a NALU in the middle of the currentEncodedPacket and returned true
    */
    if (!*currentEncodedPacketSearchPtr)
    {
        printf("currentEncodedPacketSearchPtr null, setting to beggining %02X\n", (void *)*currentEncodedPacketBegginingPtr);
        *currentEncodedPacketSearchPtr = currentEncodedPacketBegginingPtr;
    }

    if (naluSearchState == LOOKING_FOR_NALU_START)
    {
        while ((*currentEncodedPacketSearchPtr - currentEncodedPacketBegginingPtr) < (currentEncodedPacketSize - 3))
        {
            nalu_found = IS_NAL_UNIT_HEADER(*currentEncodedPacketSearchPtr) || IS_NAL_UNIT_HEADER1(*currentEncodedPacketSearchPtr);
            if (nalu_found)
            {
                printf("Found NALU start at %02X\n", (uint8_t *)*currentEncodedPacketSearchPtr);
                break;
            }
            (*currentEncodedPacketSearchPtr)++;
        }
        if (!nalu_found)
        {
            /*
                TODO: understand this category better
            */
            printf("did not find NALU start, gonna return true to query another packet\n");
            *currentEncodedPacketSearchPtr = NULL;
            naluSearchState = LOOKING_FOR_NALU_START;
            return true;
        }
        else
        {
            /*
            Copies the first 4 bytes, which form the NALU header. If the header is in 3 byte form
            it doesn't matter, we just copied another extra byte
        */
            std::memcpy(planeBufferPtr, *currentEncodedPacketSearchPtr, 4);
            planeBufferPtr += 4;
            *currentEncodedPacketSearchPtr += 4;
            bytesWritten += 4;
        }
    }

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

    while ((*currentEncodedPacketSearchPtr - currentEncodedPacketBegginingPtr) < (currentEncodedPacketSize - 3))
    {
        if (IS_NAL_UNIT_HEADER(*currentEncodedPacketSearchPtr) || IS_NAL_UNIT_HEADER1(*currentEncodedPacketSearchPtr))
        {
            printf("Found NALU end at %02X\n", (uint8_t *)*currentEncodedPacketSearchPtr);
            naluSearchState = LOOKING_FOR_NALU_START;
            return false;
        }
        *planeBufferPtr = **currentEncodedPacketSearchPtr;
        planeBufferPtr++;
        (*currentEncodedPacketSearchPtr)++;
        bytesWritten++;
    }
    /*
        Reached end of packet but did not find NALU end. Remember
        we must copy the remaining 3 bytes before we return true
        to inform the called we consumed the entire packet
    */
    memcpy(planeBufferPtr, *currentEncodedPacketSearchPtr, 3);
    bytesWritten += 3;
    *currentEncodedPacketSearchPtr = NULL;
    naluSearchState = LOOKING_FOR_NALU_END;
    printf("did not find NALU end, gonna return true to query another packet ");
    printf("currentEncodedPacketSearchPtr: %02X\n", (uint8_t *)*currentEncodedPacketSearchPtr);
    printf("walked %u\n", (*currentEncodedPacketSearchPtr - currentEncodedPacketBegginingPtr));
    return true;
}

uint8_t *findNaluHeader(uint8_t *const currentEncodedPacketBegginingPtr,
                        size_t currentEncodedPacketSize,
                        uint8_t *currentEncodedPacketSearchPtr)
{
    bool nalu_found = false;
    while ((currentEncodedPacketSearchPtr - currentEncodedPacketBegginingPtr) < (currentEncodedPacketSize - 3))
    {
        nalu_found = IS_NAL_UNIT_HEADER(currentEncodedPacketSearchPtr) || IS_NAL_UNIT_HEADER1(currentEncodedPacketSearchPtr);
        if (nalu_found)
        {
            printf("Found NALU start at %02X\n", (uint8_t *)currentEncodedPacketSearchPtr);
            return currentEncodedPacketSearchPtr;
        }
    }
    return NULL;
}
//Starts searchig at beggining of currentEncodedPacket
currentEncodedPacketSearchPtr = currentEncodedPacket.frameBuffer.get();
while (true)
{
    bytesWritten = 0;
    uint8_t *naluStart = findNaluHeader(currentEncodedPacket.frameBuffer.get(),
                                        currentEncodedPacket.frameSize,
                                        currentEncodedPacketSearchPtr);
    if (!naluStart)
    {
        //Something very strange happened
    }

    uint8_t *naluEnd = findNaluHeader(currentEncodedPacket.frameBuffer.get(),
                                      currentEncodedPacket.frameSize,
                                      naluStart);
    if (naluEnd)
    {
        /*
            We found the end, copy bytes. Next iteration will continue 
            from here
        */
        memcpy(naluStart, naluEnd, naluEnd - naluStart);
        bytesWritten += naluEnd - naluStart;
    }
    else
    {
        while (!naluEnd)
        {
            currentEncodedPacket = std::move(encodedPacketsFifo->pop_front());
            currentEncodedPacketSearchPtr = currentEncodedPacket.frameBuffer.get();
            naluEnd = findNaluHeader(currentEncodedPacket.frameBuffer.get(),
                                     currentEncodedPacket.frameSize,
                                     currentEncodedPacketSearchPtr);
            memcpy(currentEncodedPacketSearchPtr, naluEnd, naluEnd - currentEncodedPacketSearchPtr);
            bytesWritten += naluEnd - currentEncodedPacketSearchPtr;
        }
    }
}