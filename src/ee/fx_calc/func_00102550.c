#include "dotr/ee/fx_calc.h"
#include "dotr/ee/view_state.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* ShFx_Spline: for a block of 3..8 control points (else returns 0), parameterises them by
 * cumulative chord length (func_001a6c60, square root) normalised to 0..1, builds the natural
 * cubic spline tables of x, y and z with ShFx_Spline_MakeTable and writes (count - 1) * 4
 * samples at u = 0, step 1 / (4 * (count - 1)), ... into the output points; returns 1.
 * Compiled warm (the bisection midpoint is a float-to-int cast). An inline helper samples the
 * spline. */

static inline float Sample_ShFxSpline(int n, float *t, float *p, float *m, float u) {
    int lo;
    int hi;
    int mid;
    float d;
    float h;

    lo = 0;
    hi = n - 1;
    while (lo < hi) {
        mid = (int)((lo + hi) / 2.0f);
        if (t[mid] < u) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    if (lo > 0) {
        lo--;
    }
    h = t[lo + 1] - t[lo];
    d = u - t[lo];
    return p[lo] + d * ((p[lo + 1] - p[lo]) / h - h * (2.0f * m[lo] + m[lo + 1]) +
                        d * (3.0f * m[lo] + d * (m[lo + 1] - m[lo]) / h));
}

int ShFx_Spline(void *arg0) {
    struct DotrFxSpline *s = arg0;
    float t[8];
    float mx[8];
    float my[8];
    float mz[8];
    int i;
    float dx;
    float dy;
    float dz;
    float u;
    float step;

    if (s->count < 3 || s->count > 8) {
        return 0;
    }
    t[0] = 0.0f;
    for (i = 1; i < s->count; i++) {
        dx = s->x[i] - s->x[i - 1];
        dy = s->y[i] - s->y[i - 1];
        dz = s->z[i] - s->z[i - 1];
        t[i] = t[i - 1] + func_001a6c60(dx * dx + dy * dy + dz * dz);
    }
    for (i = 1; i < s->count; i++) {
        t[i] /= t[s->count - 1];
    }
    ShFx_Spline_MakeTable(s->count, t, s->x, mx);
    ShFx_Spline_MakeTable(s->count, t, s->y, my);
    ShFx_Spline_MakeTable(s->count, t, s->z, mz);
    step = 1.0f / (4.0f * (float)(s->count - 1));
    u = 0.0f;
    for (i = 0; i < (s->count - 1) * 4; i++) {
        s->out[i].x = Sample_ShFxSpline(s->count, t, s->x, mx, u);
        s->out[i].y = Sample_ShFxSpline(s->count, t, s->y, my, u);
        s->out[i].z = Sample_ShFxSpline(s->count, t, s->z, mz, u);
        u += step;
    }
    return 1;
}
