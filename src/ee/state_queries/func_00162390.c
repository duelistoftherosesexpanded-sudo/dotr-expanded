#include "dotr/ee/state_queries.h"

int TaLogic019_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2)
{
    int ylo;
    int yhi;
    int xlo;
    int xhi;

    ylo = y1 < y2 ? y1 : y2;
    yhi = y1 > y2 ? y1 : y2;
    xlo = x1 < x2 ? x1 : x2;
    xhi = x1 > x2 ? x1 : x2;
    if ((xhi - xlo) + (yhi - ylo) != 2) {
        return 0;
    }
    if (func_001df540_word(side, pos) == 0) {
        return 0;
    }
    if (SzDuel_GetUnitStatus(side, pos, 0) == 0) {
        if (CheckLandAgreement2(SzDuel_GetField(x1, y1), side, pos) != 1) {
            return 0;
        }
        return 1;
    }
    return 0;
}
