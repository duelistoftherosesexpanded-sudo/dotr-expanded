/* Synthetic host tests for the trade_bag group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/trade_bag.h"

union DotrTradeBagState dotr_trade_bag_state;
void *dotr_trade_bag_ptrs[4];
unsigned char gSzDuel_Icon_tradebag[0xa50];
static signed char sort_names[10][8];
signed char *data_00290fd0_tradebag[10];
static signed char label_texts[3][8] = { "lab0", "label1", "lb2" };
struct DotrTradeBagLabels data_00291490_tradebag;
unsigned char data_002be8a4_tradebag[4];
char data_002be944_tradebag[4];
char data_002bef98_tradebag[] = "description";
char data_002bf021_tradebag[] = "T1";
char data_002bf024_tradebag[] = "T0\0sub";
unsigned char data_0031814e_tradebag[2];

/* Every cursor call is one trace row: kind, then up to ten words. The cursor must arrive in
 * order (each call returns cursor + 1). */
enum { FRAME = 1, FONT, DIGIT, ICON, BOARD, CARD, SORT };
static long trace[80][11];
static int n_trace, pad_calls, pad_result, bag_calls;
static DotrTradeBagWord next_cursor;
static struct DotrTradeBagPacket sorted;
static unsigned short lists[2][16];
static unsigned char counts[2][0x356];

static DotrTradeBagWord row(DotrTradeBagWord cursor, long kind, long a0, long a1, long a2, long a3, long a4, long a5,
                            long a6, long a7, long a8, long a9)
{
    long *r = trace[n_trace++];

    assert(n_trace <= 80 && cursor == next_cursor);
    r[0] = kind; r[1] = a0; r[2] = a1; r[3] = a2; r[4] = a3; r[5] = a4;
    r[6] = a5; r[7] = a6; r[8] = a7; r[9] = a8; r[10] = a9;
    return ++next_cursor;
}

int SzPad_Check(int port, int kind, int mask)
{
    assert(kind == 1 && mask == 0x100);
    pad_calls++;
    return pad_result ? port + 1 : 0;
}
int func_0019bef0(const char *s)
{
    return (int)strlen(s);
}
DotrTradeBagWord func_001b4780_tradebag(DotrTradeBagWord cursor, int a1, int a2, int a3, int x, int y, int w, int h,
                                        long long a8)
{
    return row(cursor, FRAME, a1, a2, a3, x, y, w, h, (long)a8, 0, 0);
}
DotrTradeBagWord func_001b3cc0_tradebag(DotrTradeBagWord cursor, unsigned char *src, int x, int y, int background,
                                        int image, int border, int shrink, int flip_x, int flip_y, int bright,
                                        unsigned char *rgb, int blend, unsigned char alpha)
{
    assert(background == 0 && image == 1 && border == 0 && shrink == 0 && flip_x == 0 && flip_y == 0 && bright == 1);
    return row(cursor, ICON, src - gSzDuel_Icon_tradebag, x, y, (long)rgb, blend, alpha, 0, 0, 0, 0);
}
DotrTradeBagWord func_001b65f0_tradebag(DotrTradeBagWord cursor, int colour, int x, int y, int w, int h, int shadow,
                                        int style)
{
    return row(cursor, BOARD, colour, x, y, w, h, shadow, style, 0, 0, 0);
}
DotrTradeBagWord func_001b6ea0_tradebag(DotrTradeBagWord cursor, int x, int y, int a, int flags, int width, int number)
{
    return row(cursor, DIGIT, x, y, a, flags, width, number, 0, 0, 0, 0);
}
DotrTradeBagWord func_001b6f70_tradebag(DotrTradeBagWord cursor, int x, int y, int font, int flags, int count,
                                        signed char *text, unsigned char *color)
{
    assert(count == (int)strlen((char *)text));
    return row(cursor, FONT, x, y, font, flags, count, (long)text, (long)color, 0, 0, 0);
}
DotrTradeBagWord func_001c4b00_tradebag(DotrTradeBagWord cursor, int side, int id, int x, int y, int a5)
{
    return row(cursor, CARD, side, id, x, y, a5, 0, 0, 0, 0, 0);
}
DotrTradeBagWord func_00207650_tradebag(DotrTradeBagWord cursor, DotrTradeBagWord packet, int a2, int a3)
{
    assert(a2 == 0 && a3 == 0);
    memcpy(&sorted, (void *)packet, sizeof(sorted));
    return row(cursor, SORT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
int SzData_GetBagCountAll(int side)
{
    bag_calls++;
    return 300 + side;
}

static int at;
static void ex(long kind, long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7)
{
    long *r = trace[at++];

    if (r[0] != kind || r[1] != a0 || r[2] != a1 || r[3] != a2 || r[4] != a3 || r[5] != a4 || r[6] != a5 ||
        r[7] != a6 || r[8] != a7) {
        fprintf(stderr, "row %d: %ld %ld %ld %ld %ld %ld %ld %ld %ld\n", at - 1, r[0], r[1], r[2], r[3], r[4], r[5],
                r[6], r[7], r[8]);
        assert(0);
    }
}
static void icon(long off, long x, long y, long rgb, long blend)
{
    ex(ICON, off, x, y, rgb, blend, 0x80, 0, 0);
}
static void font(long x, long y, long f, long flags, const void *text, const void *color)
{
    ex(FONT, x, y, f, flags, (long)strlen(text), (long)text, (long)color, 0);
}

static void reset(void)
{
    unsigned char *d = dotr_trade_bag_state.b;
    int i;

    memset(&dotr_trade_bag_state, 0, sizeof(dotr_trade_bag_state));
    memset(trace, 0, sizeof(trace));
    memset(&sorted, 0, sizeof(sorted));
    n_trace = pad_calls = bag_calls = at = 0;
    for (i = 0; i < 10; i++) {
        snprintf((char *)sort_names[i], sizeof(sort_names[i]), "s%d", i);
        data_00290fd0_tradebag[i] = sort_names[i];
    }
    for (i = 0; i < 3; i++) {
        data_00291490_tradebag.text[i] = label_texts[i];
    }
    for (i = 0; i < 16; i++) {
        lists[0][i] = (unsigned short)(100 + i);
        lists[1][i] = (unsigned short)(200 + i);
    }
    memset(counts, 0, sizeof(counts));
    counts[0][0] = 3; counts[0][0x355] = 4;  /* side 0 total 7 (first and last entry) */
    counts[1][10] = 200; counts[1][11] = 100; /* side 1 total 300 */
    dotr_trade_bag_ptrs[0] = lists[0];
    dotr_trade_bag_ptrs[1] = lists[1];
    dotr_trade_bag_ptrs[2] = counts[0];
    dotr_trade_bag_ptrs[3] = counts[1];
    *(int *)(d + 0xC58) = 0x123456;
    *(unsigned short *)(d + 0xC8A) = 2;  /* side 0 top, length */
    *(unsigned short *)(d + 0xC8C) = 9;
    *(unsigned short *)(d + 0xC8E) = 5;  /* side 1 top, length */
    *(unsigned short *)(d + 0xC90) = 0;
    d[0xD5E] = 3;
    d[0xD5F] = 1;
    d[0xCB6] = 4; d[0xCB7] = 6;  /* side 0 sort bytes */
    d[0xCB9] = 7; d[0xCBA] = 2;  /* side 1 sort bytes */
    d[0xCC0] = 1; d[0xCC1] = 0;  /* chosen label per side */
}

/* Branch and loop coverage: side 0 (left column, no sort selection, a non-empty list: the bar
 * packet and the page numbers), side 1 with sort selection enabled (right column, coloured tab
 * and name, empty list: no bar), side 1 special but the button not held. */
static void run_side(int side, int special, int held, int expect_count)
{
    unsigned char *d = dotr_trade_bag_state.b;
    const int X = 1000, Y = 2000;
    int sel = special && held;
    unsigned char *color = sel ? data_002be8a4_tradebag : NULL;
    int cur = d[(side ? 0xCB9 : 0xCB6) + sel];
    int top = side ? 5 : 2;
    int len = side ? 0 : 9;
    int pos = side ? 1 : 3;
    int i;
#define S(l, r) (side ? (r) : (l))

    reset();
    if (side) {
        *(unsigned short *)(d + 0xC90) = (unsigned short)len;
    }
    cur = d[(side ? 0xCB9 : 0xCB6) + sel];
    data_0031814e_tradebag[side] = (unsigned char)(special ? 1 : 2);
    data_0031814e_tradebag[!side] = 1;
    pad_result = held;
    next_cursor = 0x5000;
    assert(lSzDuel_DrawTrade_Bag_Pipe(0x5000, side, X, Y) == 0x5000 + expect_count);
    assert(n_trace == expect_count && pad_calls == (special ? 1 : 0) && bag_calls == 1);

    ex(FRAME, 0xC, -1, 0xB, X + S(0x10, 0x240), Y + 0x10, -0x30, -0x30, 1);
    font(X + S(0x10, 0x240) + 0x18, Y + 0x1F, 2, 3, side ? data_002bf021_tradebag : data_002bf024_tradebag, NULL);
    ex(FRAME, 0xC, -1, 6, X + S(0x40, 0x1B0), Y + 0x10, -0x8C, -0x2C, 1);
    font(X + S(0x40, 0x1B0) + 0x48, Y + 0x15, 1, 3, data_002bf024_tradebag + 3, NULL);
    font(X + S(0x40, 0x1B0) + 0x24, Y + 0x23, 1, 3, label_texts[1], NULL);
    ex(DIGIT, X + S(0x40, 0x1B0) + 0x24, Y + 0x31, 1, 2, 4, side ? 300 : 7, 0, 0);
    font(X + S(0x40, 0x1B0) + 0x6C, Y + 0x23, 1, 3, label_texts[2], NULL);
    ex(DIGIT, X + S(0x40, 0x1B0) + 0x6C, Y + 0x31, 1, 2, 4, side ? 7 : 300, 0, 0);
    ex(FRAME, 0xC, -1, 5, X + S(0x10, 0x230), Y + 0x40, -0x40, -0x14, 1);
    icon(0x7ee, X + 0x6C2 + S(0x10, 0x230), Y + 0x762, 0, -1);
    ex(DIGIT, X + S(0x10, 0x230) + 0x17, Y + 0x44, 1, 0, 4, 300 + side, 0, 0);
    ex(BOARD, 3, X + S(0x10, 0x230), Y + 0x54, -0x40, -0x1C, -1, 0, 0);
    icon(0xa28, X + 0x6C0 + S(0x15, 0x255), Y + 0x77D, 0, -1);
    icon(0x7a8, X + 0x6C0 + S(0x2C, 0x234), Y + 0x77C, 0, -1);
    ex(BOARD, 3, X + S(0x50, 0x140), Y + 0x40, -0xF0, -0x30, -1, 0, 0);
    icon(0x816, X + 0x6CE + S(0x50, 0x140), Y + 0x765, 0, -1);
    icon(0xa3c, X + 0x6C7 + S(0x50, 0x140), Y + 0x77E, 0, -1);
    for (i = 0; i < 10; i++) {
        icon((i + 0xD1) * 10, X + 0x6E8 + S(0x50, 0x140) + 16 * i, Y + 0x769, (long)color,
             i == cur ? (sel ? 2 : 1) : -1);
    }
    icon(0x820, X + 0x78A + S(0x50, 0x140), Y + 0x765, 0, -1);
    icon(0xa46, X + 0x793 + S(0x50, 0x140), Y + 0x77E, 0, -1);
    font(X + S(0x50, 0x140) + 0x78, Y + 0x5E, 0, sel ? 11 : 3, sort_names[cur], color);
    for (i = 0; i < 5; i++) {
        ex(CARD, side, lists[side][top + i], X + S(8, 0x148), Y + 0x70 + 0x36 * i, 1, 0, 0, 0);
    }
    ex(BOARD, 3, X + S(0x10, 0x184), Y + 0x17E, -0xEC, -0x32, -1, 0, 0);
    icon(0x7b2, X + 0x6C0 + S(0x2C, 0x234), Y + 0x8A2, 0, -1);
    icon(0xa32, X + 0x6C0 + S(0x15, 0x255), Y + 0x8A7, 0, -1);
    icon(0x9ec, X + 0x6C0 + S(0x17, 0x259), Y + 0x8BA, 0, -1);
    font(X + S(0x2C, 0x231), Y + 0x19D, 1, 0, data_002bef98_tradebag, NULL);
    for (i = 0; i < 3; i++) {
        int chosen = d[0xCC0 + side] == i;

        ex(BOARD, 3, X + S(0x7E, 0x18A), Y + 0x182 + 0xE * i, -0x78, -0xE, -1, 1, 0);
        font(X + S(0x7E, 0x18A) + 1, Y + 0x183 + 0xE * i, 1, 0, label_texts[i], chosen ? NULL : data_002be944_tradebag);
    }
    ex(BOARD, 3, X + S(0xFC, 0x148), Y + 0x17E, -0x3C, -0x32, -1, 0, 0);
    if (len) {
        ex(SORT, 0, 0, 0, 0, 0, 0, 0, 0);
        assert(sorted.header.type == 3 && sorted.header.count == 6 && sorted.header.pad == 0);
        assert(sorted.header.registers == 0x441);
        assert(sorted.r == 0xA0 && sorted.g == 0xA0 && sorted.b == 0xA0 && sorted.a == 0x80 && sorted.q == 1.0f);
        assert(sorted.first.x == (unsigned)(((X + S(0xFC, 0x148) + 0x6CA) << 4) & 0xffff));
        assert(sorted.first.y == (unsigned)(((Y + 0x17E + 0x738) << 4) & 0xffff));
        assert(sorted.second.x == (unsigned)(((X + S(0xFC, 0x148) + 0x6F2) << 4) & 0xffff));
        assert(sorted.second.y == (unsigned)(((Y + 0x17E + 0x739) << 4) & 0xffff));
        assert(sorted.first.z == 0x123456 && sorted.second.z == 0x123456 && sorted.first.fog == 0);
        ex(DIGIT, X + S(0xFC, 0x148) + 0x1E, Y + 0x186, 1, 2, 3, top + pos + 1, 0, 0);
        ex(DIGIT, X + S(0xFC, 0x148) + 0x1E, Y + 0x186 + 0x17, 1, 2, 3, len, 0, 0);
    }
    assert(at == n_trace);
#undef S
}

int main(void)
{
    run_side(0, 0, 0, 50);
    run_side(1, 1, 1, 47);
    run_side(1, 1, 0, 47);
    run_side(0, 1, 1, 50);
    puts("trade_bag: ok");
    return 0;
}
