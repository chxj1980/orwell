#ifndef EncodedUnits_H
#define EncodedUnits_H
#include <iostream>
#include <vector>
#include "EncodedUnit.h"
/*
    A class to hold together multiple EncodedUnit instances
    in order to do NALU finding. It also enables to copy the NALUs
    to a buffer.
    How it works?
    It stores lots of EncodedUnit instances, and treat them as a 
    large file, so given an index in this large file, it will end
    up accessing one of the internal elements. 
*/

class EncodedUnits
{
public:
    EncodedUnits(const EncodedUnits &decodedFrame) = delete;
    EncodedUnits &operator=(const EncodedUnits &) = delete;
    EncodedUnits(EncodedUnits &&) = default;
    EncodedUnits &operator=(EncodedUnits &&) = default;

    EncodedUnits()
    {
    }

    void addUnit(EncodedUnit &&encodedUnit)
    {
        size += encodedUnit.frameSize;
        encodedUnits.push_back(std::move(encodedUnit));
    }
    //Returns the EncodedUnit element that holds data for the index
    EncodedUnit &whichElement(std::streamsize index)
    {
        if (index > size)
        {
            //throw exception
        }
        std::streamsize accumulatedSize = 0;
        std::streamsize futureAccumulatedSize = 0;

        for (auto encodedUnit = encodedUnits.begin(); encodedUnit != encodedUnits.end();)
        {
            accumulatedSize += (*encodedUnit).frameSize;
            if (encodedUnit != encodedUnits.end())
                ++encodedUnit;
            futureAccumulatedSize = accumulatedSize + (*encodedUnit).frameSize;
            
        }
    }

private:
    std::streamsize size = 0;
    std::vector<EncodedUnit> encodedUnits;
};
#endif //EncodedUnits_H