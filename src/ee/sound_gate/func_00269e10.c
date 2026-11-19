#include "dotr/ee/sound_gate.h"

int Ta_Eff_036_GetSound(unsigned int *p)
{
    switch (p[3] >> 14) {
    case 536:
        return SzSound_PlaySE(1, 59);
    case 540:
        return SzSound_PlaySE(1, 75);
    case 541:
        return SzSound_PlaySE(1, 78);
    case 542:
        return SzSound_PlaySE(1, 79);
    default:
        return SzSound_PlaySE(1, 74);
    }
}
