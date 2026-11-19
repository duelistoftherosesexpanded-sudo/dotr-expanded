#ifndef DOTR_EE_PRESET_WAITS_H
#define DOTR_EE_PRESET_WAITS_H

/* 0x260320: a step routine over the preset queue object of InitEffectTemp /
 * TaView_RestoreOriginalView (counters at +0, +4, +0x102).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

#include "dotr/ee/preset_words.h"

/* The first argument of Ta_Eff_WAIT: only the words at +8 (compared with
 * 0xffff, passed on as TaView_ReqFocusCamera's second argument) and +0xc (an unsigned
 * bound) are read. */
struct DotrWaitOwner {
    int unknown_00[2];
    int unknown_08;
    unsigned int unknown_0c;
};

/* Callees as declared in their own groups (state_records, cell_sends,
 * state_grid); InitEffectTemp and TaView_RestoreOriginalView come from preset_words.h. */
extern void SzDuel_SetWindowDraw(int value);
extern int SzDuel_GetWindowDisp(void);
extern void TaView_ReqFocusCamera(unsigned char who, int packed, int add, int value);
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
extern void SzDuel_CloseMsgPopup(void);

/* `queue` is the object InitEffectTemp and TaView_RestoreOriginalView take, read here as
 * halfwords: counters at +0, +4 and +0x102 (unsigned loads, so an unsigned
 * view; preset_words.h has the last one signed). */
int Ta_Eff_WAIT(struct DotrWaitOwner *owner, unsigned short *queue, int mode);

/* Ta_Eff_018's view of the same queue object: halfword counters at +0 (a level,
 * 48..128, whose low byte goes to SzEffect_SetBlackOutForDuel), +4 (the stage) and +0x102,
 * a byte list at +6 and its count at +0xd4. */
struct DotrWaitQueue {
    unsigned short unknown_00;
    unsigned short unknown_02;
    unsigned short unknown_04;
    unsigned char unknown_06[0xce];
    unsigned char unknown_d4;
    unsigned char unknown_d5[0x2d];
    unsigned short unknown_102;
};
/* Callees as declared in index_fanout, pool_table and side_table, with the queue
 * and the four-word job (id, three zero words) under this group's types. Ta_IsFinishDestEfc
 * is defined with two parameters; the original passes the job as a third here.
 * For Ta_Do_Effect, Ta_ManageDestroyEffect and Ta_SetEffectDependOnFlagLst the signatures follow
 * the argument registers set here. */
extern void SzEffect_SetBlackOutForDuel(int blackoutAmount);
extern int SzEffect_IsUse(int index);
extern int Ta_IsFinishFade(void);
extern void Ta_AllUnitFade(unsigned char *mask, unsigned char *owner, int c, int dir, int flag, int value);
extern int Ta_Do_Effect(int *job, struct DotrWaitQueue *queue);
extern int Ta_ManageDestroyEffect(void *owner, struct DotrWaitQueue *queue, int *job);
extern int Ta_IsFinishDestEfc(void *owner, struct DotrWaitQueue *queue, int *job);
extern void Ta_SetEffectDependOnFlagLst(void *owner, struct DotrWaitQueue *queue, int *job, int id);
int Ta_Eff_018(void *owner, struct DotrWaitQueue *queue, int mode);
/* Same shape and assumptions as Ta_Eff_018; they differ in the ids sent and in
 * where TaView_ReqStdCompleteView(16) is called. Ta_Eff_016 also waits on the +0x102 counter
 * in stage 0. */
int Ta_Eff_017_A(void *owner, struct DotrWaitQueue *queue, int mode);
int Ta_Eff_016(void *owner, struct DotrWaitQueue *queue, int mode);
/* Four stages (no job of its own, no SzEffect_IsUse wait); same assumptions. */
int Ta_Eff_021(void *owner, struct DotrWaitQueue *queue, int mode);
/* Ta_Eff_021 with TaView_ReqFlatCompleteView(16), id 523 and the +0x102 wait in stage 0. */
int Ta_Eff_023(void *owner, struct DotrWaitQueue *queue, int mode);
/* View of the wait queue used by Ta_Eff_019: a word per sent job at +0x20, indexed
 * like the id bytes at +6 (only word 0 is read back). Field types are assumptions. */
struct DotrWaitFlagQueue {
    unsigned short unknown_00;
    unsigned short unknown_02;
    unsigned short unknown_04;
    unsigned char unknown_06[26];
    int unknown_20[45];
    unsigned char unknown_d4;
};
/* The owner's +8 word as Ta_Eff_019 shifts it (unsigned; struct DotrWaitOwner has it signed). */
struct DotrWaitFlagOwner {
    int unknown_00[2];
    unsigned int unknown_08;
};
extern int SzEffect_GetStatus(int index);
extern int Ta_DelUnit(int side, int pos);
/* Three stages; owner is read as struct DotrWaitFlagOwner (+8: 22 flag bits). */
int Ta_Eff_019(void *owner, struct DotrWaitQueue *queue, int mode);
extern int Ta_GetMinimumOrderUnitFromFlagList(int flagList);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int Ta_IsUnitControl(int a, int b);
extern void SzDuel_AddUnit(int side, int col, int row, int cardNo, int e);
extern int SzEffect_SetBusyFlag2(int packed, int flag);
/* Three stages, a `switch`; owner is read as struct DotrWaitOwner (+8, +0xc), the queue's
 * +2 word holds Ta_GetMinimumOrderUnitFromFlagList's packed result and +0x20 a done flag
 * (DotrWaitFlagQueue). */
int Ta_Eff_060(void *owner, struct DotrWaitQueue *queue, int mode);

#endif
