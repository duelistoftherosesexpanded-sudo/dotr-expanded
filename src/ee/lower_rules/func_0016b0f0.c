#include "dotr/ee/lower_rules.h"
int TaLogic021_IsUseBlackHole(int side)
{
    int i;
    int aiUnitCount;
    int low;
    int col;
    int pos;
    int row;
    aiUnitCount = 0;
    pos = Ta_GetUnitOwnerIDFromLstCount(SearchUnitByRange(0, 0, side, 3, 33, 14), 0);
    row = pos >> 8;
    col = pos & 0xff;
    if (side != func_00250360_pair(row, col)) {
        low = SzDuel_GetUnitLoc(row, col) & 0xff;
        if (TaUtil_GetUnitAP(row, col, low, SzDuel_GetUnitLoc(row, col) >> 8) >= 3000) {
            for (i = 1; i < 11; i++) {
                if (func_001df540_word(side, i) != 0) {
                    aiUnitCount++;
                }
            }
        }
    }
    return aiUnitCount < 3 ? 1 : 0;
}
