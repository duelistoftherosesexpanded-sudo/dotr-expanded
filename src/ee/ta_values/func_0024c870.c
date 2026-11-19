#include "dotr/ee/ta_values.h"

int Ta_ConvBitLengthToNumericLength(int bitLength)
{
    int i;
    int retVal;
    for (i = 0, retVal = 0; i < bitLength; i++) {
        retVal = (int)((double)retVal + func_001a6be8_ta(2.0, (double)i));
    }
    return retVal;
}
