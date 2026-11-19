#include "dotr/ee/small_misc.h"

/* Converts through Ex3d_RotTransPers with the table at 0x299f90. On success
 * stores (out[0] >> 4) - 1728 and (out[1] >> 4) - 1824; otherwise -1 in both. */
void Taku_3D_2D_Set(int *x, int *y, void *a)
{
    unsigned short out[6];
    int w;
    if (Ex3d_RotTransPers(a, data_00299f90, out, &w)) {
        *x = (out[0] >> 4) - 1728;
        *y = (out[1] >> 4) - 1824;
    } else {
        *x = -1;
        *y = -1;
    }
}
