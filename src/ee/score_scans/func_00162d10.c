#include "dotr/ee/score_scans.h"

int TaLogic019_IsUseRevolt(int side)
{
    int found[1];
    unsigned short ownerIds[22];
    int i;
    unsigned short *q;
    int r;
    int y;
    int cell;
    unsigned short *p;
    int row;
    int score;
    int first;
    int best;
    int column;
    int pos;

    cell = SzDuel_GetUnitLoc(side == 0 ? 1 : 0, 0);
    if (Ta_SearchUncTypeNaturalEffectUserNum(57) > 0) {
        return -1;
    }
    first = Ta_RemoveObjFromFlagLst(SearchUnitByRange(0, 0, side, 2, 33, 14), side);
    p = ownerIds;
    for (i = 0, q = p; i < 22; i++) {
        *q++ = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst(side == 0 ? 1 : 0, ownerIds, 52, 0);
    found[0] = -1;
    pos = found[0];
    best = 1;
    i = 0;
    if (0 < TaLogic_GetUnitCtlLstLen(ownerIds)) {
        y = cell >> 8;
        cell &= 0xFF;
        do {
            r = SzDuel_GetUnitLoc(*p >> 8, *p & 0xFF);
            row = r >> 8;
            column = r & 0xFF;
            if (((column < cell ? cell : column) - (cell < column ? cell : column)) + ((row < y ? y : row) - (y < row ? y : row)) == 1) {
                if (SzDuel_GetUnitStatus(*p >> 8, *p & 0xFF, 1) == 0 && SzDuel_GetUnitCurseRest(*p >> 8, *p & 0xFF) == 0
                    && SzDuel_GetUnitSpellRest(*p >> 8, *p & 0xFF) == 0) {
                    if ((score = TaUtil_GetUnitAP(*p >> 8, *p & 0xFF, column, row)) >= best) {
                        pos = *p;
                        if (pos == Ta_GetUnitOwnerIDFromLstCount(first, 0)) {
                            found[0] = pos;
                        }
                        best = score;
                    }
                }
            }
            p++;
            i++;
        } while (i < TaLogic_GetUnitCtlLstLen(ownerIds));
    }
    if (pos < 0) {
        return -1;
    }
    return (pos & 0xFFFF) << 16 | (found[0] & 0xFFFF);
}
