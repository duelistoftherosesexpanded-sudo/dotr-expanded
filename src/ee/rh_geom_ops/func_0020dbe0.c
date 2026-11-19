#include "dotr/ee/rh_geom_ops.h"

/* 0x20dbe0. C with inline VU0 blocks. The host build replaces each block with its plain-C effect
 * (rh_geom_ops.h). */

/* RhGeom_ScaleRotXYZTransMatrix for the current matrix and its normal matrix: translate
   (vf28-vf31), rotate both (vf28-vf30 and vf24-vf26), scale (vf28-vf30). */
void RhGeomEx_ScaleRotXYZTransMatrix(register void *scale, void *rot, register void *trans) {
    float m[4][4];
    float a0, b0, a1, b1, a2, b2;

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
        a0 = RhMath_CosI(((int *)rot)[0]);
        b0 = RhMath_SinI(((int *)rot)[0]);
        a1 = RhMath_CosI(((int *)rot)[1]);
        b1 = RhMath_SinI(((int *)rot)[1]);
        a2 = RhMath_CosI(((int *)rot)[2]);
        b2 = RhMath_SinI(((int *)rot)[2]);
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
            vmulax.xyzw ACC, vf24, vf8x
            vmadday.xyzw ACC, vf25, vf8y
            vmaddaz.xyzw ACC, vf26, vf8z
            vmaddw.xyzw vf20, vf27, vf8w
            vmulax.xyzw ACC, vf24, vf9x
            vmadday.xyzw ACC, vf25, vf9y
            vmaddaz.xyzw ACC, vf26, vf9z
            vmaddw.xyzw vf21, vf27, vf9w
            vmulax.xyzw ACC, vf24, vf10x
            vmadday.xyzw ACC, vf25, vf10y
            vmaddaz.xyzw ACC, vf26, vf10z
            vmaddw.xyzw vf22, vf27, vf10w
            vmove.xyzw vf24, vf20
            vmove.xyzw vf25, vf21
            vmove.xyzw vf26, vf22
        }
#else
        dotr_rh_geom_ops_rotate(28, p);
        dotr_rh_geom_ops_rotate(24, p);
#endif
        }
    }
    if (scale != NULL) {
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
    }
}
