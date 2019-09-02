#ifndef NaluUtils_H
#define NaluUtils_H
#include <iostream>
#include <cstring>
bool transferNalu(uint8_t *const currentEncodedPacketBegginingPtr,
                         uint8_t *currentEncodedPacketSearchPtr,
                         size_t currentEncodedPacketSize,
                         uint8_t *planeBufferPtr,
                         uint32_t &bytesWritten);
#endif //NaluUtils_H