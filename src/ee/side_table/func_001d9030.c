#include "dotr/ee/view_state.h"

/* Returns 0 for a null object. Otherwise converts (a, b) and (c, d) to
 * floats through Ex3d_GetPointOnScreen into the vectors at +0x10 and +0x20, stores
 * the two halfwords at +0x9a and +0x98, calls SzDuel_DoCardControl0 and returns 1. */
int lSzDuel_SetCardCtl2(struct DotrViewRect *p, int a, int b, int c, int d, short e, short f)
{
    if (p == 0) {
        return 0;
    }
    Ex3d_GetPointOnScreen(&p->unknown_10, a, b);
    Ex3d_GetPointOnScreen(&p->unknown_20, c, d);
    p->unknown_98 = f;
    p->unknown_9a = e;
    func_001cdeb0_view(p);
    return 1;
}
