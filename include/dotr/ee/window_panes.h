#ifndef DOTR_EE_WINDOW_PANES_H
#define DOTR_EE_WINDOW_PANES_H

/* Duel and deck-building window panes (dialog window, card information, deck bag and card detail)
 * drawn into the GS packet from the control block at 0x317490.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A packet cursor or an address kept as a 32-bit EE word (pointer-sized on the host). */
#ifdef __MWERKS__
typedef int DotrPaneWord;
#else
#include <stdint.h>
typedef intptr_t DotrPaneWord;
#endif

/* The control block at 0x317490 as bytes, in a union whose int member gives it int alignment.
 * Read at raw offsets, e.g. +0x6d8 list pointers and +0xc58 the depth word. */
union DotrPaneControl {
    unsigned char b[0xd70];
    int align;
};
extern union DotrPaneControl data_00317490_panes;

/* ---- Views. Assumed: field meanings from use.
 * lSzDuel_DrawWindow_CardInfo_Pipe: the duel card record (active word, id at +0x3c, type and
 * attribute bytes, owner and state at +0x44/+0x45, two counters at +0x49/+0x4a and a flag byte
 * at +0x58) and the MAX/MIN macros it uses. */
typedef struct DotrPaneInfoFlags {
    signed char bit0 : 1;
    signed char bit1 : 1;
    signed char bit2 : 1;
    signed char rest : 5;
} DotrPaneInfoFlags;

typedef struct DotrPaneInfoCard {
    int active;
    unsigned char pad04[0x38];
    unsigned short id;
    unsigned char type;
    unsigned char attribute;
    unsigned char pad40[4];
    unsigned char owner;
    unsigned char state;
    unsigned char pad46[3];
    unsigned char count1;
    unsigned char count2;
    unsigned char pad4B[0xD];
    DotrPaneInfoFlags flags;
} DotrPaneInfoCard;

#define DOTR_PANE_INFO_MAX(a, b) ((a) > (b) ? (a) : (b))

#define DOTR_PANE_INFO_MIN(a, b) ((a) < (b) ? (a) : (b))

/* lSzDuel_DrawBuildDeck_Bag_Pipe: the flat-rectangle packet (a leading word, header, colour,
 * q and two XYZ vertices) and a float-or-bytes colour word. */
typedef struct DotrPaneBagVertex {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrPaneBagVertex;

typedef struct DotrPaneBagHeader {
    unsigned short type;
    unsigned short count;
    unsigned int pad;
    unsigned long long registers;
} DotrPaneBagHeader;

typedef struct DotrPaneBagPacket {
    unsigned long long tag;
    DotrPaneBagHeader header;
    unsigned char r, g, b, a;
    float q;
    DotrPaneBagVertex first;
    DotrPaneBagVertex second;
} DotrPaneBagPacket;

typedef union DotrPaneBagColor {
    float value;
    unsigned char bytes[4];
} DotrPaneBagColor;

/* Data declared at file scope (each symbol links by address; the functions declare further rows
 * of the icon table and strings inside their bodies): the icon table at 0x290080 (10-byte rows)
 * and rows of it, the ten sort names (0x290fd0), the bag colour word (0x291360), colour bytes
 * (0x2be8a4), highlight colours (0x2be940, 0x2be944), strings (0x2bef8d, 0x2bef93, 0x2bef98),
 * the two cursor bytes at 0x3181ec/0x3181ed and the halfword at 0x318118. */
extern unsigned char gSzDuel_Icon_panes[];
extern unsigned char data_002909b8_panes[];
extern unsigned char data_00290828_panes[];
extern unsigned char data_00290832_panes[];
extern unsigned char data_00290896_panes[];
extern unsigned char data_002908a0_panes[];
extern unsigned char data_00290a6c_panes[];
extern unsigned char data_00290aa8_panes[];
extern unsigned char data_00290ab2_panes[];
extern unsigned char data_00290abc_panes[];
extern unsigned char data_00290ac6_panes[];
extern signed char *data_00290fd0_panes[];
extern DotrPaneBagColor data_00291360_panes;
extern unsigned char data_002be8a4_panes[];
extern char data_002bef8d_panes[];
extern char data_002bef93_panes[];
extern unsigned char DeckBuildingSide_panes;
extern unsigned char BuildDeckHorizontalCursorIndex_panes;
extern unsigned short data_00318118_panes;
extern char data_002bef98_panes[];
extern char data_002be944_panes[];
extern char gSzDuel_aicolMsg_panes[];

/* Callees with file-scope prototypes. Left out here because the functions declare them inside
 * their bodies (with other types): the icon, board, digit, font, font-digit, rank and
 * SzMsg_DrawRatioX_Pipe drawers. lSzDuel_DrawWindow_ExIcon_Pipe is declared inside the detail
 * pane; the bag pane calls it through a view (icon row as void *). SzMsg_GetInfo returns an int
 * pointer here, as the detail pane declares it. SzMsg_DrawRect_Pipe is only passed by address.
 * func_0019bef0 is unnamed (a string length). Assumed: these types. */
int CheckLandAgreement(int field, void *unit);
int ExSortPrim_Pipe(int arg0, DotrPaneWord arg1, int arg2, int arg3);
int GetMoveRev(int arg0, int arg1, int arg2);
int RhText_SetTextureFlush(int arg0);
int SzData_GetBagCount(int player, int cardNo, int category);
int SzData_GetBagCountAll(int arg0);
int SzData_GetDeckCount(int side, int deckNo);
int SzData_GetLeader(int side, int deckNo);
unsigned char *SzDuel_GetCardConstInfo(int cardNo);
int SzDuel_GetDeckCount(int side);
int SzDuel_GetField(int arg0, int arg1);
int SzDuel_GetHandCount(int side);
int SzDuel_GetMsgName(int cardNo);
int SzDuel_GetUnitAP2(void *unit, int ignoreBoosts);
void *SzDuel_GetUnitConstEffect2(void *unit, int eff);
int SzDuel_GetUnitDP2(void *unit, int ignoreBoosts);
int SzDuel_GetUnitLeaderAbility2(void *unit, int abil, int extraDataOut);
int SzDuel_Kind2Attr(int type, int fallback);
int SzDuel_Kind2Type(int kind);
int SzFile_InqRequestEx(void);
int SzGame_GetFrameCount(void);
int SzGame_GetMenuNum(int arg0);
void SzMsg_DrawRect_Pipe(void);
void SzMsg_Free(int arg0);
int *SzMsg_GetInfo(int arg0);
int SzMsg_LoadFont_Pipe(int arg0, int msgInfoIndex, int stringIndex, int arg3, int arg4);
int SzPad_Check(int arg0, int arg1, int arg2);
int Ta_GetParamOfs(int arg0, int arg1, int arg2, int arg3);
int Taku_Get_Duel5_flg(void);
int func_0019bef0(const char *s);
void *lSzDuel_DrawBuildDeck_Card_Pipe(void *arg0, int card, int x, int y, int flag, int flag2);
int lSzDuel_DrawBuildDeck_Detail_Ability_Pipe(int arg0, int cardNo, int arg2, int arg3, int arg4, int arg5);
int lSzDuel_DrawBuildDeck_Detail_Basic_Pipe(int arg0, int cardNo, int arg2, int arg3, int arg4);
int lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe(int arg0, int cardId, int arg2, int arg3, int arg4, int arg5);
int lSzDuel_DrawBuildDeck_Detail_Special_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
int lSzDuel_DrawWindow_ExBoard_Pipe(int packet, int colorIndex, int x, int y, int w, int h, int shadow, int style);
int func_001b3cc0_bag(int packet, void *src, int x, int y, int background, int image, int border, int shrink,
                      int flipX, int flipY, int bright, unsigned char *rgb, int blend, unsigned char alpha);
int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows,
                                  long long bright);
int lSzDuel_DrawWindow_PicItem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, long long arg6);
int lSzDuel_DrawWindow_PicMini_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, long long arg6, int arg7);
void lSzDuel_GetFontSize(int arg0, int arg1, int arg2, signed char *arg3, int *arg4, int *arg5);
int lSzDuel_IsUnitHide(void *unit);

/* Helpers shared by the panes (the rectangle helper's int casts of pointers are written with the
 * cursor word type, int on the target). */
static inline int dotr_pane_dlg_abs(int value)
{
    if (value >= 0) {
        return value;
    }
    return -value;
}

static inline int dotr_pane_bag_text(int cursor, int x, int y, int align,
                               int flags, signed char *text, void *color)
{
    extern int SzDuel_DrawWindow_Font_Pipe();
    int length = func_0019bef0((char *)text);
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, align, flags,
                        length, text, color);
}

static inline int dotr_pane_bag_rectangle(int cursor, DotrPaneBagPacket *packet,
                                    int x, int y, unsigned char *data)
{
    ((DotrPaneBagHeader *)(DotrPaneWord)&packet->header)->count = 6;
    ((DotrPaneBagHeader *)(DotrPaneWord)&packet->header)->type = 3;
    ((DotrPaneBagHeader *)(DotrPaneWord)&packet->header)->registers = 0x441;
    ((DotrPaneBagHeader *)(DotrPaneWord)&packet->header)->pad = 0;
    packet->r = 0xA0;
    packet->g = 0xA0;
    packet->b = 0xA0;
    packet->a = 0x80;
    packet->q = 1.0f;
    packet->first.x = (x + 0x6CA) << 4;
    packet->first.y = (y + 0x738) << 4;
    packet->first.z = *(int *)(data + 0xC58);
    packet->first.fog = 0;
    packet->second.x = (x + 0x6F2) << 4;
    packet->second.y = (y + 0x739) << 4;
    packet->second.z = *(int *)(data + 0xC58);
    packet->second.fog = 0;
    return ExSortPrim_Pipe(cursor, (DotrPaneWord)&packet->header, 0, 0);
}

static inline int dotr_pane_bag_number(int cursor, int x, int y, int player)
{
    extern int SzDuel_DrawWindow_FontDigit_Pipe();
    int count = SzData_GetBagCountAll(player);
    return SzDuel_DrawWindow_FontDigit_Pipe(cursor, x, y, 1, 0, 4, count);
}

static inline int dotr_pane_bag_ready(int player, int deck)
{
    int count;
    int card;

    count = SzData_GetDeckCount(player, deck);
    card = SzData_GetLeader(player, deck);
    return (count >= 0x28 && card != 0x3E7) ? 1 : 0;
}

static inline int dotr_pane_detail_frame(int id, int color, unsigned int x, unsigned int y)
{
    extern int lSzDuel_DrawWindow_Frame_Pipe();
    return lSzDuel_DrawWindow_Frame_Pipe(id, color, -1, 0, x - 0xD, y - 0xF, -0x116, -0x104, 1);
}

/* 0x1b0ce0. Assumed: cursor, colour row of the title, card or picture id (0x8000 and above:
 * a card portrait), message id, centre x, top y, width (negative: fixed, the message scaled
 * to fit), height, title length and text address, and a flag that adds the blinking arrow. */
int lSzDuel_DrawDialog_Window_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7,
                                   int arg8, DotrPaneWord arg9, int arg10);

/* 0x1b22f0. Assumed: cursor, card record, its board cell, player side, frame colour, position,
 * mode and two flag words (frame style and icon kind). */
int lSzDuel_DrawWindow_CardInfo_Pipe(int arg0, void *arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7,
                                     long arg8, long arg9);

/* 0x1bb940. Assumed: cursor, player and a flag. */
int lSzDuel_DrawBuildDeck_Bag_Pipe(int arg0, int arg1, int arg2);

/* 0x1bdaf0. Assumed: cursor, card id, position, three display flags and a mode. */
int lSzDuel_DrawBuildDeck_Detail_Pipe(int arg0, int cardNo, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);

#endif
