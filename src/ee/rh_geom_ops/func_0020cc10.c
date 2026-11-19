#include "dotr/ee/rh_geom_ops.h"

/* 0x20cc10. C with inline VU0 blocks. The host build replaces each block with its plain-C effect
 * (rh_geom_ops.h). */

/* Apply translate, rotate (X, Y, Z angles) and scale to the current VU0 matrix (vf28-vf31),
   or with a destination matrix build the transform there (RhGeom_GetScaleRotXYZTransMatrix) and multiply. */
void RhGeom_ScaleRotXYZTransMatrix(void *dst, register float *scale, int *rot, register float *trans) {
    float m[4][4];
    float a0, b0, a1, b1, a2, b2;

    if (dst == NULL) {
    if (trans != NULL) {
#ifdef __MWERKS__
        asm {
            lqc2 vf8, 0x0(trans)
            vmulax.xyzw ACC, vf28, vf8x
            vmadday.xyzw ACC, vf29, vf8y
            vmaddaz.xyzw ACC, vf30, vf8z
            vmaddw.xyzw vf31, vf31, vf0w
        }
#else
        dotr_rh_geom_ops_trans(trans);
#endif
    }
    if (rot != NULL) {
        a0 = RhMath_CosI(rot[0]);
        b0 = RhMath_SinI(rot[0]);
        a1 = RhMath_CosI(rot[1]);
        b1 = RhMath_SinI(rot[1]);
        a2 = RhMath_CosI(rot[2]);
        b2 = RhMath_SinI(rot[2]);
        m[0][0] = a1 * a2;
        m[1][0] = a2 * (b0 * b1) - a0 * b2;
        m[2][0] = a2 * (a0 * b1) + b0 * b2;
        m[0][1] = a1 * b2;
        m[1][1] = b2 * (b0 * b1) + a0 * a2;
        m[2][1] = b2 * (a0 * b1) - b0 * a2;
        m[0][2] = -b1;
        m[1][2] = b0 * a1;
        m[2][2] = a0 * a1;
        m[0][3] = 0.0f;
        m[1][3] = 0.0f;
        m[2][3] = 0.0f;
        {
        register float *p = m[0];
#ifdef __MWERKS__
        asm {
            lqc2 vf8, 0x0(p)
            lqc2 vf9, 0x10(p)
            lqc2 vf10, 0x20(p)
            vmulax.xyzw ACC, vf28, vf8x
            vmadday.xyzw ACC, vf29, vf8y
            vmaddaz.xyzw ACC, vf30, vf8z
            vmaddw.xyzw vf20, vf31, vf8w
            vmulax.xyzw ACC, vf28, vf9x
            vmadday.xyzw ACC, vf29, vf9y
            vmaddaz.xyzw ACC, vf30, vf9z
            vmaddw.xyzw vf21, vf31, vf9w
            vmulax.xyzw ACC, vf28, vf10x
            vmadday.xyzw ACC, vf29, vf10y
            vmaddaz.xyzw ACC, vf30, vf10z
            vmaddw.xyzw vf22, vf31, vf10w
            vmove.xyzw vf28, vf20
            vmove.xyzw vf29, vf21
            vmove.xyzw vf30, vf22
        }
#else
        dotr_rh_geom_ops_rotate(28, p);
#endif
        }
    }
    if (scale != NULL) {
#ifdef __MWERKS__
        asm {
            lqc2 vf8, 0x0(scale)
            vmulx.xyz vf28, vf28, vf8x
            vmuly.xyz vf29, vf29, vf8y
            vmulz.xyz vf30, vf30, vf8z
        }
#else
        dotr_rh_geom_ops_scale(28, scale, 3);
#endif
    }
    } else {
        RhGeom_GetScaleRotXYZTransMatrix(m, scale, rot, trans);
        RhGeom_MulMatrix((DotrRhGeomOpsWord)dst, m);
    }
}
