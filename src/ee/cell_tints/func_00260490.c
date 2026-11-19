#include "dotr/ee/cell_tints.h"

#define DOTR_TINT_REACH(reach) ((reach) == 1 ? 2 : 1)

void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int visualType)
{
    unsigned char colour[4];
    int i;
    int j;
    int x0;
    int x1;
    int y0;
    int y1;
    unsigned char *p = colour;

    if (visualType > 0) {
        switch (visualType) {
        case 16:
            p[2] = 0;
            p[1] = 255;
            p[0] = 0;
            p[3] = 128;
            break;
        case 15:
            p[2] = 255;
            p[1] = 0;
            p[0] = 128;
            p[3] = 128;
            break;
        case 13:
            p[2] = 0;
            p[1] = 128;
            p[0] = 255;
            p[3] = 128;
            break;
        case 11:
            p[2] = 0;
            p[1] = 255;
            p[0] = 255;
            p[3] = 128;
            break;
        default:
            p[2] = 255;
            p[1] = 255;
            p[0] = 255;
            p[3] = 128;
            break;
        }
    } else {
        p[2] = 0;
        p[1] = 0;
        p[0] = 0;
        p[3] = 255;
    }
    if (mode == 18) {
        for (i = 0; i < 7; i++) {
            func_001e3060_tint(i, y, c, p, e);
            func_001e3060_tint(x, i, c, p, e);
        }
        return;
    }
    x0 = GetCap(0, x - DOTR_TINT_REACH(reach));
    x1 = x + DOTR_TINT_REACH(reach) >= 7 ? 6 : x + DOTR_TINT_REACH(reach);
    y0 = GetCap(0, y - DOTR_TINT_REACH(reach));
    y1 = y + DOTR_TINT_REACH(reach) >= 7 ? 6 : y + DOTR_TINT_REACH(reach);
    for (i = x0; i <= x1; i++) {
        for (j = y0; j <= y1; j++) {
            func_001e3060_tint(i, j, c, p, e);
        }
    }
}
