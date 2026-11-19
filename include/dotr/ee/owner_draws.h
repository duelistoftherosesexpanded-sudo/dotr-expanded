#ifndef DOTR_EE_OWNER_DRAWS_H
#define DOTR_EE_OWNER_DRAWS_H

/* Functions that fill a local 144-byte owner block with 0x1cd560 and draw it through
 * SzDuel_GetCardNodeOnScreen / SzDuel_SetupCard2_Pipe / SzDuel_DrawCard_Pipe (first member 0x1b83f0).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#ifdef __MWERKS__
typedef int DotrOwnerDrawWord;
#else
#include <stdint.h>
typedef intptr_t DotrOwnerDrawWord;
#endif

/* 144-byte block (the frame of 0x1b83f0 leaves exactly that much for it).
 * Member widths are those of the stores in 0x1cd560, which sets every member
 * below except unknown_58 and unknown_84; float members are the ones given
 * float constants (1.0f, 0.1f), the zero-only words are typed int by default.
 * The 64-bit member at +0x50 is one `sd`. draw_state.h types the first 0x84
 * bytes of the same block as DotrDrawRowOwner (colour, flag bit 15, count). */
struct DotrOwnerDrawBlock {
    int unknown_00[3];
    float unknown_0c;
    int unknown_10;
    int unknown_14[7];
    float unknown_30;
    float unknown_34;
    float unknown_38;
    int unknown_3c;
    unsigned char color[4];
    unsigned char unknown_44[4];
    short unknown_48;
    short unknown_4a;
    int unknown_4c;
    long unknown_50;
    unsigned char unknown_58[0x20];
    int unknown_78;
    int unknown_7c;
    short unknown_80_bits_0_11 : 12;
    short flag_12 : 1;
    short flag_13 : 1;
    short flag_14 : 1;
    short flag_15 : 1;
    short count : 12;
    unsigned short unknown_82_bits_12_13 : 2;
    short flag_30 : 1;
    short flag_31 : 1;
    unsigned char unknown_84[12];
};
typedef char DotrOwnerDrawBlockSizeMustBe144[(sizeof(struct DotrOwnerDrawBlock) == 144) ? 1 : -1];
typedef char DotrOwnerDrawLongMustBe64Bits[(sizeof(long) == 8) ? 1 : -1];

/* The control block at 0x317490, as far as 0x1b83f0 reads it: two words that
 * are passed to 0x1dbc40 and a byte copied to the block's +0x43. Other headers
 * hold other views of this storage under suffixed names. */
struct DotrOwnerDrawControl {
    unsigned char unknown_000[0x6c8];
    int first;
    int second;
    unsigned char unknown_6d0[0x66b];
    unsigned char alpha;
    /* Through 0x3181da, the last byte lSzDuel_InitHandUp (0x1d7d10) writes. */
    unsigned char unknown_d3c[0x14];
};
extern struct DotrOwnerDrawControl data_00317490;

/* Callee signatures are assumed from these call sites; other headers declare
 * some of them with other pointer types for the block. */
int SzDuel_ClearDrawCard(struct DotrOwnerDrawBlock *block);
extern DotrOwnerDrawWord SzDuel_StartCardSession_Pipe(DotrOwnerDrawWord cursor);
extern void SzDuel_SetCardDefPrioZ(int value);
extern int SzDuel_GetCardNodeOnScreen(struct DotrOwnerDrawBlock *block, int a, float x, float y);
extern DotrOwnerDrawWord SzDuel_SetupCard2_Pipe(DotrOwnerDrawWord cursor, int unit, struct DotrOwnerDrawBlock *block);
extern DotrOwnerDrawWord SzDuel_DrawCard_Pipe(DotrOwnerDrawWord cursor, struct DotrOwnerDrawBlock *block);
extern DotrOwnerDrawWord SzDuel_FinishCardSession_Pipe(DotrOwnerDrawWord cursor);

DotrOwnerDrawWord SzDuel_DrawJoinCard_Pipe(DotrOwnerDrawWord cursor);

/* 0x23bff0: a second 96-byte local is filled by SzDuel_InitCardInfo(&item, 999) and gets
 * the kind byte at +0x44; nothing else of its layout is known. The callees
 * are declared here with this function's argument types under suffixed
 * names where another header already holds a different view (0x205c40's float
 * result is not used). */
struct DotrOwnerDrawItem {
    unsigned char unknown_00[68];
    unsigned char kind;
    unsigned char unknown_45[27];
};
extern void func_001dcd70_owner(struct DotrOwnerDrawItem *item, int id);
extern float func_00205c40_owner(int *out);
extern void func_00205cb0_owner(struct DotrOwnerDrawBlock *block, float x, float y);
extern int SzDuel_GetCardNode(struct DotrOwnerDrawBlock *block, int a);
extern DotrOwnerDrawWord func_001dbc40_item(DotrOwnerDrawWord cursor, struct DotrOwnerDrawItem *item, struct DotrOwnerDrawBlock *block);
extern DotrOwnerDrawWord func_001d9e70_owner(DotrOwnerDrawWord cursor);
DotrOwnerDrawWord Taku_Camp_Card_Get(DotrOwnerDrawWord cursor, int kind, float x, float y, int value);

/* Taku_DECK_CARD_DISP (0x22a140, three deck cards sliding in and out). Its view of the
 * block at the literal 0x1ed0000 (glide_steps.h and camp_tikei.h hold other views):
 * member widths are those of the loads and stores, names are descriptive guesses. The
 * phase byte is read once through its literal address before the loop and through the
 * block pointer inside it; the host build maps both onto one object. The camp state's
 * stage halfword is camp_tikei.h's structure view of 0x299e10 (the source includes that
 * header). 0x217710 as in cue_steps.h; 0x1dcd70, 0x1dbc40 and 0x1d9e70 as declared above
 * for 0x23bff0. One int cursor in and out is assumed. */
struct DotrOwnerDeckBank {
    unsigned char unknown_0000[0x53a6];
    signed char step;
    char unknown_53a7;
    signed char top;
    signed char frac;
    signed char dir;
    char unknown_53ab[0x53];
    short cards[75];
    signed char phase;
    signed char last;
    signed char seen[3];
    char unknown_5499;
    short pos[3];
};
#ifdef __MWERKS__
#define DOTR_OWNER_DECK_BANK ((struct DotrOwnerDeckBank *)0x1ed0000)
#define DOTR_OWNER_DECK_PHASE (*(signed char *)0x1ed5494)
#else
extern struct DotrOwnerDeckBank dotr_owner_deck_bank_01ed0000;
#define DOTR_OWNER_DECK_BANK (&dotr_owner_deck_bank_01ed0000)
#define DOTR_OWNER_DECK_PHASE (dotr_owner_deck_bank_01ed0000.phase)
#endif
extern int SzSound_PlaySE(int index, int soundEffectNumber);
DotrOwnerDrawWord Taku_DECK_CARD_DISP(DotrOwnerDrawWord cursor);

/* SzDuel_DrawCardTest (0x1cea50). Its view of the same 144-byte block: the function
 * reads and writes the four flags as one-bit members of the byte at +0x81 (`lb`/`sb`
 * through a kept pointer, where the other members of this group use the halfword), copies
 * four floats from gSzDuel_dt+0x5c (0x31825c) to +0x10 and passes +0x20 to 0x205c40.
 * The vector type and member names are descriptive guesses. The session, setup and draw
 * callees are the non-pipe ones (draw_state.h declares them with int arguments);
 * SzPad_Check as in pad_toggles.h, SzDuel_Xax2New as in object_seeds.h. 0x1da2b0 and
 * 0x205cb0 are declared again under suffixed names with this view of the block: a
 * pointer cast at those two calls (the ones with float arguments) changes the order
 * of the generated argument set-up, a cast at the other calls does not. */
struct DotrOwnerDrawVec {
    float x, y, z, w;
};
struct DotrOwnerDrawCardBlock {
    struct DotrOwnerDrawVec pos;
    struct DotrOwnerDrawVec rot;
    struct DotrOwnerDrawVec look;
    float unknown_30;
    float unknown_34;
    float unknown_38;
    int unknown_3c;
    unsigned char unknown_40[0x40];
    char unknown_80;
    char unknown_81_bits_0_3 : 4;
    char flag_12 : 1;
    char flag_13 : 1;
    char flag_14 : 1;
    char flag_15 : 1;
    unsigned char unknown_82[14];
};
typedef char DotrOwnerDrawCardBlockSizeMustBe144[(sizeof(struct DotrOwnerDrawCardBlock) == 144) ? 1 : -1];
extern struct DotrOwnerDrawVec data_0031825c;
extern int data_00318270;
extern int SzDuel_Xax2New(int index);
extern int func_001da2b0_card(struct DotrOwnerDrawCardBlock *block, int a, float x, float y);
extern void func_00205cb0_card(struct DotrOwnerDrawCardBlock *block, float x, float y);
extern void SzDuel_StartCardSession(void);
extern void SzDuel_SetupCard(struct DotrOwnerDrawItem *item, struct DotrOwnerDrawCardBlock *block);
extern void SzDuel_DrawCard(struct DotrOwnerDrawCardBlock *block);
extern int SzPad_Check(int unknown, int zero, int mask);
extern void SzDuel_FinishCardSession(void);
void SzDuel_DrawCardTest(void);

#endif
