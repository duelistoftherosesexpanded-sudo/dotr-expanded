#include "dotr/ee/slot_table.h"

void SzMsg_GetDefRect(struct DotrSlotTableWords *destination)
{
    *destination = gSzMsg_defRect[0];
}
