#include "dotr/ee/fx_calc.h"

int ShFx_isClipSC(unsigned short *pos)
{
    if ((float)pos[0] < 27648.0f) {
        return 0;
    }
    if ((float)pos[0] > 37888.0f) {
        return 0;
    }
    if ((float)pos[1] < 29184.0f) {
        return 0;
    }
    if ((float)pos[1] > 36352.0f) {
        return 0;
    }
    return 1;
}
