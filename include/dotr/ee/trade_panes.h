#ifndef DOTR_EE_TRADE_PANES_H
#define DOTR_EE_TRADE_PANES_H

/* Trade screen card, cursor and ready panes that draw the card trade between two players.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A packet cursor or an address kept as a 32-bit EE word (pointer-sized on the host). */
#ifdef __MWERKS__
typedef int DotrTradePaneWord;
#else
#include <stdint.h>
typedef intptr_t DotrTradePaneWord;
#endif

/* The code compares pointers with NULL, defined as 0 here. */
#ifndef NULL
#define NULL 0
#endif

/* The duel control block at 0x317490, read as bytes (.b) or by field (.s). Assumed: field names
 * and meanings from use. */
typedef struct DotrTradePaneDuel317490_s {
    unsigned char players[2][0x350];          /* 0x000: one record per player (callers use their own views) */
    unsigned char pad6A0[0x6D0 - 0x6A0];
    struct {
        unsigned char pad00[0x20];
        unsigned short ids[3];                /* 0x20: the three cards picked */
    } *pick;                       /* 0x6D0 */
    unsigned char pad6D4[0x6D8 - 0x6D4];
    unsigned short *list;                     /* 0x6D8: the trunk list */
    unsigned short *list2;                    /* 0x6DC: the deck list (40) */
    unsigned char pad6E0[0xC58 - 0x6E0];
    int depth;                     /* 0xC58: GS z of the duel screen sprites */
    unsigned char padC5C[0xC86 - 0xC5C];
    unsigned short wave1;                     /* 0xC86 */
    unsigned short wave2;                     /* 0xC88 */
    unsigned short offset;                    /* 0xC8A */
    unsigned short count;                     /* 0xC8C */
    unsigned short offset2;                   /* 0xC8E */
    unsigned short count2;                    /* 0xC90 */
    short scroll;                    /* 0xC92 */
    short scrollTo;                  /* 0xC94 */
    unsigned short idle;                      /* 0xC96 */
    unsigned char padC98[0xCB6 - 0xC98];
    unsigned char sort[3];                    /* 0xCB6 */
    unsigned char sort2[3];                   /* 0xCB9 */
    unsigned char sort3;                      /* 0xCBC */
    unsigned char sort4;                      /* 0xCBD */
    unsigned char pstate[4];                  /* 0xCBE: per-player state bytes, [player] */
    char code[8];                  /* 0xCC2: password digits */
    unsigned char padCCA[0xD32 - 0xCCA];
    unsigned char dirty;                      /* 0xD32 */
    unsigned char padD33[0xD35 - 0xD33];
    unsigned char prevMode;                   /* 0xD35 */
    unsigned char sel;                        /* 0xD36 */
    unsigned char padD37[0xD57 - 0xD37];
    unsigned char step;                       /* 0xD57 */
    unsigned char padD58[0xD5C - 0xD58];
    unsigned char player;                     /* 0xD5C */
    unsigned char mode;                       /* 0xD5D */
    unsigned char row;                        /* 0xD5E */
    unsigned char row2;                       /* 0xD5F */
    unsigned char digit[3];                   /* 0xD60 */
    unsigned char hold;                       /* 0xD63 */
    struct {
        signed char dir1 : 2;               /* bits 0-1 */
        signed char dir2 : 2;               /* bits 2-3 */
        signed char b4 : 1;
        signed char b5 : 1;
        signed char b6 : 1;
        signed char b7 : 1;
    } dirs;                        /* 0xD64 */
    struct {
        signed char b0 : 1;
        signed char b1 : 1;
        signed char b2 : 1;
        signed char b3 : 1;
        signed char b4 : 1;
        signed char b5 : 1;
        signed char b6 : 1;
        signed char b7 : 1;
    } f65;                         /* 0xD65: one-bit flags (bit 0 first) */
    struct {
        signed char b0 : 1;
        unsigned char b1 : 1;                 /* unsigned: func_0017bf28 reads it zero-extended (lbu, dsrl32) */
        signed char b2 : 1;
        signed char b3 : 1;
        signed char b4 : 1;
        signed char b5 : 1;
        signed char b6 : 1;
        signed char b7 : 1;                 /* SzDuel_GetWindowDisp returns it */
    } f66;                         /* 0xD66 */
    struct {
        signed char b0 : 1;
        signed char b1 : 1;
        signed char b2 : 1;
        signed char b3 : 1;
        signed char b4 : 1;
        signed char b5 : 1;
        signed char b6 : 1;
        signed char b7 : 1;
    } f67;                         /* 0xD67 */
    unsigned char padD68[0xD70 - 0xD68];
} DotrTradePaneDuel317490_s;

/* The duel state, read as bytes (.b) or by field (.s). */
typedef union DotrTradePaneDuel317490 {
    unsigned char b[0xD70];
    DotrTradePaneDuel317490_s s;
} DotrTradePaneDuel317490;

extern DotrTradePaneDuel317490 data_00317490_tradepane;

extern char data_002bf00b_tradepane;

extern char data_002be944_tradepane[];

extern char gSzDuel_aicolMsg_tradepane[];

/* Callees with file-scope prototypes (names from the Japanese build). Assumed: these types. A
 * prototype shown only to the target compiler is one that a function here also declares inside its
 * body with another type, which GCC rejects. */
int ExAssignAlpha(void *arg0, long long arg1);

long long ExGetAlphaVal(int arg0);

int ExSortPrim_Pipe(int arg0, int arg1, int arg2, int arg3);

int RhText_SetTextureFlush(int arg0);

int SzData_GetBagCount(int player, int cardNo, int category);

long long SzData_GetRank(int side, int cardNo);

int SzData_IsBagNew(int side, int cardNo);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
void SzDuel_DrawWindow_FontDigit_Pipe(int arg0, int arg1, int arg2, int arg3, int flags, int width, int number);
#endif

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int SzDuel_DrawWindow_Font_Pipe(int packet, int x, int y, int font, int flags, int count, signed char *text, unsigned char *color);
#endif

unsigned char *SzDuel_GetCardConstInfo(int cardNo);

int SzDuel_GetMsgName(int cardNo);

int SzDuel_Kind2Type(int kind);

int SzDuel_Type2Attr(long long arg0, int arg1);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int SzMsg_DrawRatioX_Pipe(int arg0, int arg1, long long arg2, long long arg3, float scaleX, float scaleY, int arg4, void (*arg5)(void));
#endif

int SzMsg_DrawRect_Pipe(int arg0, int arg1, int arg2, int arg3, int *arg4, unsigned char *arg5);

void SzMsg_Free(int arg0);

int SzMsg_LoadFont_Pipe(int arg0, int msgInfoIndex, int stringIndex, int arg3, int arg4);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
#endif

int lSzDuel_DrawWindow_ExBoard_Pipe(int packet, int colorIndex, int x, int y, int w, int h, int shadow, int style);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet, unsigned short *src, int x, int y, int background, int image, int border, int shrink, int flipX, int flipY, int bright, unsigned char *rgb, int blend, unsigned char alpha);
#endif

int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows, long long bright);

void lSzDuel_GetFontSize(int arg0, int arg1, int arg2, signed char *arg3, int *arg4, int *arg5);

int func_0019bef0(const char *s);

typedef struct {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
} DotrTradePaneTag1c4b00;

typedef struct {
    unsigned char r, g, b, a;
    float q;
} DotrTradePaneColor1c4b00;

typedef struct {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrTradePaneVertex1c4b00;

typedef struct {
    unsigned long long tag[2];
    DotrTradePaneColor1c4b00 color;
    DotrTradePaneVertex1c4b00 vertex0;
    DotrTradePaneVertex1c4b00 vertex1;
    unsigned long long end;
} DotrTradePaneBox1c4b00;

typedef struct {
    unsigned char low : 4;
    unsigned char high : 4;
} DotrTradePaneNibbles1c4b00;

static inline int dotr_tradepane_offset_1c4b00(int side, int left, int right) {
    return side != 0 ? right : left;
}

extern unsigned char data_00290020_tradepane[];

/* Data (each symbol links by address; other rows of the icon table at 0x290080 and strings are
 * declared inside the function bodies).
 * Assumed: the types, from use. */
extern unsigned char gSzDuel_Icon_tradepane[];

extern unsigned char data_0029086e_tradepane[];

extern unsigned char data_00290878_tradepane[];

extern unsigned char data_002908be_tradepane[];

extern unsigned char data_002908c8_tradepane[];

extern int data_002908e6_tradepane;

extern unsigned char data_002908f0_tradepane[];

extern unsigned char data_00290a12_tradepane[];

extern signed char data_002bf02d_tradepane;

extern char data_002bf030_tradepane[];

extern char data_002be948_tradepane[];

/* 0x1c4b00. Assumed: cursor, side, card id, position and a flag that greys a card with none to give. */
int lSzDuel_DrawTrade_Card_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);

/* 0x1c5550. Assumed: cursor, side, position and the cursor row. */
int lSzDuel_DrawTrade_Cursor_Pipe(int id, int player, int arg2, int arg3, int arg4);

/* 0x1c5870. Assumed: cursor and position. */
int lSzDuel_DrawTrade_Ready_Pipe(int arg0, int arg1, int arg2);

#endif
