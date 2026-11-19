#include "dotr/ee/view_state.h"

int RhView_PopFakeMirror(struct DotrViewBlock *out)
{
    struct DotrViewControl *p = gRhView_FakeMirror;
    if (gRhView_FakeMirror[0].unknown_44 == 0) {
        return 0;
    }
    if (gRhView_nStackIndex[0] > 0) {
        gRhView_nStackIndex[0]--;
    }
    p->unknown_44 = 0;
    *out = gRhView_avl[0].unknown_0c0;
    return 1;
}
