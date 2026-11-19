#include "dotr/ee/small_state.h"

/* The cast at the store reproduces the sign extension before the mask. */
void SzPad_SetParam(int index, int value, int nibble)
{
    gSzPad_pl.unknown_200[index].unknown_59 = value;
    gSzPad_pl.unknown_200[index].unknown_5a.unknown_bits_0_3 = (signed char)nibble;
}
