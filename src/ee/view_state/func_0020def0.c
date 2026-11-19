#include "dotr/ee/view_state.h"

int RhView_GetCurView(struct DotrViewBlock *out)
{
    *out = gRhView_avl[gRhView_nStackIndex[0]].unknown_0c0;
    return 1;
}
