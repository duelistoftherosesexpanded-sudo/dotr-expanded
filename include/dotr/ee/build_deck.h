#ifndef DOTR_EE_BUILD_DECK_H
#define DOTR_EE_BUILD_DECK_H

/* The deck editor's per-frame input handler: one state machine over the editor's mode byte in the
 * duel control block.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* An address kept as a 32-bit word in the control block (the work area at +0x6d0). The host
 * build widens it so the test can point it at an object. */
#ifdef __MWERKS__
typedef int DotrBuildWord;
#else
typedef long DotrBuildWord;
#endif

/* View of the duel control block at 0x317490 with the fields 0x1b8840 uses. Names are this
 * function's uses of them; list_reset.h and small_misc.h have other views of the same block
 * under their own symbols. Offsets hold for the target build (4-byte pointers). Assumed:
 * every name; the two direction fields as signed two-bit fields of the byte at +0xd64. */
struct DotrBuildDeckControl {
    unsigned char pad000[0x6d0];
    DotrBuildWord work;
    unsigned char *table;
    unsigned short *bag;
    unsigned short *deck;
    unsigned char pad6e0[0xc86 - 0x6e0];
    unsigned short fadeA;
    unsigned short fadeB;
    unsigned short bagTop;
    unsigned short bagCount;
    unsigned short deckTop;
    unsigned short deckCount;
    short slide;
    short slideTo;
    unsigned short idle;
    unsigned char padc98[0xcb6 - 0xc98];
    unsigned char bagSort[3];
    unsigned char deckSort[3];
    unsigned char tabA;
    unsigned char tabB;
    unsigned char padcbe[4];
    char pass[8];
    unsigned char padcca[0xd32 - 0xcca];
    unsigned char reload;
    unsigned char padd33[2];
    unsigned char prevMode;
    unsigned char sel;
    unsigned char padd37[0xd57 - 0xd37];
    unsigned char step;
    unsigned char padd58[4];
    unsigned char side;
    unsigned char mode;
    unsigned char bagCur;
    unsigned char deckCur;
    unsigned char gridTop;
    unsigned char gridCol;
    unsigned char gridRow;
    unsigned char fadeOn;
    signed char dirA : 2;
    signed char dirB : 2;
};
extern struct DotrBuildDeckControl data_00317490_build;
#define DOTR_BUILD_DECK data_00317490_build

/* Callees as this function calls them (argument counts from the call sites; results used as
 * shown). SzPad_Check(pad, kind, mask): kind 0, 1 and 2 are three ways of asking about the
 * buttons in mask (assumed: pressed now, held, pressed with repeat). */
extern int SzData_GetCurrentDeck(int side);
extern int SzFile_InqRequestEx(void);
extern int SzText_LoadMerge(int a, int b);
extern int SzPad_Check(int pad, int kind, int mask);
extern int SzSound_PlaySE(int a, int soundEffectNumber);
extern int SzData_GetBagCount(int side, int cardNo, int kind);
extern int SzData_GetLookFlag(int side, int id);
extern void SzDuel_Init3DView(int id);
extern void SzGame_Push(int a, int b);
extern void SzGame_Pop(void);
extern void SzPad_ResetCounter(int pad, int mask);
extern int SzData_CheckDeck(int side, int deckNo, int cardNo);
extern int Taku_Get_Duel5_flg(void);
extern void Taku_Set_Duel5_flg(void);
extern int lSzDuel_IsDeckReady(int side, int deckNo);
extern int SzData_GetRank(int side, int cardNo);
extern void SzData_PullDeckLeader(int side, int deckNo);
extern int SzData_PushDeckLeader(int side, int deckNo, int cardNo);
extern void lSzDuel_ReqGhostCard(int id, int row, int a, int b, int c);
extern void SzText_Free(int a, int b);
extern int SzData_PushDeck(int side, int deckNo, int cardNo);
extern int SzData_PullDeck(int side, int deckNo, int deckSlot);
extern void lSzDuel_SortBag(void);
extern void lSzDuel_SortDeck(void);
extern void SzData_SetCurrentDeck(int side, int deckNo);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzData_GetCardNo(int side, int deckNo, int cardSlot);
extern int Ta_GetRecycle(int side, int id);
extern void SzData_DelBag(int side, int cardNo);
extern void SzData_AddBag(int side, int a, int cardNo);
extern void lSzDuel_ReBuildBag(void);
extern int lSzDuel_HitPassword(char *text);
extern int SzData_GetPassFlag(int side, int cardNo);
extern void SzData_SetPassFlag(int side, int n, int v);
extern int SzPad_IsAnalog(int pad);
extern int SzPad_GetAnalogInfo(int pad, int which);

/* 0x1b8840 SzDuel_ActBuildDeck (name from the Japanese build's symbol table). One frame of
 * the deck editor: advances the two colour pulses, slides the panel towards its target and
 * returns while it moves, otherwise runs the state for the mode byte (0 the card grid, ten
 * numbers to a row, 1 and 6 the two tab rows, 2/3 the bag list, 4/5 the deck list, 7/8
 * recycling a card into three, 9 password entry, 10-13 message boxes), then applies
 * analog-stick and page scrolling to the two lists. */
void SzDuel_ActBuildDeck(void);

#endif
