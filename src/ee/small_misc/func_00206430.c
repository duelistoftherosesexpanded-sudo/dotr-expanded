#include "dotr/ee/small_misc.h"

/* Intersects the segment p1..p2 with a plane; out receives the point with w = 1. */
void Ex3d_IntersectP(float *plane, float *p1, float *p2, float *out)
{
    float num = -plane[3] - (plane[0] * p1[0] + plane[1] * p1[1] + plane[2] * p1[2]);
    float den = plane[0] * (p2[0] - p1[0]) + plane[1] * (p2[1] - p1[1]) + plane[2] * (p2[2] - p1[2]);
    float t;
    float a;
    if (den >= 0.0f) {
        a = den;
    } else {
        a = -den;
    }
    if (a < 1.0e-6f) {
        den = 1.0e-6f;
    }
    t = num / den;
    out[0] = p1[0] + t * (p2[0] - p1[0]);
    out[1] = p1[1] + t * (p2[1] - p1[1]);
    out[2] = p1[2] + t * (p2[2] - p1[2]);
    out[3] = 1.0f;
}
