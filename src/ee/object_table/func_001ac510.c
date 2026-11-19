#include "dotr/ee/object_table.h"

/* Calls SzModel_SetDrawFlag(i, 0) for each of the 84 model table entries, then
 * SzGame_InitMemCardProc(6). */
void lSzGame_InitMemCheck(void)
{
    int i;
    for (i = 0; i < 84; i++) {
        SzModel_SetDrawFlag(i, 0);
    }
    SzGame_InitMemCardProc(6);
}
