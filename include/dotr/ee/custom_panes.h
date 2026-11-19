#ifndef DOTR_EE_CUSTOM_PANES_H
#define DOTR_EE_CUSTOM_PANES_H

/* Custom duel screen panes: the main pipe, title, player information and the map-select menus.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A packet cursor or an address kept as a 32-bit EE word (pointer-sized on the host). */
#ifdef __MWERKS__
typedef int DotrCustPaneWord;
#else
#include <stdint.h>
typedef intptr_t DotrCustPaneWord;
#endif

/* The custom-duel settings block at 0xa9c000, which the code reads through literal addresses
 * on the target; a host array stands in for it. Assumed: 0x100 bytes cover the offsets used
 * (+0x02..+0x8d). */
#ifdef __MWERKS__
#define DOTR_CUSTPANE_SAVE(address) (address)
#else
extern unsigned char dotr_custpane_save[0x100];
#define DOTR_CUSTPANE_SAVE(address) ((DotrCustPaneWord)dotr_custpane_save + ((address) - 0xA9C000))
#endif

#ifndef NULL
#define NULL 0 /* the code compares pointers with NULL as 0 */
#endif

/* The duel control block at 0x317490, read as bytes (.b) or by field (.s). Assumed: field names and
 * meanings from use. */
typedef struct DotrCustPaneDuel317490_s {
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
        unsigned char b1 : 1;                 /* func_0017bf28 unsigned (lbu/dsrl32) */
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
} DotrCustPaneDuel317490_s;

/* The duel state: some functions read it as bytes (.b), others by field (.s) */
typedef union DotrCustPaneDuel317490 {
    unsigned char b[0xD70];
    DotrCustPaneDuel317490_s s;
} DotrCustPaneDuel317490;

extern DotrCustPaneDuel317490 data_00317490_custpane;

extern int data_003180e8_custpane;

extern char data_002be944_custpane[];

/* Callees with file-scope prototypes (names from the Japanese build). Assumed: these types. A
 * prototype shown only to the target compiler is one that a function here also declares inside its
 * body with another type, which GCC rejects. */
int ExSortPrim_Pipe(int arg0, int arg1, int arg2, int arg3);

int RhText_SetTextureFlush(int arg0);

int SzData_GetCurrentDeck(int side);

void *SzData_GetData(int side);

int SzData_GetDeckCost(int arg0, int arg1);

int SzData_GetLeader(int side, int deckNo);

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

int SzDuel_GetLP(int side);

int SzDuel_GetMsgName(int cardNo);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
unsigned char SzDuel_GetSummonCharge(int side);
#endif

int SzDuel_StartWindowSessionWithPalette_Pipe(int arg0, int arg1);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int SzMsg_DrawRatioX_Pipe(int arg0, int arg1, long long arg2, long long arg3, float scaleX, float scaleY, int arg4, void (*arg5)(void));
#endif

int SzMsg_DrawRect_Pipe(int arg0, int arg1, int arg2, int arg3, int *arg4, unsigned char *arg5);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int SzMsg_DrawX_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
#endif

void SzMsg_Free(int arg0);

int SzMsg_LoadFont_Pipe(int arg0, int msgInfoIndex, int stringIndex, int arg3, int arg4);

int lSzDuel_DrawCustomDuel_Base_Pipe(int arg0);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
void lSzDuel_DrawCustomDuel_Exit_Pipe(int arg0, int arg1, int arg2);
#endif

int lSzDuel_DrawCustomDuel_Menu_Pipe(int arg0, unsigned char *arg1, int arg2, int arg3);

int lSzDuel_DrawCustomDuel_Menu_SelectMap_Pipe(int id, int player, int x, int y);

int lSzDuel_DrawCustomDuel_PlayerInfo_Pipe(int arg0, int side, int arg2, int arg3);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
void lSzDuel_DrawCustomDuel_Title_Pipe(int arg0, int arg1, int arg2);
#endif

int lSzDuel_DrawWindow_Base_Pipe(int packet, int x, int y, int columns, int rows);

int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);

int lSzDuel_DrawWindow_ExBoard_Pipe(int packet, int colorIndex, int x, int y, int w, int h, int shadow, int style);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet, unsigned short *src, int x, int y, int background, int image, int border, int shrink, int flipX, int flipY, int bright, unsigned char *rgb, int blend, unsigned char alpha);
#endif

int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows, long long bright);

int lSzDuel_DrawWindow_PicMini_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, long long arg6, int arg7);

#ifdef __MWERKS__ /* the functions declare it inside their bodies with another type */
void lSzDuel_Draw_Fade_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
#endif

void lSzDuel_GetFontSize(int arg0, int arg1, int arg2, signed char *arg3, int *arg4, int *arg5);

int func_0019bef0(const char *s);

/* Data (each symbol links by address; other rows of the icon table at 0x290080 and the
 * strings are declared inside the function bodies). Assumed: the types, from use. */
extern unsigned char gSzDuel_Icon_custpane[];

extern int data_002908e6_custpane;

extern unsigned char data_002908f0_custpane[];

extern int data_00290a1c_custpane;

extern char data_002bf051_custpane[];

extern char data_002bf056_custpane[];

extern unsigned char data_0029099a_custpane[];

typedef struct {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
} DotrCustPaneTag1ca7c0;

typedef struct {
    unsigned char r, g, b, a;
    float q;
} DotrCustPaneColor1ca7c0;

typedef struct {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrCustPaneVertex1ca7c0;

typedef struct {
    DotrCustPaneColor1ca7c0 color;
    DotrCustPaneVertex1ca7c0 xyz;
} DotrCustPanePoint1ca7c0;

typedef struct {
    unsigned long long tag[2];
    DotrCustPanePoint1ca7c0 points[4];
    unsigned long long end;
} DotrCustPanePacket1ca7c0;

typedef struct {
    unsigned long long tag[2];
    DotrCustPaneColor1ca7c0 color;
    DotrCustPaneVertex1ca7c0 xyz[2];
    unsigned long long end;
} DotrCustPaneRect1ca7c0;

typedef struct {
    unsigned char b, g, r, a;
} DotrCustPaneSource1ca7c0;

typedef struct {
    unsigned char pad[0xFEA3];
    unsigned char maps[5][7][7];
} DotrCustPaneMaps1ca7c0;

static inline void dotr_custpane_set_color_1ca7c0(DotrCustPaneColor1ca7c0 *a, DotrCustPaneColor1ca7c0 *b,
                                      DotrCustPaneSource1ca7c0 *src) {
    a->r = b->r = src->r;
    a->g = b->g = src->g;
    a->b = b->b = src->b;
    a->a = b->a = src->a;
    a->q = b->q = 1.0f;
}

static inline void dotr_custpane_set_xyz_1ca7c0(DotrCustPaneVertex1ca7c0 *v, int x, int y, unsigned char *state) {
    v->x = x << 4;
    v->y = y << 4;
    v->z = *(int *)(state + 0xC58);
    v->fog = 0;
}

static inline void dotr_custpane_set_corner_1ca7c0(DotrCustPaneVertex1ca7c0 *v, int x, int y) {
    v->x = x << 4;
    v->y = y << 4;
    v->z = data_003180e8_custpane;
    v->fog = 0;
}

typedef struct {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
} DotrCustPaneTag1cb6b0;

typedef struct {
    unsigned char r, g, b, a;
    float q;
} DotrCustPaneColor1cb6b0;

typedef struct {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrCustPaneVertex1cb6b0;

typedef struct {
    DotrCustPaneColor1cb6b0 color;
    DotrCustPaneVertex1cb6b0 xyz;
} DotrCustPanePoint1cb6b0;

typedef struct {
    unsigned long long tag[2];
    DotrCustPanePoint1cb6b0 points[4];
    unsigned long long end;
} DotrCustPanePacket1cb6b0;

typedef struct {
    unsigned long long tag[2];
    DotrCustPaneColor1cb6b0 color;
    DotrCustPaneVertex1cb6b0 xyz[2];
    unsigned long long end;
} DotrCustPaneRect1cb6b0;

typedef struct {
    unsigned char b, g, r, a;
} DotrCustPaneSource1cb6b0;

typedef struct {
    unsigned char pad[0xFEA3];
    unsigned char maps[5][7][7];
} DotrCustPaneMaps1cb6b0;

static inline void dotr_custpane_set_color_1cb6b0(DotrCustPaneColor1cb6b0 *a, DotrCustPaneColor1cb6b0 *b,
                                      DotrCustPaneSource1cb6b0 *src) {
    a->r = b->r = src->r;
    a->g = b->g = src->g;
    a->b = b->b = src->b;
    a->a = b->a = src->a;
    a->q = b->q = 1.0f;
}

static inline void dotr_custpane_set_xyz_1cb6b0(DotrCustPaneVertex1cb6b0 *v, int x, int y, unsigned char *state) {
    v->x = x << 4;
    v->y = y << 4;
    v->z = *(int *)(state + 0xC58);
    v->fog = 0;
}

static inline void dotr_custpane_set_corner_1cb6b0(DotrCustPaneVertex1cb6b0 *v, int x, int y) {
    v->x = x << 4;
    v->y = y << 4;
    v->z = data_003180e8_custpane;
    v->fog = 0;
}

/* 0x1c7460. Assumed: the argument is the packet cursor. The panes it calls are declared above
 * (the title pane as void, although this function's body declares
 * it returning int). */
int SzDuel_DrawCustomDuel_Pipe(int arg0);

/* 0x1cb6b0. Assumed: cursor, side (frame colour) and position. */
int lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe(int id, int player, int x, int y);

#endif
