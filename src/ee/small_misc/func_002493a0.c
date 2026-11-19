#include "dotr/ee/small_misc.h"

/* The halfword table value for a kind byte of 0, 1000 for kinds 1 and 4,
 * otherwise 0. Kinds 1 and 4 share one case body. */
int TaEnc_GetEncScore(int cardID)
{
    switch (EnchantId[cardID]) {
    case 0:
        return gAnmPat_Bakura[cardID];
    case 1:
    case 4:
        return 1000;
    }
    return 0;
}
