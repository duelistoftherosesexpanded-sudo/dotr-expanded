#ifndef DOTR_EE_ITEM_RELEASE_H
#define DOTR_EE_ITEM_RELEASE_H

/* Functions that give back what a checked state item holds (its object, grid cell word or
 * 160-byte slot) before clearing the item.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/state_grid.h"

/* The object an item (+4) or a slot (+0) points to: only the word at +0x90, cleared
 * after SzDuel_ClearDrawCard, is known here. */
struct DotrReleaseCurrent;
struct DotrReleaseObject {
    unsigned char unknown_00[0x10];
    int unknown_10[4]; /* cleared by lSzDuel_InitHandInfo / lSzDuel_InitHandInfo2 */
    int unknown_20[4]; /* passed to Ex3d_GetLookVector */
    float unknown_30;
    float unknown_34;
    float unknown_38;
    int unknown_3c;
    unsigned char unknown_40[0x41];
    signed char unknown_81_bits_0_3 : 4;
    signed char unknown_81_bit_4 : 1;
    signed char unknown_81_bit_5 : 1;
    signed char unknown_81_bit_6 : 1;
    signed char unknown_81_bit_7 : 1;
    unsigned char unknown_82;
    signed char unknown_83_bits_0_5 : 6;
    signed char unknown_83_bit_6 : 1;
    signed char unknown_83_bit_7 : 1; /* cleared by lSzDuel_InitHandCancel */
    unsigned char unknown_84[0xc];
    struct DotrReleaseCurrent *unknown_90; /* the item that holds the object, or 0 */
    unsigned char unknown_94[0xc];
};
/* The 32 objects at 0x3193c0, 160 bytes each, as lSzDuel_InitHandInfo and lSzDuel_InitHandInfo2 search
 * them for one whose +0x90 is zero (owner_draws.h types the same elements differently). */
extern struct DotrReleaseObject gSzDuel_aCardRequest[32];
/* One 160-byte element of the six at 0x31a7c0 (small_misc.h, which cannot be included
 * together with state_grid.h, declares the same storage as gSzDuel_aCardControl). */
struct DotrReleaseSlot {
    struct DotrReleaseObject *object;
    unsigned char unknown_04[0xc];
    struct DotrStateVector unknown_10; /* +0x10..+0x8c as lSzDuel_InitHandCombo fills them */
    struct DotrStateVector unknown_20;
    struct DotrStateVector unknown_30;
    int unknown_40[4];
    int unknown_50[4];
    int unknown_60[4]; /* passed to Ex3d_GetLookVector */
    struct DotrStateVector unknown_70;
    float unknown_80;
    float unknown_84;
    float unknown_88;
    int unknown_8c;
    unsigned char unknown_90[8];
    short unknown_98;
    short unknown_9a;
    int unknown_9c_bits_0_25 : 26; /* the slot's own index, stored by lSzDuel_InitHandInfo;
                                      read signed by lSzDuel_InitHandCombo */
    signed char unknown_9f_bit_2 : 1;
    signed char unknown_9f_bit_3 : 1;
    signed char unknown_9f_bit_4 : 1;
    signed char unknown_9f_bit_5 : 1;
    signed char unknown_9f_bit_6 : 1;
    signed char unknown_9f_bit_7 : 1;
};
extern struct DotrReleaseSlot gSzDuel_aCardControl_release[];
struct DotrOwnerDrawBlock;
struct DotrMiscSlot160;
/* As declared in object_table.h, owner_draws.h and small_misc.h. */
extern int SzModel_Preset(int modelId, int modelType, int modelNumber);
extern int SzDuel_ClearDrawCard(struct DotrOwnerDrawBlock *block);
extern int SzDuel_ClearCardControl(struct DotrMiscSlot160 *p);

/* lSzDuel_InitHandCancel's view of the control block at 0x317490: the pointer at +0x6cc (also
 * read through its own address 0x317b5c, declared separately because the original loads
 * it absolutely once), a byte list at +0xd2c and its count at +0xd4a. The element behind
 * the pointer is only known by its object pointer (+4) and three flag bits. */
struct DotrReleaseCurrent {
    int constInfo;
    struct DotrReleaseObject *object;
    unsigned char unknown_08[0x34];
    unsigned short cardNo; /* 999 = no slot work */
    unsigned char unknown_3e[2];
    unsigned char locCol;
    unsigned char locRow;
    unsigned char unknown_42[2];
    unsigned char team;
    unsigned char slot;
    unsigned char unknown_46[2];
    unsigned char lvRev;
    unsigned char unknown_49[0xa];
    unsigned char ctlRateM; /* cleared in lSzDuel_InitHandCombo's copy */
    unsigned char unknown_54[3];
    signed char unknown_57_bits_0_6 : 7;
    signed char unknown_57_bit_7 : 1;
    signed char unknown_58_bit_0 : 1;
    signed char unknown_58_bit_1 : 1;
    signed char unknown_58_bit_2 : 1;
    signed char unknown_58_bits_3_7 : 5;
};
struct DotrReleaseControl {
    unsigned char unknown_000[0x6cc];
    struct DotrReleaseCurrent *current;
    unsigned char unknown_6d0[0x65c];
    signed char unknown_d2c[30];
    unsigned char unknown_d4a;
    unsigned char unknown_d4b[3];
    unsigned char unknown_d4e;
    unsigned char unknown_d4f;
};
extern struct DotrReleaseControl data_00317490_release;
extern struct DotrReleaseCurrent *data_00317b5c_release;
/* As declared in state_records.h; lSzDuel_SetCardCtl as in small_misc.h, with the slot
 * passed where that header names struct DotrMiscRect. */
extern int SzDuel_InqHand(int side, int position);
extern int SzDuel_GetHandCount(int side);
extern int lSzDuel_SetCardCtl(struct DotrReleaseSlot *p, int a, int b, int c, int d, short e, short f);
int lSzDuel_InitHandCancel(int index);

/* lSzDuel_InitHandInfo / lSzDuel_InitHandInfo2 index 0x317490 as 848-byte elements and read two bytes
 * of each. The same storage is struct DotrReleaseControl above; a cast of that object
 * does not reproduce the original address arithmetic, hence this second symbol. */
struct DotrReleaseSide {
    unsigned char unknown_000[0x33d];
    unsigned char selectedCol;
    unsigned char selectedRow;
    unsigned char unknown_33f[4];
    unsigned char numberOfUnitsSummoned; /* running number given to each unit SzDuel_AddUnit2 places */
    unsigned char unknown_344[0xc];
};
extern struct DotrReleaseSide data_00317490_sides[];
/* As declared in state_queries.h; Ex3d_GetLookVector under owner_draws.h's name for its
 * pointer view; lSzDuel_SetCardCtl2 with the slot where its own header names struct DotrViewRect.
 * SzDuel_GetCardInfo and SzDuel_InitCardInfo come from state_grid.h and are cast at the call. */
extern int Ta_GetSummonRev(int side, int id);
extern float func_00205c40_owner(int *out);
extern int lSzDuel_SetCardCtl2(struct DotrReleaseSlot *p, int a, int b, int c, int d, short e, short f);
int lSzDuel_InitHandInfo(int a, int fieldSlot, int cardId, int d, int e, int f, int g);
int lSzDuel_InitHandInfo2(int a, int unitArrayPos, int c, int d, int e);

/* lSzDuel_InitHandCombo (0x1d82c0). The first 0x90 bytes of an object are copied as nine
 * quadwords, hence this view; the byte at 0x3181de is loaded absolutely once and through
 * the control block afterwards. Ex3d_GetPointOnScreen and SzDuel_DoCardControl0 as
 * view_state.h and card_control.h declare them, with this group's types. */
#ifdef __MWERKS__
typedef __int128 DotrReleaseQuad;
#else
__extension__ typedef __int128 DotrReleaseQuad __attribute__((aligned(4), may_alias));
#endif
struct DotrReleaseObjectBody {
    DotrReleaseQuad unknown_00[9];
};
extern void Ex3d_GetPointOnScreen(struct DotrStateVector *out, float x, float y);
extern int SzDuel_DoCardControl0(struct DotrReleaseSlot *c);
int lSzDuel_InitHandCombo(int side);

/* As declared in object_table.h (struct DotrObjectVector4 there; same four floats).
 * SzDuel_MoveUnit returns the moved item, or 0 when nothing moved; the shorts it clears
 * at item +0x34 / +0x36 are written through a cast of state_records.h's byte field. */
extern int SzModel_GetPos(int modelId, struct DotrStateVector *xyzCoords);
extern int SzModel_SetPos(int modelId, struct DotrStateVector *pos);
struct DotrStateItem *SzDuel_MoveUnit(int a, int b, int x, int y);

/* SzDuel_AddUnit2 (0x1df5f0). The five pairs at item +8 are copied as floats from
 * EffIdx_L (state_grid.h types both as words; cast at the use). The three vectors at
 * 0x2915d0 / 0x2915e0 / 0x2915f0 are copied whole into locals; the middle one is passed to
 * SzModel_SetRot and gets a word written at +4, so it is typed as four words. The model
 * setters are declared as their own groups declare them, with this group's vector
 * types; SzDuel_CalcUnitPos and lSzDuel_DoUnitRequest come from state_grid.h. */
struct DotrReleasePair {
    float unknown_00;
    float unknown_04;
};
/* The start of a struct DotrStateItem with the pairs typed as floats (state_records.h has
 * them as words); used for the leader's copy, where the original indexes from the item. */
struct DotrReleaseItemPairs {
    void *unknown_00; /* pointers, as in struct DotrStateItem, so host offsets agree */
    void *unknown_04;
    struct DotrReleasePair unknown_08[5];
};
struct DotrReleaseAngles {
    int x;
    int y;
    int z;
    int w;
};
extern unsigned short AlternateCardIds[854];
extern struct DotrStateVector data_002915d0;
extern struct DotrReleaseAngles data_002915e0;
extern struct DotrStateVector data_002915f0;
extern int SzData_SetLookFlag(int side, int id, int flag);
extern int SzModel_SetAnimDirect(int index, int slot);
extern int SzModel_SetAnimDir(int modelId, int animDir);
extern int SzModel_SetAnimSpeed(int index, float value);
extern int SzModel_SetRot(int index, struct DotrReleaseAngles *in);
extern int SzModel_SetRotC(int index, struct DotrStateVector *in);
extern int SzModel_SetRgba(int index, int value, void *in);
extern int SzModel_SetFilter(int index, int value, void *in);
/* SzDuel_AddUnit2 itself keeps the declaration its callers use
 * (state_records.h: int result, struct DotrStateBlock * source). The result is the new
 * unit's address as a 32-bit word; the block is read as a struct DotrStateItem. */

/* 0x1d8880 lSzDuel_InitHandDark (name from the Japanese build's symbol table): sets the three
 * colour bytes at +0x40 of each of the six hand objects to 64 when `flag` survives and the
 * side's summon power is below the card's level (SzDuel_GetUnitLevel2 of the object's item,
 * flag 0), else to 128; returns 1. The flag is dropped when menu number 29 has the side's
 * bit and the side's player is below 32 with the other side's at 100 or more, or when the
 * bit is clear and SzPad_Check(side, 1, 2) has no low-halfword bit. What those tests mean
 * is not established here. Callees as declared in state_grid.h and state_records.h. */
int lSzDuel_InitHandDark(int side, int flag);

#endif
