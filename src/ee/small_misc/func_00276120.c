#include "dotr/ee/small_misc.h"

int TaLogic_DoUnitAssign(unsigned short ownerId, DotrMiscSlot16 *table)
{
    int i;
    DotrMiscSlot16 *currentUnit;

    for (i = 0, currentUnit = table; i < 22; i++, currentUnit++) {
        if (currentUnit->key == 0xffff) {
            DotrMiscSlot16 *unit = &table[i];

            unit->key = ownerId;
            (&table[i])->value = SzDuel_GetUnitSummonOrder(ownerId >> 8, ownerId & 0xff);
            table[i].ref = -1;
            table[i].link = 0xffff;
            table[i].mode = 0;
            return 1;
        }
    }
    return 0;
}
