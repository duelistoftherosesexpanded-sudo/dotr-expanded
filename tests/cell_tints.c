/* Synthetic host tests for the cell_tints group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cell_tints.h"

static struct { int a, b, c, e; unsigned char colour[4]; } calls[64];
static int call_n, floor_calls;

void func_001e3060_tint(int a, int b, int c, unsigned char *colour, int e)
{
    assert(call_n < 64);
    calls[call_n].a = a;
    calls[call_n].b = b;
    calls[call_n].c = c;
    calls[call_n].e = e;
    memcpy(calls[call_n].colour, colour, 4);
    call_n++;
}

/* Assumed semantics for a limit of 0: the value, or 0 when it is negative. */
int GetCap(int limit, int value)
{
    assert(limit == 0);
    floor_calls++;
    return value < 0 ? 0 : value;
}

static void run(int x, int y, int reach, int mode, int kind)
{
    call_n = floor_calls = 0;
    Ta_SetRangeVisual(x, y, 33, reach, 44, mode, kind);
}

/* every call carries c, e and the same four bytes */
static void same_bytes(int b0, int b1, int b2, int b3)
{
    int i;

    for (i = 0; i < call_n; i++) {
        assert(calls[i].c == 33 && calls[i].e == 44);
        assert(calls[i].colour[0] == b0 && calls[i].colour[1] == b1 && calls[i].colour[2] == b2 && calls[i].colour[3] == b3);
    }
}

/* the calls cover x0..x1 by y0..y1 in row order */
static void block(int x0, int x1, int y0, int y1)
{
    int i, j, n = 0;

    assert(floor_calls == 2 && call_n == (x1 - x0 + 1) * (y1 - y0 + 1));
    for (i = x0; i <= x1; i++) {
        for (j = y0; j <= y1; j++) {
            assert(calls[n].a == i && calls[n].b == j);
            n++;
        }
    }
}

/* Ta_SetRangeVisual (branch / loop coverage: kind <= 0, 11, 13, 15, 16 and others;
 * mode 18 and not; reach 1 and not; both clamps at and beyond each edge). */
static void check_tint(void)
{
    int i;

    /* kinds, one cell at reach 0 -> d = 1 in the middle: 3 x 3 */
    run(3, 3, 0, 0, 0);
    block(2, 4, 2, 4);
    same_bytes(0, 0, 0, 255);
    run(3, 3, 0, 0, -5);
    same_bytes(0, 0, 0, 255);
    run(3, 3, 0, 0, 11);
    same_bytes(255, 255, 0, 128);
    run(3, 3, 0, 0, 13);
    same_bytes(255, 128, 0, 128);
    run(3, 3, 0, 0, 15);
    same_bytes(128, 0, 255, 128);
    run(3, 3, 0, 0, 16);
    same_bytes(0, 255, 0, 128);
    run(3, 3, 0, 0, 1);
    same_bytes(255, 255, 255, 128);
    run(3, 3, 0, 0, 12);
    same_bytes(255, 255, 255, 128);
    run(3, 3, 0, 0, 14);
    same_bytes(255, 255, 255, 128);
    run(3, 3, 0, 0, 17);
    block(2, 4, 2, 4);
    same_bytes(255, 255, 255, 128);

    /* mode 18: the row and the column, alternating, no 0x250860 call */
    run(2, 5, 1, 18, 11);
    assert(call_n == 14 && floor_calls == 0);
    for (i = 0; i < 7; i++) {
        assert(calls[2 * i].a == i && calls[2 * i].b == 5);
        assert(calls[2 * i + 1].a == 2 && calls[2 * i + 1].b == i);
    }
    same_bytes(255, 255, 0, 128);

    /* reach 1 -> d = 2; other reach values -> d = 1 */
    run(3, 3, 1, 0, 11);
    block(1, 5, 1, 5);
    run(3, 3, 2, 17, 11);
    block(2, 4, 2, 4);
    /* upper clamp: x + d of 6 is kept, 7 and 8 become 6 */
    run(4, 5, 1, 0, 11);
    block(2, 6, 3, 6);
    run(5, 6, 1, 0, 11);
    block(3, 6, 4, 6);
    run(5, 6, 0, 0, 11);
    block(4, 6, 5, 6);
    /* lower clamp through 0x250860 */
    run(0, 1, 1, 0, 11);
    block(0, 2, 0, 3);
    run(1, 0, 0, 0, 11);
    block(0, 2, 0, 1);
    same_bytes(255, 255, 0, 128);
}

struct DotrTintCell data_00318e60_tint[7][7];
unsigned short data_00318118_tint[8];

static void quad_is(const struct DotrTintQuad *q, int b0, int b1, int b2, int b3)
{
    assert(q->blue == b0 && q->green == b1 && q->red == b2 && q->alpha == b3);
}

/* One call on the cell (2, 5) filled with 0x77 bytes. */
static struct DotrTintCell *paint(struct DotrTintItem *item, struct DotrTintQuad *colour, int e, int f, int half)
{
    struct DotrTintCell *cell = &data_00318e60_tint[5][2];

    memset(data_00318e60_tint, 0x77, sizeof(data_00318e60_tint));
    cell->unit = item;
    data_00318118_tint[0] = (unsigned short)half;
    assert(SzDuel_SetMapColor2(2, 5, 0x21, colour, e, f) == 1);
    assert(cell->unit == item && cell->TargetSelectionHighlight == 0x21 && cell->unknown_14 == 0x77777777);
    assert(data_00318e60_tint[5][1].TargetSelectionHighlight == 0x77 && data_00318e60_tint[5][3].quads[0].blue == 0x77);
    assert(data_00318e60_tint[2][5].TargetSelectionHighlight == 0x77);
    return cell;
}

/* SzDuel_SetMapColor2 (branch coverage). */
static void check_paint(void)
{
    static const int outside[][2] = { { -1, 0 }, { 7, 0 }, { 0, -1 }, { 0, 7 } };
    struct DotrTintQuad colour = { 1, 2, 3, 4 };
    struct DotrTintItem item;
    struct DotrTintCell *cell;
    size_t i, j;

    assert(offsetof(struct DotrTintItem, team) == 0x44 && sizeof(struct DotrTintItem) == 0x58);
    assert(sizeof(struct DotrTintQuad) == 4);
    assert(offsetof(struct DotrTintCell, TargetSelectionHighlight) - offsetof(struct DotrTintCell, quads) == 0x14);
    /* each bound: nothing is written */
    memset(data_00318e60_tint, 0x77, sizeof(data_00318e60_tint));
    for (i = 0; i < 4; i++) {
        assert(SzDuel_SetMapColor2(outside[i][0], outside[i][1], 0x21, &colour, 1, 1) == 0);
    }
    for (i = 0; i < sizeof(data_00318e60_tint); i++) {
        assert(((unsigned char *)data_00318e60_tint)[i] == 0x77);
    }
    /* the corners are inside */
    assert(SzDuel_SetMapColor2(0, 0, 0x21, &colour, 0, 0) == 1 && data_00318e60_tint[0][0].TargetSelectionHighlight == 0x21);
    assert(SzDuel_SetMapColor2(6, 6, 0x21, &colour, 0, 0) == 1 && data_00318e60_tint[6][6].TargetSelectionHighlight == 0x21);

    memset(&item, 0, sizeof(item));
    /* colour given, f and e clear: quads 1 and 3 kept */
    cell = paint(&item, &colour, 0, 0, 0x4000);
    quad_is(&cell->quads[0], 1, 2, 3, 4);
    quad_is(&cell->quads[2], 0, 0, 0, 0);
    quad_is(&cell->quads[1], 0x77, 0x77, 0x77, 0x77);
    quad_is(&cell->quads[3], 0x77, 0x77, 0x77, 0x77);
    /* no colour: zeros */
    cell = paint(&item, 0, 0, 0, 0x4000);
    quad_is(&cell->quads[0], 0, 0, 0, 0);
    /* f set but no item: quad 2 stays zero; e copies both */
    cell = paint(0, &colour, 1, 1, 0x4000);
    quad_is(&cell->quads[2], 0, 0, 0, 0);
    quad_is(&cell->quads[1], 1, 2, 3, 4);
    quad_is(&cell->quads[3], 0, 0, 0, 0);
    /* item with byte +0x44 zero / not zero */
    cell = paint(&item, &colour, 0, 1, 0x4000);
    quad_is(&cell->quads[2], 0, 0, 255, 128);
    item.team = 9;
    cell = paint(&item, &colour, 1, 1, 0x4000);
    quad_is(&cell->quads[2], 255, 255, 255, 128);
    quad_is(&cell->quads[3], 255, 255, 255, 128);
    quad_is(&cell->quads[1], 1, 2, 3, 4);
    /* bit 0 alone does not select the blend */
    item.unknown_57_bit_0 = -1;
    cell = paint(&item, &colour, 0, 1, 0x4000);
    quad_is(&cell->quads[2], 255, 255, 255, 128);
    /* bit 1: blend from the high byte of the halfword (low byte ignored) */
    item.unknown_57_bit_0 = 0;
    item.unknown_57_bit_1 = -1;
    for (j = 0; j < 4; j++) {
        static const int high[4] = { 0, 0x40, 0x80, 0xc0 };
        int hi = high[j], lo = (128 - hi) & 0xff;

        cell = paint(&item, &colour, 0, 1, hi << 8 | 0x5a);
        quad_is(&cell->quads[2], (lo * 255 >> 7) & 0xff, (lo * 255 >> 7) & 0xff,
                ((hi * 255 >> 7) + (lo * 255 >> 7)) & 0xff, 128);
    }
    cell = paint(&item, &colour, 0, 1, 0x4000);
    quad_is(&cell->quads[2], 127, 127, 254, 128);
    /* f clear with the same item: no blend */
    cell = paint(&item, &colour, 0, 0, 0x4000);
    quad_is(&cell->quads[2], 0, 0, 0, 0);
}

static void check_fade(void)
{
    static const int outside[4][2] = { { -1, 0 }, { 7, 0 }, { 0, -1 }, { 0, 7 } };
    struct DotrTintCell *cell = &data_00318e60_tint[2][5];
    size_t i;

    assert(offsetof(struct DotrTintCell, SelectionHighlightType) - offsetof(struct DotrTintCell, TargetSelectionHighlight) == 1);
    /* each bound: nothing is written */
    memset(data_00318e60_tint, 0x77, sizeof(data_00318e60_tint));
    for (i = 0; i < 4; i++) {
        assert(SzDuel_CalcMapColor(outside[i][0], outside[i][1]) == 0);
    }
    for (i = 0; i < sizeof(data_00318e60_tint); i++) {
        assert(((unsigned char *)data_00318e60_tint)[i] == 0x77);
    }
    /* the corners are inside */
    assert(SzDuel_CalcMapColor(0, 0) == 1 && SzDuel_CalcMapColor(6, 6) == 1);

    /* kinds differ, byte 3 of quad 1 above 8, exactly 8, below 8, then 0 */
    memset(cell, 0x40, sizeof(*cell));
    cell->TargetSelectionHighlight = 3;
    cell->SelectionHighlightType = 4;
    cell->quads[1].alpha = 23;
    assert(SzDuel_CalcMapColor(5, 2) == 1 && cell->quads[1].alpha == 15 && cell->SelectionHighlightType == 4);
    assert(SzDuel_CalcMapColor(5, 2) == 1 && cell->quads[1].alpha == 7);
    cell->quads[1].alpha = 8;
    assert(SzDuel_CalcMapColor(5, 2) == 1 && cell->quads[1].alpha == 0);
    cell->quads[1].alpha = 7;
    assert(SzDuel_CalcMapColor(5, 2) == 1 && cell->quads[1].alpha == 0 && cell->SelectionHighlightType == 4);
    quad_is(&cell->quads[1], 0x40, 0x40, 0x40, 0);
    quad_is(&cell->quads[3], 0x40, 0x40, 0x40, 0x40);
    assert(SzDuel_CalcMapColor(5, 2) == 1 && cell->SelectionHighlightType == 3 && cell->TargetSelectionHighlight == 3);
    quad_is(&cell->quads[1], 0x40, 0x40, 0x40, 0);

    /* kinds equal: above by more than 8, by 8, by less; below likewise; equal */
    cell->quads[0].blue = 10; cell->quads[1].blue = 30;
    cell->quads[0].green = 10; cell->quads[1].green = 18;
    cell->quads[0].red = 10; cell->quads[1].red = 13;
    cell->quads[0].alpha = 200; cell->quads[1].alpha = 100;
    cell->quads[2].blue = 50; cell->quads[3].blue = 42;
    cell->quads[2].green = 50; cell->quads[3].green = 45;
    cell->quads[2].red = 50; cell->quads[3].red = 50;
    cell->quads[2].alpha = 255; cell->quads[3].alpha = 0;
    assert(SzDuel_CalcMapColor(5, 2) == 1);
    quad_is(&cell->quads[1], 22, 10, 10, 108);
    quad_is(&cell->quads[3], 50, 50, 50, 8);
    quad_is(&cell->quads[0], 10, 10, 10, 200);
    quad_is(&cell->quads[2], 50, 50, 50, 255);
    assert(cell->TargetSelectionHighlight == 3 && cell->SelectionHighlightType == 3);
}

int main(void)
{
    check_paint();
    check_tint();
    check_fade();
    return 0;
}
