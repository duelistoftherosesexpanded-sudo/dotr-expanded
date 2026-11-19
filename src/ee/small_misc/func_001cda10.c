#include "dotr/ee/small_misc.h"

/* Runs SzDuel_FreeCardRequest on each of the 32 elements of gSzDuel_aCardRequest. */
void SzDuel_InitCardRequest(void)
{
    struct DotrMiscEntry160 *p = gSzDuel_aCardRequest;
    int i;
    for (i = 0; i < 32; i++) {
        SzDuel_FreeCardRequest((struct DotrMiscNode *)p);
        p++;
    }
}
