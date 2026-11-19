#include "dotr/ee/small_misc.h"

/* Runs SzDuel_ClearCardControl on each of the six elements of gSzDuel_aCardControl. */
void SzDuel_InitCardControl(void)
{
    struct DotrMiscEntry160 *p = gSzDuel_aCardControl;
    int i;
    for (i = 0; i < 6; i++) {
        SzDuel_ClearCardControl((struct DotrMiscSlot160 *)p);
        p++;
    }
}
