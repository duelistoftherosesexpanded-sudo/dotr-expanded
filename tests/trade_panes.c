/* Synthetic host tests for the trade_panes group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/trade_panes.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

DotrTradePaneDuel317490 data_00317490_tradepane;
/* The icon table at 0x290080, its rows and the strings the functions name (separate host
 * fixtures; only their addresses matter on the paths run). */
unsigned char data_00290020_tradepane[0x100], gSzDuel_Icon_tradepane[0x1000];
unsigned char data_0029086e_tradepane[10], data_00290878_tradepane[10], data_002908be_tradepane[10];
unsigned char data_002908c8_tradepane[10], data_002908f0_tradepane[10], data_00290a12_tradepane[10];
int data_002908e6_tradepane, data_00290a4e_tradepane, data_00290a58_tradepane, data_00290a62_tradepane;
int data_00290a76_tradepane;
char data_002bf00b_tradepane;
signed char data_002bf02d_tradepane;
char data_002bf030_tradepane[] = "ABCD", data_002befec_tradepane[] = "B", data_002bf03a_tradepane[] = "READY";
char gSzDuel_aicolMsg_tradepane[4], data_002be944_tradepane[4], data_002be948_tradepane[4];

static int frame_calls;
static int frame_style;
static int frame_x;
static int font_x;
static int font_y;
static int card_query;

/* Callees with the header's prototypes. */
int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows,
                                  long long bright)
{
    frame_calls++;
    frame_style = style;
    frame_x = x;
    return packet + 1;
}
int ExAssignAlpha(void *arg0, long long arg1) { return 0; }
long long ExGetAlphaVal(int arg0) { return 0; }
int ExSortPrim_Pipe(int arg0, int arg1, int arg2, int arg3) { return arg0 + 1; }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
int SzData_GetBagCount(int player, int card, int category) { return 0; }
long long SzData_GetRank(int arg0, int arg1) { return 0; }
int SzData_IsBagNew(int arg0, int arg1) { return 0; }
unsigned char *SzDuel_GetCardConstInfo(int arg0)
{
    card_query = arg0;
    return NULL;
}
int SzDuel_GetMsgName(int arg0) { return 0; }
int SzDuel_Kind2Type(int arg0) { return 0; }
int SzDuel_Type2Attr(long long arg0, int arg1) { return 0; }
int SzMsg_DrawRect_Pipe(int arg0, int arg1, int arg2, int arg3, int *arg4, unsigned char *arg5) { return 0; }
void SzMsg_Free(int arg0) { }
int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4) { return arg0 + 1; }
int lSzDuel_DrawWindow_ExBoard_Pipe(int packet, int colorIndex, int x, int y, int w, int h, int shadow, int style)
{
    return packet + 1;
}
void lSzDuel_GetFontSize(int arg0, int arg1, int arg2, signed char *arg3, int *arg4, int *arg5)
{
    *arg4 = 100;
    *arg5 = 20;
}
int func_0019bef0(const char *s) { return 0; }

/* Callees the functions declare inside their bodies; host signatures for the paths run. */
int SzDuel_DrawWindow_Font_Pipe(int packet, int x, int y, int font, int flags, int count, signed char *text, long color)
{
    font_x = x;
    font_y = y;
    return packet + 1;
}
int SzDuel_DrawWindow_FontDigit_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_Emblem_Pipe(int packet) { return packet + 1; }
int SzMsg_DrawRatioX_Pipe(int packet) { return packet + 1; }
int SzMsg_GetInfo(int index) { return 0; }

int main(void)
{
    /* lSzDuel_DrawTrade_Card_Pipe: a card id with no card record draws only the empty frame (style 9). */
    assert(lSzDuel_DrawTrade_Card_Pipe(0x100, 0, 7, 30, 40, 0) == 0x101);
    assert(card_query == 7 && frame_calls == 1 && frame_style == 9 && frame_x == 30);

    /* lSzDuel_DrawTrade_Cursor_Pipe: a non-zero halfword at +0xc96 (side 0) stops after the cursor frame. */
    data_00317490_tradepane.b[0xc96] = 1;
    frame_calls = 0;
    assert(lSzDuel_DrawTrade_Cursor_Pipe(0x200, 0, 0, 0, 0) == 0x201 && frame_calls == 1 && frame_style == 6);

    /* lSzDuel_DrawTrade_Ready_Pipe: the caption is centred at x + 0x140, y + 0xe8 - height / 2
     * (height 20 + 12). */
    assert(lSzDuel_DrawTrade_Ready_Pipe(0x300, 10, 20) == 0x302);
    assert(font_x == 10 + 0x140 && font_y == 20 + 0xe8 - 16);
    return 0;
}
