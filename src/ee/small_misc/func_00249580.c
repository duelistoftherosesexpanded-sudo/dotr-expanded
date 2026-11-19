#include "dotr/ee/small_misc.h"

int TaEnc_GetEncID(int cardNumber)
{
    return EnchantId[(unsigned short)cardNumber];
}
