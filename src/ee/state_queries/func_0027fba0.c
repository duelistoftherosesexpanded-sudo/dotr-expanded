#include "dotr/ee/state_queries.h"

/* While *p is above 0: subtracts 80 from it and calls
 * SzDuel_OpenMsgPopup(1, 0, 480 - *p, func_0027fda0_ptr(a)), returning 0. Otherwise
 * calls SzDuel_CloseMsgPopup and returns 1. */
int TaTuto_CloseMsgPopup(void *a, unsigned short *p)
{
    if (*p > 0) {
        int v = func_0027fda0_ptr(a);
        SzDuel_OpenMsgPopup(1, 0, 480 - (*p -= 80), v);
    } else {
        SzDuel_CloseMsgPopup();
        return 1;
    }
    return 0;
}
