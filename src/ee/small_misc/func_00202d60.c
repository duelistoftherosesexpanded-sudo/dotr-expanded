#include "dotr/ee/small_misc.h"

int SzText_FreeRequest(int textureFlags, int textureId)
{
    struct DotrMiscScanEntry *pTextItem = gSzText_aItem;
    int r = 0;
    int i;
    for (i = 0; i < 1024; i++, pTextItem++) {
        if (pTextItem->unknown_00 == 0) {
            continue;
        }
        if (!pTextItem->unknown_08.unknown_bit_0) {
            continue;
        }
        if (textureFlags >= 0 && pTextItem->unknown_08.unknown_bits_3_15 != textureFlags) {
            continue;
        }
        if (textureId >= 0 && pTextItem->unknown_0a != textureId) {
            continue;
        }
        pTextItem->unknown_08.unknown_bit_0 = 0;
        r = 1;
    }
    return r;
}
