#include "dotr/ee/block_table.h"

int SzView_SetPointLight(int index, int k, union DotrBlockMatrix48 *pointLightParameters)
{
    struct DotrBlock *p;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return 0;
    }
    if (k < 0 || k >= 10) {
        return 0;
    }
    if (pointLightParameters != 0) {
        if (pointLightParameters != (union DotrBlockMatrix48 *)1) {
            p->unknown_600[k] = *pointLightParameters;
            p->unknown_600[k].unknown_vectors[1].unknown_w = 1.0f;
        }
        p->lightingFlags |= 1 << k;
    } else {
        p->lightingFlags &= ~(1 << k);
    }
    return 1;
}
