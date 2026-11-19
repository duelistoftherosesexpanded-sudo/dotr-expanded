#ifndef DOTR_EE_DIGIT_ROWS_H
#define DOTR_EE_DIGIT_ROWS_H

/* Functions that lay out a twenty-row page of labelled entries and draw each selected entry value
 * as digits in its own radix through the slot table (SzMsg_GetInfo, 0x1feab0) and the text
 * helpers SzMsg_LoadFont (0x1fd610) and SzMsg_DrawX (0x1fe750).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/game-modes.md. */

/* Callees. Each is defined in draw_state, slot_table or (SzMsg_DrawRect_Pipe)
 * cursor_packets with word-typed arguments; this header declares its own views
 * of them (an entry view for SzMsg_GetInfo, an int array for SzMsg_GetDefRect
 * and the third argument of SzMsg_DrawGlass, a function pointer for the fifth
 * argument of SzMsg_DrawX) so that the host test can pass host pointers. The
 * tests stub all of them. */
struct DotrDigitEntry {
    int unknown_00;
    int words[4];
};
extern void SzMsg_GetDefRect(int *destination);
extern struct DotrDigitEntry *SzMsg_GetInfo(int index);
extern int RhBase_GetUserGsAddress(void);
extern int SzMsg_Init(int a0, int a1);
extern void SzMsg_LoadFont(int a0, int a1, int a2, int a3);
extern void ExFlushTexture(void);
extern void SzMsg_DrawGlass(int a0, int a1, int *words, int a3);
extern void SzMsg_DrawX(int a0, int a1, int a2, int a3, void (*draw)(void));
extern void SzMsg_Free(int index);
extern void SzMsg_DrawRect_Pipe(void);

/* View of the storage at 0x317250 used by 0x1a6ff0: per-label words from
 * +0x10 (digit count), +0xa4 (tested for zero) and +0x138 (value), per-label
 * bytes from +0x1cc (radix), a byte at +0x223 (selected row) and a 3-bit
 * field in the byte at +0x22d (page). The bound 37 is the spacing of the
 * arrays, not an established count. The function reads the page through the
 * symbol and the rest through a pointer kept on the stack, hence the second
 * view symbol (same address; the same object on the host). */
struct DotrDigitView {
    unsigned char unknown_000[0x10];
    unsigned int count[37];
    int flag[37];
    unsigned int value[37];
    unsigned char radix[37];
    unsigned char unknown_1f1[0x32];
    unsigned char cursor;
    unsigned char unknown_224[9];
    unsigned char page : 3;
};
extern struct DotrDigitView gSzGame_gl_digits;
#ifdef __MWERKS__
extern struct DotrDigitView gSzGame_gl_digitview;
#else
#define gSzGame_gl_digitview gSzGame_gl_digits
#endif
/* Label number per row, 20 rows per page; rows at page * 20 + row >= 29 are
 * not read. The extent is not established. */
extern unsigned char DebugMenuOptions[];

/* Four floats; used only to spell the copy of four words as four float moves. */
struct DotrDigitWords {
    float x, y, z, w;
};

/* 0x1a6ff0. Takes the four words of 0x1fea80, the entries 20 and 21 and
 * SzMsg_Init(0, RhBase_GetUserGsAddress()); for the rows 0..19 of the page that are below
 * row 29 overall calls SzMsg_LoadFont(row, label + 284, row < 10 ? 0 : 320,
 * (row % 10 + 2) * 22), then SzMsg_LoadFont(20, 21, 0, 0) and
 * SzMsg_LoadFont(21, 24, 0, 22), stores 22 in word 2 of the entries 20 and 21 and
 * calls 0x208460. Then for each row (y = row * 22): SzMsg_GetInfo(row); when the
 * row is below 29 overall: for the selected row SzMsg_DrawGlass(0, y, words, 0)
 * with word 2 replaced by the entry's word 2, or by (count + 10) * 22 when
 * the label's flag is set; SzMsg_DrawX(row, 0, y, 0, 0x1fdfe0); and, when the
 * flag is set, the label's value as `count` digits in its radix, most
 * significant first, at x = 220 + 22 * position: a digit below 10 stores
 * digit * 22 in word 0 of entry 20 and draws slot 20, another stores
 * (digit - 10) * 22 in word 0 of entry 21 and draws slot 21. SzMsg_Free(row)
 * closes every row. Assumed: no arguments or result. */
void SzGame_DrawMenu(void);

#endif
