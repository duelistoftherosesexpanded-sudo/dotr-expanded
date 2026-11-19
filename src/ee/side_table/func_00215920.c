#include "dotr/ee/side_table.h"

int SzData_SetCurrentDeck(int side, int deckNo)
{
    struct DotrSide *p = DOTR_SIDE(side);
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (deckNo < 0 || deckNo >= 3) {
        return 0;
    }
    p->flags.unknown_bits_0_1 = deckNo;
    return 1;
}
