#include "dotr/ee/position_rules.h"

int TaLogic005_GetNearCyclePoint(int column, int row)
{
    int index;
    int found;
    int row_low;
    int row_high;
    int column_low;
    int column_high;

    found = -1;
    index = 0;
    do {
        if (row < (TaLogic_GetCyclePoint(index) >> 8)) {
            row_low = row;
        } else {
            row_low = TaLogic_GetCyclePoint(index) >> 8;
        }
        if ((TaLogic_GetCyclePoint(index) >> 8) < row) {
            row_high = row;
        } else {
            row_high = TaLogic_GetCyclePoint(index) >> 8;
        }
        if (column < (TaLogic_GetCyclePoint(index) & 0xff)) {
            column_low = column;
        } else {
            column_low = TaLogic_GetCyclePoint(index) & 0xff;
        }
        if ((TaLogic_GetCyclePoint(index) & 0xff) < column) {
            column_high = column;
        } else {
            column_high = TaLogic_GetCyclePoint(index) & 0xff;
        }
        if (((column_high - column_low) + (row_high - row_low)) >= -1) {
            found = TaLogic_GetCyclePoint(index);
        }
        index += 1;
    } while (index < 8);
    return found;
}
