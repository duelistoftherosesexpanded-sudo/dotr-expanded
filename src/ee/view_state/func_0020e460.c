#include "dotr/ee/view_state.h"

int RhView_PushStack(void)
{
    int depth = gRhView_nStackIndex[0];
    if (!(depth + 1 < 2)) {
        return 0;
    }
    gRhView_avl[depth + 1] = gRhView_avl[depth];
    gRhView_nStackIndex[0]++;
    return 1;
}
