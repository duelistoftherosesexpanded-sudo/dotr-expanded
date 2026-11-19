#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitModelID(int side, int pos)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    return p ? p->modelId : -1;
}
