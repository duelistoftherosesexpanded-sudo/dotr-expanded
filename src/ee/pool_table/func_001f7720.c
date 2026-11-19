#include "dotr/ee/pool_table.h"

int SzView_Destroy(int viewId)
{
    int **slot;
    if (viewId < 0 || viewId >= 1) {
        return 0;
    }
    slot = &gSzView_hView[viewId];
    if (*slot == 0) {
        return 0;
    }
    SzMem_Free(*slot);
    *slot = 0;
    return 1;
}
