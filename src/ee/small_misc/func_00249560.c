#include "dotr/ee/small_misc.h"

int TaEnc_IsActReflexEnc(int index)
{
    return EnchantId[(unsigned short)index] == 0 ? 1 : 0;
}
