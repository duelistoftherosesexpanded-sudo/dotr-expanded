#include "dotr/ee/small_state.h"

int SzSound_IsStandby(void)
{
    return data_0034f6e8[0].unknown_bit_0 ? 1 : 0;
}
