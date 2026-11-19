#include "dotr/ee/score_scans.h"

int TaLogicCom_GetUseXYRangeSpell__0016cbb0(int side, unsigned *effect)
{
    unsigned short words[22];
    int i;
    unsigned short *q;
    unsigned short *p;
    int a;
    int b;
    unsigned char n;
    int cell;
    int y;
    int k;
    int word;
    int x;
    int lo;
    int hi;

    p = words;
    for (i = 0, q = p; i < 22; i++) {
        *q++ = 0xFFFF;
    }
    switch (*effect & 0x3F) {
    case 29:
        TaLogic_GetUnitCtlLst2(side == 0 ? 1 : 0, words, 29, 0, 0);
        a = TaLogic_GetMaxApUnit(words);
        for (i = 0; i < 22; i++) {
            *p++ = 0xFFFF;
        }
        TaLogic_GetUnitCtlLst(side, words, 29, 1);
        b = TaLogic_GetMaxApUnit(words);
        if (a >= 0 && b >= 0) {
            int r = SzDuel_GetUnitLoc(a >> 8, a & 0xFF);

            if (TaLogic_IsUnitWin(a, b, r & 0xFF, r >> 8) >= 0) {
                return r;
            }
        }
        break;
    case 30:
        for (x = 0; x < 7; x++) {
            for (y = 0; y < 7; y++) {
                n = 0;
                word = SearchUnitByRange(x, y, side, 1, 52, 18);
                for (k = 0; k < Ta_GetUnitNumFrom22bitFlagList(word); k++) {
                    cell = Ta_GetUnitOwnerIDFromLstCount(word, k);
                    hi = cell >> 8;
                    lo = cell & 0xFF;
                    if (SzDuel_GetUnitStatus(hi, lo, 4) == 0) {
                        n++;
                    } else if (SzDuel_GetUnitKind(hi, lo) >> 5) {
                        n++;
                    }
                }
                if (n > 2) {
                    return y << 8 | x;
                }
            }
        }
        break;
    }
    return -1;
}
