#include "dotr/ee/side_table.h"

int TaSlot_GetFeverGain(unsigned short nCardNo)
{
    int i;
    int Count;
    unsigned short wCardNo[854];
    struct jp_8d5b *pConstInfo;

    switch (nCardNo) {
    case 60: return 87;
    case 837: return 83;
    case 754: return 699;
    case 716: return 715;
    case 272: return 702;
    case 539: return 700;
    case 722: return 811;
    case 61: return 733;
    }
    for (i = 0, Count = 0; i < 854; i++) {
        if (i == 87) continue;
        if (i == 83) continue;
        if (i == 699) continue;
        if (i == 715) continue;
        if (i == 702) continue;
        if (i == 700) continue;
        if (i == 733) continue;
        if (i == 811) continue;
        pConstInfo = (struct jp_8d5b *)SzDuel_GetCardConstInfo(i);
        if (pConstInfo != 0 && pConstInfo->fGetFever) {
            wCardNo[Count++] = i;
        }
    }
    if (Count > 0) {
        return wCardNo[func_0019a5c8() % Count];
    }
    return 999;
}
