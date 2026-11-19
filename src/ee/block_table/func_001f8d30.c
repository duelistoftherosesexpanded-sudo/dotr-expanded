#include "dotr/ee/block_table.h"

struct DotrDrawTag *SzView_SetLight_Pipe(struct DotrDrawTag *tag, int index, int k)
{
    /* Declaration order is the saved-register order of the original (s0..s4). */
    struct DotrBlockElement *light;
    union DotrBlockMatrix48 *point;
    int light_k;
    int point_k;
    struct DotrBlock *p;
    struct DotrBlockVector4 v;

    if (index < 0 || index >= 1) {
        return tag;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return tag;
    }
    if (k < 0 || k >= 10) {
        return tag;
    }
    light = &p->modelViewData[k];
    point = &p->unknown_600[k];
    point_k = k;
    light_k = k;
    if (k != 9) {
        if (!(p->unknown_8a8 & (1 << k))) {
            light = &p->modelViewData[9];
            light_k = 9;
        }
        if (!(p->lightingFlags & (1 << k)) || (p->unknown_8ae & (1 << k))) {
            point = &p->unknown_600[9];
            point_k = 9;
        }
    }
    RhVu1Mac_SetStraightLightToVu1_Ex(0, 0, 0, -1);
    if (p->unknown_8a8 & (1 << light_k)) {
        light->unknown_00[0].unknown_00[2].unknown_w = 0;
        RhGeom_MulMatrixVector((struct DotrViewVector *)&v, 0, (struct DotrViewVector *)&light->unknown_00[0].unknown_00[2]);
        RhVu1Mac_SetStraightLightToVu1_Ex(0, &light->unknown_00[0], &v, 0);
        light->unknown_00[1].unknown_00[2].unknown_w = 0;
        RhGeom_MulMatrixVector((struct DotrViewVector *)&v, 0, (struct DotrViewVector *)&light->unknown_00[1].unknown_00[2]);
        RhVu1Mac_SetStraightLightToVu1_Ex(0, &light->unknown_00[1], &v, 1);
        light->unknown_00[2].unknown_00[2].unknown_w = 0;
        RhGeom_MulMatrixVector((struct DotrViewVector *)&v, 0, (struct DotrViewVector *)&light->unknown_00[2].unknown_00[2]);
        RhVu1Mac_SetStraightLightToVu1_Ex(0, &light->unknown_00[2], &v, 2);
    }
    tag = RhVu1Mac_SetStraightLightToVu1_Ex(tag, 0, 0, -1);
    if (p->lightingFlags & (1 << point_k)) {
        RhGeom_MulMatrixVector((struct DotrViewVector *)&v, 0, (struct DotrViewVector *)&point->unknown_vectors[1]);
        v.unknown_w = 1.0f;
        return RhVu1Mac_SetPointLightToVu1(tag, &point->unknown_vectors[0].x, &v.x, point->unknown_vectors[2].x,
                                           point->unknown_vectors[2].y, point->unknown_vectors[2].z);
    }
    return RhVu1Mac_SetPointLightToVu1(tag, 0, 0, 0.0f, 0.0f, 0.0f);
}
