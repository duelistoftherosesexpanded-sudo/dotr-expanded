#include "dotr/ee/small_misc.h"

/* Both results are masked explicitly; an unsigned short return type drops
 * the first mask. */
int TaLogic_GetHandFusion(unsigned short cardNo, unsigned short equipNo)
{
    int result;
    if (cardNo == 999 || equipNo == 999) {
        return 999;
    }
    result = Ta_GetHandEnchant(cardNo, equipNo);
    if (result >> 16 == 0) {
        if (TaEnc_IsEncDo(equipNo) == 1) {
            return result & 0xffff;
        }
    }
    return Fusion(cardNo, equipNo) & 0xffff;
}
