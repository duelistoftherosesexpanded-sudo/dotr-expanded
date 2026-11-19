#include "dotr/ee/state_records.h"

void SzDuel_AddUnit(int side, int col, int row, int cardNo, int e)
{
    struct DotrStateBlock unit;
    SzDuel_InitCardInfo(&unit, cardNo);
    SzDuel_AddUnit2(side, col, row, &unit, e);
}
