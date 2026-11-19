#ifndef DOTR_EE_MEMCARD_PROC_H
#define DOTR_EE_MEMCARD_PROC_H

/* SzGame_ActMemCardProc, the per-frame step of the memory-card dialogue over the game block's
 * kind, state and message fields.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/game-modes.md. */

/* View of the game block gSzGame_gl at 0x317250 with the memory-card dialogue's fields. It is
 * DotrKindStateView of kind_dispatch.h (the view of SzGame_InitMemCardProc, which starts the
 * dialogue) with the message word added and the halfword at +0x21c read unsigned, as this
 * function reads it. Assumed: every name; `msg` as the text number shown (negative: none),
 * `res` as the last result of the card routine (high byte: its state), `kind` as which card
 * operation runs (0-6), `state` as the dialogue step (0 idle) and `second` as the yes/no
 * cursor; unsigned bit-fields (they are read with zero extension). The bit-field positions
 * (kind at bits 3-5 of +0x22d; state at bits 0-2 and second at bit 3 of +0x22e) are those
 * of the target compiler; the host test checks them for the host compiler. */
struct DotrMemCardProcView {
    unsigned char pad000[0x210];
    int msg;                            /* 0x210 */
    unsigned char pad214[8];
    unsigned short res;                 /* 0x21c */
    unsigned char pad21e[0xf];
    unsigned char b22d_0 : 3;           /* 0x22d */
    unsigned char kind : 3;
    unsigned char b22d_6 : 2;
    unsigned char state : 3;            /* 0x22e */
    unsigned char second : 1;
    unsigned char b22e_4 : 4;
};

/* Two symbols for the one block at 0x317250, as the rule for incompatible views of
 * shared storage asks (docs/DECOMPILATION.md): the function tests the state once through
 * the structure symbol and does everything else through a pointer made by casting a
 * byte-array symbol. Both link at the same address in the target. On the host there is one
 * object (gSzGame_gl_mcview) and the byte symbol is a cast of it, so both spellings reach
 * the same fixture. */
extern struct DotrMemCardProcView gSzGame_gl_mcview;
#ifdef __MWERKS__
extern signed char gSzGame_gl_mcbytes[];
#else
#define gSzGame_gl_mcbytes ((signed char *)&gSzGame_gl_mcview)
#endif

/* Callees as this function calls them. The six card routines, lSzGame_GetMCardMsg,
 * SzFile_InqRequestEx and the two popup calls are declared as in the groups that define
 * them (small_misc.h, mode_steps.h, text_picks.h, resource_helpers.h, state_grid.h).
 * Assumed: the card routines take the step (state - 2, or 1 to begin a save) and return a
 * result whose high byte is 0 while a message is shown, 1 on an error and 7 when a yes/no
 * answer is wanted; lSzGame_GetMCardMsg(kind, result, &text) stores the text number and
 * returns the message class (0 none, 2 yes/no, 3 and others acknowledge);
 * SzPad_Check(pad, kind, mask) as in build_deck.h; SzSound_PlaySE(0, n) with 3 cursor,
 * 4 confirm, 5 cancel; SzDuel_OpenMsgPopup(operation, 0, 0, text). */
extern int SzFile_InqRequestEx(void);
extern int TkMc_Load_Main(int mode);
extern int TkMc_Save_Main(int mode);
extern int TkMc_Custom_Main(int mode);
extern int TkMc_Trade_Load_Main(int mode);
extern int TkMc_Trade_Save_Main(int mode);
extern int TkMc_Check_Main(int mode);
extern int lSzGame_GetMCardMsg(int kind, int flags, int *out);
extern int SzPad_Check(int pad, int kind, int mask);
extern int SzSound_PlaySE(int a, int soundEffectNumber);
extern void SzDuel_CloseMsgPopup(void);
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);

/* 0x1ab1d0 SzGame_ActMemCardProc (name from the Japanese build's symbol table; prototype as
 * pad_toggles.h and movie_operations.h declare it for their callers). One frame of the
 * memory-card dialogue. Returns -1 when the dialogue is idle (state 0) and 0 while it runs
 * or a file request is pending; in the frame the text number goes negative it closes the
 * popup and returns 2 when the dialogue finished through its own end sequence, else 1.
 * Otherwise it shows the text with SzDuel_OpenMsgPopup: operation 6 for message class 1 or
 * 3, 7 for class 2 (bit 15 of the text number carries the yes/no cursor), 5 for others. */
int SzGame_ActMemCardProc(void);

#endif
