#include "dotr/ee/paint_prims.h"

/* 0x109100 MATRIX_XYZ_ININ_4 (name from the Japanese build's symbols). */
int MATRIX_XYZ_ININ_4(struct DotrPaintVector *rows, struct DotrPaintVector *v, int arg2, int arg3)
{
    struct DotrPaintVector copy;

    copy = *v;
    MATRIX_XYZ_ININ33(&rows[0], &copy, arg2, arg3);
    MATRIX_XYZ_ININ33(&rows[1], &copy, arg2, arg3);
    MATRIX_XYZ_ININ33(&rows[2], &copy, arg2, arg3);
    MATRIX_XYZ_ININ33(&rows[3], &copy, arg2, arg3);
    return 0;
}
