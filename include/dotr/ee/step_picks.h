#ifndef DOTR_EE_STEP_PICKS_H
#define DOTR_EE_STEP_PICKS_H

/* Copies that store a position in a result record, ask a per-copy kind callee and then choose
 * among per-copy pick callees before a per-copy finishing call.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The result record these copies fill: a halfword at +0, a byte at +2, a
 * halfword at +4 and a word at +0xc (the offsets of lower_rules.h's result
 * view). Nothing else is written here. */
struct DotrStepOut {
    unsigned short position;
    unsigned char kind;
    unsigned char unknown_03;
    unsigned short loc;
    unsigned short unknown_06;
    int unknown_08;
    int shouldFlipUp;              /* 0 face down, 1 face up, 2 no change */
};
/* The 96-byte local handed to the callees; only the halfword at +0x3c is read
 * here. Its size is taken from the frame. */
struct DotrStepBlock {
    unsigned char unknown_00[0x3c];
    unsigned short unknown_3c;
    unsigned char unknown_3e[0x22];
};

/* Shared callees, declared with the arguments these callers pass. 0x276f10
 * (driver_scans) takes ten arguments, the last two read from the stack. */
extern int SzDuel_InqHandPhase(int side);
extern int TaLogic_GetUnitNum(int side, int countSpells);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitMonsterCount(int side);
extern int TaLogic_GetNearCard2(struct DotrStepBlock *block, int a, int b, int c, int d, int e, int f, int g, int h, int i);
/* Per-copy callees, all as `_step` views with this group's record and block
 * types; their own sources in other groups (for example pick_copies.h,
 * lower_rules.h, position_rules.h, low_leaves.h) declare other parameter
 * types. Results are assumed int. */
extern int func_00114310_step(int side, struct DotrStepBlock *block);
extern int func_00114d40_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00117c30_step(int side);
extern int func_00115340_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00115160_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00114b40_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
extern int func_001280c0_step(int side, struct DotrStepBlock *block);
extern int func_00128720_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0012b1b0_step(int side);
extern int func_00128d40_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00128b50_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00128520_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
extern int func_001326f0_step(int side, struct DotrStepBlock *block);
extern int func_00132d00_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00134ae0_step(int side);
extern int func_00133320_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00133130_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00132b00_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
extern int func_00164680_step(int side, struct DotrStepBlock *block);
extern int func_00164ce0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_001676d0_step(int side);
extern int func_001652f0_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00165100_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00164ae0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
extern int func_00168a30_step(int side, struct DotrStepBlock *block);
extern int func_00169000_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0016b0e0_step(int side);
extern int func_00169610_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00169420_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00168e00_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
/* Per-copy callees of the three 648-byte copies, as `_step` views too; their
 * own sources in other groups declare other parameter types. */
extern int func_0014ce30_step(int side, struct DotrStepBlock *block);
extern int func_0014d3e0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0014f9b0_step(int side);
extern int func_0014da40_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_0014d800_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0014d1e0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
extern int func_0015a5f0_step(int side, struct DotrStepBlock *block);
extern int func_0015ab10_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0015d120_step(int side);
extern int func_0015af30_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_0015ad00_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0015a910_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
extern int func_0016f760_step(int side, struct DotrStepBlock *block);
extern int func_0016fef0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00172310_step(int side);
extern int func_00170620_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_001703e0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0016fcf0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);

/* Five copies differing in the callees, a bound N (4, 5, 5, 6, 6) and two
 * constants A, B (3, 0 for the first three; 2, 1 for the last two). Stores the
 * position at +0. -1 when SzDuel_InqHandPhase(side) is 0, when TaLogic_GetUnitNum(side, 0) and
 * TaLogic_GetUnitNum(side, 1) are both 5, or when kind = kind callee(side, &block) is
 * negative. Otherwise stores 10 at +2 and 0 at +0xc and takes the 0x1e1090
 * halfword of the position. Kind 0 with a non-negative pick callee answer, and
 * every other kind, compute score = TaLogic_GetNearCard2(&block, low byte, high byte,
 * side == 0, A, 31, 2, 2, B, level callee(side)). Kind 0 with that pick
 * answer and (score >= 0 or SzDuel_GetUnitMonsterCount(side) < N) goes straight to the finish.
 * Otherwise kind 2 calls the kind-2 callee and any other kind the pass callee
 * with the block's +0x3c halfword, a negative answer giving -1. The finish:
 * finishing callee(side, kind, &block, score), negative gives -1, else the
 * answer is stored at +4 and 1 returned. Kind 0 with a negative pick answer
 * reaches the finish with score never assigned. */
int TaLogic_GetMinimumTask_Summon__00114080(int position, int side, struct DotrStepOut *out);
int TaLogic_GetMinimumTask_Summon__00127e30(int position, int side, struct DotrStepOut *out);
int TaLogic_GetMinimumTask_Summon__00132460(int position, int side, struct DotrStepOut *out);
int TaLogic_GetMinimumTask_Summon__001643f0(int position, int side, struct DotrStepOut *out);
int TaLogic_GetMinimumTask_Summon__001687a0(int position, int side, struct DotrStepOut *out);

/* Three more copies that store 2 instead of 0 at +0xc (648 bytes): bound N =
 * 6, 6, 5 and constants A, B = 3, 0. Otherwise as above. */
int TaLogic_GetMinimumTask_Summon__0014cba0(int position, int side, struct DotrStepOut *out);
int TaLogic_GetMinimumTask_Summon__0015a360(int position, int side, struct DotrStepOut *out);
int TaLogic_GetMinimumTask_Summon__0016f4d0(int position, int side, struct DotrStepOut *out);

/* A ninth copy, TaLogic_GetMinimumTask_Summon__0013ab70 (644 bytes): as
 * TaLogic_GetMinimumTask_Summon__001643f0 (N = 6, A = 2, 0 at +0xc) with B = 0 and its own callees;
 * the level callee TaLogic_GetWarnHideLvl__0013da70 (position_rules) is called here through a view.
 * The other five have signatures assumed as for the other copies. */
extern int func_0013ae00_step(int side, struct DotrStepBlock *block);
extern int func_0013b420_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0013da70_step(int side);
extern int func_0013ba30_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_0013b840_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0013b220_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__0013ab70(int position, int side, struct DotrStepOut *out);
/* 0x155770 has the shape of the copies above with two differences: the
 * 0x276f10 score (arguments 3 and 0, level from 0x158780) is taken before the
 * kind is tested, and kind 3 has its own callee (0x155c90) like kind 2. Its
 * callees as `_step` views with assumed prototypes; their own sources declare
 * other parameter types. */
extern int func_00155a00_step(int side, struct DotrStepBlock *block);
extern int func_001564e0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00155c90_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00156910_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_001562e0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
extern int func_00156b40_step(int side, unsigned char kind, struct DotrStepBlock *block, int s);
extern int func_00158780_step(int side);
int TaLogic_GetMinimumTask_Summon__00155770(int position, int side, struct DotrStepOut *out);
/* 0x15f300 is a copy of the 0x1687a0 shape (word 2 at +0xc, score arguments 3
 * and 0, level from 0x162580, 0x1e0150 bound 5) with a kind-3 callee
 * (0x15fa00) added like 0x155770's. Its callees as `_step` views with assumed
 * signatures; their own sources declare other parameter types. */
extern int func_0015f5e0_step(int side, struct DotrStepBlock *block);
extern int func_0015fe50_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00162580_step(int side);
extern int func_001604b0_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00160270_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0015fa00_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0015fc50_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__0015f300(int position, int side, struct DotrStepOut *out);

/* 0x148020 and 0x150c40 are copies of 0x15f300 with word 0 at +0xc and
 * 0x1e0150 bounds 4 and 6. Their callees as `_step` views with assumed
 * signatures; their own sources declare other parameter types. */
extern int func_001482f0_step(int side, struct DotrStepBlock *block);
extern int func_00148b60_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0014b330_step(int side);
extern int func_001491b0_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00148f80_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00148630_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00148960_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__00148020(int position, int side, struct DotrStepOut *out);
extern int func_00150f10_step(int side, struct DotrStepBlock *block);
extern int func_001516a0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00153ed0_step(int side);
extern int func_00151cf0_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00151ac0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00151170_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_001514a0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__00150c40(int position, int side, struct DotrStepOut *out);

/* TaLogic_GetMinimumTask_Summon__0011dfb0 (692 bytes) has the shape of
 * TaLogic_GetMinimumTask_Summon__00148020 (bound 4, word 0) with three differences: the 0x276f10
 * score is only tested, never kept; the finishing callee 0x11e270 takes (side, kind, &block) only;
 * kinds 2 and 3 share one callee, 0x11f3c0, with a fourth argument 0 or 1. Callees as `_step` views
 * with assumed prototypes; their own sources declare other parameter types. */
extern int func_0011e6e0_step(int side, struct DotrStepBlock *block);
extern int func_0011ef60_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0011fa80_step(int side);
extern int func_0011e270_step(int side, unsigned char kind, struct DotrStepBlock *block);
extern int func_0011f3c0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int mode);
extern int func_0011ed50_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__0011dfb0(int position, int side, struct DotrStepOut *out);

/* TaLogic_GetMinimumTask_Summon__00122d30 (724 bytes) has the shape of
 * TaLogic_GetMinimumTask_Summon__00148020 (bound 4, word 0) with 0x276f10 arguments 2 and 0, kinds
 * 3 and 2 (tested in that order) sharing 0x124c90 with a fourth argument 1 or 0, and the finish
 * after the pass callee given -1 instead of the score. Callees as `_step` views with assumed
 * signatures; their own sources declare other parameter types. */
extern int func_00123740_step(int side, struct DotrStepBlock *block);
extern int func_00124810_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_001254a0_step(int side);
extern int func_00123010_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00124c90_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int mode);
extern int func_00124610_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__00122d30(int position, int side, struct DotrStepOut *out);

/* TaLogic_GetMinimumTask_Summon__0013eee0 (652 bytes) takes the score before the kind test like
 * 0x155770, with bound 4. Differences: the kind-0 finishes pass a literal 0 as the kind; kind
 * 0 that does not finish goes on at the kind-3 test (kind 2 is an else branch
 * of kind 0); the pass callee's answer, when not negative, is the kind byte
 * given to the last finish together with -1 as the score. Callees as `_step`
 * views with assumed prototypes; their own sources declare other parameter
 * types. */
extern int func_0013f170_step(int side, struct DotrStepBlock *block);
extern int func_0013f850_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0013f3b0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00141b30_step(int side);
extern int func_0013feb0_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_0013fcc0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0013f640_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__0013eee0(int position, int side, struct DotrStepOut *out);

/* TaLogic_GetMinimumTask_Summon__00143ba0 (576 bytes) is 0x13eee0 without the kind-3 callee: kind 0 that does
 * not finish, and every kind but 2, goes to the pass callee. Callees as `_step`
 * views with assumed prototypes; their own sources declare other parameter
 * types. */
extern int func_00143de0_step(int side, struct DotrStepBlock *block);
extern int func_00144450_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_001461c0_step(int side);
extern int func_00144a60_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00144860_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00144250_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__00143ba0(int position, int side, struct DotrStepOut *out);

/* 0x10dd50: the step shape with bound 6, a kind callee that refuses with
 * exactly -1, a three-argument pass callee and a kind 2 that ends without the
 * finish (0 stored at +4). Callees as `_step` views with assumed prototypes;
 * their own sources declare other parameter types. */
extern int func_0010dfc0_step(int side, struct DotrStepBlock *block);
extern int func_0010e320_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0010eda0_step(int side);
extern int func_0010e890_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_0010e540_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int mode);
extern int func_0010f450_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
int TaLogic_GetMinimumTask_Summon__0010dd50(int position, int side, struct DotrStepOut *out);

/* 0x136210: the 0x13ab70 shape with bound 4 and 0x276f10 given 3, 1 and a
 * literal 0 where the other copies pass a level callee's answer. Callees as
 * `_step` views with assumed prototypes; their own sources declare other
 * parameter types. */
extern int func_00136490_step(int side, struct DotrStepBlock *block);
extern int func_00136b80_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00137120_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_00136f30_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_00136980_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__00136210(int position, int side, struct DotrStepOut *out);

/* 0x119300: the 0x11dfb0 shape with bound 6 and no kind-2 or kind-3 callee:
 * every kind that does not finish in kind 0 goes to the pass callee. The finish
 * takes three arguments. Callees as `_step` views with assumed prototypes;
 * their own sources declare other parameter types. */
extern int func_00119a40_step(int side, struct DotrStepBlock *block);
extern int func_0011a0d0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0011aa10_step(int side);
extern int func_00119520_step(int side, unsigned char kind, struct DotrStepBlock *block);
extern int func_00119d30_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__00119300(int position, int side, struct DotrStepOut *out);

/* 0x12d270: the 0x143ba0 shape with a fourth argument handed to the kind
 * callee TaLogic007_IsHandPlay (called `limit` here; its meaning is not known). The score
 * is taken before the kind test and again after an accepted kind-0 pick;
 * bound 6; the finish after the pass callee gets the pass answer as kind and
 * the score. The `_step` views give callees defined in other groups this
 * group's block and output types (assumed from the shared stack block). */
extern int func_0012d520_step(int side, struct DotrStepBlock *block, int limit);
extern int func_00130780_step(int side);
extern int func_0012dde0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0012e3f0_step(int side, unsigned char kind, struct DotrStepBlock *block, int score);
extern int func_0012e1f0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block);
extern int func_0012dbe0_step(int side, struct DotrStepOut *out, struct DotrStepBlock *block, int id);
int TaLogic_GetMinimumTask_Summon__0012d270(int position, int side, struct DotrStepOut *out, int limit);

#endif
