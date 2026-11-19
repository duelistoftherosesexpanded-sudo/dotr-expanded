#include "dotr/ee/state_grid.h"

int SzDuel_PutUnit(int a, int b, int c)
{
    struct DotrStateItem *item = dotr_state_item_checked(a, b);
    struct DotrStateBlock local;
    int first, second;
    if (item == 0) return 0;
    a = item->team;
    b = item->slot;
    first = item->locCol;
    second = item->locRow;
    SzDuel_DelUnit(a, b);
    SzDuel_InitCardInfo(&local, c);
    return SzDuel_AddUnit2(a, first, second, &local, b == 0 ? 1 : 0);
}
