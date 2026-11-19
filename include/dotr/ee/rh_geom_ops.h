#ifndef DOTR_EE_RH_GEOM_OPS_H
#define DOTR_EE_RH_GEOM_OPS_H

/* RhGeom and RhGeomEx matrix routines that update the persistent VU0 current matrix
 * (vf28-vf31, with vf24-vf27 as the normal matrix) through inline VU0 blocks.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* The code passes a matrix address to RhGeom_MulMatrix as an integer: `int` on the target,
 * pointer-sized on the host. */
#ifdef __MWERKS__
typedef int DotrRhGeomOpsWord;
#else
#include <stdint.h>
typedef intptr_t DotrRhGeomOpsWord;
#endif

#ifndef NULL
#define NULL 0
#endif

/* Callees. RhGeom_UnitMatrix,
 * RhGeom_MulMatrix and RhGeomEx_SetMatrixForPointLight are assembly functions (asm/ee/vu0);
 * RhMath_CosI / RhMath_SinI are defined in small_misc and RhGeom_GetScaleRotXYZTransMatrix in
 * view_state. */
void RhGeom_UnitMatrix(void *matrix);
void RhGeom_MulMatrix(DotrRhGeomOpsWord matrix, void *other);
float RhMath_CosI(int phase);
float RhMath_SinI(int phase);
void RhGeom_GetScaleRotXYZTransMatrix(void *matrix, void *scale, void *rot, void *trans);
void RhGeomEx_SetMatrixForPointLight(void);

/* 0x20cb20: scales the current matrix's rows by scale x/y/z (null matrix) or multiplies the
 * matrix by a scale matrix. */
void RhGeom_ScaleMatrix(void *matrix, void *scale);
/* 0x20cc10: translates, rotates (X, Y, Z angles) and scales the current matrix, or builds that
 * transform and multiplies `dst` by it. */
void RhGeom_ScaleRotXYZTransMatrix(void *dst, register float *scale, int *rot, register float *trans);
/* 0x20d9f0: RhGeomEx_SetMatrixForPointLight, then the current matrix times `matrix`. */
void RhGeomEx_SetMatrixForStraightLight(void *matrix);
/* 0x20dbe0: RhGeom_ScaleRotXYZTransMatrix for the current matrix and the normal matrix. */
void RhGeomEx_ScaleRotXYZTransMatrix(register void *scale, void *rot, register void *trans);

#ifndef __MWERKS__
/* Host only: a model of the VU0 registers the inline blocks use (vf0 is (0, 0, 0, 1)), and the
 * effect of each block in plain C. The VU0 blocks themselves are verified only by make match. */
extern float dotr_rh_geom_ops_vf[32][4];
#define DOTR_RHGEOM_VF dotr_rh_geom_ops_vf
/* vmul{x,y,z}: rows base..base+2 times component 0/1/2 of s, over the first n lanes. */
static inline void dotr_rh_geom_ops_scale(int base, const float *s, int n)
{
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < n; j++) {
            DOTR_RHGEOM_VF[base + i][j] *= s[i];
        }
    }
}
/* vmula/vmadda chain: vf31 = vf28 * t.x + vf29 * t.y + vf30 * t.z + vf31 * vf0.w. */
static inline void dotr_rh_geom_ops_trans(const float *t)
{
    int j;

    for (j = 0; j < 4; j++) {
        DOTR_RHGEOM_VF[31][j] = DOTR_RHGEOM_VF[28][j] * t[0] + DOTR_RHGEOM_VF[29][j] * t[1]
            + DOTR_RHGEOM_VF[30][j] * t[2] + DOTR_RHGEOM_VF[31][j];
    }
}
/* Rows base..base+2 become sum over i of row base+i times p[k][i] (rows 0-2 of the 4x4 p). */
static inline void dotr_rh_geom_ops_rotate(int base, const float *p)
{
    float t[3][4];
    int j, k;

    for (k = 0; k < 3; k++) {
        for (j = 0; j < 4; j++) {
            t[k][j] = DOTR_RHGEOM_VF[base][j] * p[k * 4] + DOTR_RHGEOM_VF[base + 1][j] * p[k * 4 + 1]
                + DOTR_RHGEOM_VF[base + 2][j] * p[k * 4 + 2] + DOTR_RHGEOM_VF[base + 3][j] * p[k * 4 + 3];
        }
    }
    for (k = 0; k < 3; k++) {
        for (j = 0; j < 4; j++) {
            DOTR_RHGEOM_VF[base + k][j] = t[k][j];
        }
    }
}
/* Each row r of vf28-vf31 becomes m row 0 * r.x + m row 1 * r.y + m row 2 * r.z + m row 3 * r.w. */
static inline void dotr_rh_geom_ops_premul(const float *m)
{
    float v[4];
    int j, r;

    for (r = 28; r < 32; r++) {
        for (j = 0; j < 4; j++) {
            v[j] = DOTR_RHGEOM_VF[r][j];
        }
        for (j = 0; j < 4; j++) {
            DOTR_RHGEOM_VF[r][j] = m[j] * v[0] + m[4 + j] * v[1] + m[8 + j] * v[2] + m[12 + j] * v[3];
        }
    }
}
#endif

#endif
