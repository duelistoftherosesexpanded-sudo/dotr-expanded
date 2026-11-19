#include "dotr/ee/small_misc.h"

int RhBase_Term(void)
{
    RhView_Term();
    RhFile_Term();
    RhGeom_Term();
    RhMemCard_Term();
    return 1;
}
