#include "dotr/ee/cell_forwards.h"

int SzEffect_ReqMotionBlur2(int packed, int a, int b, struct DotrObjectQuad *c, struct DotrObjectQuad *d, int e)
{
    int x = packed >> 8;
    int y = packed & 0xff;

    if (x < 0 || x >= 3 || y < 0 || y >= 11) {
        return 0;
    }
    packed = SzDuel_GetUnitModelID(x, y);
    if (packed < 0) {
        return 0;
    }
    return SzModel_ReqMotionBlur(packed, a, b, c, d, e);
}
