#include "dotr/ee/state_grid.h"

int SzDuel_CalcUnitAP(int side, int pos, int operation, int value)
{
    return SzDuel_CalcUnitAP2(dotr_state_item_checked(side, pos), operation, value);
}
