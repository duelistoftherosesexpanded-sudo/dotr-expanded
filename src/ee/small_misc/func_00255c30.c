#include "dotr/ee/small_misc.h"


int TaEffect_IsLoadModule2(struct DotrJobIds *job, int id)
{
    int i;
    struct DotrJobPool *e;
    int state;
    if (job == 0) {
        return 0;
    }
    for (i = 0; i < 24; i++) {
        if (id == job->ids[i]) {
            e = SzEffect_GetModuleInfo(0, job->ids[i]);
            if (e) {
                state = e->state;
                if (state < 2) {
                    return state == 0 ? 0 : -1;
                }
            }
            return 1;
        }
    }
    return 0;
}
