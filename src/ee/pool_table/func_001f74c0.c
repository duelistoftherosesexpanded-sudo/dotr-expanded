#include "dotr/ee/pool_table.h"

int SzView_Create(int viewIndex)
{
    int *p;
    if (viewIndex < 0 || viewIndex >= 1) {
        return 0;
    }
    p = SzMem_Alloc(2240);
    if (p == 0) {
        return 0;
    }
    gSzView_hView[viewIndex] = p;
    RhMem_FillMemory4(p, 0, 560);
    lSzView_InitVars(viewIndex);
    return 1;
}
