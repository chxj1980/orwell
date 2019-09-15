#include "NaluUtils.h"
//(buffer_ptr) so it works when we pass pointer to pointer too
#define IS_NAL_UNIT_HEADER(buffer_ptr) (!(buffer_ptr)[0] && !(buffer_ptr)[1] && !(buffer_ptr)[2] && ((buffer_ptr)[3] == 1))
#define IS_NAL_UNIT_HEADER1(buffer_ptr) (!(buffer_ptr)[0] && !(buffer_ptr)[1] && ((buffer_ptr)[2] == 1))
#include "SLog.h"
//TODO: take this log off, this header is supposed to be free of types from outside
SLOG_CATEGORY("NaluUtils");

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
            //printf("Found NALU header at %02X\n", (uint8_t *)currentEncodedPacketSearchPtr);
            return currentEncodedPacketSearchPtr;
        }
        currentEncodedPacketSearchPtr++;
    }
    return NULL;
}

void printPacket(uint8_t* pointer, size_t size)
{
    printf("----------Entire packet: \n");
    for (int i = 0; i < size; i++)
    {
        printf("%02X ", pointer[i]);
    }
    printf("\n");
}