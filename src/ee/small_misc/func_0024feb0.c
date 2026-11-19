#include "dotr/ee/small_misc.h"

int Ta_IsVirusAll(int *out)
{
    int i;
    unsigned word;
    int cell;

    word = 0;
    for (i = 1; i < 11; i++) {
        cell = SzDuel_GetUnitLoc(0, i);
        if (Ta_IsVirus_Chk(0, i) == 1 && SzDuel_GetField(cell & 0xff, cell >> 8) == 9) {
            word = ConvUnitIDtoFlagList(word, 0, i);
            TaUtil_DoOpen(0, i, 1);
        }
        cell = SzDuel_GetUnitLoc(1, i);
        if (Ta_IsVirus_Chk(1, i) == 1 && SzDuel_GetField(cell & 0xff, cell >> 8) == 9) {
            word = ConvUnitIDtoFlagList(word, 1, i);
            TaUtil_DoOpen(1, i, 1);
        }
    }
    if (Ta_GetUnitNumFrom22bitFlagList(word) > 0) {
        out[0] = 0x70;
        out[1] = 0;
        out[2] = word;
        out[3] = 0;
        return 1;
    }
    return 0;
}
