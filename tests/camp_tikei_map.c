/* Synthetic host tests for the camp_tikei_map group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_tikei_map.h"

signed char data_00299e10[200];
struct DotrTikeiWindow camp_window_Map_data[4];
struct DotrTikeiItem data_0029afa0;
struct DotrTikeiItem data_0029afd0;
unsigned char gSzDuel_MapData[46][7][7];
struct DotrTikeiItem data_002986b0[10][2];
struct DotrTikeiCounts dotr_tikei_counts_01ed0000;

static struct {
    int n;
    short code[49], x[49], y[49], tag[49];
} obj;
static struct {
    int n;
    struct DotrTikeiItem copy[10];
    short x[10], y[10];
} bars;
static struct {
    int n;
    struct DotrTikeiItem *quad[20];
    short x[20], y[20];
    short width[20], height[20];
} quads;
static struct {
    int n;
    int x[10], y[10], a[10], flags[10], width[10], value[10];
} digits;

DotrTikeiWord Taku_Obj_Set(DotrTikeiWord cursor, struct DotrTikeiItem *item, short x, short y)
{
    obj.code[obj.n] = item->code;
    obj.tag[obj.n] = item->w[0];
    obj.x[obj.n] = x;
    obj.y[obj.n++] = y;
    return cursor + 1;
}
DotrTikeiWord Taku_G4_Set(DotrTikeiWord cursor, struct DotrTikeiItem *quad, short dx, short dy)
{
    bars.copy[bars.n] = *quad;
    bars.x[bars.n] = dx;
    bars.y[bars.n++] = dy;
    return cursor + 1;
}
DotrTikeiWord func_00218ab0_tikeimap(DotrTikeiWord cursor, void *quad, short dx, short dy, short width,
                                     short height)
{
    quads.quad[quads.n] = quad;
    quads.x[quads.n] = dx;
    quads.y[quads.n] = dy;
    quads.width[quads.n] = width;
    quads.height[quads.n++] = height;
    return cursor + 1;
}
DotrTikeiWord SzDuel_DrawWindow_FontDigit_Pipe(DotrTikeiWord cursor, int x, int y, int a, int flags, int width,
                                               int value)
{
    digits.x[digits.n] = x;
    digits.y[digits.n] = y;
    digits.a[digits.n] = a;
    digits.flags[digits.n] = flags;
    digits.width[digits.n] = width;
    digits.value[digits.n++] = value;
    return cursor + 1;
}

static void reset(void)
{
    int i, m, k;

    memset(&obj, 0, sizeof(obj));
    memset(&bars, 0, sizeof(bars));
    memset(&quads, 0, sizeof(quads));
    memset(&digits, 0, sizeof(digits));
    memset(gSzDuel_MapData, 0, sizeof(gSzDuel_MapData));
    memset(data_002986b0, 0, sizeof(data_002986b0));
    memset(data_00299e10, 0, sizeof(data_00299e10));
    for (i = 0; i < 10; i++) {
        dotr_tikei_counts_01ed0000.count[i] = 77;
    }
    dotr_tikei_counts_01ed0000.unknown_00[2] = 55;
    data_0029afa0.w[0] = 11;
    data_0029afa0.code = 1;
    data_0029afd0.unknown_10[0] = 22;
    camp_window_Map_data[3].x = 10;
    camp_window_Map_data[3].y = 20;
    camp_window_Map_data[3].w = 30;
    camp_window_Map_data[3].h = 40;
    for (m = 0; m < 2; m++) {
        for (k = 0; k < 8; k++) {
            data_002986b0[0][m].w[k] = (short)(100 * (m + 1) + k);
        }
    }
}

/* Branch coverage: one case per outcome of each test in the function, loop bounds checked. */
static void check_map_hyouji(void)
{
    int i, j;

    /* Map 2 all zero: every cell counts as terrain 0, drawn unmirrored. */
    reset();
    assert(Taku_Camp_TIKEI_Map_Hyouji(1000, 2, 300, 200) == 1000 + 49 + 10 + 20 + 10);
    assert(obj.n == 49 && bars.n == 10 && quads.n == 20 && digits.n == 10);
    assert(dotr_tikei_counts_01ed0000.count[0] == 49 && dotr_tikei_counts_01ed0000.count[1] == 0);
    assert(dotr_tikei_counts_01ed0000.count[9] == 0 && dotr_tikei_counts_01ed0000.unknown_00[2] == 55);
    assert(obj.code[0] == 65 && obj.tag[0] == 11 && obj.x[0] == 300 && obj.y[0] == 200);
    assert(obj.x[1] == 316 && obj.y[1] == 200 && obj.x[7] == 300 && obj.y[7] == 216);
    assert(obj.x[48] == 396 && obj.y[48] == 296 && data_0029afa0.code == 1);
    /* Bars: the fourth window record as four corners, 16 apart; the template is not written. */
    assert(bars.copy[0].w[0] == 10 && bars.copy[0].w[1] == 20 && bars.copy[0].w[2] == 30 && bars.copy[0].w[3] == 20);
    assert(bars.copy[0].w[4] == 10 && bars.copy[0].w[5] == 40 && bars.copy[0].w[6] == 30 && bars.copy[0].w[7] == 40);
    assert(bars.copy[0].unknown_10[0] == 22 && data_0029afd0.w[0] == 0);
    assert(bars.x[0] == 300 && bars.y[0] == 200 && bars.x[9] == 300 && bars.y[9] == 200 + 144);
    /* Count 49 or more: full width 76, two digits; count 0: width 0, one digit. */
    assert(quads.width[0] == 76 && quads.width[1] == 76 && quads.width[2] == 0 && quads.width[19] == 0);
    assert(quads.quad[0] == &data_002986b0[0][0] && quads.quad[1] == &data_002986b0[0][1]);
    assert(quads.quad[19] == &data_002986b0[9][1] && quads.width[0] == 76 && quads.height[0] == 0);
    assert(quads.x[19] == 300 && quads.y[19] == 344 && quads.width[19] == 0);
    assert(data_002986b0[9][0].w[0] == 100 && data_002986b0[9][0].w[7] == 107 && data_002986b0[9][1].w[7] == 207);
    assert(data_002986b0[9][0].unknown_10[0] == 0);
    assert(digits.x[0] == 300 + 20 && digits.y[0] == 200 + 101 && digits.a[0] == 1 && digits.flags[0] == 2);
    assert(digits.width[0] == 2 && digits.value[0] == 49);
    assert(digits.y[9] == 200 + 144 + 101 && digits.width[9] == 1 && digits.value[9] == 0);

    /* Codes 0-10 in turn, mirrored: 0-4 five times, 5-9 four times, 10 drawn as 65 uncounted. */
    reset();
    data_00299e10[0] = 1;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            gSzDuel_MapData[5][i][j] = (unsigned char)((i * 7 + j) % 11);
        }
    }
    assert(Taku_Camp_TIKEI_Map_Hyouji(0, 5, 0, 0) == 89);
    for (i = 0; i < 10; i++) {
        assert(dotr_tikei_counts_01ed0000.count[i] == (i < 5 ? 5 : 4));
        assert(obj.code[i] == 65 + i && digits.value[i] == (i < 5 ? 5 : 4) && digits.width[i] == 1);
    }
    assert(obj.code[10] == 65 && obj.code[11] == 65 && obj.code[12] == 66);
    assert(obj.x[0] == 96 && obj.y[0] == 96 && obj.x[1] == 80 && obj.y[1] == 96 && obj.x[48] == 0 && obj.y[48] == 0);
    /* Between the clamps: 76 * count / 49, truncated to a halfword (7.75 and 6.2). */
    assert(quads.width[0] == 7 && quads.width[1] == 7 && quads.width[18] == 6 && quads.height[18] == 0);
    assert(quads.x[18] == 0 && quads.y[18] == 144 && digits.x[0] == 20 && digits.y[0] == 101);
}

int main(void)
{
    check_map_hyouji();
    puts("camp_tikei_map: ok");
    return 0;
}
