#include "dotr/ee/small_state.h"

int SzPad_GetAnalogInfo(int index, int slot)
{
    struct DotrSmallEntry *e = &gSzPad_pl.unknown_200[index];
    return e->unknown_06[slot];
}
