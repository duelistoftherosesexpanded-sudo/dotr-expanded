#ifndef DOTR_EE_OBJECT_STARTS_H
#define DOTR_EE_OBJECT_STARTS_H

/* Functions that start work on one gSzModel_ahModel object picked through a side's packed
 * cell (SzBattle_InqEntry, SzDuel_GetUnitModelID), as in 0x1f0cd0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* View of the objects behind gSzModel_ahModel limited to the fields SzModel_BattleSeq touches; the
 * object_table group holds the wider view of the same storage. Field types are assumed from
 * the access widths (lw, lhu). */
struct DotrStartObject {
    unsigned char unknown_000[0x144];
    int effectTextureB[8];
    unsigned char unknown_164[8];
    void *pCameraMotion;
    unsigned char unknown_170[0x7c];
    int effectCallback;
    unsigned char unknown_1f0[0x12];
    unsigned short cameraFrameCurrent;
    unsigned short cameraFrameTotal;
    unsigned short cameraAxisStored;
    unsigned short unknown_208;
};

/* 84 entries by the bound SzModel_BattleSeq applies; same address as object_table's gSzModel_ahModel. */
extern struct DotrStartObject *gSzModel_ahModel_start[84];

/* Callees defined in other groups (state_grid, object_table, small_misc, packed_item). */
extern int SzBattle_InqEntry(int index);
extern int SzDuel_GetUnitModelID(int side, int pos);
extern int SzModel_SetAnimIntr(int modelId, int animIndex);
extern int SzText_FreeRequest(int textureFlags, int textureId);
extern int SzEffect_ClearRefID2(int packed, int value);

/* Assumed prototypes: argument counts from the call sites, types assumed. lSzModel_EffectCB is
 * only passed by address (to SzEffect_EntryCallback); its own signature is not established. */
extern int SzText_AddFile(int group, int id, const char *name, int srcImageAddress);
extern int SzEffect_EntryCallback(int moduleType, int b, int c, int d, int e, void (*callback)(void), int g);
extern void func_001fa8a0_start(int a, int b, int c, int d, int index, void *data);
extern void lSzModel_EffectCB(void);

/* side selects a packed cell through SzBattle_InqEntry; flag and alt are tested for zero only. */
extern int SzModel_BattleSeq(int side, int flag, int alt);

#endif
