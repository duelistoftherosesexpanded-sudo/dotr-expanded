#include "dotr/ee/fx_calc.h"

float SIN_COS_INRAD(float *tbl, short deg, int mode)
{
    float r;
    short t;

    if (mode == 0) {
        if (deg >= 360) {
            deg %= 360;
        }
        if (deg < -359) {
            deg %= 360;
        }
        if (deg < 0) {
            deg += 360;
        }
        if (deg < 90) {
            tbl += deg;
            r = *tbl;
        } else if (deg < 180) {
            t = deg - 90;
            tbl += 90 - t;
            r = *tbl;
        } else if (deg < 270) {
            t = deg - 180;
            tbl += t;
            r = *tbl;
            r *= -1.0f;
        } else {
            t = deg - 270;
            tbl += 90 - t;
            r = *tbl;
            r *= -1.0f;
        }
    } else {
        if (deg >= 360) {
            deg %= 360;
        }
        if (deg < -359) {
            deg %= 360;
        }
        if (deg < 0) {
            deg += 360;
        }
        if (deg < 90) {
            tbl += 90 - deg;
            r = *tbl;
        } else if (deg < 180) {
            t = deg - 90;
            tbl += t;
            r = *tbl;
            r *= -1.0f;
        } else if (deg < 270) {
            t = deg - 180;
            tbl += 90 - t;
            r = *tbl;
            r *= -1.0f;
        } else {
            t = deg - 270;
            tbl += t;
            r = *tbl;
        }
    }
    return r;
}
