#include "dotr/ee/fx_calc.h"

void GSet_WindowUv(struct DotrWindowUv *out, unsigned short *xy, int dx, int dy)
{
    int x;
    int y;

    x = (xy[0] >> 4) - 1728 + dx;
    y = (xy[1] >> 4) - 1824 + dy;
    if (x <= 0) {
        x = 1;
    } else if (x > 639) {
        x = 639;
    }
    if (y <= 0) {
        y = 1;
    } else if (y > 447) {
        y = 447;
    }
    out->u = x << 4;
    out->v = y << 4;
}
