#include "dotr/ee/small_misc.h"

void lRhModel_ChangeL2VtoL2S(unsigned char *base, int arg, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        RhGeom_MulMatrixRev(base + i * 112, arg);
    }
}
