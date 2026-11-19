#include "dotr/ee/small_misc.h"

/* Returns the first of the 32 elements at 0x3193c0 whose word at +0x90 is
 * zero, after passing it to SzDuel_ClearDrawCard; 0 when none is. */
struct DotrMiscNode *SzDuel_GetCardRequest(void)
{
    struct DotrMiscNode *p = (struct DotrMiscNode *)gSzDuel_aCardRequest;
    int i;
    for (i = 0; i < 32; i++) {
        if (p->unknown_90 == 0) {
            SzDuel_ClearDrawCard(p);
            return p;
        }
        p = (struct DotrMiscNode *)((char *)p + 160);
    }
    return 0;
}
