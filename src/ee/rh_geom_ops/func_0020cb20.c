#include "dotr/ee/rh_geom_ops.h"

/* 0x20cb20. C with an inline VU0 block. The host build replaces the block with its plain-C effect
 * (rh_geom_ops.h). */

void RhGeom_ScaleMatrix(void *arg0, void *arg1) {
    float m[4][4];
    register float *scale = arg1;

    if (arg0 == NULL) {
#ifdef __MWERKS__
        asm {
            lqc2 vf8, 0x0(scale)
            vmulx.xyzw vf28, vf28, vf8x
            vmuly.xyzw vf29, vf29, vf8y
            vmulz.xyzw vf30, vf30, vf8z
        }
#else
        dotr_rh_geom_ops_scale(28, scale, 4);
#endif
    } else {
        RhGeom_UnitMatrix(m);
        m[0][0] = scale[0];
        m[1][1] = scale[1];
        m[2][2] = scale[2];
        RhGeom_MulMatrix((DotrRhGeomOpsWord)arg0, m);
    }
}
