#include "dotr/ee/object_table.h"

int SzModel_Init(void)
{
    int i;
    for (i = 0; i < 84; i++) {
        gSzModel_ahModel[i] = 0;
    }
    for (i = 0; i < 32; i++) {
        gSzModel_aBlur[i].unknown_68_bits_0_26 = -1;
    }
    gSzModel_nBlurMode[0] = 0;
    if (func_00197548(data_002cf9d0) != 0 || func_00197548(data_002cf9d5) != 0) {
        gSzModel_fPacking[0] = 1;
    }
    return 1;
}
