#ifndef DOTR_EE_DECK_DETAIL_H
#define DOTR_EE_DECK_DETAIL_H

/* Deck-builder detail, library and deck-cursor panes that draw from the duel control block at
 * 0x317490.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A packet cursor or an address kept as a 32-bit EE word (pointer-sized on the host). */
#ifdef __MWERKS__
typedef int DotrDeckPaneWord;
#else
#include <stdint.h>
typedef intptr_t DotrDeckPaneWord;
#endif

#ifndef NULL
#define NULL 0 /* the code compares pointers with NULL as 0 */
#endif

/* The duel control block at 0x317490, read as bytes (.b) or by field (.s). Assumed: field names and
 * meanings from use. */
typedef struct DotrDeckPaneDuel317490_s {
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
} DotrDeckPaneDuel317490_s;

/* The duel state: some functions read it as bytes (.b), others by field (.s) */
typedef union DotrDeckPaneDuel317490 {
    unsigned char b[0xD70];
    DotrDeckPaneDuel317490_s s;
} DotrDeckPaneDuel317490;

extern DotrDeckPaneDuel317490 data_00317490_deckpane;

extern unsigned char DeckBuildingSide_deckpane;

extern char data_002be944_deckpane[];

extern char gSzDuel_aicolMsg_deckpane[];

/* Callees with file-scope prototypes (names from the Japanese build). Assumed: these types. A
 * prototype shown only to the target compiler is one that a function here also declares inside its
 * body with another type, which GCC rejects. */
int ExSortPrim_Pipe(int arg0, int arg1, int arg2, int arg3);

int RhText_SetTextureFlush(int arg0);

int SzData_ExchSpecial(int side, int cardNo);

int SzData_GetBagCount(int player, int cardNo, int category);

int SzData_GetBagCountAll(int arg0);

int SzData_GetLookFlag(int arg0, int arg1);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
long long SzData_GetRank(int side, int cardNo);
#endif

int SzDuel_DrawWindow_ExDigit_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
void SzDuel_DrawWindow_FontDigit_Pipe(int arg0, int arg1, int arg2, int arg3, int flags, int width, int number);
#endif

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int SzDuel_DrawWindow_Font_Pipe(int packet, int x, int y, int font, int flags, int count, signed char *text, unsigned char *color);
#endif

unsigned char *SzDuel_GetCardConstInfo(int cardNo);

int SzDuel_Kind2Type(int kind);

int SzDuel_Type2Attr(long long arg0, int arg1);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int SzMsg_DrawRatioX_Pipe(int arg0, int arg1, long long arg2, long long arg3, float scaleX, float scaleY, int arg4, void (*arg5)(void));
#endif

int SzMsg_DrawRect_Pipe(int arg0, int arg1, int arg2, int arg3, int *arg4, unsigned char *arg5);

int SzMsg_DrawX_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);

void SzMsg_Free(int arg0);

int SzMsg_LoadFont_Pipe(int arg0, int msgInfoIndex, int stringIndex, int arg3, int arg4);

void *SzText_GetReadyItem(int arg0, int arg1);

int func_0019b648(char *str, const char *fmt, ...);

int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);

int lSzDuel_DrawWindow_ExBoard_Pipe(int packet, int colorIndex, int x, int y, int w, int h, int shadow, int style);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet, unsigned short *src, int x, int y, int background, int image, int border, int shrink, int flipX, int flipY, int bright, unsigned char *rgb, int blend, unsigned char alpha);
#endif

int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows, long long bright);

int lSzDuel_DrawWindow_PicItem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, long long arg6);

int func_0019bef0(const char *s);

/* Data (each symbol links by address; other rows of the icon table at 0x290080 and the
 * strings are declared inside the function bodies). Assumed: the types, from use. */
extern unsigned char gSzDuel_Icon_deckpane[];

extern char data_002bef93_deckpane[];

extern unsigned char BuildDeckHorizontalCursorIndex_deckpane;

extern unsigned short data_00318118_deckpane;

typedef struct DotrDeckPaneVertex1be5f0 {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrDeckPaneVertex1be5f0;

typedef struct DotrDeckPaneHeader1be5f0 {
    unsigned short type;
    unsigned short count;
    unsigned int pad;
    unsigned long long registers;
} DotrDeckPaneHeader1be5f0;

typedef struct DotrDeckPanePacket1be5f0 {
    DotrDeckPaneHeader1be5f0 header;
    unsigned char r, g, b, a;
    float q;
    DotrDeckPaneVertex1be5f0 first;
    DotrDeckPaneVertex1be5f0 second;
    unsigned long long unused;
} DotrDeckPanePacket1be5f0;

typedef union DotrDeckPaneColor1be5f0 {
    float value;
    unsigned char bytes[4];
} DotrDeckPaneColor1be5f0;

typedef struct DotrDeckPaneCardBits1be5f0 {
    unsigned long long unused : 16;
    unsigned long long subtype : 4;
    unsigned long long level : 4;
    unsigned long long rest : 40;
} DotrDeckPaneCardBits1be5f0;

typedef union DotrDeckPaneCard1be5f0 {
    struct {
        unsigned char kind, unused, stats, cost;
        unsigned short code, field, attack, defense;
    } data;
    DotrDeckPaneCardBits1be5f0 bits;
} DotrDeckPaneCard1be5f0;

extern unsigned char data_00290020_deckpane[];

extern unsigned char data_0029008a_deckpane[];

extern DotrDeckPaneColor1be5f0 data_00291368_deckpane;

extern unsigned char data_002be94c_deckpane[];

extern char data_002befcb_deckpane[];

extern char data_002befdb_deckpane[];

static inline int dotr_deckpane_text_1be5f0(int cursor, int x, int y, int align,
                               int flags, signed char *text, void *color) {
    extern int SzDuel_DrawWindow_Font_Pipe();
    int length = func_0019bef0((char *)text);
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, align, flags,
                        length, text, color);
}

static inline int dotr_deckpane_loading_1be5f0(int cursor, DotrDeckPanePacket1be5f0 *packet,
                                  int x, int y, unsigned char *data) {
    ((DotrDeckPaneHeader1be5f0 *)(DotrDeckPaneWord)&packet->header)->count = 6;
    ((DotrDeckPaneHeader1be5f0 *)(DotrDeckPaneWord)&packet->header)->type = 3;
    ((DotrDeckPaneHeader1be5f0 *)(DotrDeckPaneWord)&packet->header)->registers = 0x441;
    ((DotrDeckPaneHeader1be5f0 *)(DotrDeckPaneWord)&packet->header)->pad = 0;
    packet->r = 0;
    packet->g = 0;
    packet->b = 0;
    packet->a = 0x80;
    packet->q = 1.0f;
    packet->first.x = x << 4;
    packet->first.y = y << 4;
    packet->first.z = *(int *)(data + 0xC58);
    packet->first.fog = 0;
    packet->second.x = (x + 0x80) << 4;
    packet->second.y = (y + 0x80) << 4;
    packet->second.z = *(int *)(data + 0xC58);
    packet->second.fog = 0;
    return ExSortPrim_Pipe(cursor, (DotrDeckPaneWord)&packet->header, 0, 0);
}

static inline int dotr_deckpane_pos_1be5f0(int x, int slide) {
    return x + slide;
}

static inline float dotr_deckpane_fit_1be5f0(int index, int limit) {
    extern int *SzMsg_GetInfo(int);
    int width = SzMsg_GetInfo(index)[3];
    if (width > limit) {
        return (float)limit / width;
    }
    return 1.0f;
}

/* 0x1be5f0. Assumed: cursor, card id (0x29f the special no-card entry), position and the side
 * flag (non-zero: the right-hand pane). */
int lSzDuel_DrawBuildDeck_Detail_Basic_Pipe(int arg0, int cardNo, int arg2, int arg3, int arg4);

/* 0x1bf710. Assumed: cursor and position. */
int lSzDuel_DrawBuildDeck_Library_Pipe(int cursor, int x, int y);

/* 0x1c02e0. Assumed: cursor, position and the cursor row. */
int lSzDuel_DrawBuildDeck_DeckCursor_Pipe(int id, int arg1, int arg2, int arg3);

#endif
