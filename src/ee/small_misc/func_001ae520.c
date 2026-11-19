#include "dotr/ee/small_misc.h"

/* Sets the four bytes at 0x317490 + 0xcb2 to 24, then calls
 * SzDuel_DelUnit(0, i) and SzDuel_DelUnit(1, i) for i = 1..10. */
void SzDuel_Term(void)
{
    struct DotrMiscControlCb2 *g = &data_00317490_cb2;
    int i;
    for (i = 0; i < 4; i++) {
        g->unknown_cb2[i] = 24;
    }
    for (i = 1; i < 11; i++) {
        SzDuel_DelUnit(0, i);
        SzDuel_DelUnit(1, i);
    }
}
