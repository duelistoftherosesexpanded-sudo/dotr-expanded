#include "dotr/ee/fx_calc.h"
#include "dotr/ee/view_state.h"

int GJudge_FrontBack2(unsigned short *a, unsigned short *b, unsigned short *c)
{
    struct DotrViewVector normal;
    struct DotrViewVector pts[3];
    int front = 1;

    pts[0].x = (float)a[0];
    pts[0].y = (float)a[1];
    pts[0].z = 0.0f;
    pts[0].unknown_w = 1.0f;
    pts[1].x = (float)b[0];
    pts[1].y = (float)b[1];
    pts[1].z = 0.0f;
    pts[1].unknown_w = 1.0f;
    pts[2].x = (float)c[0];
    pts[2].y = (float)c[1];
    pts[2].z = 0.0f;
    pts[2].unknown_w = 1.0f;
    Ex3d_GetSurfaceNormal(3, pts, &normal);
    if (normal.z >= 0.0f) {
        front = 0;
    }
    return front;
}
