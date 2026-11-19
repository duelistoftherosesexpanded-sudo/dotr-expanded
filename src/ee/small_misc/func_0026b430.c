#include "dotr/ee/small_misc.h"

/* Writes {102, 1, Ta_IsActiveTrapB(0, 46), 0} and returns 1. */
int Ta_DoUncTrapB(int *out)
{
    out[0] = 102;
    out[1] = 1;
    out[2] = Ta_IsActiveTrapB(0, 46);
    out[3] = 0;
    return 1;
}
