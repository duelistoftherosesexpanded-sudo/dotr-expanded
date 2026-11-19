#include "dotr/ee/small_state.h"

int SzPad_IsAnalog(int index)
{
    return gSzPad_pl.unknown_200[index].unknown_5b.unknown_bit_6 ? 1 : 0;
}
