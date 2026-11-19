#include "dotr/ee/small_misc.h"

int SzFade_Destroy(int index)
{
    /* Signed one-bit field: -1 is the set state. */
    gSzFade_fl[index].unknown_07_bit_6 = -1;
    return 1;
}
