#include "dotr/ee/fx_calc.h"

int ShFx_isClip(unsigned short *pos)
{
    if ((float)pos[0] < 22528.0f) {
        return 0;
    }
    if ((float)pos[0] > 43008.0f) {
        return 0;
    }
    if ((float)pos[1] < 25600.0f) {
        return 0;
    }
    if ((float)pos[1] > 39936.0f) {
        return 0;
    }
    return 1;
}
