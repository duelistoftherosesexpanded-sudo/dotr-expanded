#include "dotr/ee/shoki_obj.h"

struct DotrShokiObj *SHOKI_OBJ(struct DotrShokiWork *w, int unused, int count, int skip)
{
    int i;
    (void)unused; /* never read; the statement only quiets the host compiler */
    for (i = 0; i < count; i++) {
        if (skip == 0) {
            if (w->obj[i].id == 0) {
                func_00195108(&w->obj[i], 192);
                w->obj[i].id = i + 1;
                return &w->obj[i];
            }
        }
    }
    return 0;
}
