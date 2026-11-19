#include "dotr/ee/score_scans.h"

int TaLogic006_IsBlackHoleCondition(int side)
{
    unsigned short unitArray[22];
    int i;
    unsigned short *q;
    int count;
    unsigned short *p;
    int a;
    int k;
    int b;
    int y;
    int k2;
    int unitApPlayer;
    int y2;
    int j;

    p = unitArray;
    count = 0;
    for (i = 0, q = p; i < 22; i++) {
        *q++ = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst2(side == 0 ? 1 : 0, unitArray, 29, 0, 0);
    a = TaLogic_GetMaxApUnit(unitArray);
    for (i = 0; i < 22; i++) {
        *p++ = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst2(side, unitArray, 29, 1, 1);
    b = TaLogic_GetMaxApUnit(unitArray);
    y = a & 0xFF;
    a >>= 8;
    k = SzDuel_GetUnitLoc(a, y) & 0xFF;
    unitApPlayer = TaUtil_GetUnitAP(a, y, k, SzDuel_GetUnitLoc(a, y) >> 8);
    y2 = b & 0xFF;
    b >>= 8;
    k2 = SzDuel_GetUnitLoc(b, y2) & 0xFF;
    if (TaUtil_GetUnitAP(b, y2, k2, SzDuel_GetUnitLoc(b, y2) >> 8) < unitApPlayer) {
        if (unitApPlayer >= 3000) {
            for (j = 1; j < 11; j++) {
                if (func_001df540_word(side, j) == 0) {
                    continue;
                }
                count++;
            }
        }
    }
    return count < 3 ? 1 : 0;
}
