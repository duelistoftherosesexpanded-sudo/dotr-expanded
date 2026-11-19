#ifndef DOTR_EE_DUEL_ACT_STRUCTS_H
#define DOTR_EE_DUEL_ACT_STRUCTS_H

/* Declarations for the SzDuel_Act original-file unit (src/ee/duel_act/func_001cee60.c;
 * docs/modules/duel.md). Only the prototypes the unit uses are kept. Types and signatures are
 * assumed views, not recovered declarations. This file holds the shared types. */

/* Types and globals that more than one function uses. A type used by one function only stays
   beside it. */

/* Two 12-byte records (SzFade_GetRestCount, SzFade_IsDoing). */
typedef struct {
    u8 pad0[5];
    u8 lo;
    u8 hi;
    s8 pad7 : 6;
    s8 flag : 1;
    s8 pad7b : 1;
    u8 pad8[4];
} Entry_0031C050;

extern Entry_0031C050 gSzFade_fl_raw[];

typedef struct {
    f32 x, y, z, w;
} Vec4f;

typedef struct {
    u8 pad00[4];
    s32 f4;
} ObjSlotInner_0031ABB0;

typedef struct {
    u8 pad00[0x14];
    ObjSlotInner_0031ABB0 *f14;
} ObjSlot_0031ABB0;

/* gSzModel_ahModel_ptrs holds 0x54 pointers to these (SzModel_SetScale, lSzModel_LoadMotionCB,
 * SzModel_GetAnimParam). */
typedef struct {
    u8 pad000[0x30];
    Vec4f v;                     /* 0x030 */
    u8 pad040[0x138];
    ObjSlot_0031ABB0 *slots[18]; /* 0x178 */
    s32 vals[8];                 /* 0x1C0 */
    s8 lv[4];                    /* 0x1E0 */
} Obj_0031ABB0;

extern Obj_0031ABB0 *gSzModel_ahModel_ptrs[];

typedef struct PadSlot_00192A88 {
    u8 *buf;
    s32 unk4[3];
    s32 open;
    s32 unk14[2];
} PadSlot_00192A88;

typedef struct {
    u32 DIR : 1;
    u32 p0 : 1;
    u32 MOD : 2;
    u32 ASP : 2;
    u32 TTE : 1;
    u32 TIE : 1;
    u32 STR : 1;
    u32 p1 : 7;
    u32 TAG : 16;
} sceDmaChcr;

typedef struct {
    sceDmaChcr chcr;
    u32 p0[3];
    void *madr;
    u32 p1[3];
    u32 qwc;
    u32 p2[3];
    void *tadr;
    u32 p3[3];
} sceDmaChan;

/* TaLogic_GetFusionTree's tree node (also used by TaLogic_TreeAloc and TaLogic_InitTree). */
struct TreeTag {
    unsigned short nRoot;
    unsigned char nComboLink[5];
    struct TreeTag *pNextLink[4];
};

/* The hand-card node SzDuel_DrawHandCard_Pipe takes as its second parameter. */
typedef struct CardNode_001AF180_s CardNode_001AF180;

/* SzDuel_SetupCard_Pipe's third parameter: the card being drawn. */
typedef struct Draw_001DA7F0_s Draw_001DA7F0;

/* Ta_Manage_ActEfc's parameters: the effect's id/param words, the
   effect record it fills, the kind pair it fills. */
typedef struct ActEfcInfo_00263930_s ActEfcInfo_00263930;
typedef struct ActEfcEffect_00263930_s ActEfcEffect_00263930;
typedef struct ActEfcKind_00263930_s ActEfcKind_00263930;

#endif
