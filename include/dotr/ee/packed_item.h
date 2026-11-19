#ifndef DOTR_EE_PACKED_ITEM_H
#define DOTR_EE_PACKED_ITEM_H

#include "dotr/ee/state_grid.h"
#include "dotr/ee/object_table.h"

/* Wrappers taking a packed (row << 8) | column item position, two stored
 * words at EE 0x31be98/0x31be9c, and two readers of a separate 24-entry table.
 * Views are offset/stride hypotheses. Evidence: docs/modules/effects.md. */
struct DotrPackedEntry {
    unsigned short unknown_bits_0_2 : 3;
    unsigned short unknown_bits_3_15 : 13;
    unsigned char unknown_02[14];
    int unknown_10;
    int unknown_14;
};

struct DotrPackedTable {
    unsigned char unknown_000[0x100];
    struct DotrPackedEntry unknown_100[24];
    int unknown_340;
};

extern struct DotrPackedTable gSzEffect_el;
extern int data_0031be98[];
extern int data_0031be9c[];

/* 0x1f53a0 reads the word at 0x31be9c; 0x1f53b0 passes the word at 0x31be98 as 0x1f53c0's
 * packed position. `out` is given the object-table vector type; 0x1e3b50 takes the same
 * four floats as a DotrStateVector. Argument names and the selector's meaning are assumed. */
int SzEffect_GetFrom(void);
int SzEffect_GetPos(int kind, struct DotrObjectVector4 *out);
int SzEffect_GetPos2(int posType, int cell, struct DotrObjectVector4 *out);
void SzEffect_SetTarget(int effectTarget);
int SzEffect_GetTarget(void);
void SzEffect_SetFrom(int effectSource);
int SzEffect_GetStatus(int index);
int SzEffect_GetCount(int effectIndex);
int SzEffect_SetBusyFlag2(int packed, int flag);
int SzEffect_SetCardBusyFlag2(int packed, int flag);
int SzEffect_SetCardColBusyFlag2(int packed, int flag);
int SzEffect_SetModelBusyFlag2(int packed, int flag);
int SzEffect_IsCardStandby2(int packed);
struct DotrStateItemObject *SzEffect_GetCardRequest2(int packed);
int SzEffect_SetCardPos2(int packed, struct DotrStateVector *v);
int SzEffect_GetCardPos2(int packed, struct DotrStateVector *v);
int SzEffect_SetCardRot2(int packed, struct DotrStateVector *v);
int SzEffect_GetCardRot2(int packed, struct DotrStateVector *v);
int SzEffect_SetCardScale2(int packed, struct DotrStateVector *v);
int SzEffect_GetCardScale2(int packed, struct DotrStateVector *v);
int SzEffect_SetCardRgba2(int packed, struct DotrStateCellQuad *v);
int SzEffect_GetCardRgba2(int packed, struct DotrStateCellQuad *v);
int SzEffect_IsModelStandby(void);
int SzEffect_IsModelStandby2(int packed);

int SzEffect_SetModelPos2(int packed, struct DotrObjectVector4 *v);
int SzEffect_GetModelPos2(int packed, struct DotrObjectVector4 *v);
int SzEffect_SetModelRot2(int packed, struct DotrObjectVector4 *v);
int SzEffect_SetModelRotC(struct DotrObjectVector4 *v);
int SzEffect_SetModelRotC2(int packed, struct DotrObjectVector4 *v);
int SzEffect_GetModelRot2(int packed, struct DotrObjectVector4 *v);
int SzEffect_GetModelRotC(struct DotrObjectVector4 *v);
int SzEffect_GetModelRotC2(int packed, struct DotrObjectVector4 *v);
int SzEffect_SetModelScale2(int packed, struct DotrObjectVector4 *v);
int SzEffect_GetModelScale2(int packed, struct DotrObjectVector4 *v);
int SzEffect_SetModelRgba2(int packed, struct DotrObjectQuad *q);
int SzEffect_SetModelRgbaX(int packed, int value, struct DotrObjectQuad *q);
int SzEffect_GetModelRgba(struct DotrObjectQuad *q);
int SzEffect_GetModelRgba2(int packed, struct DotrObjectQuad *q);
int SzEffect_GetModelRgbaX(int packed, struct DotrObjectQuad *q);
int SzEffect_SetModelFilter(struct DotrObjectQuad *q);
int SzEffect_SetModelFilter2(int packed, struct DotrObjectQuad *q);
int SzEffect_SetModelFilterX(int packed, int value, struct DotrObjectQuad *q);
int SzEffect_GetAnim2(int packed);
int SzEffect_SetAnimIntr2(int packed, int value);
int SzEffect_SetAnimDir2(int packed, int value);
float SzEffect_GetAnimFrame2(int packed);
int SzEffect_SetAnimFrame2(int packed, float animFrame);

int SzEffect_GetRefNum2(int packed);
int SzEffect_SetRefID2(int packed, int value, int unknown);
int SzEffect_GetRef2(int packed, int value, struct DotrObjectVector4 *v);
float SzEffect_GetRadius2(int packed);
int SzEffect_GetCamDist2(int packed);
int SzEffect_InqDuelResult(void);

/* Callee of the stored-word comparison wrapper (defined in cell_forwards). */
extern int SzEffect_GetPlayMotion(int flag);

/* Defined in the object_table group. */
extern int SzModel_IsModelDisp(int modelId);

/* Defined in the resource_support group. */
extern int SzDuel_Xax2New(int index);
extern int SzDuel_GetCamDist(int cardId);

/* Object-table routines, declared as these wrappers call them. */
extern int SzModel_SetRefID(int modelId, int value, int unknown);
extern int SzModel_GetRef(int modelId, int value, struct DotrObjectVector4 *out);

/* The 396-byte span from 0x1f56d0 to 0x1f5860 holds four functions.
 * 0x1f56d0: 0 for a rejected position or a negative 0x1e1360 answer; otherwise
 * for slots 0..7 a 0x1f2be0 word in 0..7 whose bits 8..14 equal value (so only
 * value 0 can match) is cleared through SzModel_SetRefID_RefNo(index, -1, slot); answers 1.
 * 0x1f57c0: the same position checks, then SzModel_SetMaxOT(index, value).
 * 0x1f57b0: 0x1f57c0 on the stored word at 0x31be9c (tail jump).
 * 0x1f5850: 0x1f5860 on the stored word at 0x31be98 (tail jump); the float
 * result is taken from that callee, the jump itself shows none. */
int SzEffect_ClearRefID2(int packed, int value);
int SzEffect_SetMaxOT(int value);
int SzEffect_SetMaxOT2(int packed, int value);
float SzEffect_GetRadius(void);

/* TaEffect_SetOuchMotion: unless SzEffect_GetAnim2 gives 4, stores 4 through SzEffect_SetAnimIntr2;
 * otherwise lowers the SzEffect_GetAnimFrame2 value through SzEffect_SetAnimFrame2. Assumed to
 * return nothing; func_0019a5c8 (library code) is assumed to return int. */
extern int func_0019a5c8(void);
void TaEffect_SetOuchMotion(int packed);

#endif
