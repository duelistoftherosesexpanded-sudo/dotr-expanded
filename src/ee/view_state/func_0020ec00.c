#include "dotr/ee/view_state.h"

void RhView_GetViewScreenMatrix(struct DotrViewMatrix *out)
{
    *out = gRhView_avl[gRhView_nStackIndex[0]].unknown_080;
}
