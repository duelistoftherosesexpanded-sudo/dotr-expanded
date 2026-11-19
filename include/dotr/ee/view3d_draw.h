#ifndef DOTR_EE_VIEW3D_DRAW_H
#define DOTR_EE_VIEW3D_DRAW_H

/* SzDuel_Draw3DView_Pipe (0x1c1b00): the duel's 3D view pass over model 29, the point light at
 * gSzDuel_PointLight and the board model draws.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* Four floats copied and passed by address (positions, the camera point, the look
 * vector); member names are this group's. */
struct DotrView3dVector {
    float x;
    float y;
    float z;
    float w;
};

/* Four words passed as the rotation argument of SzModel_DrawWithOutCalc_Pipe; only y is
 * written here. */
struct DotrView3dAngles {
    int x;
    int y;
    int z;
    int w;
};

/* gSzDuel_PointLight (0x291430, 48 bytes by the Japanese build's symbol table): the
 * position at +0x10 and the two floats at +0x20 / +0x24 are written here; the rest is
 * not interpreted. */
struct DotrView3dLight {
    float unknown_00[4];
    struct DotrView3dVector position;
    float unknown_20;
    float unknown_24;
    float unknown_28[2];
};

/* This group's view of the control storage at 0x317490: only the byte at +0xd35, a
 * counter that is zero until the first pass has placed the model. */
struct DotrView3dControl {
    unsigned char unknown_0000[0xd35];
    unsigned char unknown_d35;
};

extern struct DotrView3dVector data_00291460;
extern struct DotrView3dAngles data_00291470;
extern struct DotrView3dVector data_00291480;
extern struct DotrView3dLight gSzDuel_PointLight;
extern struct DotrView3dControl data_00317490;

/* Callees. Pointer parameters carry this group's vector types; results the caller ignores
 * are declared as the groups that define the callees have them. */
extern int SzModel_IsModelDisp(int modelId);
extern void *SzView_GetLook(int index);
extern float SzModel_GetRadius(int modelId);
extern int SzModel_GetCenter(int modelId, struct DotrView3dVector *xyzCoords);
extern void *SzView_GetCP(int index);
extern int SzView_SetCP(int index, struct DotrView3dVector *in);
extern int SzModel_GetCenterOfs(int modelId, struct DotrView3dVector *centerArray);
extern int SzModel_GetModelNo(int modelId);
extern int SzDuel_Xax2New(int index);
extern int SzDuel_GetCamDist(int cardId);
extern int SzModel_SetPos(int modelId, struct DotrView3dVector *pos);
extern int SzView_SetLookSP(int index, struct DotrView3dVector *in);
extern void *SzView_GetSP(int index);
extern int SzView_SetPointLight(int index, int position, struct DotrView3dLight *pointLightParameters);
extern int SzModel_DrawWithOutCalc_Pipe(int cursor, int modelId, struct DotrView3dVector *c,
                                        void *d, int e, int f, int g);
/* Declared void in scratch_quads (the group that defines it); this caller takes v0 after the
 * call as the next cursor, so the result is declared here. */
extern int lSzDuel_DrawFadeBoard_Pipe(int a, struct DotrView3dVector *centre, int alpha,
                                      float dx, float dz);
extern int SzModel_Draw_Pipe(int cursor, int modelId, int c, void *d, int e, int f, int g);

/* Callees of SzDuel_Act3DView (0x1c12b0), declared as the groups that define them have them;
 * func_001a6c30 (library block, no original name) is assumed to take and return
 * single floats, as block_table declares it. */
extern int SzModel_ReqSE(int channel, int modelId);
extern int SzPad_Check(int index, int mode, int mask);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern void SzGame_Pop(void);
extern int SzPad_IsAnalog(int index);
extern int SzPad_GetAnalogInfo(int index, int slot);
extern int SzPad_IsPress(int index);
extern int SzPad_GetPressInfo(int index, int slot);
extern float func_001a6c30(float a, float b);

int SzDuel_Draw3DView_Pipe(int cursor);
/* No parameters are read and v0 is not set. */
void SzDuel_Act3DView(void);

#endif
