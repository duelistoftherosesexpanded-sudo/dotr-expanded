#include "dotr/ee/score_scans.h"

int TaLogicCom_IsUseObjEffect__0016c6b0(int side, int sideComp, int searchMode)
{
    int cell;
    int i;
    int other;
    int own;
    int word;
    int y;
    int x;
    int r;

    for (i = 0; i < 22; i++) {
    }
    own = -1;
    other = own;
    word = SearchUnitByRange(0, 0, side, sideComp, searchMode, 14);
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        cell = Ta_GetUnitOwnerIDFromLstCount(word, i);
        x = cell >> 8;
        y = cell & 0xFF;
        if (side == func_00250360_pair(x, y)) {
            r = TaUtil_GetUnitAP(x, y, SzDuel_GetUnitLoc(x, y) & 0xFF, SzDuel_GetUnitLoc(x, y) >> 8);
            if (own < r) {
                own = r;
            }
        } else {
            r = TaUtil_GetUnitAP(x, y, SzDuel_GetUnitLoc(x, y) & 0xFF, SzDuel_GetUnitLoc(x, y) >> 8);
            if (other < r) {
                other = r;
            }
        }
    }
    if (own < other && other > 2500) {
        return 1;
    }
    if (sideComp & 1) {
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, searchMode, 14)) > 0) {
            return 0;
        }
    }
    if (sideComp >> 1) {
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 2, searchMode, 14)) > 3) {
            return 1;
        }
    }
    return 0;
}
