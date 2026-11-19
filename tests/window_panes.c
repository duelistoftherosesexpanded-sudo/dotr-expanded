/* Synthetic host tests for the window_panes group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/window_panes.h"

union DotrPaneControl data_00317490_panes;
/* The icon table at 0x290080 and the rows and strings the functions name (separate host
 * fixtures; their contents are not read on the paths tested beyond being addresses). */
unsigned char data_00290020_panes[16], gSzDuel_Icon_panes[0x1000];
unsigned char data_002901d4_panes[10], data_002901de_panes[10], data_002901e8_panes[10], data_002901f2_panes[10];
unsigned char data_002901fc_panes[10], data_00290206_panes[10], data_00290210_panes[10], data_0029021a_panes[10];
unsigned char data_00290224_panes[10], data_0029022e_panes[10], data_00290238_panes[10], data_00290242_panes[10];
unsigned char data_0029024c_panes[10], data_00290828_panes[10], data_00290832_panes[10], data_0029086e_panes[10];
unsigned char data_00290896_panes[10], data_002908a0_panes[10], data_002909a4_panes[10], data_002909ae_panes[10];
unsigned char data_002909b8_panes[10], data_002909c2_panes[10], data_002909e0_panes[10], data_002909ea_panes[10];
unsigned char data_00290a6c_panes[10], data_00290aa8_panes[10], data_00290ab2_panes[10], data_00290abc_panes[10];
unsigned char data_00290ac6_panes[10];
int data_00290a76_panes;
signed char sort_name[] = "SORT";
signed char *data_00290fd0_panes[10] = {sort_name, sort_name, sort_name, sort_name, sort_name,
                                        sort_name, sort_name, sort_name, sort_name, sort_name};
DotrPaneBagColor data_00291360_panes;
unsigned char data_002be8a4_panes[4];
char gSzDuel_aicolMsg_panes[4], data_002be944_panes[4], data_002be948_panes[4], data_002be94c_panes[4];
char data_002bef8d_panes[] = "TITLE", data_002bef93_panes[] = "A", data_002bef98_panes[] = "DESC";
char data_002bef9d_panes[] = "B", data_002befc3_panes[] = "C";
unsigned char DeckBuildingSide_panes, BuildDeckHorizontalCursorIndex_panes;
unsigned short data_00318118_panes;

static long frame_args[16][9];
static int frame_calls;
static int card_ids[8];
static int card_calls;
static int digit_values[4];
static int digit_calls;
static int frees[8];
static int free_calls;
static int detail_calls;
static int info_words[8] = {0, 0, 0, 100, 20, 0, 0, 0};

int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows,
                                  long long bright)
{
    long *a = frame_args[frame_calls++];

    a[0] = packet; a[1] = colorIndex; a[2] = icons; a[3] = style; a[4] = x; a[5] = y; a[6] = cols; a[7] = rows;
    a[8] = (long)bright;
    return packet + 1;
}
/* Callees the functions declare inside their bodies; host signatures for the paths tested. */
int lSzDuel_DrawWindow_Icon_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_Board_Pipe(int packet) { return packet + 1; }
int SzDuel_DrawWindow_Digit_Pipe(int packet) { return packet + 1; }
int SzData_GetRank(int side, int id) { (void)side; (void)id; return 0; }
int SzDuel_DrawWindow_Font_Pipe(int packet) { return packet + 1; }
int SzDuel_DrawWindow_FontDigit_Pipe(int packet, int x, int y, int a, int flags, int width, int value)
{
    (void)x; (void)y; (void)a; (void)flags; (void)width;
    digit_values[digit_calls++] = value;
    return packet + 1;
}
int SzMsg_DrawRatioX_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet) { return packet + 1; }
int func_001b3cc0_bag(int packet, void *src, int x, int y, int background, int image, int border, int shrink,
                      int flipX, int flipY, int bright, unsigned char *rgb, int blend, unsigned char alpha)
{
    (void)src; (void)x; (void)y; (void)background; (void)image; (void)border; (void)shrink; (void)flipX; (void)flipY;
    (void)bright; (void)rgb; (void)blend; (void)alpha;
    return packet + 1;
}
int CheckLandAgreement(int arg0, void *arg1) { (void)arg0; (void)arg1; return 0; }
int ExSortPrim_Pipe(int arg0, DotrPaneWord arg1, int arg2, int arg3) { (void)arg1; (void)arg2; (void)arg3; return arg0 + 1; }
int GetMoveRev(int arg0, int arg1, int arg2) { (void)arg0; (void)arg1; (void)arg2; return 0; }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
int SzData_GetBagCount(int player, int card, int category) { (void)player; (void)card; (void)category; return 0; }
int SzData_GetBagCountAll(int arg0) { (void)arg0; return 0; }
int SzData_GetDeckCount(int arg0, int arg1) { (void)arg0; (void)arg1; return 0; }
int SzData_GetLeader(int arg0, int arg1) { (void)arg0; (void)arg1; return 0; }
unsigned char *SzDuel_GetCardConstInfo(int arg0) { (void)arg0; return 0; }
int SzDuel_GetDeckCount(int arg0) { (void)arg0; return 0; }
int SzDuel_GetField(int arg0, int arg1) { (void)arg0; (void)arg1; return 0; }
int SzDuel_GetHandCount(int arg0) { (void)arg0; return 0; }
int SzDuel_GetMsgName(int arg0) { return arg0; }
int SzDuel_GetUnitAP2(void *arg0, int arg1) { (void)arg0; (void)arg1; return 0; }
void *SzDuel_GetUnitConstEffect2(void *arg0, int arg1) { (void)arg0; (void)arg1; return 0; }
int SzDuel_GetUnitDP2(void *arg0, int arg1) { (void)arg0; (void)arg1; return 0; }
int SzDuel_GetUnitLeaderAbility2(void *arg0, int arg1, int arg2) { (void)arg0; (void)arg1; (void)arg2; return 0; }
int SzDuel_Kind2Attr(int arg0, int arg1) { (void)arg0; (void)arg1; return 0; }
int SzDuel_Kind2Type(int arg0) { (void)arg0; return 0; }
int SzFile_InqRequestEx(void) { return 0; }
int SzGame_GetFrameCount(void) { return 0; }
int SzGame_GetMenuNum(int arg0) { (void)arg0; return 0; }
void SzMsg_DrawRect_Pipe(void) {}
void SzMsg_Free(int arg0) { frees[free_calls++ & 7] = arg0; }
int *SzMsg_GetInfo(int arg0) { (void)arg0; return info_words; }
int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4) { (void)arg1; (void)arg2; (void)arg3; (void)arg4; return arg0 + 1; }
int SzPad_Check(int arg0, int arg1, int arg2) { (void)arg0; (void)arg1; (void)arg2; return 0; }
int Ta_GetParamOfs(int arg0, int arg1, int arg2, int arg3) { (void)arg0; (void)arg1; (void)arg2; (void)arg3; return 0; }
int Taku_Get_Duel5_flg(void) { return 0; }
int func_0019bef0(const char *s) { return (int)strlen(s); }
void *lSzDuel_DrawBuildDeck_Card_Pipe(void *arg0, int card, int x, int y, int flag, int flag2)
{
    (void)x; (void)y; (void)flag; (void)flag2;
    card_ids[card_calls++] = card;
    return (void *)((DotrPaneWord)arg0 + 1);
}
int lSzDuel_DrawBuildDeck_Detail_Ability_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; detail_calls++; return arg0;
}
int lSzDuel_DrawBuildDeck_Detail_Basic_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4)
{
    (void)arg1; (void)arg2; (void)arg3; (void)arg4; detail_calls++; return arg0;
}
int lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; detail_calls++; return arg0;
}
int lSzDuel_DrawBuildDeck_Detail_Special_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4)
{
    (void)arg1; (void)arg2; (void)arg3; (void)arg4; detail_calls++; return arg0;
}
int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; return arg0 + 1;
}
int lSzDuel_DrawWindow_ExBoard_Pipe(int packet, int colorIndex, int x, int y, int w, int h, int shadow, int style)
{
    (void)colorIndex; (void)x; (void)y; (void)w; (void)h; (void)shadow; (void)style; return packet + 1;
}
int lSzDuel_DrawWindow_PicItem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, long long arg6)
{
    (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; return arg0 + 1;
}
int lSzDuel_DrawWindow_PicMini_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, long long arg6, int arg7)
{
    (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; (void)arg7; return arg0 + 1;
}
void lSzDuel_GetFontSize(int arg0, int arg1, int arg2, signed char *arg3, int *arg4, int *arg5)
{
    (void)arg0; (void)arg1; (void)arg2; (void)arg3; *arg4 = 0; *arg5 = 0;
}
int lSzDuel_IsUnitHide(void *arg0) { (void)arg0; return 0; }

/* lSzDuel_DrawDialog_Window_Pipe: no title, message or picture, a 100 by 50 window centred on
 * x = 320: one frame of colour 0xd, style 1, at x - 50 - 4, sized -100 by -50. */
static void check_dialog_window(void)
{
    frame_calls = 0;
    assert(lSzDuel_DrawDialog_Window_Pipe(1000, 0, -1, -1, 320, 40, 100, 50, 0, 0, 0) == 1001);
    assert(frame_calls == 1 && frame_args[0][1] == 0xd && frame_args[0][3] == 1 && frame_args[0][4] == 320 - 50 - 4);
    assert(frame_args[0][5] == 40 && frame_args[0][6] == -100 && frame_args[0][7] == -50);
}

/* lSzDuel_DrawWindow_CardInfo_Pipe: without a card record only the frame is drawn first, in
 * the side's colour, icon kind 7 for a non-zero last flag and the style word as given. */
static void check_card_info(void)
{
    frame_calls = 0;
    lSzDuel_DrawWindow_CardInfo_Pipe(2000, 0, 1, 2, 1, 30, 40, 0, 3, 1);
    assert(frame_calls >= 1 && frame_args[0][0] == 2000 && frame_args[0][1] == 1 && frame_args[0][2] == 7);
    assert(frame_args[0][3] == 3 && frame_args[0][4] == 30 && frame_args[0][5] == 40 && frame_args[0][6] == 0x33);
}

/* lSzDuel_DrawBuildDeck_Bag_Pipe: the ordinary (non-special) pane: five card rows from the
 * list at +0x6d8 starting at the top +0xc8a, and the page and count digits at the end. */
static void check_bag(void)
{
    static unsigned short list[16] = {0, 0, 11, 12, 13, 14, 15, 16};
    unsigned short *pointer = list;
    unsigned short top = 2;
    unsigned short count = 9;
    unsigned char *data = data_00317490_panes.b;

    memset(data, 0, sizeof(data_00317490_panes));
    memcpy(data + 0x6d8, &pointer, sizeof(pointer));
    memcpy(data + 0xc8a, &top, 2);
    memcpy(data + 0xc8c, &count, 2);
    data[0xd5e] = 1;
    card_calls = digit_calls = 0;
    lSzDuel_DrawBuildDeck_Bag_Pipe(3000, 10, 20);
    assert(card_calls == 5 && card_ids[0] == 11 && card_ids[4] == 15);
    assert(digit_values[digit_calls - 2] == 2 + 1 + 1 && digit_values[digit_calls - 1] == 9);
}

/* lSzDuel_DrawBuildDeck_Detail_Pipe: an id without card information (and not 0x29f) draws the
 * frames and captions but no detail page, and frees message slots 2..5. */
static void check_detail(void)
{
    free_calls = 0;
    detail_calls = 0;
    lSzDuel_DrawBuildDeck_Detail_Pipe(4000, 5, 0, 0, 0, 1, 1, 0);
    assert(detail_calls == 0 && free_calls >= 4 && frees[free_calls - 1] == 5 && frees[free_calls - 4] == 2);
}

int main(void)
{
    check_dialog_window();
    check_card_info();
    check_bag();
    check_detail();
    return 0;
}
