#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitSpellRest(int side, int pos)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    return unit ? unit->spellRest : -1;
}
