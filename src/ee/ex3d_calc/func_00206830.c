#include "dotr/ee/ex3d_calc.h"

/* 0x206830. An inline helper computes the plane through three vertices. */

static inline void surface_00206830(float *v0, float *v1, float *v2, float *plane) {
    plane[0] = v0[1] * (v1[2] - v2[2])
             + v1[1] * (v2[2] - v0[2])
             + v2[1] * (v0[2] - v1[2]);
    plane[1] = v0[2] * (v1[0] - v2[0])
             + v1[2] * (v2[0] - v0[0])
             + v2[2] * (v0[0] - v1[0]);
    plane[2] = v0[0] * (v1[1] - v2[1])
             + v1[0] * (v2[1] - v0[1])
             + v2[0] * (v0[1] - v1[1]);
    plane[3] = v0[0] * (v1[2] * v2[1] - v1[1] * v2[2])
             + v0[1] * (v1[0] * v2[2] - v1[2] * v2[0])
             + v0[2] * (v1[1] * v2[0] - v1[0] * v2[1]);
    plane[4] = func_001a6c60(plane[0] * plane[0]
                            + plane[1] * plane[1]
                            + plane[2] * plane[2]);
}

void Ex3d_InitClipPlane(void)
{
    float near;
    float far;
    float screenCenterX;
    float screenCenterY;
    float screenDistance;
    float v[5][3];
    float nw;
    float w;
    float f;
    float *planes = (float *)data_00325cc0_ex3dcalc;
    float *p;

    RhView_GetClipZ(&near, &far);
    RhView_GetScreenCenter(&screenCenterX, &screenCenterY);
    RhView_GetScreenDist(&screenDistance);
    screenCenterX += 10.0f;
    screenCenterY += 10.0f;
    nw = -screenCenterX;
    w = screenCenterX;
    f = screenDistance;
    v[0][0] = 0.0f; v[0][1] = 0.0f; v[0][2] = 0.0f;
    v[1][0] = nw; v[1][1] = nw;
    v[2][1] = nw;
    v[4][0] = nw;
    v[2][0] = v[3][0] = v[3][1] = v[4][1] = w;
    v[1][2] = v[2][2] = v[3][2] = v[4][2] = f;
    p = (float *)data_00325cc0_ex3dcalc;
    surface_00206830(v[0], v[1], v[2], p);
    p = planes + 5;
    surface_00206830(v[0], v[4], v[1], p);
    p = planes + 10;
    surface_00206830(v[0], v[3], v[4], p);
    p = planes + 15;
    surface_00206830(v[0], v[2], v[3], p);
    v[1][2] = near; v[2][2] = near; v[4][2] = near;
    p = planes + 20;
    surface_00206830(v[1], v[2], v[4], p);
    v[1][2] = far; v[2][2] = far; v[4][2] = far;
    p = planes + 25;
    surface_00206830(v[4], v[2], v[1], p);
}
