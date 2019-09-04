#ifndef NaluUtils_H
#define NaluUtils_H
#include <iostream>
#include <cstring>
enum NaluSearchState{
    LOOKING_FOR_NALU_START,
    LOOKING_FOR_NALU_END
};
bool transferNalu(uint8_t *const currentEncodedPacketBegginingPtr,
                         uint8_t *currentEncodedPacketSearchPtr,
                         size_t currentEncodedPacketSize,
                         uint8_t *planeBufferPtr,
                         uint32_t &bytesWritten,
                         NaluSearchState &naluSearchState);
#endif //NaluUtils_H