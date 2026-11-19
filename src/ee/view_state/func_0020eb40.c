#include "dotr/ee/view_state.h"

void RhView_GetWorldScreenMatrix(struct DotrViewMatrix *out)
{
    *out = gRhView_avl[gRhView_nStackIndex[0]].unknown_000;
}
