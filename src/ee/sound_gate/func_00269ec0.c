#include "dotr/ee/sound_gate.h"

void Ta_Eff_036_ManageSound(unsigned int *p, struct DotrSoundStep *step)
{
    int n;
    int limit;

    switch (p[3] >> 14) {
    case 536:
        limit = 64;
        break;
    case 540:
        limit = 66;
        break;
    case 541:
        limit = 64;
        break;
    case 542:
        limit = 72;
        break;
    default:
        limit = 48;
        break;
    }
    if (p[3] == 999) {
        n = step->count_d0++;
    switch (n) {
    case 0:
        SzSound_PlaySE(1, 56);
        break;
    case 20:
        SzSound_PlaySE(1, 57);
        break;
    case 30:
        SzSound_PlaySE(1, 76);
        break;
    case 36:
        Ta_Eff_036_GetSound(p);
        break;
    default:
        if (step->count_d0 == limit) {
            SzSound_PlaySE(1, 60);
        }
        break;
    }
        return;
    }
    n = step->count_d0++;
switch (n) {
case 0:
    SzSound_PlaySE(1, 56);
    break;
case 20:
    SzSound_PlaySE(1, 57);
    break;
case 30:
    SzSound_PlaySE(1, 76);
    break;
case 36:
    Ta_Eff_036_GetSound(p);
    break;
case 37:
    if (p[3] >> 14 == 541 || p[3] >> 14 == 542) {
        SzSound_PlaySE(1, 59);
    }
    break;
default:
    if (step->count_d0 == limit) {
        SzSound_PlaySE(1, 60);
    }
    break;
}
}
