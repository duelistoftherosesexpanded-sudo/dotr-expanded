#include "dotr/ee/glide_steps.h"

void Taku_Screen_Init(void)
{
    short i;
    struct DotrGlideVector3 v;
    struct DotrGlideView *b;

    Taku_Coord_Set(&prim_data_glide);
    DOTR_GLIDE_VIEW->to.x = data_00298ba0_glide[data_00299e10_glide.kind[0]].x;
    DOTR_GLIDE_VIEW->to.y = 0.0f;
    DOTR_GLIDE_VIEW->to.z = data_00298ba0_glide[data_00299e10_glide.kind[0]].y;
    DOTR_GLIDE_VIEW->to.x = data_00298ba0_glide[data_00299e10_glide.kind[1]].x;
    DOTR_GLIDE_VIEW->to.y = 0.0f;
    DOTR_GLIDE_VIEW->to.z = data_00298ba0_glide[data_00299e10_glide.kind[1]].y;
    DOTR_GLIDE_VIEW->from.x = DOTR_GLIDE_VIEW->to.x;
    DOTR_GLIDE_VIEW->from.y = 96.0f + DOTR_GLIDE_VIEW->to.y;
    DOTR_GLIDE_VIEW->from.z = 96.0f + DOTR_GLIDE_VIEW->to.z;
    DOTR_GLIDE_VIEW->from.w = DOTR_GLIDE_VIEW->to.w;
    SzView_SetCamera(0, &DOTR_GLIDE_VIEW->from, &DOTR_GLIDE_VIEW->to);
    b = DOTR_GLIDE_VIEW;
    for (i = 0; i <= 21; i++) {
        v.x = data_00298ba0_glide[i].x;
        v.y = 8.65f + data_00298ba0_glide[i].unknown_04;
        v.z = data_00298ba0_glide[i].y;
        Taku_3D_2D_Set(&b->unknown_8c[i], &b->unknown_10c[i], &v);
    }
    DOTR_GLIDE_VIEW->width = 320;
    DOTR_GLIDE_VIEW->height = 190;
}
