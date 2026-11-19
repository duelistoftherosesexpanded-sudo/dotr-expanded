#include "dotr/ee/rh_geom_ops.h"

/* 0x20d9f0. C with an inline VU0 block. The host build replaces the block with its plain-C effect
 * (rh_geom_ops.h). */

/* normalize the current matrix (RhGeomEx_SetMatrixForPointLight), then multiply it by m */
void RhGeomEx_SetMatrixForStraightLight(void *arg0) {
    register void *m = arg0;

    RhGeomEx_SetMatrixForPointLight();
#ifdef __MWERKS__
    asm {
        lqc2 vf8, 0x0(m)
        lqc2 vf9, 0x10(m)
        lqc2 vf10, 0x20(m)
        lqc2 vf11, 0x30(m)
        vmulax.xyzw ACC, vf8, vf28x
        vmadday.xyzw ACC, vf9, vf28y
        vmaddaz.xyzw ACC, vf10, vf28z
        vmaddw.xyzw vf28, vf11, vf28w
        vmulax.xyzw ACC, vf8, vf29x
        vmadday.xyzw ACC, vf9, vf29y
        vmaddaz.xyzw ACC, vf10, vf29z
        vmaddw.xyzw vf29, vf11, vf29w
        vmulax.xyzw ACC, vf8, vf30x
        vmadday.xyzw ACC, vf9, vf30y
        vmaddaz.xyzw ACC, vf10, vf30z
        vmaddw.xyzw vf30, vf11, vf30w
        vmulax.xyzw ACC, vf8, vf31x
        vmadday.xyzw ACC, vf9, vf31y
        vmaddaz.xyzw ACC, vf10, vf31z
        vmaddw.xyzw vf31, vf11, vf31w
    }
#else
    dotr_rh_geom_ops_premul(m);
#endif
}
