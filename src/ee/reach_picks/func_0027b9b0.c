#include "dotr/ee/reach_picks.h"

int TaLogic_IsUnitReadyMove(unsigned short wUnit)
{
    int i;
    int j;
    int Loc;
    unsigned int dwMoveRev;

    if ((Loc = SzDuel_GetUnitLoc(wUnit >> 8, wUnit & 0xff)) < 0) {
        return 0;
    }
    if (SzDuel_GetUnitStatus(wUnit >> 8, wUnit & 0xff, 0)) {
        return 0;
    }
    dwMoveRev = func_002464f0_rule(func_001df540_word(wUnit >> 8, wUnit & 0xff), Loc & 0xff, Loc >> 8);
    for (i = GetCap(0, (Loc & 0xff) - 1); i <= ((Loc & 0xff) + 1 >= 7 ? 6 : (Loc & 0xff) + 1); i++) {
        for (j = GetCap(0, (Loc >> 8) - 1); j <= ((Loc >> 8) + 1 >= 7 ? 6 : (Loc >> 8) + 1); j++) {
            if (((i < (Loc & 0xff) ? (Loc & 0xff) : i) - ((Loc & 0xff) < i ? (Loc & 0xff) : i)) + ((j < (Loc >> 8) ? (Loc >> 8) : j) - ((Loc >> 8) < j ? (Loc >> 8) : j)) == 1) {
                if (SzDuel_GetMapUnit(i, j) == 0) {
                    if (SzDuel_GetField(i, j) == 8 && !(dwMoveRev & 0x10)) {
                        continue;
                    }
                    if (SzDuel_GetField(i, j) == 9 && func_0024fe40_rule(func_001df540_word(wUnit >> 8, wUnit & 0xff), i, j) == 1) {
                        continue;
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}
