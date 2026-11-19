#include "dotr/ee/side_table.h"

void SzData_SetupFromConstDeck(int side, int deckIndex)
{
    struct DotrSide *p = DOTR_SIDE(side);
    unsigned short *tbl = gSzData_ConstDeck[deckIndex];
    unsigned short *q;
    int i;
    int n;

    if (deckIndex < 0 || deckIndex >= 51) {
        return;
    }
    SzData_ClearData(side);
    if (deckIndex < 17) {
        n = 0;
        q = &StringBlob[playerNameOffset] + 2;
        for (;;) {
            n += *q & 0x1fff;
            if (*q & 0x8000) {
                break;
            }
            q++;
        }
        do {
            p->unknown_ff98 = (n + func_0019a5c8()) & 0x7fffffff;
        } while (p->unknown_ff98 == 0);
    } else {
        p->unknown_ff98 = deckIndex + 0x40000000;
    }
    for (i = 0; i < 41; i++) {
        SzData_AddBag(side, 0, tbl[i] & 0xfff);
    }
    for (i = 0; i < 5; i++) {
        p->newcomers[i] = 999;
    }
    SzData_SetRankDirect(side, tbl[0] & 0xfff, tbl[0] >> 12);
    SzData_PushDeckLeader(side, 0, tbl[0] & 0xfff);
    for (i = 1; i < 41; i++) {
        SzData_PushDeck(side, 0, tbl[i] & 0xfff);
    }
    SzText_Free(0, 1);
    SzText_Free(0, 0);
    SzText_Free(1, -1);
}
