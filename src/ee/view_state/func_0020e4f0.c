#include "dotr/ee/view_state.h"

int RhView_PopStack(void)
{
    int v = gRhView_nStackIndex[0];
    if (v <= 0) {
        return 0;
    }
    gRhView_nStackIndex[0] = v - 1;
    return 1;
}
