/* Synthetic host tests for the scaled_sprite group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/scaled_sprite.h"

unsigned char gSzDuel_aicolMsg_bytes[4];
static struct DotrSpriteEntry entry;
static int calls;
/* Deck menu mode: the stubs record their arguments instead of checking the fixed ones. */
static int deck_mode, deck_n;
static long deck_trace[40][11];
static DotrSpriteWord deck_log(long id, long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9)
{
    long row[11];

    row[0] = id; row[1] = a0; row[2] = a1; row[3] = a2; row[4] = a3; row[5] = a4;
    row[6] = a5; row[7] = a6; row[8] = a7; row[9] = a8; row[10] = a9;
    memcpy(deck_trace[deck_n++], row, sizeof row);
    return (DotrSpriteWord)a0 + 1;
}
static int expect_x;
static int expect_y = 7 + 48;
static int expect_a1 = 5 + 1174;
static void *expect_e = gSzDuel_aicolMsg_bytes;
static int frame_calls;
static float expect_sx;
static float expect_sy;

void SzMsg_DrawRect_Pipe(void)
{
}

DotrSpriteWord SzMsg_LoadFont_Pipe(DotrSpriteWord cursor, int a0, int a1, int a2, int a3)
{
    if (deck_mode) {
        return deck_log(1, cursor, a0, a1, a2, a3, 0, 0, 0, 0, 0);
    }
    assert(cursor == 0x1000 && a0 == 1 && a1 == expect_a1 && a2 == 0 && a3 == 22);
    calls++;
    return 0x2000;
}

DotrSpriteWord RhText_SetTextureFlush(DotrSpriteWord cursor)
{
    if (deck_mode) {
        return deck_log(2, cursor, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    assert(cursor == 0x2000);
    calls++;
    return 0x3000;
}

struct DotrSpriteEntry *SzMsg_GetInfo(int index)
{
    if (deck_mode) {
        deck_log(3, index, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        return &entry;
    }
    assert(index == 1);
    calls++;
    return &entry;
}

DotrSpriteWord SzMsg_DrawRatioX_Pipe(DotrSpriteWord cursor, int index, int x, int y, void *e, void (*function)(void),
                             float sx, float sy)
{
    if (deck_mode) {
        return deck_log(4, cursor, index, x, y, e == 0, function == SzMsg_DrawRect_Pipe, (long)(sx * 1000.0f), (long)(sy * 1000.0f), 0, 0);
    }
    assert(cursor == 0x3000 && index == 1 && x == expect_x && y == expect_y);
    assert(e == expect_e && function == SzMsg_DrawRect_Pipe && sx == expect_sx && sy == expect_sy);
    calls++;
    return 0x4000;
}

void SzMsg_Free(int index)
{
    if (deck_mode) {
        deck_log(5, index, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        return;
    }
    assert(index == 1 && calls == 4);
    calls++;
}

static int mark_mode, mark_cursor, mark_x, mark_y, mark_step, mark_frames;

DotrSpriteWord lSzDuel_DrawWindow_Frame_Pipe(DotrSpriteWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8)
{
    if (deck_mode) {
        return deck_log(6, cursor, a1, a2, a3, x, y, a6, a7, a8, 0);
    }
    if (mark_mode) {
        assert(cursor == mark_cursor && a1 == -1 && a2 == -1 && a3 == 8 && a6 == 12 && a7 == 19 && a8 == 1);
        assert(x == mark_x && y == mark_y - 4 && mark_step == 0);
        mark_frames++;
        return mark_cursor += 1;
    }
    if (frame_calls == 0) {
        assert(cursor == 0x3000 && a1 == -1 && a2 == 2 && a3 == 3 && x == 100 && y == 7 && a6 == 9 && a7 == 18);
    } else {
        assert(cursor == 0x3000 && a1 == 5 && a2 == -1 && a3 == 3 && x == 100 && y == 7 + 64 && a6 == 9 && a7 == 2);
    }
    assert(a8 == 1 && calls == 2);
    frame_calls++;
    return 0x3000;
}

int func_001a22c0(float value)
{
    return (int)value;
}

/* lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe (branch coverage: flag byte clear and set, title
 * width below and above 154, a short deck, a leader of 999, the selected and the other columns,
 * both sides). */
struct DotrDeckMenuBlock dotr_deck_menu_00a9c000;
char gSzDuel_Icon_bytes[2560], data_002bf05b[8], data_002bef93[8];
unsigned char data_002be944[12]; /* hint colour: never passed here */
static int deck_counts[3], deck_leaders[3];
int SzData_GetDeckCount(int side, int list)
{
    deck_log(7, side, list, 0, 0, 0, 0, 0, 0, 0, 0);
    return deck_counts[list];
}
int SzData_GetLeader(int side, int list)
{
    deck_log(8, side, list, 0, 0, 0, 0, 0, 0, 0, 0);
    return deck_leaders[list];
}
int func_0019bef0(const char *string)
{
    deck_log(9, string == data_002bf05b ? 1 : 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return 7;
}
static const char *font_texts[8];
static int font_text_n;
DotrSpriteWord SzDuel_DrawWindow_Font_Pipe(DotrSpriteWord cursor, int x, int y, int a, int flags, int n, const char *text, int c)
{
    if (font_text_n < 8) {
        font_texts[font_text_n++] = text;
    }
    return deck_log(10, cursor, x, y, a, flags, n, text == data_002bf05b ? 1 : 2, c, 0, 0);
}
DotrSpriteWord lSzDuel_DrawWindow_PicMini_Pipe(DotrSpriteWord cursor, int id, int x, int y, int width, int height, unsigned int z, int bright)
{
    return deck_log(11, cursor, id, x, y, width, height, z, bright, 0, 0);
}
DotrSpriteWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrSpriteWord cursor, char *def, int x, int y, int a4, int a5, int a6, int a7,
                                              int a8, int a9, int a10, int a11, int a12, int a13)
{
    assert(a4 == 0 && a5 == 1 && a6 == 0 && a7 == 0 && a8 == 0 && a9 == 0);
    return deck_log(12, cursor, def - gSzDuel_Icon_bytes, x, y, a10, a11, a12, a13, 0, 0);
}
DotrSpriteWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrSpriteWord cursor, int side, int x, int y, int w, int h, int a6, int a7)
{
    return deck_log(13, cursor, side, x, y, w, h, a6, a7, 0, 0);
}
/* lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe (branch and loop coverage: id 682 and 683, no entry,
 * two entries, both flag values, name width 254 and 255; the count's byte result; the fifth word is
 * the address of gSzDuel_aicolMsg_bytes). */
static int enchant_n, enchant_id;
unsigned char func_00249410_enchant(unsigned short id, unsigned short *list)
{
    int i;

    assert(id == enchant_id);
    for (i = 0; i < enchant_n; i++) {
        list[i] = 700 + i;
    }
    return (unsigned char)(0x100 | enchant_n);
}
int *func_001feab0_enchant(int index)
{
    deck_log(3, index, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return (int *)&entry; /* word 3 is unknown_0c */
}
DotrSpriteWord func_001fe660_enchant(DotrSpriteWord cursor, int index, int x, int y, float sx, float sy,
                                     DotrSpriteAddr e, void (*function)(void))
{
    return deck_log(4, cursor, index, x, y, e != (DotrSpriteAddr)gSzDuel_aicolMsg_bytes, function == SzMsg_DrawRect_Pipe,
                    (long)(sx * 1000.0f), (long)(sy * 1000.0f), 0, 0);
}
int SzDuel_GetMsgName(int index)
{
    return deck_log(15, index, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
static void expect_row(int at, long id, long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7)
{
    long row[11] = {0};

    row[0] = id; row[1] = a0; row[2] = a1; row[3] = a2; row[4] = a3; row[5] = a4;
    row[6] = a5; row[7] = a6; row[8] = a7;
    assert(memcmp(deck_trace[at], row, sizeof row) == 0);
}
static void check_enchant_list(int n, int flag, int width)
{
    int base = flag ? 640 : -320;
    long sx = width > 254 ? (long)(254.0f / (float)width * 1000.0f) : 1000;
    int i, at = 0;
    DotrSpriteWord c = 0x1000;

    deck_mode = 1;
    deck_n = 0;
    enchant_n = n;
    enchant_id = 682;
    entry.unknown_0c = width;
    assert(lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe(0x1000, 682, 100, 7, flag, 3) == 0x1001 + 3 * n);
    for (i = 0; i < n; i++) {
        expect_row(at++, 15, 700 + i, 0, 0, 0, 0, 0, 0, 0);
        expect_row(at++, 1, c++, i + 1, 701 + i, 0, (i + 1) * 22, 0, 0, 0);
    }
    expect_row(at++, 2, c++, 0, 0, 0, 0, 0, 0, 0);
    for (i = 0; i < n; i++) {
        expect_row(at++, 3, i + 1, 0, 0, 0, 0, 0, 0, 0);
        expect_row(at++, 13, c++, 3, 100 + base + 28, 7 + 48 + i * 26, -262, -26, -1, 0);
        expect_row(at++, 4, c++, i + 1, 100 + base + 33, 7 + 50 + i * 26, 0, 1, sx, 1000);
        expect_row(at++, 5, i + 1, 0, 0, 0, 0, 0, 0, 0);
    }
    assert(deck_n == at);
    deck_n = 0;
    assert(lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe(0x1000, 683, 100, 7, flag, 3) == 0x1000 && deck_n == 0);
    deck_mode = 0;
}

/* lSzDuel_DrawBuildDeck_Detail_Ability_Pipe (ids 682 and 683, no ability, abilities 4, 9 and 16,
 * both flag values, name width 226 and 227). */
struct DotrSpriteAbilityPair EffIdx_L_ability[683][5];
unsigned char DeckBuildingSide_ability;
static unsigned int ability_mask;
/* lSzDuel_DrawBuildDeck_Detail_Ability_Pipe declares SzMsg_DrawRatioX_Pipe in its body under
 * this view name (scale words before the colour word, the colour as a 32-bit word); this host
 * forwarder calls the stub above. */
int func_001fe660_ability(int cursor, int index, int x, int y, float sx, float sy, int e, void (*function)(void))
{
    return SzMsg_DrawRatioX_Pipe(cursor, index, x, y, (void *)(long)e, function, sx, sy);
}
void func_001dcd70_ability(struct DotrSpriteAbilityItem *item, int id)
{
    assert(id == 682);
    memset(item, 0, sizeof *item);
    item->owner = 9;
}
int func_001e0c80_ability(struct DotrSpriteAbilityItem *item, int which, int *out, int flag)
{
    int k;

    assert(out == 0 && flag == 1 && item->owner == 1);
    for (k = 0; k < 5; k++) {
        assert(item->pair[k].unknown_00 == (float)k && item->pair[k].unknown_04 == (float)(k + 10));
    }
    return (ability_mask >> which) & 1;
}
static void check_ability_list(unsigned int mask, int flag, int width)
{
    int base = flag ? 640 : -320;
    long sx = width > 226 ? (long)(226.0f / (float)width * 1000.0f) : 1000;
    int i, k, at = 0, row = 0;
    DotrSpriteWord c = 0x1000;

    for (k = 0; k < 5; k++) {
        EffIdx_L_ability[682][k].unknown_00 = (float)k;
        EffIdx_L_ability[682][k].unknown_04 = (float)(k + 10);
    }
    DeckBuildingSide_ability = 1;
    deck_mode = 1;
    deck_n = 0;
    ability_mask = mask;
    entry.unknown_0c = width;
    lSzDuel_DrawBuildDeck_Detail_Ability_Pipe(0x1000, 682, 100, 7, flag, 3);
    for (i = 0; i < 13; i++) {
        expect_row(at++, 1, c++, i + 1, i + 101, 0, (i + 1) * 22, 0, 0, 0);
    }
    expect_row(at++, 2, c++, 0, 0, 0, 0, 0, 0, 0);
    for (i = 4; i < 17; i++) {
        if ((mask >> i) & 1) {
            expect_row(at++, 12, c++, (i + 30) * 10, 100 + 1756 + base, 7 + 1873 + row * 26, 1, 0, -1, 128);
            expect_row(at++, 13, c++, 3, 100 + base + 54, 7 + 48 + row * 26, -236, -26, -1, 0);
            expect_row(at++, 3, i - 3, 0, 0, 0, 0, 0, 0, 0);
            expect_row(at++, 4, c++, i - 3, 100 + base + 59, 7 + 50 + row * 26, 0, 1, sx, 1000);
            row++;
        }
        expect_row(at++, 5, i - 3, 0, 0, 0, 0, 0, 0, 0);
    }
    assert(deck_n == at);
    deck_n = 0;
    assert(lSzDuel_DrawBuildDeck_Detail_Ability_Pipe(0x1000, 683, 100, 7, flag, 3) == 0x1000 && deck_n == 0);
    deck_mode = 0;
}

static void check_deck_menu(void)
{
    static const long expect[31][11] = {
        {1, 0x1000, 1, 134, 0, 22},
        {2, 0x1001},
        {6, 0x1002, 12, 3, 4, 149, 50, -178, -32, 1},
        {3, 1},
        {4, 0x1003, 1, 188, 55, 1, 1, 1000, 1000},
        {5, 1},
        {6, 0x1004, 0, -1, 11, 100, 82, -276, -150, 1},
        {7, 0, 0},
        {8, 0, 0},
        {12, 0x1005, 2270, 1885, 1917, 0, 0, -1, 128},
        {11, 0x1006, 10, 145, 120, 48, 48, 0x123456, 0},
        {6, 0x1007, -1, -1, 7, 145, 120, -48, -48, 1},
        {7, 0, 1},
        {8, 0, 1},
        {12, 0x1008, 2280, 1954, 1917, 1, 0, -1, 128},
        {11, 0x1009, 11, 214, 120, 48, 48, 0x123456, 1},
        {6, 0x100a, 5, -1, 4, 214, 120, -48, -48, 1},
        {7, 0, 2},
        {8, 0, 2},
        {12, 0x100b, 2290, 2023, 1917, 0, 0, -1, 128},
        {11, 0x100c, 12, 283, 120, 48, 48, 0x123456, 0},
        {6, 0x100d, 5, -1, 7, 283, 120, -48, -48, 1},
        {13, 0x100e, 0, 118, 172, -240, -52, -1, 0},
        {12, 0x100f, 2520, 1890, 2003, 1, 0, -1, 128},
        {12, 0x1010, 2530, 1906, 2003, 1, 0, -1, 128},
        {9, 1},
        {10, 0x1011, 178, 206, 1, 2, 7, 1, 0},
        {12, 0x1012, 2540, 1958, 2003, 1, 0, -1, 128},
        {9, 2},
        {10, 0x1013, 238, 206, 1, 2, 7, 2, 0},
    };
    int word = 0x123456;
    int i;

    deck_mode = 1;
    memcpy(data_00317490_bytes + 3160, &word, sizeof word);
    dotr_deck_menu_00a9c000.unknown_02[0] = 0x35;
    dotr_deck_menu_00a9c000.unknown_02[1] = 0x1f;
    dotr_deck_menu_00a9c000.unknown_04[0] = 1;
    dotr_deck_menu_00a9c000.unknown_04[1] = 2;
    dotr_deck_menu_00a9c000.unknown_88 = 0;
    entry.unknown_0c = 100;
    deck_counts[0] = 40;
    deck_counts[1] = 39;
    deck_counts[2] = 40;
    deck_leaders[0] = 5;
    deck_leaders[1] = 5;
    deck_leaders[2] = 999;
    deck_n = 0;
    assert(lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe(0x1000, 0, 100, 50) == 0x1014);
    assert(deck_n == 30);
    for (i = 0; i < 30; i++) {
        assert(memcmp(deck_trace[i], expect[i], sizeof expect[i]) == 0);
    }

    /* Side 1, wide window, a title of 308 scaled to 154, all decks usable, column 2 selected. */
    dotr_deck_menu_00a9c000.unknown_88 = 1;
    entry.unknown_0c = 308;
    deck_counts[1] = 40;
    deck_leaders[2] = 998;
    deck_n = 0;
    assert(lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe(0x1000, 1, 100, 50) == 0x1014);
    assert(deck_n == 30 && deck_trace[0][3] == 144);
    assert(deck_trace[2][5] == 301 && deck_trace[4][3] == 313 && deck_trace[4][7] == 500 && deck_trace[4][8] == 1000);
    assert(deck_trace[6][2] == 1 && deck_trace[6][7] == -580);
    assert(deck_trace[9][3] == 100 + 1716 + 145 && deck_trace[9][5] == 0);
    assert(deck_trace[10][2] == 20 && deck_trace[15][2] == 21 && deck_trace[20][2] == 22);
    assert(deck_trace[11][2] == -1 && deck_trace[16][2] == -1 && deck_trace[21][2] == -1);
    assert(deck_trace[11][4] == 7 && deck_trace[16][4] == 7 && deck_trace[21][4] == 4 && deck_trace[20][8] == 1);
    assert(deck_trace[22][2] == 1 && deck_trace[22][5] == -544);
    assert(deck_trace[23][3] == 100 + 1730 + 136 && deck_trace[27][3] == 100 + 1738 + 272);
    assert(deck_trace[26][2] == 100 + 18 + 136 && deck_trace[29][2] == 100 + 18 + 272);
    deck_mode = 0;
}

/* 0x1b3640: width at the bound (no scaling) and above it. */
static void check_framed(int width, int x, float sx)
{
    entry.unknown_0c = width;
    entry.unknown_10 = 9999;
    expect_a1 = 5 + 71;
    expect_x = x;
    expect_y = 7 + 67;
    expect_e = 0;
    expect_sx = sx;
    expect_sy = 1.0f;
    calls = frame_calls = 0;
    assert(lSzDuel_DrawWindow_Field_Pipe(0x1000, 5, 100, 7) == 0x4000);
    assert(calls == 5 && frame_calls == 2);
}

static void check_scaled(int width, int height, int flag, int x, float sx, float sy)
{
    entry.unknown_0c = width;
    entry.unknown_10 = height;
    expect_x = x;
    expect_sx = sx;
    expect_sy = sy;
    calls = 0;
    assert(lSzDuel_DrawBuildDeck_Detail_Special_Pipe(0x1000, 5, 100, 7, flag) == 0x4000);
    assert(calls == 5);
}

unsigned char data_00317490_bytes[0xd50], turnOwner[1];
char data_002902a6[4];
static int mark_ready, mark_count, mark_index, mark_pieces, mark_digits;
static struct DotrSpritePoint mark_points[4];
static struct DotrSpriteMark mark_blocks[4];
static int mark_present[4], mark_words[4];

int SzGame_GetMode(int a)
{
    assert(a == 1);
    return mark_ready;
}

int SzDuel_GetHandCount(int index)
{
    assert(index == 5);
    return mark_count;
}

struct DotrSpriteMark *func_001df540_mark(int a, int b)
{
    assert(a == 2 && b >= 1 && b <= mark_count);
    mark_index = b - 1;
    mark_x = (int)mark_points[mark_index].x + 260;
    mark_y = (int)mark_points[mark_index].y + 139;
    mark_step = 0;
    return mark_present[mark_index] ? &mark_blocks[mark_index] : 0;
}

int SzDuel_InqHand(int index, int position)
{
    assert(index == 5 && position == mark_index);
    return mark_words[position];
}

DotrSpriteWord func_001b3c80_cursor(DotrSpriteWord cursor, char *b, int c, int d, int e, int f, int g, int h,
                                    long i, long j, long k)
{
    assert(cursor == mark_cursor && b == data_002902a6 && c == mark_x + 1728 && d == mark_y + 1796);
    assert(e == 1 && f == 1 && g == 0 && h == 0 && i == 1 && j == 0 && k == -1 && mark_step == 0);
    mark_step = 1;
    mark_pieces++;
    return mark_cursor += 16;
}

DotrSpriteWord SzDuel_DrawWindow_FontDigit_Pipe(DotrSpriteWord cursor, int x, int y, int a, int flags, int width, int value)
{
    if (deck_mode) {
        return deck_log(14, cursor, x, y, a, flags, width, value, 0, 0, 0);
    }
    assert(cursor == mark_cursor && x == mark_x + 16 && y == mark_y - 23 && a == 0 && flags == 2 && width == 1);
    assert(value == ((mark_words[mark_index] >> 12) & 15) && mark_step == 1);
    mark_step = 2;
    mark_digits++;
    return mark_cursor += 256;
}

static int mark_run(int ready, int on, int off, int count)
{
    mark_mode = 1;
    mark_ready = ready;
    data_00317490_bytes[3397] = on;
    data_00317490_bytes[3399] = off;
    mark_count = count;
    mark_cursor = 0x4000;
    mark_frames = mark_pieces = mark_digits = 0;
    mark_cursor = SzDuel_DrawHandCursor_Pipe(0x4000) == mark_cursor ? mark_cursor : -1;
    mark_mode = 0;
    return mark_cursor;
}

/* SzDuel_DrawHandCursor_Pipe (branch coverage) with all eight callees stubbed. */
static void check_marks(void)
{
    int i;
    turnOwner[0] = 5;
    data_00317490_bytes[3407] = 9;
    for (i = 0; i < 4; i++) {
        mark_points[i].x = 10.0f * i;
        mark_points[i].y = 3.0f * i;
        mark_blocks[i].unknown_04 = &mark_points[i];
        mark_present[i] = 1;
        mark_words[i] = 0x2000 + i;
    }
    /* the three gates return the cursor untouched */
    assert(mark_run(0, 1, 0, 4) == 0x4000 && mark_pieces == 0);
    assert(mark_run(2, 1, 0, 4) == 0x4000 && mark_pieces == 0);
    assert(mark_run(1, 0, 0, 4) == 0x4000 && mark_pieces == 0);
    assert(mark_run(1, 1, 1, 4) == 0x4000 && mark_pieces == 0);
    /* count 0: no entry is fetched */
    assert(mark_run(1, 1, 0, 0) == 0x4000 && mark_pieces == 0);
    /* every entry draws a piece and its digit; no frame without a selected index */
    assert(mark_run(1, 1, 0, 4) == 0x4000 + 4 * 272 && mark_pieces == 4 && mark_digits == 4 && mark_frames == 0);
    /* missing block, missing point pointer, negative word: skipped */
    mark_present[0] = 0;
    mark_blocks[1].unknown_04 = 0;
    mark_words[2] = -1;
    assert(mark_run(1, 1, 0, 4) == 0x4000 + 272 && mark_pieces == 1 && mark_digits == 1);
    mark_present[0] = 1;
    mark_blocks[1].unknown_04 = &mark_points[1];
    /* the selected index gets the frame first, also when nothing else is drawn;
     * low bits 999 and digit 0 draw no piece; a skipped entry gets no frame */
    data_00317490_bytes[3407] = 0;
    mark_words[0] = 0x3000 + 999;
    mark_words[1] = 0x0fff;
    mark_words[2] = 0xf005;
    assert(mark_run(1, 1, 0, 4) == 0x4000 + 1 + 2 * 272 && mark_frames == 1 && mark_pieces == 2);
    data_00317490_bytes[3407] = 2;
    assert(mark_run(1, 1, 0, 4) == 0x4000 + 1 + 2 * 272 && mark_frames == 1 && mark_pieces == 2);
    mark_words[2] = -1;
    assert(mark_run(1, 1, 0, 4) == 0x4000 + 272 && mark_frames == 0 && mark_pieces == 1);
}

/* lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe: every call in order for side 1 in the narrow
 * window with a title too wide (scaled), then the wide window for side 0 with a title that fits. */
static void check_summon_charge(void)
{
    static const long expect[18][11] = {
        {1, 0x1000, 1, 129 + 0xf, 0, 22},
        {2, 0x1001},
        {6, 0x1002, 12, 3, 4, 238 - 89, 50, -178, -32, 1},
        {3, 1},
        {4, 0x1003, 1, 238 - 77, 55, 1, 1, 500, 1000},
        {5, 1},
        {6, 0x1004, 1, -1, 11, 100, 82, -276, -150, 1},
        {12, 0x1005, 2070, 100 + 1672 + 138, 50 + 1892, 1, 0, -1, 128},
        {6, 0x1006, 12, -1, 5, 238 - 30, 120, -60, -20, 1},
        {12, 0x1007, 2150, 100 + 1700 + 138, 50 + 1896, 1, 0, -1, 128},
        {14, 0x1008, 238 - 6, 124, 0, 0, 2, 9},
        {12, 0x1009, 2080, 100 + 1760 + 138, 50 + 1892, 1, 0, -1, 128},
        {13, 0x100a, 1, 118, 172, -240, -52, -1, 0},
        {12, 0x100b, 2520, 100 + 1730 + 60, 50 + 1953, 1, 0, -1, 128},
        {12, 0x100c, 2530, 100 + 1746 + 60, 50 + 1953, 1, 0, -1, 128},
        {9, 1},
        {10, 0x100d, 100 + 18 + 60, 206, 1, 2, 7, 1, 0},
        {12, 0x100e, 2540, 100 + 1738 + 120, 50 + 1953, 1, 0, -1, 128},
    };
    int i;

    deck_mode = 1;
    dotr_deck_menu_00a9c000.unknown_02[0] = 0x31;
    dotr_deck_menu_00a9c000.unknown_02[1] = 0x2f;
    ((unsigned char *)&dotr_deck_menu_00a9c000)[6] = 4;
    ((unsigned char *)&dotr_deck_menu_00a9c000)[7] = 9;
    dotr_deck_menu_00a9c000.unknown_88 = 0;
    entry.unknown_0c = 308;
    deck_n = 0;
    assert(lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe(0x1000, 1, 100, 50) == 0x1010);
    assert(deck_n == 20);
    for (i = 0; i < 18; i++) {
        assert(memcmp(deck_trace[i], expect[i], sizeof expect[i]) == 0);
    }
    assert(deck_trace[18][0] == 9 && deck_trace[18][1] == 2);
    assert(deck_trace[19][0] == 10 && deck_trace[19][2] == 100 + 18 + 120 && deck_trace[19][3] == 206 &&
           deck_trace[19][7] == 2 && deck_trace[19][8] == 0);

    /* wide window (580), side 0: message 129 + 1, unscaled title, the side's charge 4 */
    dotr_deck_menu_00a9c000.unknown_88 = 1;
    entry.unknown_0c = 100;
    deck_n = 0;
    lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe(0x1000, 0, 100, 50);
    assert(deck_n == 20 && deck_trace[0][3] == 130);
    assert(deck_trace[2][5] == 390 - 89 && deck_trace[4][3] == 390 - 50 && deck_trace[4][7] == 1000);
    assert(deck_trace[6][2] == 0 && deck_trace[6][7] == -580);
    assert(deck_trace[7][3] == 100 + 1672 + 290 && deck_trace[10][2] == 390 - 6 && deck_trace[10][7] == 4);
    assert(deck_trace[12][2] == 0 && deck_trace[12][5] == -544);
    assert(deck_trace[13][3] == 100 + 1730 + 136 && deck_trace[17][3] == 100 + 1738 + 272);
    deck_mode = 0;
}

/* lSzDuel_DrawBuildDeck_BagCursor_Pipe: every call in order with the halfword at +0xc96 clear,
 * for both values of the byte at 0x3181ed (3 and not 3), and the early return when it is set. */
char data_002befe7[4], data_002befec[4], data_002beff3[4], data_002beffb[4];
unsigned char BuildDeckHorizontalCursorIndex_bag;
static void check_bag_cursor(void)
{
    static const long expect[13][11] = {
        {6, 0x1000, -1, 4, 6, 16, 238, -304, -54, 1},
        {6, 0x1001, 13, -1, 7, 18, 294, -304, -20, 1},
        {12, 0x1002, 2530, 1751, 2120, 1, 0, -1, 128},
        {12, 0x1003, 2520, 1768, 2120, 1, 0, -1, 128},
        {9, 2},
        {10, 0x1004, 62, 299, 1, 0, 7, 2, 0},
        {12, 0x1005, 2510, 1837, 2120, 1, 0, -1, 128},
        {9, 2},
        {10, 0x1006, 131, 299, 1, 0, 7, 2, 0},
        {12, 0x1007, 2550, 1947, 2120, 1, 0, -1, 128},
        {9, 2},
        {10, 0x1008, 241, 299, 1, 0, 7, 2, 0},
        {12, 0x1009, 2020, 2029, 2120, 1, 0, -1, 128},
    };
    unsigned short half;
    int i;

    deck_mode = 1;
    half = 0;
    memcpy(data_00317490_bytes + 0xc96, &half, sizeof half);
    BuildDeckHorizontalCursorIndex_bag = 2;
    deck_n = 0;
    font_text_n = 0;
    /* x = 10 + 8, y = 20 + 0x70 + 2 * 0x36 = 240 */
    assert(lSzDuel_DrawBuildDeck_BagCursor_Pipe(0x1000, 10, 20, 2) == 0x100a);
    assert(deck_n == 13);
    for (i = 0; i < 13; i++) {
        assert(memcmp(deck_trace[i], expect[i], sizeof expect[i]) == 0);
    }
    assert(font_text_n == 3 && font_texts[0] == data_002befe7 && font_texts[1] == data_002beff3 &&
           font_texts[2] == data_002beffb);

    /* byte 3: icon 251 at sx + 0x6b and the second caption at x + 0x81 */
    BuildDeckHorizontalCursorIndex_bag = 3;
    deck_n = 0;
    font_text_n = 0;
    assert(lSzDuel_DrawBuildDeck_BagCursor_Pipe(0x1000, 10, 20, 2) == 0x100a);
    assert(deck_n == 13 && deck_trace[6][3] == 1746 + 0x6b && deck_trace[8][2] == 18 + 0x81);
    assert(font_texts[1] == data_002befec);

    /* the halfword set: only the outer frame */
    half = 0x100;
    memcpy(data_00317490_bytes + 0xc96, &half, sizeof half);
    deck_n = 0;
    assert(lSzDuel_DrawBuildDeck_BagCursor_Pipe(0x1000, 10, 20, 0) == 0x1001);
    assert(deck_n == 1 && deck_trace[0][6] == 20 + 0x70 - 2);
    half = 0;
    memcpy(data_00317490_bytes + 0xc96, &half, sizeof half);
    deck_mode = 0;
}

/* lSzDuel_DrawCustomDuel_Menu_LeaderAbility_Pipe and _CardDisp_Pipe: every call in order for
 * side 1 with the choice byte clear, then set (the two captions swap colours). */
char data_002bf051[4], data_002bf056[4], data_002bf062[4], data_002bf066[4];
DotrSpriteWord func_001fe7d0_sprite(DotrSpriteWord cursor, int index, int x, int y, int e, void (*function)(void))
{
    return deck_log(16, cursor, index, x, y, e, function == SzMsg_DrawRect_Pipe, 0, 0, 0, 0);
}
static void check_choice_window(DotrSpriteWord (*draw)(DotrSpriteWord, DotrSelDeckSide, int, int), int byte,
                                const char *first, const char *second)
{
    long colour = (int)(intptr_t)data_002be944;
    long expect[19][11] = {
        {1, 0x1000, 1, 129 + 0xf, 0, 22},
        {2, 0x1001},
        {6, 0x1002, 12, 3, 4, 149, 50, -178, -32, 1},
        {3, 1},
        {16, 0x1003, 1, 218, 55, 0, 1},
        {5, 1},
        {6, 0x1004, 1, -1, 11, 100, 82, -276, -150, 1},
        {9, 2},
        {10, 0x1005, 192, 127, 0, 3, 7, 2, 0},
        {9, 2},
        {10, 0x1006, 284, 127, 0, 3, 7, 2, 0},
        {13, 0x1007, 1, 118, 172, -240, -52, -1, 0},
        {12, 0x1008, 2520, 1890, 2003, 1, 0, -1, 128},
        {12, 0x1009, 2530, 1906, 2003, 1, 0, -1, 128},
        {9, 1},
        {10, 0x100a, 178, 206, 1, 2, 7, 1, 0},
        {12, 0x100b, 2540, 1958, 2003, 1, 0, -1, 128},
        {9, 2},
        {10, 0x100c, 238, 206, 1, 2, 7, 2, 0},
    };
    int i;
    int set;

    deck_mode = 1;
    dotr_deck_menu_00a9c000.unknown_02[1] = 0x2f;
    entry.unknown_0c = 40;
    for (set = 0; set < 2; set++) {
        ((unsigned char *)&dotr_deck_menu_00a9c000)[byte + 1] = (unsigned char)(set ? 4 : 0);
        expect[8][8] = set ? colour : 0;
        expect[10][8] = set ? 0 : colour;
        deck_n = 0;
        font_text_n = 0;
        assert(draw(0x1000, 1, 100, 50) == 0x100d);
        assert(deck_n == 19);
        for (i = 0; i < 19; i++) {
            assert(memcmp(deck_trace[i], expect[i], sizeof expect[i]) == 0);
        }
        assert(font_text_n == 4 && font_texts[0] == first && font_texts[1] == second &&
               font_texts[2] == data_002bf05b && font_texts[3] == data_002bef93);
    }
    ((unsigned char *)&dotr_deck_menu_00a9c000)[byte + 1] = 0;
    /* side 0: the title byte of side 0 */
    dotr_deck_menu_00a9c000.unknown_02[0] = 0x31;
    deck_n = 0;
    draw(0x1000, 0, 100, 50);
    assert(deck_trace[0][3] == 130 && deck_trace[6][2] == 0 && deck_trace[11][2] == 0);
    deck_mode = 0;
}

int main(void)
{
    check_choice_window(lSzDuel_DrawCustomDuel_Menu_LeaderAbility_Pipe, 8, data_002bf066, data_002bf062);
    check_choice_window(lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe, 12, data_002bf056, data_002bf051);
    check_bag_cursor();
    check_marks();
    check_deck_menu();
    check_summon_charge();
    assert(offsetof(struct DotrSpriteEntry, unknown_0c) == 0xc);
    assert(offsetof(struct DotrSpriteEntry, unknown_10) == 0x10);
    check_scaled(262, 256, 1, 100 + 640 + 28, 1.0f, 1.0f);
    check_scaled(262, 256, 0, 100 - 320 + 28, 1.0f, 1.0f);
    check_scaled(524, 256, 1, 100 + 640 + 28, 0.5f, 1.0f);
    check_scaled(262, 512, 1, 100 + 640 + 28, 1.0f, 0.5f);
    check_scaled(263, 257, 0, 100 - 320 + 28, 262.0f / 263, 256.0f / 257);
    check_framed(84, 100 + 48 - 42, 1.0f);
    check_framed(168, 100 + 48 - 42, 0.5f);
    check_framed(85, 100 + 48 - (int)(42 * (84.0f / 85)), 84.0f / 85);
    check_enchant_list(0, 0, 254);
    check_enchant_list(2, 0, 254);
    check_enchant_list(2, 1, 255);
    check_ability_list(0, 0, 226);
    check_ability_list(1u << 4 | 1u << 9 | 1u << 16, 1, 226);
    check_ability_list(1u << 9, 0, 227);
    puts("scaled_sprite: ok");
    return 0;
}
