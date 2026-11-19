#include "dotr/ee/block_table.h"

int SzView_SetStraightLightDirection(int index, int k, int m, struct DotrBlockVector4 *in)
{
    struct DotrBlockMatrix64 matrix;
    struct DotrBlock *p;
    struct DotrBlockElement *e;
    struct DotrBlockTriple *t;
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
    if (m < 0 || m >= 3) {
        return 0;
    }
    e = &p->modelViewData[k];
    if (in != 0) {
        e->unknown_00[m].unknown_00[1] = *in;
    }
    t = &e->unknown_00[m];
    RhGeom_GetRotXYZMatrix((float *)&matrix, (int *)&t->unknown_00[1]);
    RhGeom_MulMatrixVector((struct DotrViewVector *)&t->unknown_00[2], (struct DotrViewMatrix *)&matrix, (struct DotrViewVector *)gSzView_vecUnit);
    return 1;
}
