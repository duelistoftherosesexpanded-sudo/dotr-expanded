#include "dotr/ee/state_grid.h"

int SzDuel_CalcUnitDP(int side, int pos, int operation, int value)
{
    return SzDuel_CalcUnitDP2(dotr_state_item_checked(side, pos), operation, value);
}
