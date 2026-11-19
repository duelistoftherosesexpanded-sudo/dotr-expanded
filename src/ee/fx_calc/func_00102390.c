#include "dotr/ee/fx_calc.h"

void ShFx_Spline_MakeTable(int n, float *x, float *y, float *z)
{
    int i;
    float t;
    float h[8];
    float d[8];

    z[n - 1] = 0.0f;
    z[0] = 0.0f;
    for (i = 0; i < n - 1; i++) {
        h[i] = x[i + 1] - x[i];
        d[i + 1] = (y[i + 1] - y[i]) / h[i];
    }
    z[1] = d[2] - d[1] - h[0] * z[0];
    d[1] = 2.0f * (x[2] - x[0]);
    for (i = 1; i < n - 2; i++) {
        t = h[i] / d[i];
        z[i + 1] = d[i + 2] - d[i + 1] - z[i] * t;
        d[i + 1] = 2.0f * (x[i + 2] - x[i]) - h[i] * t;
    }
    z[n - 2] -= h[n - 2] * z[n - 1];
    for (i = n - 2; i > 0; i--) {
        z[i] = (z[i] - h[i] * z[i + 1]) / d[i];
    }
}
