#include "dotr/ee/spline_paths.h"

/* Natural cubic spline through n points: per coordinate a tridiagonal system
 * solved by tri_diag, then per segment the cubic coefficients and samples written
 * to x[m], y[m]; *count = m - 1 after every segment.
 * MATCHING DEVICE: k is reused for the component loops and the sample loops,
 * num is a short, and m starts in the outer for header. */
void Taku_spline2(float *x, float *y, short *count, float (*pts)[2], float *h, int n, short e, float scale)
{
    float lower[64];
    float diag[64];
    float upper[64];
    float sol[2][64];
    float right[2][64];
    float a[2];
    float b[2];
    float c[2];
    float d[2];
    int i;
    int k;
    int m;
    short num;
    float t;
    float dx;
    float dy;

    diag[0] = 2.0f * h[0];
    upper[0] = h[0];
    lower[n - 1] = h[n - 2];
    diag[n - 1] = 2.0f * h[n - 2];
    for (i = 1; i <= n - 2; i++) {
        lower[i] = h[i];
        diag[i] = 2.0f * (h[i - 1] + h[i]);
        upper[i] = h[i - 1];
    }
    for (k = 0; k <= 1; k++) {
        right[k][0] = 3.0f * (pts[1][k] - pts[0][k]);
        right[k][n - 1] = 3.0f * (pts[n - 1][k] - pts[n - 2][k]);
        for (i = 1; i <= n - 2; i++) {
            right[k][i] = func(pts[i - 1][k], pts[i][k], pts[i + 1][k], h[i - 1], h[i]);
        }
        tri_diag(lower, diag, upper, right[k], sol[k], n);
    }
    for (i = 0, m = 0; i < n - 1; i++) {
        for (k = 0; k <= 1; k++) {
            a[k] = (sol[k][i] + sol[k][i + 1]) / (h[i] * h[i])
                 + 2.0f * (pts[i][k] - pts[i + 1][k]) / func_001a6c48_spline(h[i], 3.0f);
            b[k] = 3.0f * (pts[i + 1][k] - pts[i][k]) / (h[i] * h[i]) - (2.0f * sol[k][i] + sol[k][i + 1]) / h[i];
            c[k] = sol[k][i];
            d[k] = pts[i][k];
        }
        if (e == 1) {
            if (i < n - 2) {
                for (t = 0.0f; t <= h[i] - 0.1f; t += 0.1f) {
                    x[m] = d[0] + t * (c[0] + t * (b[0] + a[0] * t));
                    y[m] = d[1] + t * (c[1] + t * (b[1] + a[1] * t));
                    m++;
                }
            } else {
                for (t = 0.0f; t <= h[i]; t += 0.1f) {
                    x[m] = d[0] + t * (c[0] + t * (b[0] + a[0] * t));
                    y[m] = d[1] + t * (c[1] + t * (b[1] + a[1] * t));
                    m++;
                }
            }
        } else {
            dx = pts[i + 1][0] - pts[i][0];
            dy = pts[i + 1][1] - pts[i][1];
            num = func_001a22c0(func_001a6c60(func_001a6c48_spline(dx, 2.0f) + func_001a6c48_spline(dy, 2.0f)) / scale);
            if (i < n - 2) {
                for (k = 0; k < num; k++, m++) {
                    t = (float)k * h[i] / (float)num;
                    x[m] = d[0] + t * (c[0] + t * (b[0] + a[0] * t));
                    y[m] = d[1] + t * (c[1] + t * (b[1] + a[1] * t));
                }
            } else {
                for (k = 0; k <= num; k++, m++) {
                    t = (float)k * h[i] / (float)num;
                    x[m] = d[0] + t * (c[0] + t * (b[0] + a[0] * t));
                    y[m] = d[1] + t * (c[1] + t * (b[1] + a[1] * t));
                }
            }
        }
        *count = m - 1;
    }
}
