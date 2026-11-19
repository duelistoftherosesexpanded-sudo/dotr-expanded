/* Synthetic host tests for the window_font group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/window_font.h"

unsigned char gSzDuel_abFontSize_bytes[6] = { 16, 20, 10, 12, 8, 8 }; /* width, height per font */
struct DotrFontGlyph data_00290b70[3 * 37];
unsigned char icolRgba_2580[4] = { 1, 2, 3, 4 };
struct DotrCursorPicControl data_00317490;
struct DotrModeLastView gSzGame_gl_last;
struct DotrModeLastColour data_0028ff20 = { { 10, 20, 30, 40 } };
char data_002be815[8] = "AB";

static DotrFontPacket prims[640];
static intptr_t prim_texture[640];
static int prim_count, texture_count, texture_flag, sort_args;
static int alpha_index, alpha_calls;
static long alpha_value;

/* Stub: 'A'..'E' are glyphs 0..4; anything else has no glyph. */
int lSzDuel_GetFontIndex(char c)
{
    return c >= 'A' && c <= 'E' ? c - 'A' : -1;
}

long ExGetAlphaVal(int index)
{
    alpha_index = index;
    return 0x1000 + index;
}

int ExAssignAlpha(struct DotrCursorPacketHeader *packet, long value)
{
    (void)packet;
    alpha_calls++;
    alpha_value = value;
    return 0;
}

int ExAssignTexture(struct DotrCursorPacketHeader *packet, int *texture, int a)
{
    (void)packet;
    prim_texture[texture_count++] = (intptr_t)texture;
    texture_flag |= a;
    return 0;
}

DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, void *packet, int a1, int a2)
{
    /* Five register words: the two-vertex rectangle of SzGame_ModeDrawLast; nine: a glyph. */
    memcpy(&prims[prim_count++], packet,
           ((struct DotrCursorPacketHeader *)packet)->unknown_00 == 5 ? sizeof(struct DotrModeLastPacket)
                                                                    : sizeof(DotrFontPacket));
    sort_args |= a1 | a2;
    return cursor + 16;
}

static int mode_arg, start_calls, prio_value, texture_arg, finish_arg, focus_calls, focus_args[6];
static int strlen_calls;
static DotrDrawWord start_arg, focus_cursor, area_set;
static int *window_texture;

int SzGame_GetMode(int a)
{
    mode_arg = a;
    return 0;
}

DotrDrawWord RhBase_GetVifPacketArea(void)
{
    return 0x1000;
}

void RhBase_SetVifPacketArea(DotrDrawWord cursor)
{
    area_set = cursor;
}

DotrDrawWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrDrawWord cursor, int a1)
{
    start_calls++;
    start_arg = cursor + a1;
    return cursor + 0x100;
}

void SzDuel_SetWindowDefPrioZ(int value)
{
    prio_value = value;
}

int *SzDuel_GetWindowTexture(int index)
{
    texture_arg = index;
    return window_texture;
}

DotrDrawWord SzDuel_FinishWindowSession_Pipe(DotrDrawWord a)
{
    finish_arg = (int)a;
    return a + 0x10000;
}

DotrDrawWord ExSortSoftFocus_Pipe(DotrDrawWord cursor, int a1, int a2, int a3, int a4, int a5, unsigned char *a6)
{
    focus_calls++;
    focus_cursor = cursor;
    focus_args[0] = a1;
    focus_args[1] = a2;
    focus_args[2] = a3;
    focus_args[3] = a4;
    focus_args[4] = a5;
    focus_args[5] = a6 != 0;
    return cursor + 0x100000;
}

/* Stub: the string length (a library function outside the decompilation's scope; assumed from this
 * caller). */
int func_0019bef0(const char *string)
{
    strlen_calls++;
    return (int)strlen(string);
}

static void reset(void)
{
    memset(prims, 0, sizeof(prims));
    prim_count = texture_count = texture_flag = sort_args = 0;
    alpha_index = -1;
    alpha_calls = 0;
    alpha_value = 0;
    mode_arg = start_calls = prio_value = texture_arg = finish_arg = focus_calls = strlen_calls = 0;
    area_set = 0;
}

static void glyph(int font, int index, int texture, int u0, int v0, int u1, int v1)
{
    struct DotrFontGlyph *g = &data_00290b70[font * 37 + index];
    g->texture = texture;
    g->u0 = u0;
    g->v0 = v0;
    g->u1 = u1;
    g->v1 = v1;
}

static void expect_quad(int k, int x0, int x1, int x2, int x3, int y0, int y1)
{
    DotrFontPacket *p = &prims[k];
    assert((int)p->vertex[0].xyz.x == x0 && (int)p->vertex[0].xyz.y == y0);
    assert((int)p->vertex[1].xyz.x == x1 && (int)p->vertex[1].xyz.y == y0);
    assert((int)p->vertex[2].xyz.x == x2 && (int)p->vertex[2].xyz.y == y1);
    assert((int)p->vertex[3].xyz.x == x3 && (int)p->vertex[3].xyz.y == y1);
}

static void expect_texels(int k, int u0, int v0, int u1, int v1)
{
    DotrFontPacket *p = &prims[k];
    struct DotrCursorSlotUv *uv[4];
    int i;
    for (i = 0; i < 4; i++) {
        uv[i] = (struct DotrCursorSlotUv *)&p->vertex[i].uv;
        assert(p->vertex[i].xyz.z == 0x123456 && p->vertex[i].xyz.f == 0);
    }
    assert(uv[0]->u == u0 * 16 + 8 && uv[0]->v == v0 * 16 + 8);
    assert(uv[1]->u == u1 * 16 + 8 && uv[1]->v == v0 * 16 + 8);
    assert(uv[2]->u == u0 * 16 + 8 && uv[2]->v == v1 * 16 + 8);
    assert(uv[3]->u == u1 * 16 + 8 && uv[3]->v == v1 * 16 + 8);
}

static void check_font_pipe(void)
{
    static unsigned char colour[4] = { 9, 8, 7, 6 };
    struct DotrCursorPacketHeader *header;

    glyph(0, 0, 3, 0, 0, 8, 16);
    glyph(0, 1, 5, 8, 0, 20, 16);
    glyph(0, 2, 3, 20, 0, 36, 16);
    glyph(1, 0, 5, 2, 4, 8, 14);
    glyph(1, 1, 3, 0, 0, 10, 12);
    data_00317490.z = 0x123456;
    data_00317490.textures[3] = 0x3000;
    data_00317490.textures[5] = 0x5000;

    /* No characters: the packet is prepared (alpha mode 0) and the cursor returned unchanged. */
    reset();
    assert(SzDuel_DrawWindow_Font_Pipe(0x100, 10, 20, 0, 0, 0, "A", 0) == 0x100);
    assert(prim_count == 0 && texture_count == 0 && alpha_calls == 1 && alpha_index == 0 && alpha_value == 0x1000);

    /* Fixed pitch, default colour: glyph, line feed, glyph, blank, glyph. Each glyph is
     * centred in the 16-wide cell and the pen advances by the cell width. */
    reset();
    assert(SzDuel_DrawWindow_Font_Pipe(0x100, 10, 20, 0, 0, 5, "A\nB C", 0) == 0x130);
    assert(prim_count == 3 && texture_count == 3 && texture_flag == 0 && sort_args == 0);
    assert(alpha_calls == 1 && alpha_index == 0 && alpha_value == 0x1000);
    header = (struct DotrCursorPacketHeader *)prims[0].header;
    assert(header->unknown_00 == 9 && header->unknown_02 == (36 | 0x800) && header->unknown_04 == 0);
    assert(header->unknown_08 == 0x424242421);
    assert(prims[0].colour[0] == 3 && prims[0].colour[1] == 2 && prims[0].colour[2] == 1 && prims[0].colour[3] == 4);
    assert(prims[0].unknown_14 == 1.0f);
    assert(prim_texture[0] == 0x3000 && prim_texture[1] == 0x5000 && prim_texture[2] == 0x3000);
    expect_quad(0, 1738 * 16 + 64, 1746 * 16 + 64, 1738 * 16 + 64, 1746 * 16 + 64, 1844 * 16, 1860 * 16);
    expect_texels(0, 0, 0, 8, 16);
    expect_quad(1, 1738 * 16 + 32, 1750 * 16 + 32, 1738 * 16 + 32, 1750 * 16 + 32, 1864 * 16, 1880 * 16);
    expect_texels(1, 8, 0, 20, 16);
    expect_quad(2, 1770 * 16, 1786 * 16, 1770 * 16, 1786 * 16, 1864 * 16, 1880 * 16);
    expect_texels(2, 20, 0, 36, 16);

    /* Proportional, centred, slanted, alpha mode 2, caller's colour. Line widths are 20 and
     * 8 + 8 (half cell for the blank) + 12 = 28: half of the wider one comes off x, the top
     * edge moves right by (16 * 16) / 6 = 42 and the pen advances by the glyph width. */
    reset();
    assert(SzDuel_DrawWindow_Font_Pipe(0x200, 100, 0, 0, 1 | 2 | 4 | 8, 6, "AB\nA B", colour) == 0x240);
    assert(prim_count == 4 && alpha_index == 2 && alpha_value == 0x1002);
    assert(prims[0].colour[0] == 7 && prims[0].colour[1] == 8 && prims[0].colour[2] == 9 && prims[0].colour[3] == 6);
    expect_quad(0, 1814 * 16 + 42, 1822 * 16 + 42, 1814 * 16, 1822 * 16, 1824 * 16, 1840 * 16);
    expect_quad(1, 1822 * 16 + 42, 1834 * 16 + 42, 1822 * 16, 1834 * 16, 1824 * 16, 1840 * 16);
    expect_quad(2, 1814 * 16 + 42, 1822 * 16 + 42, 1814 * 16, 1822 * 16, 1844 * 16, 1860 * 16);
    expect_quad(3, 1830 * 16 + 42, 1842 * 16 + 42, 1830 * 16, 1842 * 16, 1844 * 16, 1860 * 16);

    /* Centred, fixed pitch, alpha mode 3, second font (cell 10 x 12, glyphs 37 entries on):
     * the first line (20) stays the widest at the second line feed and at the end. */
    reset();
    assert(SzDuel_DrawWindow_Font_Pipe(0x300, 0, 0, 1, 2 | 16, 6, "AB\nA\n ", 0) == 0x330);
    assert(prim_count == 3 && alpha_index == 3 && alpha_value == 0x1003);
    assert(prim_texture[0] == 0x5000 && prim_texture[1] == 0x3000);
    expect_quad(0, 1718 * 16 + 32, 1724 * 16 + 32, 1718 * 16 + 32, 1724 * 16 + 32, 1824 * 16, 1834 * 16);
    expect_texels(0, 2, 4, 8, 14);
    expect_quad(1, 1728 * 16, 1738 * 16, 1728 * 16, 1738 * 16, 1824 * 16, 1836 * 16);
    expect_quad(2, 1718 * 16 + 32, 1724 * 16 + 32, 1718 * 16 + 32, 1724 * 16 + 32, 1836 * 16, 1846 * 16);

    /* Centred, the later line is the widest (measured at the end of the text). */
    reset();
    assert(SzDuel_DrawWindow_Font_Pipe(0x400, 0, 0, 0, 2, 4, "A\nAA", 0) == 0x430);
    expect_quad(0, 1712 * 16 + 64, 1720 * 16 + 64, 1712 * 16 + 64, 1720 * 16 + 64, 1824 * 16, 1840 * 16);
    expect_quad(2, 1728 * 16 + 64, 1736 * 16 + 64, 1728 * 16 + 64, 1736 * 16 + 64, 1844 * 16, 1860 * 16);
}

static void expect_cell(int k, int x, int y)
{
    struct DotrModeLastPacket *p = (struct DotrModeLastPacket *)&prims[k];
    assert((int)p->vertex[0].xyz.x == (x + 1728) * 16 && (int)p->vertex[0].xyz.y == (y + 1824) * 16);
    assert((int)p->vertex[1].xyz.x == (x + 1746) * 16 && (int)p->vertex[1].xyz.y == (y + 1848) * 16);
    assert(p->vertex[0].xyz.z == 0xffffff && p->vertex[0].xyz.f == 0);
    assert(p->vertex[1].xyz.z == 0xffffff && p->vertex[1].xyz.f == 0);
}

static void expect_tile(int k, int x, int y, int alpha)
{
    struct DotrModeLastPacket *p = (struct DotrModeLastPacket *)&prims[k];
    struct DotrCursorSlotUv *a = (struct DotrCursorSlotUv *)&p->vertex[0].uv;
    struct DotrCursorSlotUv *b = (struct DotrCursorSlotUv *)&p->vertex[1].uv;
    assert(a->u == x * 16 + 8 && a->v == y * 16 + 8 && b->u == (x + 18) * 16 + 8 && b->v == (y + 24) * 16 + 8);
    assert(p->colour[0] == 128 && p->colour[1] == 128 && p->colour[2] == 128 && p->colour[3] == alpha);
    assert(p->unknown_14 == 1.0f);
}

static void check_mode_draw_last(void)
{
    struct DotrModeLastView *view = &gSzGame_gl_last;
    struct DotrCursorPacketHeader *header;
    static int texture[2];

    glyph(2, 0, 3, 0, 0, 6, 8);
    glyph(2, 1, 3, 6, 0, 12, 8);

    /* Mode below 3 and no fade: only the mode query. */
    reset();
    memset(view, 0, sizeof(*view));
    view->enabled = -1;
    view->mode = 2;
    SzGame_ModeDrawLast();
    assert(mode_arg == 1 && start_calls == 0 && focus_calls == 0 && view->counter == 0);

    /* Mode 3 but not enabled: nothing either. */
    view->mode = 3;
    view->enabled = 0;
    SzGame_ModeDrawLast();
    assert(start_calls == 0 && focus_calls == 0);

    /* Enabled, no window texture: the session is opened, closed and handed on; no drawing
     * and no state change. */
    view->enabled = -1;
    view->counter = 7;
    window_texture = 0;
    SzGame_ModeDrawLast();
    assert(start_calls == 1 && start_arg == 0x1000 && prio_value == 0xffffff && texture_arg == 6);
    assert(prim_count == 0 && strlen_calls == 0 && view->counter == 7);
    assert(finish_arg == 0x1100 && focus_calls == 1 && focus_cursor == 0x11100);
    assert(focus_args[0] == 0 && focus_args[1] == 0 && focus_args[2] == 0 && focus_args[3] == 0);
    assert(focus_args[4] == 12 && focus_args[5] == 0 && area_set == 0x111100);

    /* Counter 2: tile 0 at (200, 136), two cells, tile alpha = the counter, no second run
     * (n is not below the counter). Text alpha (10 + 64) * 2 >> 7 = 1; the alpha byte
     * advances by step * 4 and stays in range, so the step is kept. */
    reset();
    window_texture = texture;
    view->counter = 2;
    view->alpha = 10;
    view->step = 1;
    SzGame_ModeDrawLast();
    assert(texture_count == 3 && prim_texture[0] == (intptr_t)texture && alpha_calls == 2);
    assert(prim_count == 4 && strlen_calls == 1);
    header = (struct DotrCursorPacketHeader *)prims[0].header;
    assert(header->unknown_00 == 5 && header->unknown_02 == (38 | 0x800) && header->unknown_04 == 0);
    assert(header->unknown_08 == 0x42421);
    expect_tile(0, 200, 136, 2);
    expect_cell(0, 5, 4);
    expect_cell(1, 23, 4);
    /* The two glyphs: centred proportional text of width 12 at (320, 212), font 2. */
    assert(((struct DotrCursorPacketHeader *)prims[2].header)->unknown_00 == 9);
    assert(prims[2].colour[0] == 30 && prims[2].colour[1] == 20 && prims[2].colour[2] == 10 && prims[2].colour[3] == 1);
    expect_quad(2, 2042 * 16, 2048 * 16, 2042 * 16, 2048 * 16, 2036 * 16, 2044 * 16);
    expect_quad(3, 2048 * 16, 2054 * 16, 2048 * 16, 2054 * 16, 2036 * 16, 2044 * 16);
    assert(view->alpha == 14 && view->step == 1 && view->busy == 0 && view->counter == 3);
    assert(finish_arg == 0x1100 + 4 * 16 && focus_calls == 1);

    /* Counter 3 * 560 + 1 with the fade byte set in mode 0: tile 3 at (80, 128) for one
     * cell, then the previous tile 2 at (236, 136) for the other 559; rows from the ninth
     * sit 48 lower. Tile alpha is the fade byte, which drops by 4. The text alpha term is
     * capped (126 + 64 -> 128), and the step is negated when the alpha byte passes 127. */
    reset();
    view->mode = 0;
    view->busy = 8;
    view->counter = 3 * 560 + 1;
    view->alpha = 126;
    view->step = 1;
    SzGame_ModeDrawLast();
    assert(prim_count == 562);
    expect_tile(0, 80, 128, 8);
    expect_cell(0, 5, 4);
    expect_tile(1, 236, 136, 8);
    expect_cell(1, 23, 4);
    expect_cell(279, 34 * 18 + 5, 7 * 24 + 4);
    expect_cell(280, 5, 8 * 24 + 4 + 48);
    expect_tile(559, 236, 136, 8);
    expect_cell(559, 34 * 18 + 5, 15 * 24 + 4 + 48);
    assert(prims[560].colour[3] == 8);
    assert(view->alpha == 130 && view->step == -1 && view->busy == 4 && view->counter == 3 * 560 + 2);

    /* Counter 200 without fade: tile alpha capped at 128; the alpha byte reaching 0 turns
     * the step round as well. */
    reset();
    view->busy = 0;
    view->mode = 3;
    view->counter = 200;
    view->alpha = 4;
    view->step = -1;
    SzGame_ModeDrawLast();
    assert(prim_count == 202);
    expect_tile(0, 200, 136, 128);
    assert(prims[200].colour[3] == 68);
    assert(view->alpha == 0 && view->step == 1 && view->busy == 0 && view->counter == 201);

    /* Tiles 1 and 4: second column of the left block, second of the right block. */
    reset();
    view->counter = 560 + 1;
    SzGame_ModeDrawLast();
    expect_tile(0, 218, 136, 128);
    expect_tile(1, 200, 136, 128);
    reset();
    view->counter = 4 * 560 + 1;
    SzGame_ModeDrawLast();
    expect_tile(0, 98, 128, 128);
    expect_tile(1, 80, 128, 128);
}

/* ---- SzDuel_DrawWindow_Pipe (0x1b11c0) ---- */
struct DotrWindowControl data_00317490_window;
#define W data_00317490_window
struct wlog { int id; long a[10]; };
static struct wlog wl[48];
static int wn, wi;
static long wcur;
static DotrDrawWord wret;
static struct DotrWindowUnit ua, ub, uh, um;
static struct DotrWindowUnit *w_hand, *w_map[10][10];
static int w_pair[2], w_adv;
static float w_float;

static long wrec(int id, long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9)
{
    long v[10] = { a0, a1, a2, a3, a4, a5, a6, a7, a8, a9 };
    assert(wn < 48);
    wl[wn].id = id;
    memcpy(wl[wn].a, v, sizeof(v));
    wn++;
    return a0 + 1;
}
int SzDuel_GetField(int x, int y) { wrec('G', x, y, 0, 0, 0, 0, 0, 0, 0, 0); return x * 10 + y; }
DotrDrawWord lSzDuel_DrawWindow_FieldPiece_Pipe(DotrDrawWord cursor, int colour, int field, float f, int x, int y)
{
    w_float = f;
    return wrec('P', cursor, colour, field, x, y, 0, 0, 0, 0, 0);
}
DotrDrawWord lSzDuel_DrawWindow_PlayerInfo_Pipe(DotrDrawWord cursor, int side, int x, int y, int active)
{
    return wrec('I', cursor, side, x, y, active, 0, 0, 0, 0, 0);
}
DotrDrawWord lSzDuel_DrawWindow_CardInfo_Pipe(DotrDrawWord cursor, struct DotrWindowUnit *unit, int x, int y,
                                              unsigned char side, int a5, int a6, int a7, long a8, long a9)
{
    return wrec('C', cursor, (long)(intptr_t)unit, x, y, side, a5, a6, a7, a8, a9);
}
struct DotrWindowUnit *SzDuel_GetUnit(int a, int b) { wrec('U', a, b, 0, 0, 0, 0, 0, 0, 0, 0); return w_hand; }
struct DotrWindowUnit *SzDuel_GetMapUnit(int x, int y) { wrec('M', x, y, 0, 0, 0, 0, 0, 0, 0, 0); return w_map[x][y]; }
int func_00250360_pair(int a, int b) { wrec('K', a, b, 0, 0, 0, 0, 0, 0, 0, 0); return w_pair[a]; }
int Ta_IsVisualAttrAdv(int a, int b, int c, int d) { wrec('A', a, b, c, d, 0, 0, 0, 0, 0, 0); return w_adv; }
DotrDrawWord lSzDuel_DrawWindow_CommandGuide_Pipe(DotrDrawWord cursor, int a1, int a2, int a3)
{
    return wrec('D', cursor, a1, a2, a3, 0, 0, 0, 0, 0, 0);
}
DotrDrawWord lSzDuel_DrawWindow_Field_Pipe(DotrDrawWord cursor, int id, int x, int y)
{
    return wrec('F', cursor, id, x, y, 0, 0, 0, 0, 0, 0);
}
DotrDrawWord lSzDuel_DrawWindow_Frame_Pipe(DotrDrawWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8)
{
    return wrec('R', cursor, a1, a2, a3, x, y, a6, a7, a8, 0);
}
DotrDrawWord SzDuel_DrawWindow_FontDigit_Pipe(DotrDrawWord cursor, int x, int y, int a, int flags, int width, int value)
{
    return wrec('N', cursor, x, y, a, flags, width, value, 0, 0, 0);
}
DotrDrawWord lSzDuel_DrawWindow_DualShock2_Pipe(DotrDrawWord cursor, int x, int y, int alpha)
{
    return wrec('S', cursor, x, y, alpha, 0, 0, 0, 0, 0, 0);
}
DotrDrawWord lSzDuel_DrawWindow_Level_Pipe(DotrDrawWord cursor, int x, int y) { return wrec('L', cursor, x, y, 0, 0, 0, 0, 0, 0, 0); }
DotrDrawWord lSzDuel_DrawWindow_SummonCost_Pipe(DotrDrawWord cursor, int x, int y) { return wrec('O', cursor, x, y, 0, 0, 0, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawHandCursor_Pipe(DotrDrawWord cursor) { return wrec('H', cursor, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
void SzMsg_Free(int index) { wrec('X', index, 0, 0, 0, 0, 0, 0, 0, 0, 0); }

/* Closing (bit 0 of +0xd67 set) from slide 20: the offset used by the draws is 12, or 4 when a
 * mode byte forces step 16. */
static void wreset(void)
{
    memset(&W, 0, sizeof(W));
    memset(w_map, 0, sizeof(w_map));
    memset(&ua, 0, sizeof(ua));
    memset(&ub, 0, sizeof(ub));
    memset(&uh, 0, sizeof(uh));
    memset(&um, 0, sizeof(um));
    ub.side = uh.side = um.side = 1;
    w_hand = 0;
    w_pair[0] = w_pair[1] = w_adv = 0;
    W.sides[0].x = 2;
    W.sides[0].y = 3;
    W.sides[1].x = 4;
    W.sides[1].y = 5;
    W.unknown_d67_bit_0 = -1;
    W.slide = 20;
}
static void wrun(void)
{
    wn = wi = 0;
    wcur = 0x1000;
    w_float = 0.0f;
    start_calls = prio_value = 0;
    wret = SzDuel_DrawWindow_Pipe(0x1000);
}
/* The next logged call: a cursor call (first word is the running cursor) when `c` is set. */
static void wex(int id, int c, long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9)
{
    long v[10] = { c ? wcur : a1, c ? a1 : a2, c ? a2 : a3, c ? a3 : a4, c ? a4 : a5, c ? a5 : a6, c ? a6 : a7, c ? a7 : a8, c ? a8 : a9, c ? a9 : 0 };
    assert(wi < wn && wl[wi].id == id);
    assert(memcmp(wl[wi].a, v, sizeof(v)) == 0);
    wi++;
    if (c) {
        wcur++;
    }
}
static void wex_head(int side, int off)
{
    int x = W.sides[side].x, y = W.sides[side].y;
    wex('G', 0, x, y, 0, 0, 0, 0, 0, 0, 0);
    wex('P', 1, side, x * 10 + y, side ? 6 : 538, off + 336, 0, 0, 0, 0, 0);
    assert(w_float == 100.0f && start_calls == 1 && start_arg == wcur);
    wcur += 0x100;
    wex('I', 1, 0, 18, 20 - off, side == 0, 0, 0, 0, 0, 0);
    wex('I', 1, 1, 440, 20 - off, side == 1, 0, 0, 0, 0, 0);
}
static void wex_tail(int side, int off, int pad, int level)
{
    int x = W.sides[side].x, y = W.sides[side].y;
    wex('D', 1, side, side ? 538 : 6, off + 336, 0, 0, 0, 0, 0, 0);
    wex('G', 0, x, y, 0, 0, 0, 0, 0, 0, 0);
    wex('F', 1, x * 10 + y, side ? 6 : 538, off + 336, 0, 0, 0, 0, 0, 0);
    wex('R', 1, 5, 3, 4, 292, 16 - off, 4, 2, 1, 0);
    wex('N', 1, 320, 22 - off, 2, 34, 2, 100 - W.unknown_d3d, 0, 0, 0);
    if (pad) {
        wex('S', 1, 320, 72 - off, pad, 0, 0, 0, 0, 0, 0);
    }
    if (level) {
        wex('L', 1, 220, 54 - off + W.unknown_d45 - 128, 0, 0, 0, 0, 0, 0, 0);
        wex('O', 1, 332, 54 - off + W.unknown_d45 - 128, 0, 0, 0, 0, 0, 0, 0);
    }
    wex('H', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    wex('X', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    assert(wi == wn && wret == wcur && prio_value == 0xfffffc);
}
static int wslide(int from)
{
    W.slide = from;
    wrun();
    return W.slide;
}
static int wslide2(int from)
{
    W.slide = 20;
    W.slide2 = from;
    wrun();
    return W.slide2;
}
static int wcount(int id)
{
    int i, n = 0;
    for (i = 0; i < wn; i++) {
        n += wl[i].id == id;
    }
    return n;
}
/* The unit pointer and the a7 word of the last card panel. */
static struct wlog *wlast_card(void)
{
    int i;
    for (i = wn - 1; i >= 0; i--) {
        if (wl[i].id == 'C') {
            return &wl[i];
        }
    }
    assert(0);
    return 0;
}
static struct DotrWindowUnit *wother(int key, int mode)
{
    wreset();
    W.unknown_d45 = 1;
    W.unit_6c8 = &ua;
    W.unit_6cc = &ub;
    W.unknown_d57 = key;
    W.unknown_d35 = mode;
    wrun();
    return (struct DotrWindowUnit *)(intptr_t)wlast_card()->a[1];
}

static void check_draw_window_slide(void)
{
    /* Bit 1 of +0xd67: nothing happens. */
    wreset();
    W.unknown_d67_bit_1 = -1;
    wrun();
    assert(wret == 0x1000 && wn == 0 && W.slide == 20 && start_calls == 0);

    /* Opening by 8 with the clamp at 128; at 128 the pending side and cell are latched and nothing
     * is drawn. */
    wreset();
    W.unknown_d67_bit_0 = 0;
    assert(wslide(100) == 108 && wn != 0);
    assert(wslide(119) == 127 && wn != 0);
    W.unknown_d54 = 1;
    W.unknown_d53 = 7;
    W.unknown_d66_bit_7 = -1;
    assert(wslide(120) == 128 && wn == 0 && wret == 0x1000);
    assert(W.side == 1 && W.unknown_d3d == 7 && W.unknown_d67_bit_0 != 0 && W.x == 4 && W.y == 5);
    wreset();
    W.unknown_d67_bit_0 = 0;
    assert(wslide(124) == 128 && wn == 0 && W.side == 0 && W.unknown_d67_bit_0 == 0 && W.x == 2 && W.y == 3);

    /* Closing by 8 with the clamp at 0; from 128 the window is drawn again. */
    wreset();
    assert(wslide(9) == 1 && wslide(8) == 0 && wslide(5) == 0);
    assert(wslide(128) == 120 && wn != 0);

    /* Bit 2 of +0xd67 or any of the three mode bytes forces closing by 16 unless bit 4 of +0xd65 is set. */
    wreset();
    W.unknown_d67_bit_0 = 0;
    W.unknown_d67_bit_2 = -1;
    assert(wslide(40) == 24 && wslide(16) == 0 && wslide(17) == 1);
    wreset();
    W.unknown_d67_bit_0 = 0;
    W.unknown_d45 = 1;
    assert(wslide(40) == 24);
    W.unknown_d65_bit_4 = -1;
    assert(wslide(40) == 48);
    wreset();
    W.unknown_d67_bit_0 = 0;
    W.unknown_d47 = 1;
    assert(wslide(40) == 24);
    wreset();
    W.unknown_d67_bit_0 = 0;
    W.unknown_d48 = 1;
    assert(wslide(40) == 24);
}

static void check_draw_window_hand(void)
{
    /* Hand mode, side 0: the hand card panel slides out (second slide 40 -> 24) and the second
     * panel shows unit +0x6cc. */
    wreset();
    W.unknown_d47 = 1;
    W.unknown_d4f = 2;
    W.slide2 = 40;
    W.unit_6c8 = &ua;
    W.unit_6cc = &ub;
    w_hand = &uh;
    wrun();
    assert(W.slide == 4 && W.slide2 == 24);
    wex_head(0, 4);
    wex('U', 0, 2, 3, 0, 0, 0, 0, 0, 0, 0);
    wex('C', 1, (long)(intptr_t)&uh, 2, 3, 1, 104, 368 - 12 + 4, -1, 6, 1);
    wex('C', 1, (long)(intptr_t)&ub, 2, 3, 1, 104, 372, -1, 3, 0);
    wex_tail(0, 4, 0, 0);

    /* Side 1, no hand unit and no unit: one panel with the side byte, flagged as first; pad icon,
     * level and cost rows. */
    wreset();
    W.side = 1;
    W.unknown_d45 = 1;
    W.unknown_d48 = 1;
    W.unknown_d44 = 200;
    W.unknown_d3d = 30;
    W.unknown_d67_bit_3 = -1;
    W.slide2 = 16;
    wrun();
    assert(W.slide == 4 && W.slide2 == 0);
    wex_head(1, 4);
    wex('U', 0, 2, 1, 0, 0, 0, 0, 0, 0, 0);
    wex('C', 1, 0, 4, 5, 1, 104, 372, -1, 3, 1);
    wex_tail(1, 4, 200, 1);

    /* Second slide: bit 3 closes it (clamp at 0); otherwise +0xd47 closes it unless bit 4 is set;
     * else it opens (clamp at 128). */
    wreset();
    W.unknown_d45 = 1;
    W.unknown_d67_bit_3 = -1;
    assert(wslide2(17) == 1 && wslide2(16) == 0 && wslide2(10) == 0);
    W.unknown_d67_bit_3 = 0;
    assert(wslide2(40) == 56 && wslide2(111) == 127 && wslide2(112) == 128 && wslide2(120) == 128);
    W.unknown_d47 = 1;
    assert(wslide2(40) == 24);
    W.unknown_d67_bit_4 = -1;
    assert(wslide2(40) == 56);
    /* An open hand panel without a hand unit takes the side byte. */
    wex_head(0, 4);
    wex('U', 0, 2, 1, 0, 0, 0, 0, 0, 0, 0);
    wex('C', 1, 0, 2, 3, 0, 104, 368 - 28 + 4, -1, 6, 1);
    wex('C', 1, 0, 2, 3, 0, 104, 372, -1, 3, 0);
    wex_tail(0, 4, 0, 0);

    /* Keys 81..83, or 32..34 while byte +0xd35 is 83, show unit +0x6c8 instead. */
    assert(wother(81, 0) == &ua && wother(82, 0) == &ua && wother(83, 0) == &ua);
    assert(wother(80, 0) == &ub && wother(84, 0) == &ub);
    assert(wother(32, 83) == &ua && wother(33, 83) == &ua && wother(34, 83) == &ua);
    assert(wother(31, 83) == &ub && wother(35, 83) == &ub && wother(32, 82) == &ub);
}

static void wboard(void)
{
    wreset();
    ua.place = 10;
    ua.x = 1;
    ua.y = 1;
    ua.unknown_57_bit_6 = -1;
    ub.place = 3;
    w_pair[1] = 1;
    W.unit_6c8 = &ua;
    W.unit_6cc = &ub;
    w_map[2][3] = &um;
}
static void check_draw_window_board(void)
{
    /* No unit: no comparison, the second slide closes, one panel for the cell at +0xd40/+0xd41. */
    wreset();
    W.x = 6;
    W.y = 7;
    w_map[6][7] = &um;
    wrun();
    wex_head(0, 12);
    wex('M', 0, 6, 7, 0, 0, 0, 0, 0, 0, 0);
    wex('C', 1, (long)(intptr_t)&um, 2, 3, 1, 104, 380, 1, 3, 1);
    wex_tail(0, 12, 0, 0);

    /* Two units of different control: the advantage word, the cursor cell latched and its panel opening. */
    wboard();
    wrun();
    assert(W.x == 1 && W.y == 1 && W.x2 == 2 && W.y2 == 3 && W.slide2 == 16);
    wex_head(0, 12);
    wex('K', 0, 0, 10, 0, 0, 0, 0, 0, 0, 0);
    wex('K', 0, 1, 3, 0, 0, 0, 0, 0, 0, 0);
    wex('A', 0, 0, 10, 1, 3, 0, 0, 0, 0, 0);
    wex('M', 0, 2, 3, 0, 0, 0, 0, 0, 0, 0);
    wex('M', 0, 2, 3, 0, 0, 0, 0, 0, 0, 0);
    wex('C', 1, (long)(intptr_t)&um, 2, 3, 1, 104, 368 - 8 + 12, 2, 3, 1);
    wex('M', 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);
    wex('C', 1, 0, 2, 3, 0, 104, 380, 0, 3, 0);
    wex_tail(0, 12, 0, 0);

    /* Each condition of the comparison; the advantage word below zero is used as is, otherwise 2 - word. */
    wboard();
    ua.place = 11;
    wrun();
    assert(wcount('K') == 0 && wcount('A') == 0);
    wboard();
    W.unit_6cc = 0;
    wrun();
    assert(wcount('K') == 0);
    wboard();
    ub.place = 11;
    wrun();
    assert(wcount('K') == 0);
    wboard();
    ub.place = 10;
    w_pair[1] = 0;
    wrun();
    assert(wcount('K') == 2 && wcount('A') == 0 && wlast_card()->a[7] == 1);
    wboard();
    W.slide2 = 32;
    W.x2 = 2;
    W.y2 = 3;
    w_adv = -1;
    wrun();
    assert(wlast_card()->a[7] == -1 && wl[wn - 10].id == 'C' && wl[wn - 10].a[7] == -1 && W.slide2 == 48);
    w_adv = 1;
    wrun();
    assert(wlast_card()->a[7] == 1 && wl[wn - 10].a[7] == 1);

    /* The cell is latched only while the second slide is 0; a mismatch, or no unit there, closes it. */
    wboard();
    W.slide2 = 32;
    W.x2 = 9;
    W.y2 = 9;
    wrun();
    assert(W.x2 == 9 && W.y2 == 9 && W.slide2 == 16 && wcount('M') == 2);
    wboard();
    W.slide2 = 32;
    W.x2 = 2;
    W.y2 = 9;
    wrun();
    assert(W.slide2 == 16 && wcount('M') == 2);
    wboard();
    w_map[2][3] = 0;
    W.slide2 = 16;
    W.x2 = 2;
    W.y2 = 3;
    wrun();
    assert(W.slide2 == 0 && wcount('M') == 2 && wcount('C') == 1 && wlast_card()->a[9] == 1);
    /* Unit on the cursor cell: nothing latched; a difference in y alone latches. */
    wboard();
    ua.x = 2;
    ua.y = 3;
    wrun();
    assert(W.x == 2 && W.y == 3 && W.x2 == 0 && W.y2 == 0 && W.slide2 == 0);
    wboard();
    ua.x = 2;
    wrun();
    assert(W.x == 2 && W.y == 1 && W.x2 == 2 && W.y2 == 3 && W.slide2 == 16);

    /* Without the unit flag (or with place 11, or no unit) keys 24, 62 and 48 move the cell to the
     * cursor cell. */
    wboard();
    ua.unknown_57_bit_6 = 0;
    W.unknown_d57 = 24;
    wrun();
    assert(W.x == 2 && W.y == 3 && W.x2 == 0);
    W.x = W.y = 0;
    W.unknown_d57 = 62;
    wrun();
    assert(W.x == 2 && W.y == 3);
    W.x = W.y = 0;
    W.unknown_d57 = 48;
    wrun();
    assert(W.x == 2 && W.y == 3);
    W.x = W.y = 0;
    W.unknown_d57 = 25;
    wrun();
    assert(W.x == 0 && W.y == 0);
    wboard();
    ua.place = 11;
    W.unknown_d57 = 24;
    wrun();
    assert(W.x == 2 && W.y == 3);
    wboard();
    W.unit_6c8 = 0;
    W.unknown_d57 = 24;
    wrun();
    assert(W.x == 2 && W.y == 3);
}

int main(void)
{
    check_font_pipe();
    check_mode_draw_last();
    check_draw_window_slide();
    check_draw_window_hand();
    check_draw_window_board();
    return 0;
}
