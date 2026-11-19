#include "dotr/ee/small_misc.h"

/* Linear search of the 26540-word pair table for the unordered pair (a, b);
 * returns the 10-bit result field at bits 20-29, or 999 if absent. */
int Fusion(int cardA, int cardB)
{
    int i;
    unsigned int *currentFusionAddr;
    int t;
    if (cardA < cardB) { t = cardA; cardA = cardB; cardB = t; }
    currentFusionAddr = FusionDat;
    for (i = 0; i < 26540 /* fusion list size */; i++, currentFusionAddr++) {
        if ((unsigned int)cardA == (*currentFusionAddr & 0x3ff) && (unsigned int)cardB == ((*currentFusionAddr & 0xffc00) >> 10))
            return (FusionDat[i] & 0x3ff00000) >> 20;
    }
    return 999;
}
