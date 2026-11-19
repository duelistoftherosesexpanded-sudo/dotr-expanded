#include "dotr/ee/view_state.h"

void RhView_GetWorldViewMatrix(struct DotrViewMatrix *out)
{
    *out = gRhView_avl[gRhView_nStackIndex[0]].unknown_040;
}
