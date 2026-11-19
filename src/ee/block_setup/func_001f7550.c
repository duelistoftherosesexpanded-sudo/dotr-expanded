#include "dotr/ee/block_setup.h"

int lSzView_InitVars(int index)
{
    struct DotrBlock *p;
    struct DotrBlockVector4 *v;
    float *f;
    struct DotrBlockElement *e;
    int *angles;
    struct DotrBlockPair *a;
    struct DotrBlockPair *b;
    int i;

    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return 0;
    }
    /* Taken here, before the stores: the two addresses, not the block pointer,
     * are what survives the calls below. */
    a = &p->unknown_7e0[9];
    b = &p->unknown_830[9];
    v = (struct DotrBlockVector4 *)&p->unknown_000[0x20];
    v->x = 0.0f;
    v->y = 50.0f;
    v->z = 300.0f;
    v->unknown_w = 1.0f;
    v = (struct DotrBlockVector4 *)p->cameraPoint;
    v->x = 0.0f;
    v->y = 50.0f;
    v->z = 0.0f;
    v->unknown_w = 1.0f;
    p->unknown_040 = 0;
    p->screenDistance = 800.0f;
    f = (float *)p->unknown_048;
    f[0] = 1.0f;
    f[1] = 0.94f;
    f[2] = 1.0f;
    f[3] = 4096.0f;
    e = &p->modelViewData[9];
    e->unknown_00[0].unknown_00[0].y = 0.5f;
    e->unknown_00[0].unknown_00[0].z = 0.5f;
    e->unknown_00[0].unknown_00[0].unknown_w = 0.5f;
    e->unknown_00[0].unknown_00[0].x = 1.0f;
    e->unknown_00[1].unknown_00[0].y = 0.0f;
    e->unknown_00[1].unknown_00[0].z = 0.0f;
    e->unknown_00[1].unknown_00[0].unknown_w = 0.0f;
    e->unknown_00[1].unknown_00[0].x = 1.0f;
    e->unknown_00[2].unknown_00[0].y = 0.0f;
    e->unknown_00[2].unknown_00[0].z = 0.0f;
    e->unknown_00[2].unknown_00[0].unknown_w = 0.0f;
    e->unknown_00[2].unknown_00[0].x = 1.0f;
    angles = (int *)&e->unknown_00[0].unknown_00[1];
    angles[0] = -14336;
    angles[1] = 2048;
    angles[2] = 0;
    angles[3] = 0;
    angles = (int *)&e->unknown_00[1].unknown_00[1];
    angles[0] = 0;
    angles[1] = 0;
    angles[2] = 0;
    angles[3] = 0;
    angles = (int *)&e->unknown_00[2].unknown_00[1];
    angles[0] = 0;
    angles[1] = 0;
    angles[2] = 0;
    angles[3] = 0;
    p->unknown_8a8 = 512;
    SzView_SetStraightLightDirection(0, 9, 0, 0);
    SzView_SetStraightLightDirection(0, 9, 1, 0);
    SzView_SetStraightLightDirection(0, 9, 2, 0);
    a->x = 2000.0f;
    a->y = 4000.0f;
    b->x = 2000.0f;
    b->y = 4000.0f;
    for (i = 0; i < 10; i++) {
        SzView_SetFogColor(index, i, 0);
    }
    lSzView_CalcRel(index);
    lSzView_CalcLook(index);
    return 1;
}
