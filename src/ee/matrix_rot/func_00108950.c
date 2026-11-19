#include "dotr/ee/matrix_rot.h"

#define S(a) SIN_COS_INRAD(unit, (a), 0)
#define C(a) SIN_COS_INRAD(unit, (a), 1)

int MATRIX_XYZ_ININ33(struct DotrPaintVector *row, struct DotrPaintVector *v, int axis, int unit)
{
    union DotrMatrixAngles rot = *(union DotrMatrixAngles *)v;
    struct DotrPaintVector out;
    int a;

    if (axis == 0) {
        a = rot.i[1];
        out.x = row->x * C(a) + row->z * -S(a);
        out.y = row->y;
        out.z = row->x * S(a) + row->z * C(a);
        *row = out;
    }
    if (axis == 1) {
        out.x = row->x;
        out.y = row->y * C(rot.i[0]) + row->z * S(rot.i[0]);
        out.z = row->y * -S(rot.i[0]) + row->z * C(rot.i[0]);
        *row = out;
    }
    if (axis == 2) {
        a = rot.i[2];
        out.x = row->x * C(a) + row->y * S(a);
        out.y = row->x * -S(a) + row->y * C(a);
        out.z = row->z;
        *row = out;
    }
    return 0;
}
