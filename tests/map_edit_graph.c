/* Synthetic host tests for the map_edit_graph group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/map_edit_graph.h"

unsigned char dotr_mapgraph_cursor[2];
unsigned char data_00317490_mapgraph[0xd00];
unsigned char gSzDuel_Icon_mapgraph[290 * 10];
unsigned char gSzDuel_aaicolField_mapgraph[30 * 4];

static int field[7][7];         /* field[y][x] */
static char calls[128];          /* one letter per call: F frame, I icon, S sort, D digit */
static int ncalls;
static long frame_args[32][9];
static int nframe;
static int icon_args[16][14];
static void *icon_def[16];
static int nicon;
static struct DotrMapGraphPacket sorted[32];
static int nsort;
static int digit_args[16][6];
static int ndigit;

int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    return field[y][x];
}

DotrMapGraphWord lSzDuel_DrawWindow_Frame_Pipe(DotrMapGraphWord cursor, int a1, int a2, int a3, int x, int y,
                                               int w, int h, long a8)
{
    long v[9] = { a1, a2, a3, x, y, w, h, a8, 0 };
    memcpy(frame_args[nframe++], v, sizeof(v));
    calls[ncalls++] = 'F';
    return cursor + 1;
}

DotrMapGraphWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrMapGraphWord cursor, void *def, int x, int y, int a4,
                                                int a5, int a6, int a7, int a8, int a9, int a10, int a11,
                                                int a12, int a13)
{
    int v[14] = { 0, x, y, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, 0 };
    icon_def[nicon] = def;
    memcpy(icon_args[nicon++], v, sizeof(v));
    calls[ncalls++] = 'I';
    return cursor + 10;
}

DotrMapGraphWord ExSortPrim_Pipe(DotrMapGraphWord cursor, DotrMapGraphWord packet, int a2, int a3)
{
    assert(a2 == 0 && a3 == 0);
    memcpy(&sorted[nsort++], (void *)packet, sizeof(struct DotrMapGraphPacket));
    calls[ncalls++] = 'S';
    return cursor + 100;
}

DotrMapGraphWord SzDuel_DrawWindow_FontDigit_Pipe(DotrMapGraphWord cursor, int x, int y, int a, int flags,
                                                  int width, int value)
{
    int v[6] = { x, y, a, flags, width, value };
    memcpy(digit_args[ndigit++], v, sizeof(v));
    calls[ncalls++] = 'D';
    return cursor + 1000;
}

static void reset(void)
{
    ncalls = nframe = nicon = nsort = ndigit = 0;
    memset(calls, 0, sizeof(calls));
}

static void check_point(const struct DotrMapGraphPoint *p, int x, int y, const unsigned char *src)
{
    assert(p->color.r == src[2] && p->color.g == src[1] && p->color.b == src[0] && p->color.a == src[3]);
    assert(p->color.q == 1.0f);
    assert((int)p->xyz.x == ((x << 4) & 0xffff) && (int)p->xyz.y == ((y << 4) & 0xffff));
    assert(p->xyz.z == 0x345678 && p->xyz.fog == 0);
}

static void check_graph(void)
{
    int k, i, x = 4, y = 6;
    int counts[10] = { 0 };
    DotrMapGraphWord r;

    /* Kind 0 fills the map except: kind 3 on 7 cells (row 2), kind 9 on 1 cell, kind 5 on
     * 14 cells (rows 4 and 5). */
    memset(field, 0, sizeof(field));
    for (i = 0; i < 7; i++) {
        field[2][i] = 3;
        field[4][i] = 5;
        field[5][i] = 5;
    }
    field[6][6] = 9;
    for (k = 0; k < 7 * 7; k++) {
        counts[field[k / 7][k % 7]]++;
    }
    assert(counts[0] == 27 && counts[3] == 7 && counts[5] == 14 && counts[9] == 1);
    *(int *)(data_00317490_mapgraph + 0xc58) = 0x345678;
    for (k = 0; k < 30 * 4; k++) {
        gSzDuel_aaicolField_mapgraph[k] = (unsigned char)(k * 7 + 1);
    }
    dotr_mapgraph_cursor[0] = 3;   /* column */
    dotr_mapgraph_cursor[1] = 2;   /* row 2: kind 3 */
    reset();
    r = lSzDuel_DrawMapEdit_Graph_Pipe(0x10, x, y);
    /* 21 frames, 10 icons, 20 sorts, 10 digit runs, in this order */
    assert(ncalls == 61 && nframe == 21 && nicon == 10 && nsort == 20 && ndigit == 10);
    assert(r == 0x10 + 21 + 10 * 10 + 20 * 100 + 10 * 1000);
    assert(calls[0] == 'F');
    for (k = 0; k < 10; k++) {
        assert(memcmp(&calls[1 + k * 6], "IFSSFD", 6) == 0);
    }
    assert(frame_args[0][0] == 0 && frame_args[0][1] == -1 && frame_args[0][2] == 11 && frame_args[0][3] == x &&
           frame_args[0][4] == y && frame_args[0][5] == -110 && frame_args[0][6] == -170 && frame_args[0][7] == 1);
    for (k = 0; k < 10; k++) {
        int w = (int)((float)counts[k] * 84.0f / 49.0f);
        int px = x + 0x6d5, py = y + 0x725 + k * 16;
        const unsigned char *c = gSzDuel_aaicolField_mapgraph + k * 12;
        const struct DotrMapGraphPacket *a = &sorted[k * 2], *b = &sorted[k * 2 + 1];
        const struct DotrMapGraphTag *t = (const struct DotrMapGraphTag *)a->tag;

        /* icon: row 270 + k of the definitions, highlighted only for the kind under the cursor */
        assert(icon_def[k] == gSzDuel_Icon_mapgraph + (270 + k) * 10);
        assert(icon_args[k][1] == x + 0x6c5 && icon_args[k][2] == y + 0x725 + k * 16);
        assert(icon_args[k][3] == 0 && icon_args[k][4] == 1 && icon_args[k][8] == 0 && icon_args[k][9] == 1);
        assert(icon_args[k][10] == 0 && icon_args[k][11] == (k == 3 ? 1 : -1) && icon_args[k][12] == 0x80);
        /* the frame around the bar, first with 13 then with -1 */
        assert(frame_args[1 + k * 2][0] == 13 && frame_args[2 + k * 2][0] == -1);
        assert(frame_args[1 + k * 2][3] == x + 21 && frame_args[1 + k * 2][4] == y + 5 + k * 16);
        assert(frame_args[1 + k * 2][5] == -84 && frame_args[1 + k * 2][6] == -16 && frame_args[2 + k * 2][7] == 1);
        /* the two bands */
        assert(t->count == 8 && t->flags == (0x14 | 0) && t->padding == 0 && t->registers == 0x41414141);
        check_point(&a->points[0], px, py, c);
        check_point(&a->points[1], px + w, py, c);
        check_point(&a->points[2], px, py + 8, c + 4);
        check_point(&a->points[3], px + w, py + 8, c + 4);
        check_point(&b->points[0], px, py + 8, c + 4);
        check_point(&b->points[1], px + w, py + 8, c + 4);
        check_point(&b->points[2], px, py + 16, c + 8);
        check_point(&b->points[3], px + w, py + 16, c + 8);
        /* the count */
        assert(digit_args[k][0] == x + 63 && digit_args[k][1] == y + 7 + k * 16 && digit_args[k][2] == 0 &&
               digit_args[k][3] == 2 && digit_args[k][4] == 2 && digit_args[k][5] == counts[k]);
    }
    /* widths: 27 cells -> 46, 7 -> 12, 14 -> 24, 1 -> 1, none -> 0 */
    assert((int)sorted[0].points[1].xyz.x - (int)sorted[0].points[0].xyz.x == 46 * 16);
    assert((int)sorted[6].points[1].xyz.x - (int)sorted[6].points[0].xyz.x == 12 * 16);
    assert((int)sorted[10].points[1].xyz.x - (int)sorted[10].points[0].xyz.x == 24 * 16);
    assert((int)sorted[18].points[1].xyz.x - (int)sorted[18].points[0].xyz.x == 1 * 16);
    assert(sorted[2].points[1].xyz.x == sorted[2].points[0].xyz.x);

    /* a full map of one kind: 49 cells -> the full 84; cursor on kind 0 */
    memset(field, 0, sizeof(field));
    for (k = 0; k < 49; k++) {
        field[k / 7][k % 7] = 7;
    }
    dotr_mapgraph_cursor[0] = 0;
    dotr_mapgraph_cursor[1] = 0;
    reset();
    lSzDuel_DrawMapEdit_Graph_Pipe(0, 0, 0);
    assert((int)sorted[14].points[1].xyz.x - (int)sorted[14].points[0].xyz.x == 84 * 16);
    assert(digit_args[7][5] == 49 && digit_args[0][5] == 0);
    for (k = 0; k < 10; k++) {
        assert(icon_args[k][11] == (k == 7 ? 1 : -1));
    }
}

int main(void)
{
    check_graph();
    return 0;
}
