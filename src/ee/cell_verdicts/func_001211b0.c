#include "dotr/ee/cell_verdicts.h"
int TaLogic_IsOpenUnit__001211b0(int x, int y, struct DotrRuleOrder *unused)
{
    int i;
    int n;
    int side;
    unsigned char count;
    int r;
    unsigned *p;
    /* The frame holds 16 entries (32 bytes at sp+112 of 144) while the 502
     * case below fills 22: observed in the original, not corrected. */
    unsigned short buf[16];
    (void)unused;
    n = 0;
    side = func_00250360_pair(x, y);
    count = 0;
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    for (i = 1; i < 5; i++) {
        if ((SzDuel_GetUnitKind(x, i) & 0x1f) == 14 && x == func_00250360_pair(x, i)) {
            count++;
        }
        if ((SzDuel_GetUnitKind(x == 0 ? 1 : 0, i) & 0x1f) == 14 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
            count++;
        }
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (SzDuel_GetUnitCardNo(x, y)) {
        case 514: /* Machine King */
            if (count >= 3) {
                return 1;
            }
            break;
        case 502: /* Barrel Dragon */
            for (i = 0; i < 22; i++) {
                buf[i] = 0xffff;
            }
            TaUtil_GetUnitLst(side == 0 ? 1 : 0, 3, buf);
            r = TaLogic_GetMaxApUnit(buf);
            if (side != func_00250360_pair(r >> 8, r & 0xff)) {
                return 1;
            }
            break;
        case 523: /* Patrol Robo */
            for (i = 1; i < 11; i++) {
                if (side != func_00250360_pair(side, i) && SzDuel_GetUnitStatus(side, i, 4) == 0) {
                    n++;
                }
                if (side != func_00250360_pair(side == 0 ? 1 : 0, i) && SzDuel_GetUnitStatus(side == 0 ? 1 : 0, i, 4) == 0) {
                    n++;
                }
            }
            if (n >= 6) {
                return 1;
            }
            break;
        }
        return 2;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        p = func_00250740_word(x, y, 0);
        if (p == 0) {
            return 2;
        }
        switch (*p >> 8) {
        case 18: /* AP/DP Buff */
            if (count >= 3) {
                return 1;
            }
            break;
        }
        return 2;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    }
    return 0;
}
