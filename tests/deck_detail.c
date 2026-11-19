/* Synthetic host tests for the deck_detail group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/deck_detail.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

DotrDeckPaneDuel317490 data_00317490_deckpane;
unsigned char DeckBuildingSide_deckpane, BuildDeckHorizontalCursorIndex_deckpane;
unsigned short data_00318118_deckpane;
/* The icon table at 0x290080, its rows and the strings the functions name (separate host
 * fixtures; only their addresses matter on the paths run). */
unsigned char gSzDuel_Icon_deckpane[0x1000], data_00290020_deckpane[0x100], data_0029008a_deckpane[10];
unsigned char data_002901b6_deckpane[10], data_002908f0_deckpane[10], data_002be94c_deckpane[4];
int data_00290a4e_deckpane, data_00290a58_deckpane, data_00290a62_deckpane;
char gSzDuel_aicolMsg_deckpane[4], data_002be944_deckpane[4];
char data_002bef93_deckpane[0x40] = "BACK", data_002befcb_deckpane[] = "LOAD", data_002befdb_deckpane[] = "%d";
char data_002befe7_deckpane[] = "A", data_002befec_deckpane[] = "B", data_002beff3_deckpane[] = "C";
DotrDeckPaneColor1be5f0 data_00291368_deckpane;

static int frame_calls;
static int frame_style;
static int font_calls;
static int font_x;
static int font_y;
static int card_query;

/* Callees with the header's prototypes. */
int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows,
                                  long long bright)
{
    frame_calls++;
    frame_style = style;
    return packet + 1;
}
int ExSortPrim_Pipe(int arg0, int arg1, int arg2, int arg3) { return arg0 + 1; }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
int SzData_ExchSpecial(int arg0, int arg1) { return arg1; }
int SzData_GetBagCount(int player, int card, int category) { return 0; }
int SzData_GetBagCountAll(int arg0) { return 0; }
int SzData_GetLookFlag(int arg0, int arg1) { return 0; }
int SzDuel_DrawWindow_ExDigit_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    return arg0 + 1;
}
unsigned char *SzDuel_GetCardConstInfo(int arg0)
{
    card_query = arg0;
    return NULL;
}
int SzDuel_Kind2Type(int arg0) { return 0; }
int SzDuel_Type2Attr(long long arg0, int arg1) { return 0; }
int SzMsg_DrawRect_Pipe(int arg0, int arg1, int arg2, int arg3, int *arg4, unsigned char *arg5) { return 0; }
int SzMsg_DrawX_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) { return arg0 + 1; }
void SzMsg_Free(int arg0) { }
int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4) { return arg0 + 1; }
void *SzText_GetReadyItem(int arg0, int arg1) { return NULL; }
int func_0019b648(char *str, const char *fmt, ...) { return 0; }
int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) { return arg0 + 1; }
int lSzDuel_DrawWindow_ExBoard_Pipe(int packet, int colorIndex, int x, int y, int w, int h, int shadow, int style)
{
    return packet + 1;
}
int lSzDuel_DrawWindow_PicItem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, long long arg6)
{
    return arg0 + 1;
}
int func_0019bef0(const char *s) { return 0; }

/* Callees the functions declare inside their bodies; host signatures for the paths run. */
int SzDuel_DrawWindow_Font_Pipe(int packet, int x, int y, int font, int flags, int count, signed char *text, long color)
{
    font_calls++;
    font_x = x;
    font_y = y;
    return packet + 1;
}
int SzDuel_DrawWindow_FontDigit_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet) { return packet + 1; }
int SzData_GetRank(int arg0, int arg1) { return 0; }
int SzMsg_DrawRatioX_Pipe(int packet) { return packet + 1; }
static int info_words[4] = {0, 0, 0, 100};
int *SzMsg_GetInfo(int index) { return info_words; }

int main(void)
{
    /* lSzDuel_DrawBuildDeck_Detail_Basic_Pipe: a card id with no card record returns the cursor. */
    assert(lSzDuel_DrawBuildDeck_Detail_Basic_Pipe(0x100, 5, 0, 0, 0) == 0x100 && card_query == 5);

    /* lSzDuel_DrawBuildDeck_Library_Pipe: mode 0 ends with the back caption at (x - 640 + 44, y + 413). */
    font_calls = 0;
    assert(lSzDuel_DrawBuildDeck_Library_Pipe(0x100, 700, 10) > 0x100);
    assert(font_calls == 2 && font_x == 700 - 640 + 44 && font_y == 10 + 413);

    /* lSzDuel_DrawBuildDeck_DeckCursor_Pipe: a non-zero halfword at +0xc96 stops after the cursor frame. */
    data_00317490_deckpane.b[0xc96] = 1;
    frame_calls = 0;
    assert(lSzDuel_DrawBuildDeck_DeckCursor_Pipe(0x200, 0, 0, 0) == 0x201 && frame_calls == 1 && frame_style == 6);
    return 0;
}
