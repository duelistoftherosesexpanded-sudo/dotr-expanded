#include "dotr/ee/fx_calc.h"

void ShFx_GetAlpha(struct DotrFxAlphaSource *source, unsigned char *out)
{
    switch (source->mode) {
    case 1:
        out[0] = 0;
        out[1] = 1;
        out[2] = 0;
        out[3] = 1;
        break;
    case 2:
        out[0] = 0;
        out[1] = 2;
        out[2] = 0;
        out[3] = 1;
        break;
    case 3:
        out[0] = 1;
        out[1] = 0;
        out[2] = 0;
        out[3] = 1;
        break;
    case 4:
        out[0] = 2;
        out[1] = 0;
        out[2] = 0;
        out[3] = 1;
        break;
    }
}
