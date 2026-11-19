#ifndef DOTR_EE_GT4_BOX_FUNCTIONS_H
#define DOTR_EE_GT4_BOX_FUNCTIONS_H

/* Declarations for the GT4_BOX_IN__2DMAP original-file unit (src/ee/gt4_box/func_001095d0.c;
 * docs/modules/other.md). Only the prototypes the unit uses are kept. Types and signatures are
 * assumed views, not recovered declarations. This file holds the prototypes; all of them are
 * visible to every compiler (the target and the host build both need them). */


/* Prototypes of the decompiled functions. */
s32 CARD_OFF(s32 arg0);
s32 CARD_ON(s32 arg0, s32 arg1);
s32 CARD_SCL_ON_1(s32 arg0, f32 fparg0);
void CARD_X_Z_IN_(f32 *arg0, f32 *arg1, void *arg2, void *arg3);
s32 Ex3d_RotTransPersAuto(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 *arg4);
s32 ExAssignAlpha(void *arg0, s64 arg1);
void ExSortPrim(s32 arg0, s32 arg1, s32 arg2);
s32 G4_BOX_IN__(void *arg0);
s32 G4_IN_RGBA(void *arg0, u8 *arg1, s32 arg2);
s32 GT4_BOX_IN__2DMAP(void *arg0, void *arg1, void *arg2);
s32 GT4_IN_RGBA(void *arg0, u8 *arg1, s32 arg2);
s32 GT4_Xyz_ZIN(void *arg0, s32 arg1);
int MATRIX_XYZ_ININ33(void *row_v, void *v_v, int axis, int unit);
s32 MATRIX_XYZ_ININ_4(u8 *arg0, void *arg1, s32 arg2, s32 arg3);
s32 MODEL_OFF(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 MODEL_ON(s32 arg0, s32 arg1, s32 arg2);
s32 MODEL_SCL_OFF_1(s32 arg0, f32 fparg0);
s32 MODEL_SCL_ON_1(s32 arg0, f32 fparg0);
s32 OBJ_GsRGB_RGB_IN(u8 *arg0, u8 *arg1);
void OBJ_GsRGB_RGB_IN2(void *arg0, void *arg1);
s32 OBJ_GsRGB_RGB_IN_DATA(u8 *arg0, s32 arg1, s16 arg2);
void OBJ_I_V_IN(s32 *dst, f32 *src);
s32 OBJ_RGB_IN(void *arg0, s8 arg1, s8 arg2, s8 arg3);
void OBJ_ROT(f32 x, f32 y, f32 z, s32 *out);
s32 OBJ_RR2H_IN(f32 fparg0, f32 fparg1, f32 fparg2, void *arg0);
void OBJ_VE_SCL_KAKE(void *arg0, void *arg1);
void OBJ_VE_SCL_KAKE_4(void *arg0, void *arg1);
s32 PAINT_PIRM_G4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 PAINT_PIRM_GT4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 PAINT_PIRM_LG2(s32 arg0, void *arg1, s32 arg2, s32 arg3);
s32 PAINT_VECTOR_4TG_SET_IN(void *arg0, f32 fparg0, f32 fparg1);
s32 PAINT_VECTOR_4TG_SET_IN2(void *dst, f32 x, f32 z);
s32 PAINT_VECTOR_4TG_SET_INMAP4(void *arg0, s32 arg1, s32 arg2, s16 arg3);
s32 PAINT__2G_SET_IN(void *arg0, void *arg1, void *arg2);
s32 PAINT__G4_SET_IN(void *arg0, s32 arg1);
s32 PAINT__GT4_SET_IN(void *arg0);
s32 POINT_IN_VECTOR_(void *arg0, void *arg1, s32 arg2);
void POLPAN_UVIN_(void *arg0, void *arg1, s32 arg2);
void POS_POS_4(f32 *dst, f32 *src);
s32 PSYS_RR2H_IN(void *arg0, f32 fparg0, f32 fparg1, f32 fparg2);
void RAD_XYZ_IN(void *arg0, void *arg1, s64 arg2, f32 arg3, f32 arg4);
void RhGeom_GetScaleRotXYZTransMatrix(void *outMatrix, void *scale, void *rot, void *pos);
void RhGeom_MulMatrix(s32 arg0, void *arg1);
void RhView_GetWorldScreenMatrix(void *arg0);
f32 SIN_COS_INRAD(s32 arg0, s32 arg1, s32 arg2);
void SIN_IN_PSYS(f32 *arg0);
void SIN_IN_PSYS2(void *arg0, s32 arg1, s32 arg2);
void S_I_P_IN(void *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 SzEffect_GetCardRgba2(s32 arg0, u8 *arg1);
s32 SzEffect_GetModelScale2(s32 arg0, f32 *arg1);
s32 SzEffect_IsCardStandby2(s32 arg0);
s32 SzEffect_IsModelStandby2(s32 arg0);
s32 SzEffect_SetCardRgba2(s32 arg0, s32 arg1);
s32 SzEffect_SetCardScale2(s32 arg0, s32 arg1);
s32 SzEffect_SetModelScale2(s32 arg0, void *arg1);
s32 V0IN_V1_V2_XZ(void *arg0, void *arg1, void *arg2);
s32 XYZ_POS_4(void *arg0, s64 arg1, f32 fparg0);
s32 abs(s32 arg0);
s64 func_001a0660(s64 u, s64 v);
s64 func_001a22c0(f32 arg_a);
s32 func_001a23b0(f32 arg_a);
f32 func_001a6898(f32 x);
f32 func_001a6a88(f32 x);
f32 func_001a6c60(f32 x);
s32 tanD_360_(s64 x, s64 y);

/* Functions declared again under the names the unit's code and its C library built-in calls
   use. */
s32 abs(s32 arg0);
void OBJ_ROT(f32 x, f32 y, f32 z, s32 *out);
void OBJ_I_V_IN(s32 *dst, f32 *src);
void OBJ_VE_SCL_KAKE(void *arg0, void *arg1);
void OBJ_VE_SCL_KAKE_4(void *arg0, void *arg1);
void POS_POS_4(f32 *dst, f32 *src);
s32 XYZ_POS_4(void *arg0, s64 arg1, f32 fparg0);
s32 POINT_IN_VECTOR_(void *arg0, void *arg1, s32 arg2);
s32 MODEL_OFF(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 CARD_OFF(s32 arg0);
s32 MODEL_ON(s32 arg0, s32 arg1, s32 arg2);
s32 MODEL_SCL_ON_1(s32 arg0, f32 fparg0);
s32 MODEL_SCL_OFF_1(s32 arg0, f32 fparg0);
s32 CARD_SCL_ON_1(s32 arg0, f32 fparg0);
s32 CARD_ON(s32 arg0, s32 arg1);
s32 OBJ_RGB_IN(void *arg0, s8 arg1, s8 arg2, s8 arg3);
s32 OBJ_GsRGB_RGB_IN(u8 *arg0, u8 *arg1);
void OBJ_GsRGB_RGB_IN2(void *arg0, void *arg1);
s32 GT4_IN_RGBA(void *arg0, u8 *arg1, s32 arg2);
s32 G4_IN_RGBA(void *arg0, u8 *arg1, s32 arg2);
void CARD_X_Z_IN_(f32 *arg0, f32 *arg1, void *arg2, void *arg3);
void SIN_IN_PSYS(f32 *arg0);
void SIN_IN_PSYS2(void *arg0, s32 arg1, s32 arg2);
s32 V0IN_V1_V2_XZ(void *arg0, void *arg1, void *arg2);
s32 OBJ_RR2H_IN(f32 fparg0, f32 fparg1, f32 fparg2, void *arg0);
s32 PSYS_RR2H_IN(void *arg0, f32 fparg0, f32 fparg1, f32 fparg2);
s32 OBJ_GsRGB_RGB_IN_DATA(u8 *arg0, s32 arg1, s16 arg2);
f32 SIN_COS_INRAD(s32 arg0, s32 arg1, s32 arg2);
s32 tanD_360_(s64 x, s64 y);
void POLPAN_UVIN_(void *arg0, void *arg1, s32 arg2);
int MATRIX_XYZ_ININ33(void *row_v, void *v_v, int axis, int unit);
void RAD_XYZ_IN(void *arg0, void *arg1, s64 arg2, f32 arg3, f32 arg4);
s32 PAINT_VECTOR_4TG_SET_IN(void *arg0, f32 fparg0, f32 fparg1);
s32 PAINT_VECTOR_4TG_SET_IN2(void *dst, f32 x, f32 z);
s32 PAINT_VECTOR_4TG_SET_INMAP4(void *arg0, s32 arg1, s32 arg2, s16 arg3);
void S_I_P_IN(void *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 PAINT_PIRM_G4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 PAINT_PIRM_GT4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 PAINT_PIRM_LG2(s32 arg0, void *arg1, s32 arg2, s32 arg3);
s32 MATRIX_XYZ_ININ_4(u8 *arg0, void *arg1, s32 arg2, s32 arg3);
s32 PAINT__2G_SET_IN(void *arg0, void *arg1, void *arg2);
s32 PAINT__GT4_SET_IN(void *arg0);
s32 PAINT__G4_SET_IN(void *arg0, s32 arg1);
s32 GT4_Xyz_ZIN(void *arg0, s32 arg1);
s32 G4_BOX_IN__(void *arg0);
s32 GT4_BOX_IN__2DMAP(void *arg0, void *arg1, void *arg2);
s32 SzEffect_IsCardStandby2(s32 arg0);
s32 SzEffect_SetCardScale2(s32 arg0, s32 arg1);
s32 SzEffect_SetCardRgba2(s32 arg0, s32 arg1);
s32 SzEffect_GetCardRgba2(s32 arg0, u8 *arg1);
s32 SzEffect_IsModelStandby2(s32 arg0);
s32 SzEffect_SetModelScale2(s32 arg0, void *arg1);
s32 SzEffect_GetModelScale2(s32 arg0, f32 *arg1);
s32 Ex3d_RotTransPersAuto(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 *arg4);
void ExSortPrim(s32 arg0, s32 arg1, s32 arg2);
s32 ExAssignAlpha(void *arg0, s64 arg1);
void RhGeom_MulMatrix(s32 arg0, void *arg1);
void RhGeom_GetScaleRotXYZTransMatrix(void *outMatrix, void *scale, void *rot, void *pos);
void RhView_GetWorldScreenMatrix(void *arg0);

#endif
