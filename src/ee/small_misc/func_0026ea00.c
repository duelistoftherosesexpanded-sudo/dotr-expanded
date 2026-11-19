#include "dotr/ee/small_misc.h"

/* Dispatches on the first word: 0, 1 and 2 return the result of
 * Ta_Eff_Hand_Fusion, Ta_Eff_Hand_Enc and Ta_Eff_Hand_Fail; anything else returns 0. */
int Ta_ManageHandEffect(int *handResult)
{
    switch (*handResult) {
    case 0:
        return Ta_Eff_Hand_Fusion(handResult);
    case 1:
        return Ta_Eff_Hand_Enc(handResult);
    case 2:
        return Ta_Eff_Hand_Fail(handResult);
    }
    return 0;
}
