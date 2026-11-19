#include "dotr/ee/state_queries.h"

/* While *p is below 480: adds 80 to it and calls
 * SzDuel_OpenMsgPopup(1, 0, 480 - *p, func_0027fda0_ptr(a)), returning 0. Returns 1
 * once *p has reached 480. */
int TaTuto_OpenMsgPopup(void *a, unsigned short *p)
{
    if (*p < 480) {
        int v = func_0027fda0_ptr(a);
        SzDuel_OpenMsgPopup(1, 0, 480 - (*p += 80), v);
    } else {
        return 1;
    }
    return 0;
}
