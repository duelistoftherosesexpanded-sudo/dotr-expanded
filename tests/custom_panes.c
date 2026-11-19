/* Synthetic host tests for the custom_panes group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/custom_panes.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

/* The settings block at 0xa9c000 and the control block at 0x317490. */
unsigned char dotr_custpane_save[0x100];
DotrCustPaneDuel317490 data_00317490_custpane;
int data_003180e8_custpane;
/* The icon table at 0x290080, its rows, colours, the map table at 0x2cee5c and the strings the
 * functions name (separate host fixtures; only their addresses matter on the paths run, except
 * the zeroed map rows, which are read as icon numbers). */
unsigned char gSzDuel_Icon_custpane[0x1000], data_002908f0_custpane[10], data_0029099a_custpane[10];
unsigned char gSzDuel_aaicolField_custpane[0x80];
unsigned char gSzDuel_MapData_custpane[0x20][7][7];
int data_002908e6_custpane, data_00290a1c_custpane, data_00290256_custpane, data_002902c4_custpane;
int data_00290828_custpane, data_00290832_custpane, data_00290a58_custpane, data_00290a62_custpane;
int data_00290a6c_custpane, data_00290aa8_custpane, data_00290ab2_custpane;
char data_002be944_custpane[4];
char data_002bf051_custpane[] = "ON", data_002bf056_custpane[] = "OFF", data_002bef93_custpane[] = "BACK";
char data_002bf03a_custpane[] = "WAIT", data_002bf047_custpane[] = "CUSTOM", data_002bf04e_custpane[] = "DUEL";
char data_002bf06e_custpane[] = "MAP", data_002bf074_custpane[] = "EDIT";

static DotrCustPaneMaps1cb6b0 maps;
static int font_x;
static int font_y;
static int start_palette;
static int menu_calls;
static long menu_side;
static int free_zero;

/* Callees with the header's prototypes. */
int ExSortPrim_Pipe(int arg0, int arg1, int arg2, int arg3) { return arg0 + 1; }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
int SzData_GetCurrentDeck(int arg0) { return 0; }
void *SzData_GetData(int arg0) { return &maps; }
int SzData_GetDeckCost(int arg0, int arg1) { return 0; }
int SzData_GetLeader(int arg0, int arg1) { return 0; }
int SzDuel_DrawWindow_ExDigit_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    return arg0 + 1;
}
unsigned char *SzDuel_GetCardConstInfo(int arg0) { return NULL; }
int SzDuel_GetLP(int arg0) { return 0; }
int SzDuel_GetMsgName(int arg0) { return 0; }
int SzDuel_StartWindowSessionWithPalette_Pipe(int arg0, int arg1)
{
    start_palette = arg1;
    return arg0 + 1;
}
int SzMsg_DrawRect_Pipe(int arg0, int arg1, int arg2, int arg3, int *arg4, unsigned char *arg5) { return 0; }
void SzMsg_Free(int arg0)
{
    if (arg0 == 0) {
        free_zero++;
    }
}
int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4) { return arg0 + 1; }
int lSzDuel_DrawCustomDuel_Base_Pipe(int arg0) { return arg0 + 1; }
int lSzDuel_DrawCustomDuel_Menu_Pipe(int arg0, unsigned char *arg1, int arg2, int arg3)
{
    menu_calls++;
    menu_side = (long)arg1;
    return arg0 + 1;
}
int lSzDuel_DrawWindow_Base_Pipe(int packet, int x, int y, int columns, int rows) { return packet + 1; }
int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) { return arg0 + 1; }
int lSzDuel_DrawWindow_ExBoard_Pipe(int packet, int colorIndex, int x, int y, int w, int h, int shadow, int style)
{
    return packet + 1;
}
int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows,
                                  long long bright)
{
    return packet + 1;
}
int lSzDuel_DrawWindow_PicMini_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, long long arg6, int arg7)
{
    return arg0 + 1;
}
void lSzDuel_GetFontSize(int arg0, int arg1, int arg2, signed char *arg3, int *arg4, int *arg5)
{
    *arg4 = 100;
    *arg5 = 20;
}
int func_0019bef0(const char *s) { return 0; }

/* Callees the functions declare inside their bodies; host signatures for the paths run. The
 * title pane is void here and the main pipe declares it returning int: its result is not checked. */
void lSzDuel_DrawCustomDuel_Title_Pipe(int arg0, int arg1, int arg2);
int SzDuel_DrawWindow_Font_Pipe(int packet, int x, int y, int font, int flags, int count, signed char *text, long color)
{
    font_x = x;
    font_y = y;
    return packet + 1;
}
int SzDuel_DrawWindow_FontDigit_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet) { return packet + 1; }
int SzMsg_DrawRatioX_Pipe(int packet) { return packet + 1; }
int SzMsg_DrawX_Pipe(int packet) { return packet + 1; }
int SzData_GetRank(int arg0, int arg1) { return 0; }
int SzDuel_GetSummonCharge(int arg0) { return 0; }
int lSzDuel_DrawCustomDuel_Exit_Pipe(int packet) { return packet + 1; }
int lSzDuel_Draw_Fade_Pipe(int packet) { return packet + 1; }
static int info_words[4] = {0, 0, 0, 100};
int *SzMsg_GetInfo(int index) { return info_words; }

int main(void)
{
    /* lSzDuel_DrawCustomDuel_Title_Pipe: the second caption at (x + 0x140, y + 0x46). */
    lSzDuel_DrawCustomDuel_Title_Pipe(0x100, 10, 20);
    assert(font_x == 10 + 0x140 && font_y == 20 + 0x46);

    /* lSzDuel_DrawCustomDuel_PlayerInfo_Pipe: no leader record and settings byte +0x88 clear: it ends
     * with the choice caption at (x + 0x100, y + 0x8a). */
    assert(lSzDuel_DrawCustomDuel_PlayerInfo_Pipe(0x100, 0, 30, 40) > 0x100);
    assert(font_x == 30 + 0x100 && font_y == 40 + 0x8a);

    /* lSzDuel_DrawCustomDuel_Menu_SelectMap_Pipe: five list rows (+0x8a), row 2 a table map (kind 2 in
     * the high byte of the halfword at +0x1c); it ends with the back caption at (x + 0x20, y + 0x99). */
    dotr_custpane_save[0x89] = 0;
    dotr_custpane_save[0x8a] = 5;
    dotr_custpane_save[0x1d] = 2;
    assert(lSzDuel_DrawCustomDuel_Menu_SelectMap_Pipe(0x200, 0, 50, 60) > 0x200);
    assert(font_x == 50 + 0x20 && font_y == 60 + 0x99);

    /* lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe: five rows (+0x8d), map 2 of the edited maps. */
    dotr_custpane_save[0x8c] = 0;
    dotr_custpane_save[0x8d] = 5;
    font_y = 0;
    assert(lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe(0x300, 1, 70, 80) > 0x300);
    assert(font_x == 70 + 0x20 && font_y == 80 + 0x99);

    /* SzDuel_DrawCustomDuel_Pipe: side 0 waiting (0x20), side 1 in the menu (0x10), byte +0x88 clear:
     * palette 2, the side-1 menu once, and message slot 0 freed. */
    dotr_custpane_save[2] = 0x20;
    dotr_custpane_save[3] = 0x10;
    SzDuel_DrawCustomDuel_Pipe(0x400);
    assert(start_palette == 2 && menu_calls == 1 && menu_side == 1 && free_zero == 1);
    return 0;
}
