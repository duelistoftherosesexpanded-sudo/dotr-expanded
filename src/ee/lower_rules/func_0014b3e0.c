#include "dotr/ee/lower_rules.h"

int TaLogic013_IsUseHarpySweaper(int side, int col, int row)
{
    if (TaLogic013_IsActiveMode(side, col, row) == 0) {
        return 0;
    }
    /* 30: non-monster cards; 18: in the same column or row */
    if (SearchUnitByRange(col, row, side, 1, 30, 18) != 0) {
        return 0;
    }
    return SearchUnitByRange(col, row, side, 2, 30, 18) != 0 ? 1 : 0;
}
