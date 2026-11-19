/* Synthetic host tests for the window_panels group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/window_panels.h"

struct DotrWindowControl data_00317490_window;
char gSzDuel_Icon_bytes[10240];
struct DotrGuideEntry data_00291180[21][4];
char data_002bef76[4];

struct PanelCall {
    char kind;
    long a[11];
};
static struct PanelCall p_calls[32];
static int p_count;
static int p_player[2];
static int p_deck = 31;
static int p_hand;
static int p_pair;
static int p_hide;
static int p_movable;
static int p_request;
static struct DotrPanelInfo p_info[4];
static int p_info_count;

static DotrPanelWord prec(char kind, long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10)
{
    struct PanelCall *call = &p_calls[p_count++];
    call->kind = kind;
    call->a[0] = a0;
    call->a[1] = a1;
    call->a[2] = a2;
    call->a[3] = a3;
    call->a[4] = a4;
    call->a[5] = a5;
    call->a[6] = a6;
    call->a[7] = a7;
    call->a[8] = a8;
    call->a[9] = a9;
    call->a[10] = a10;
    return (DotrPanelWord)(a0 + 1);
}
static void expect(int index, char kind, long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9,
                   long a10)
{
    long want[11];
    int i;

    want[0] = a0; want[1] = a1; want[2] = a2; want[3] = a3; want[4] = a4; want[5] = a5;
    want[6] = a6; want[7] = a7; want[8] = a8; want[9] = a9; want[10] = a10;
    assert(p_calls[index].kind == kind);
    for (i = 0; i < 11; i++) {
        assert(p_calls[index].a[i] == want[i]);
    }
}

int SzDuel_GetPlayer(int index) { prec('P', index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); return p_player[index]; }
int SzDuel_GetDeckCount(int index) { prec('d', index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); return p_deck; }
int SzDuel_GetUnitMonsterCount(int side) { prec('m', side, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); return 4 + side; }
int SzDuel_GetUnitSpellCount(int side) { prec('s', side, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); return 2 + side; }
DotrPanelWord lSzDuel_DrawWindow_Frame_Pipe(DotrPanelWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8)
{
    return prec('R', cursor, a1, a2, a3, x, y, a6, a7, a8, 0, 0);
}
DotrPanelWord lSzDuel_DrawWindow_PicMini_Pipe(DotrPanelWord cursor, int id, int x, int y, int width, int height, unsigned int z,
                                             long bright)
{
    return prec('I', cursor, id, x, y, width, height, z, bright, 0, 0, 0);
}
DotrPanelWord lSzDuel_DrawWindow_Board_Pipe(DotrPanelWord cursor, int side, int x, int y, int e, int f, int g)
{
    return prec('B', cursor, side, x, y, e, f, g, 0, 0, 0, 0);
}
DotrPanelWord SzDuel_DrawWindow_Digit_Pipe(DotrPanelWord cursor, int x, int y, int width, int value, int f)
{
    return prec('D', cursor, x, y, width, value, f, 0, 0, 0, 0, 0);
}
DotrPanelWord lSzDuel_DrawWindow_Icon_Pipe(DotrPanelWord cursor, char *row, int x, int y, int e, int f, int g, int h, long bright,
                                          long j, long k)
{
    return prec('C', cursor, row - gSzDuel_Icon_bytes, x, y, e, f, g, h, bright, j, k);
}
DotrPanelWord SzDuel_DrawWindow_Font_Pipe(DotrPanelWord cursor, int x, int y, int font, int flags, int n, const char *text,
                                         unsigned char *colour)
{
    return prec('T', cursor, x, y, font, flags, n, text == data_002bef76, colour == 0, 0, 0, 0);
}
DotrPanelWord SzDuel_DrawWindow_FontDigit_Pipe(DotrPanelWord cursor, int x, int y, int a, int flags, int width, int value)
{
    return prec('N', cursor, x, y, a, flags, width, value, 0, 0, 0, 0);
}

/* lSzDuel_DrawWindow_PlayerInfo_Pipe's views of four callees (declared without prototypes). */
DotrPanelWord func_001b4780_info(DotrPanelWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, int a8)
{
    return lSzDuel_DrawWindow_Frame_Pipe(cursor, a1, a2, a3, x, y, a6, a7, a8);
}
DotrPanelWord func_001b7f90_info(DotrPanelWord cursor, int id, int x, int y, int width, int height, int z, int bright)
{
    return lSzDuel_DrawWindow_PicMini_Pipe(cursor, id, x, y, width, height, (unsigned int)z, bright);
}
DotrPanelWord func_001b3c80_info(DotrPanelWord cursor, char *row, int x, int y, int e, int f, int g, int h, int bright, int j,
                                 int k)
{
    return lSzDuel_DrawWindow_Icon_Pipe(cursor, row, x, y, e, f, g, h, bright, j, k);
}
DotrPanelWord func_001b6f70_info(DotrPanelWord cursor, int x, int y, int font, int flags, int n, signed char *text, int colour)
{
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, font, flags, n, (const char *)text, colour != 0 ? (unsigned char *)text : 0);
}

int SzDuel_GetHandCount(int index) { prec('h', index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); return p_hand; }
int func_00250360_pair(int a, int b) { prec('K', a, b, 0, 0, 0, 0, 0, 0, 0, 0, 0); return p_pair; }
int Ta_IsUnitHide(struct DotrWindowUnit *unit, int side)
{
    prec('H', (long)(intptr_t)unit, side, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return p_hide;
}
int SzDuel_GetMapMovable(int x, int y) { prec('M', x, y, 0, 0, 0, 0, 0, 0, 0, 0, 0); return p_movable; }
int SzView_InqRequest(int a, int b) { prec('q', a, b, 0, 0, 0, 0, 0, 0, 0, 0, 0); return p_request; }
DotrPanelWord SzMsg_LoadFont_Pipe(DotrPanelWord cursor, int index, int message, int a3, int size)
{
    return prec('L', cursor, index, message, a3, size, 0, 0, 0, 0, 0, 0);
}
DotrPanelWord RhText_SetTextureFlush(DotrPanelWord cursor) { return prec('X', cursor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
struct DotrPanelInfo *SzMsg_GetInfo(int index)
{
    prec('g', index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return &p_info[p_info_count++];
}
void SzMsg_DrawRect_Pipe(void) {}
DotrPanelWord SzMsg_DrawRatioX_Pipe(DotrPanelWord cursor, int index, int x, int y, void *e, void (*function)(void), float sx,
                                    float sy)
{
    return prec('Q', cursor, index, x, y, e == 0, function == SzMsg_DrawRect_Pipe, (long)(sx * 1000.0f), (long)(sy * 1000.0f), 0, 0,
                0);
}
void SzMsg_Free(int index) { prec('f', index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }

static DotrPanelWord run(int side, int turn, int player0, int player1, long active)
{
    memset(&data_00317490_window, 0, sizeof(data_00317490_window));
    memset(p_calls, 0, sizeof(p_calls));
    p_count = 0;
    p_deck = 31 + side;
    p_player[0] = player0;
    p_player[1] = player1;
    data_00317490_window.side = (unsigned char)turn;
    data_00317490_window.unknown_c58 = 0x5a5a;
    data_00317490_window.sides[0].points = 4000;
    data_00317490_window.sides[0].player = (unsigned char)player0;
    data_00317490_window.sides[0].hand = 5;
    data_00317490_window.sides[1].points = 2500;
    data_00317490_window.sides[1].player = (unsigned char)player1;
    data_00317490_window.sides[1].hand = 3;
    /* The PlayerInfo C reads the side to move and the depth word at their target
     * offsets (0xd3c, 0xc58); host pointers in the view shift the named fields, so the bytes
     * are written at those offsets as well. */
    ((unsigned char *)&data_00317490_window)[0xd3c] = (unsigned char)turn;
    memcpy((unsigned char *)&data_00317490_window + 0xc58, &data_00317490_window.unknown_c58, 4);
    return lSzDuel_DrawWindow_PlayerInfo_Pipe(1000, side, side != 0 ? 440 : 18, 20, active);
}

/* Both player ids below 100: the third player query is skipped, nothing is hidden, and
 * every call of the panel is checked (side 0, so the portrait and name offset is 127 and the
 * column offset 0). */
static void check_player_info_open(void)
{
    assert(run(0, 0, 3, 7, 1) == 1015);
    assert(p_count == 20);
    expect(0, 'P', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(1, 'P', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(2, 'R', 1000, 0, 0, 0, 5, 5, 37, 13, 1, 0, 0);
    expect(3, 'I', 1001, 0, 148, 24, 48, 48, 0x5a5a, 1, 0, 0, 0);
    expect(4, 'R', 1002, -1, -1, 4, 148, 24, 3, 6, 1, 0, 0);
    expect(5, 'B', 1003, 0, 25, 29, 12, 1, -1, 0, 0, 0, 0);
    expect(6, 'D', 1004, 59, 31, 4, 4000, 0, 0, 0, 0, 0, 0);
    expect(7, 'C', 1005, 1860, 1876, 1895, 0, 1, 0, 0, 1, 0, -1);
    expect(8, 'C', 1006, 2460, 1757, 1859, 0, 1, 0, 0, 1, 0, -1);
    expect(9, 'C', 1007, 2150, 1757, 1883, 0, 1, 0, 0, 1, 0, -1);
    expect(10, 'N', 1008, 47, 60, 0, 0, 2, 5, 0, 0, 0, 0);
    expect(11, 'C', 1009, 2050, 1813, 1883, 0, 1, 0, 0, 1, 0, -1);
    expect(12, 'd', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(13, 'N', 1010, 103, 60, 0, 0, 2, 31, 0, 0, 0, 0);
    expect(14, 'C', 1011, 2250, 1757, 1899, 0, 1, 0, 0, 1, 0, -1);
    expect(15, 'm', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(16, 'N', 1011 + 1, 47, 76, 0, 0, 2, 4, 0, 0, 0, 0);
    expect(17, 'C', 1013, 2260, 1813, 1899, 0, 1, 0, 0, 1, 0, -1);
    expect(18, 's', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(19, 'N', 1014, 103, 76, 0, 0, 2, 2, 0, 0, 0, 0);
}

/* Side 1 with an id below 100 and to move, the first player's id 100 or more: the second
 * player query is skipped, the counts are hidden (three text calls, no monster or spell
 * query), the name row is 188, the offsets are 1 and 48, and the inactive flag reaches the
 * portrait, its frame and the name. */
static void check_player_info_hidden(void)
{
    assert(run(1, 1, 100, 7, 0) == 1015);
    assert(p_count == 18);
    expect(0, 'P', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(1, 'P', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(2, 'R', 1000, 1, 1, 0, 427, 5, 37, 13, 1, 0, 0);
    expect(3, 'I', 1001, 1, 444, 24, 48, 48, 0x5a5a, 0, 0, 0, 0);
    expect(4, 'R', 1002, -1, -1, 4, 444, 24, 3, 6, 0, 0, 0);
    expect(5, 'B', 1003, 1, 495, 29, 12, 1, -1, 0, 0, 0, 0);
    expect(6, 'D', 1004, 529, 31, 4, 2500, 0, 0, 0, 0, 0, 0);
    expect(7, 'C', 1005, 1880, 2172, 1895, 0, 1, 0, 0, 0, 0, -1);
    expect(8, 'C', 1006, 2460, 2227, 1859, 0, 1, 0, 0, 1, 0, -1);
    expect(9, 'C', 1007, 2150, 2227, 1883, 0, 1, 0, 0, 1, 0, -1);
    expect(10, 'T', 1008, 517, 60, 0, 0, 2, 1, 1, 0, 0, 0);
    expect(11, 'C', 1009, 2050, 2283, 1883, 0, 1, 0, 0, 1, 0, -1);
    expect(12, 'd', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(13, 'N', 1010, 573, 60, 0, 0, 2, 32, 0, 0, 0, 0);
    expect(14, 'C', 1011, 2250, 2227, 1899, 0, 1, 0, 0, 1, 0, -1);
    expect(15, 'T', 1012, 517, 76, 0, 0, 2, 1, 1, 0, 0, 0);
    expect(16, 'C', 1013, 2260, 2283, 1899, 0, 1, 0, 0, 1, 0, -1);
    expect(17, 'T', 1014, 573, 76, 0, 0, 2, 1, 1, 0, 0, 0);
}

/* The remaining outcomes of the hide test; p_calls[11] is the hand count ('N') or the text
 * ('T') in every run that makes three player queries, and the name row follows the id. */
static void check_player_info_hide_outcomes(void)
{
    /* side to move has an id below 100 and is this side: hidden */
    run(0, 0, 3, 100, 1);
    assert(p_calls[11].kind == 'T' && p_calls[8].a[1] == 1860);
    /* side to move has an id of 100 or more and is this side: shown */
    run(1, 1, 3, 100, 1);
    assert(p_calls[11].kind == 'N' && p_calls[11].a[6] == 3 && p_calls[8].a[1] == 1870);
    expect(2, 'P', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    /* side to move has an id of 100 or more and is the other side: hidden */
    run(0, 1, 3, 100, 1);
    assert(p_calls[11].kind == 'T');
    /* first player's id 100 or more: the second query is skipped; side to move (id below
     * 100) is the other side: shown, with the name row for an id below 100 on side 1 */
    run(0, 1, 100, 7, 1);
    assert(p_calls[1].kind == 'P' && p_calls[1].a[0] == 1 && p_calls[2].kind == 'R');
    assert(p_calls[10].kind == 'N' && p_calls[10].a[6] == 5 && p_calls[7].a[1] == 1850);
    /* side to move has an id below 100 and is the other side, three queries: shown */
    run(1, 0, 3, 100, 1);
    assert(p_calls[2].kind == 'P' && p_calls[2].a[0] == 0 && p_calls[11].kind == 'N' && p_calls[11].a[6] == 3);
}

static struct DotrWindowUnit g_unit;
static struct DotrWindowUnit g_other;

/* Clears the control block and gives every guide row an icon equal to its number in its
 * first entry and nothing else, so that the icon call names the row that was chosen. */
static void guide_reset(int mode, int side)
{
    int n;
    int i;

    memset(&data_00317490_window, 0, sizeof(data_00317490_window));
    memset(&g_unit, 0, sizeof(g_unit));
    memset(&g_other, 0, sizeof(g_other));
    for (n = 0; n < 21; n++) {
        for (i = 0; i < 4; i++) {
            data_00291180[n][i].icon = i == 0 ? n : 1023;
            data_00291180[n][i].dx = 0;
            data_00291180[n][i].dy = 0;
            data_00291180[n][i].message = 0xffff;
        }
    }
    data_00317490_window.unknown_d57 = (unsigned char)mode;
    data_00317490_window.side = (unsigned char)side;
    data_00317490_window.sides[side].x = 3;
    data_00317490_window.sides[side].y = 4;
    g_unit.side = 1;
    g_unit.place = 0;
    g_other.side = 0;
    g_other.place = 9;
    p_deck = 0;
    p_hand = 0;
    p_pair = side;
    p_hide = 0;
    p_movable = 0;
    p_request = 0;
    p_info_count = 0;
}
/* Runs the function and returns the chosen row, -1 when only the frame was drawn. */
static int guide_row(void)
{
    int i;
    int row = -1;

    memset(p_calls, 0, sizeof(p_calls));
    p_count = 0;
    lSzDuel_DrawWindow_CommandGuide_Pipe(500, 7, 100, 200);
    for (i = 0; i < p_count; i++) {
        if (p_calls[i].kind == 'C') {
            assert(row == -1);
            row = (int)(p_calls[i].a[1] / 10);
        }
    }
    return row;
}
static int guide_find(char kind)
{
    int i;

    for (i = 0; i < p_count; i++) {
        if (p_calls[i].kind == kind) {
            return i;
        }
    }
    return -1;
}

/* State 24: every outcome of the selected-unit tests. */
static void check_guide_select(void)
{
    guide_reset(24, 1);
    assert(guide_row() == 0 && p_count == 2);
    expect(0, 'R', 500, 7, 2, 3, 100, 200, 9, 18, 1, 0, 0);
    /* own unit in place 0: draw possible (flag and cards), by the unit's flag */
    guide_reset(24, 1);
    data_00317490_window.unit_6c8 = &g_unit;
    data_00317490_window.sides[1].unknown_346_bit_0 = -1;
    p_deck = 1;
    assert(guide_row() == 1);
    expect(1, 'K', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(2, 'd', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(3, 'h', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    g_unit.unknown_57_bit_0 = -1;
    assert(guide_row() == 3);
    /* the flag set but neither deck nor hand: falls to the last pair */
    p_deck = 0;
    assert(guide_row() == 11);
    g_unit.unknown_57_bit_0 = 0;
    assert(guide_row() == 10);
    /* hand only counts for the first pair with the flag, else gives the second pair */
    p_hand = 2;
    assert(guide_row() == 1);
    data_00317490_window.sides[1].unknown_346_bit_0 = 0;
    assert(guide_row() == 2);
    g_unit.unknown_57_bit_0 = -1;
    assert(guide_row() == 4);
    /* own unit in another place */
    g_unit.place = 5;
    assert(guide_row() == 11 && guide_find('d') == -1);
    g_unit.unknown_57_bit_0 = 0;
    assert(guide_row() == 10);
    /* the other side's unit: hidden only asked for a place above 0 */
    p_pair = 0;
    assert(guide_row() == 11);
    expect(2, 'H', (long)(intptr_t)&g_unit, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    p_hide = 1;
    assert(guide_row() == 0);
    g_unit.place = 0;
    assert(guide_row() == 11 && guide_find('H') == -1);
}

/* State 29: every outcome of the two-unit tests. */
static void check_guide_target(void)
{
    guide_reset(29, 1);
    data_00317490_window.unit_6c8 = &g_unit;
    assert(guide_row() == 5 && guide_find('K') == -1);
    data_00317490_window.unit_6cc = &g_unit;
    assert(guide_row() == 5 && guide_find('K') == -1);
    data_00317490_window.unit_6cc = &g_other;
    assert(guide_row() == 6);
    expect(1, 'K', 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    p_pair = 0;
    assert(guide_row() == 5);
    g_unit.place = 2;
    assert(guide_row() == 15);
    p_pair = 1;
    assert(guide_row() == 14);
    data_00317490_window.unit_6cc = &g_unit;
    assert(guide_row() == 12 && guide_find('K') == -1);
    g_unit.unknown_57_bit_4 = -1;
    assert(guide_row() == 13);
    data_00317490_window.unit_6cc = 0;
    assert(guide_row() == 5);
}

/* The remaining states, and a state with no guide. */
static void check_guide_states(void)
{
    guide_reset(62, 0);
    assert(guide_row() == 7);
    expect(1, 'M', 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    p_movable = 1;
    assert(guide_row() == -1 && p_count == 2);
    guide_reset(67, 0);
    assert(guide_row() == 8);
    guide_reset(83, 0);
    assert(guide_row() == 17);
    guide_reset(70, 0);
    assert(guide_row() == 9);
    guide_reset(41, 0);
    assert(guide_row() == 18);
    guide_reset(37, 0);
    assert(guide_row() == 18);
    guide_reset(45, 0);
    assert(guide_row() == 17);
    data_00317490_window.unknown_d66_bit_3 = -1;
    assert(guide_row() == 16);
    guide_reset(48, 0);
    assert(guide_row() == 19);
    expect(1, 'q', -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    p_request = 1;
    assert(guide_row() == -1 && p_count == 2);
    guide_reset(99, 0);
    assert(guide_row() == 20);
    guide_reset(0, 0);
    assert(guide_row() == -1 && p_count == 1);
}

/* The four lines of a row: icon and message (width 88, ratio 0.5), icon only, message only
 * (width 45, ratio 44/45), message at the boundary width 44 (ratio 1). */
static void check_guide_lines(void)
{
    guide_reset(99, 0);
    data_00291180[20][0].icon = 300;
    data_00291180[20][0].dx = 5;
    data_00291180[20][0].dy = 7;
    data_00291180[20][0].message = 0x123;
    data_00291180[20][1].icon = 2;
    data_00291180[20][2].message = 77;
    data_00291180[20][3].message = 78;
    p_info[0].width = 88;
    p_info[1].width = 45;
    p_info[2].width = 44;
    memset(p_calls, 0, sizeof(p_calls));
    p_count = 0;
    assert(lSzDuel_DrawWindow_CommandGuide_Pipe(500, 7, 100, 200) == 512);
    assert(p_count == 18);
    expect(0, 'R', 500, 7, 2, 3, 100, 200, 9, 18, 1, 0, 0);
    expect(1, 'C', 501, 3000, 1847, 2035, 0, 1, 0, 0, 1, 0, -1);
    expect(2, 'L', 502, 1, 0x123, 0, 22, 0, 0, 0, 0, 0, 0);
    expect(3, 'X', 503, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(4, 'g', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(5, 'Q', 504, 1, 140, 204, 1, 1, 500, 1000, 0, 0, 0);
    expect(6, 'f', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(7, 'C', 505, 20, 1842, 2050, 0, 1, 0, 0, 1, 0, -1);
    expect(8, 'L', 506, 1, 77, 0, 22, 0, 0, 0, 0, 0, 0);
    expect(9, 'X', 507, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(10, 'g', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(11, 'Q', 508, 1, 140, 248, 1, 1, 977, 1000, 0, 0, 0);
    expect(12, 'f', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(13, 'L', 509, 1, 78, 0, 22, 0, 0, 0, 0, 0, 0);
    expect(14, 'X', 510, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(15, 'g', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(16, 'Q', 511, 1, 140, 270, 1, 1, 1000, 1000, 0, 0, 0);
    expect(17, 'f', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

int main(void)
{
    check_player_info_open();
    check_player_info_hidden();
    check_player_info_hide_outcomes();
    check_guide_select();
    check_guide_target();
    check_guide_states();
    check_guide_lines();
    return 0;
}
