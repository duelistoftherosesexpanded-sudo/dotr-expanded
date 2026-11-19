#ifndef DOTR_EE_SLOT_TABLE_H
#define DOTR_EE_SLOT_TABLE_H

/* Message text and font functions around gSzMsg_ml (0x31c070).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md.
 */
#include "dotr/ee/draw_state.h"

typedef char DotrSlotTableIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];

/* 20-byte entry; only bit 31 of the first word is written by the code here. */
struct DotrSlotTableEntry {
    unsigned int unknown_000 : 31;
    signed int flag : 1;
    int unknown_004[4];
};
typedef char DotrSlotTableEntrySizeMustBe20[(sizeof(struct DotrSlotTableEntry) == 20) ? 1 : -1];

struct DotrSlotTable {
    int words[7];
    struct DotrSlotTableEntry entries[30];
    int unknown_274;
    unsigned short unknown_278;
};
extern struct DotrSlotTable gSzMsg_ml;

/* 0x1fd320 reads the leading 28 bytes as seven words (width 640, height 384,
 * their power-of-two shifts, a base index from 0x202f00, the advanced a1 and
 * a0) and indexes the halfword table 0x2d49d4 with the word at 0x2d19d0.
 * Element types are assumed from the access widths. The signature of 0x1fd4c0
 * (draw_state) is the one this call site shows. */
extern int StringOffsetTable;
extern unsigned short StringBlob[];
extern int SzText_InitClutManage(int base, int size);
extern int SzText_GetClutGsAddr(int count);
extern DotrDrawWord SzMsg_LoadClut_Pipe(DotrDrawWord cursor, int index, int slot);

/* Four initialized words copied as one object by 0x1fea80. */
struct DotrSlotTableWords {
    int words[4];
};
extern struct DotrSlotTableWords gSzMsg_defRect[];

/* 0x1fe7d0 copies the four words as one quad (lq/sq) via a 128-bit view and
 * calls its a4 argument, assumed to be a draw callback, with the words by value. */
#ifdef __MWERKS__
typedef __int128 DotrSlotWord128;
#else
__extension__ typedef __int128 DotrSlotWord128;
#endif
union DotrSlotArgsUnion {
    DotrSlotWord128 quad;
    struct DotrSlotTableWords words;
};
typedef DotrDrawWord (*DotrSlotDrawFn)(DotrDrawWord, int, int, int, struct DotrSlotTableWords, int);

/* 0x1fdb90. Traces one word of message text into per-line layout arrays and answers how
 * many words it used. A word with bit 14 is a reference (2 words): its low 6 bits are a
 * count, bits 6-12 a start and the next word's low 14 bits index the word table at
 * 0x2d19d0, whose entry is an offset into the halfword table 0x2d49d4; the function
 * traces from there until the count is used up or the last traced word has bit 15.
 * Otherwise the low 13 bits are a character: the one equal to the halfword at +0x278 of
 * gSzMsg_ml adds 1 to *line; any other adds its width (12 with bit 13, else 22) to the
 * line's width, stores half the width and the character at the line's column and adds 1
 * to the column. Assumed: 53 columns per line from the index scaling, element types from
 * the access widths, int result. */
int lSzMsg_FontTrace(unsigned short *compressedText, char (*charWidths)[53], short (*code)[53], unsigned short *cols,
                     unsigned short *widths, unsigned short *line);
/* 0x1fd680. The 20-byte entry as SzMsg_LoadFont_Pipe writes it: the first word holds the
 * widest line's column count, the texture width and height (powers of two) and the flag
 * that DotrSlotTableEntry names; the four words are x, y, width and height (the ones
 * SzMsg_DrawRect_Pipe reads). Member names are from these uses, not original. */
struct DotrSlotFontEntry {
    unsigned int cols : 8;
    unsigned int tw : 12;
    unsigned int th : 11;
    signed int flag : 1;
    int x, y, w, h;
};
typedef char DotrSlotFontEntrySizeMustBe20[(sizeof(struct DotrSlotFontEntry) == 20) ? 1 : -1];
/* Glyph images indexed by character code: 242 bytes each, read as 22 rows of 11. The
 * element type is assumed from the byte reads and the two strides. */
extern unsigned char gSzMsgImage[][242];
/* Types SzMsg_LoadFont_Pipe uses: the slot entry (the same 20 bytes as DotrSlotFontEntry) and the
 * texture context at 0x31c070 (gSzMsg_ml's leading words: width at +0, tbp at +0x14). The field
 * meanings are assumed. */
struct DotrSlotFontTexture {
    unsigned int count : 8;
    unsigned int width : 12;
    unsigned int height : 11;
    int used : 1;
    int x;
    int y;
    int pixelWidth;
    int pixelHeight;
};
struct DotrSlotFontContext {
    int width;
    unsigned char pad[0x10];
    int tbp;
};
/* Assumed: cursor type as the other packet writers, int msgInfoIndex, stringIndex, x and y. */
DotrDrawWord SzMsg_LoadFont_Pipe(DotrDrawWord cursor, int msgInfoIndex, int stringIndex, int x, int y);
void SzMsg_GetDefRect(struct DotrSlotTableWords *destination);
struct DotrSlotTableEntry *SzMsg_GetInfo(int index);
DotrDrawWord SzMsg_DrawX_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3, int a4);
void SzMsg_Free(int index);
void SzMsg_FreeAll(void);

/* Two 64-bit words 0x1fdd70 fills. The field widths are those of the GS TEX0
 * and TEX1 registers and are named after them; that the words are sent to
 * those registers is not shown by this function. Padding members are never
 * written. */
struct DotrGsTex0 {
    unsigned long tbp0 : 14;
    unsigned long tbw : 6;
    unsigned long psm : 6;
    unsigned long tw : 4;
    unsigned long th : 4;
    unsigned long tcc : 1;
    unsigned long tfx : 2;
    unsigned long cbp : 14;
    unsigned long cpsm : 4;
    unsigned long csm : 1;
    unsigned long csa : 5;
    unsigned long cld : 3;
};
struct DotrGsTex1 {
    unsigned long lcm : 1;
    unsigned long pad_1 : 1;
    unsigned long mxl : 3;
    unsigned long mmag : 1;
    unsigned long mmin : 3;
    unsigned long mtba : 1;
    unsigned long pad_10 : 9;
    unsigned long l : 2;
    unsigned long pad_21 : 11;
    unsigned long k : 12;
    unsigned long pad_44 : 20;
};
/* 0x1fdd70. When tex0 is not null: tbp0 = words[5], tbw = words[0] >> 6,
 * psm = 20, tw = words[2], th = words[3], tcc = 1, tfx = 0,
 * cbp = words[4] + words[6], cpsm = csm = csa = 0, cld = 1 (words of the
 * table at 0x31c070). When tex1 is not null: its named fields are cleared.
 * Assumed: the two pointer types, no result. */
void lSzMsg_GetTexInfo(struct DotrGsTex0 *tex0, struct DotrGsTex1 *tex1);

#endif
