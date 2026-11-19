#include "dotr/ee/small_state.h"

void SzPad_SetAnalogInfo(int index, int slot, int value)
{
    struct DotrSmallEntry *e = &gSzPad_pl.unknown_200[index];
    e->unknown_06[slot] = value;
}
