/* Synthetic host tests for the rh_geom_ops group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* The VU0 blocks are replaced on the host by their plain-C effect on a model of vf0-vf31
 * (rh_geom_ops.h); these tests check that path only. The VU0 blocks themselves are verified
 * only by make match. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/rh_geom_ops.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

float dotr_rh_geom_ops_vf[32][4];
static int point_light_calls;

void RhGeom_UnitMatrix(void *matrix) {}
void RhGeom_MulMatrix(DotrRhGeomOpsWord matrix, void *other) {}
float RhMath_CosI(int phase) { return 1.0f; }
float RhMath_SinI(int phase) { return 0.0f; }
void RhGeom_GetScaleRotXYZTransMatrix(void *matrix, void *scale, void *rot, void *trans) {}
void RhGeomEx_SetMatrixForPointLight(void) { point_light_calls++; }

/* Identity in vf28-vf31 and vf24-vf27, vf0 = (0, 0, 0, 1). */
static void reset(void)
{
    int i;

    memset(dotr_rh_geom_ops_vf, 0, sizeof(dotr_rh_geom_ops_vf));
    dotr_rh_geom_ops_vf[0][3] = 1.0f;
    for (i = 0; i < 4; i++) {
        dotr_rh_geom_ops_vf[28 + i][i] = 1.0f;
        dotr_rh_geom_ops_vf[24 + i][i] = 1.0f;
    }
}

int main(void)
{
    float scale[4] = { 2.0f, 3.0f, 4.0f, 0.0f };
    float trans[4] = { 1.0f, 2.0f, 3.0f, 0.0f };
    float m[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    /* RhGeom_ScaleMatrix, current matrix: rows scaled by x, y, z. */
    reset();
    RhGeom_ScaleMatrix(NULL, scale);
    assert(dotr_rh_geom_ops_vf[28][0] == 2.0f && dotr_rh_geom_ops_vf[29][1] == 3.0f && dotr_rh_geom_ops_vf[30][2] == 4.0f);

    /* RhGeom_ScaleRotXYZTransMatrix, translation only: vf31 becomes (1, 2, 3, 1). */
    reset();
    RhGeom_ScaleRotXYZTransMatrix(NULL, NULL, NULL, trans);
    assert(dotr_rh_geom_ops_vf[31][0] == 1.0f && dotr_rh_geom_ops_vf[31][1] == 2.0f && dotr_rh_geom_ops_vf[31][2] == 3.0f
           && dotr_rh_geom_ops_vf[31][3] == 1.0f);

    /* RhGeomEx_SetMatrixForStraightLight: after the point-light call, identity times m is m. */
    reset();
    RhGeomEx_SetMatrixForStraightLight(m);
    assert(point_light_calls == 1 && dotr_rh_geom_ops_vf[28][1] == 2.0f && dotr_rh_geom_ops_vf[31][3] == 16.0f);

    /* RhGeomEx_ScaleRotXYZTransMatrix, scale only: vf29 scaled by y over all four lanes. */
    reset();
    RhGeomEx_ScaleRotXYZTransMatrix(scale, NULL, NULL);
    assert(dotr_rh_geom_ops_vf[29][1] == 3.0f && dotr_rh_geom_ops_vf[24][0] == 1.0f);
    return 0;
}
