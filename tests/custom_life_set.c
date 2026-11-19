/* Synthetic host tests for the custom_life_set group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/custom_life_set.h"

struct DotrLifeSetBlock dotr_life_set_block;
unsigned char data_00317490_lifeset[0xd70];
int data_00290a1c_lifeset;
int data_00290896_lifeset;
int data_002908a0_lifeset;
int data_00290a58_lifeset;
int data_00290a62_lifeset;
int data_00290a6c_lifeset;
struct DotrLifeSetPair data_002914a0_lifeset = { { { 10, 20, 30, 40 }, { 50, 60, 70, 80 } } };
signed char data_002bf05b_lifeset[] = "back";
signed char data_002bef93_lifeset[] = "ok!";
char data_002be944_lifeset[4];

enum { F_LOAD = 1, F_FLUSH, F_FRAME, F_X, F_ICON, F_DIGIT, F_FONT, F_BOARD, F_SORT };

struct call {
    int fn;
    long a[8];
    const void *p;
};

static struct call trace[64];
static int ntrace, msg_width, freed;
static DotrLifeSetWord next_cursor;
static struct DotrLifeSetPacket sorted[2];
static int nsorted;

static DotrLifeSetWord add(int fn, DotrLifeSetWord cursor, long a0, long a1, long a2, long a3, long a4, long a5,
                           long a6, const void *p)
{
    struct call *t;

    assert(ntrace < 64);
    assert(cursor == next_cursor);
    t = &trace[ntrace++];
    t->fn = fn;
    t->a[0] = a0;
    t->a[1] = a1;
    t->a[2] = a2;
    t->a[3] = a3;
    t->a[4] = a4;
    t->a[5] = a5;
    t->a[6] = a6;
    t->p = p;
    return ++next_cursor;
}

static struct call *nth(int fn, int k)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == fn && k-- == 0) {
            return &trace[i];
        }
    }
    assert(0);
    return 0;
}

int func_0019bef0(const char *s) { return (int)strlen(s); }

int *SzMsg_GetInfo(int index)
{
    static int info[4];

    assert(index == 1);
    info[3] = msg_width;
    return info;
}

void SzMsg_Free(int index)
{
    assert(index == 1);
    freed++;
}

void SzMsg_DrawRect_Pipe(void) {}

DotrLifeSetWord SzMsg_LoadFont_Pipe(DotrLifeSetWord cursor, int a1, int msg, int a3, int a4)
{
    return add(F_LOAD, cursor, a1, msg, a3, a4, 0, 0, 0, 0);
}

DotrLifeSetWord RhText_SetTextureFlush(DotrLifeSetWord cursor) { return add(F_FLUSH, cursor, 0, 0, 0, 0, 0, 0, 0, 0); }

DotrLifeSetWord SzMsg_DrawX_Pipe(DotrLifeSetWord cursor, int index, int x, int y, int a4, void (*function)(void))
{
    assert(index == 1 && a4 == 0 && function == SzMsg_DrawRect_Pipe);
    return add(F_X, cursor, x, y, 0, 0, 0, 0, 0, 0);
}

DotrLifeSetWord lSzDuel_DrawWindow_Frame_Pipe(DotrLifeSetWord cursor, int a1, int a2, int a3, int x, int y, int w,
                                              int h, long long a8)
{
    assert(a8 == 1);
    return add(F_FRAME, cursor, a1, a2, a3, x, y, w, h, 0);
}

DotrLifeSetWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrLifeSetWord cursor, void *def, int x, int y, int a4, int a5,
                                               int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
    assert(a4 == 0 && a5 == 1 && a6 == 0 && a7 == 0 && a8 == 0 && a9 == 0 && a10 == 1 && a11 == 0 && a12 == -1);
    assert(a13 == 0x80);
    return add(F_ICON, cursor, x, y, 0, 0, 0, 0, 0, def);
}

DotrLifeSetWord SzDuel_DrawWindow_FontDigit_Pipe(DotrLifeSetWord cursor, int x, int y, int a, int flags, int width,
                                                 int value)
{
    return add(F_DIGIT, cursor, x, y, a, flags, width, value, 0, 0);
}

DotrLifeSetWord SzDuel_DrawWindow_Font_Pipe(DotrLifeSetWord cursor, int x, int y, int a, int flags, int n,
                                            signed char *text, DotrLifeSetWord color)
{
    assert(a == 1 && flags == 2 && color == 0);
    return add(F_FONT, cursor, x, y, n, 0, 0, 0, 0, text);
}

DotrLifeSetWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrLifeSetWord cursor, int colour, int x, int y, int w, int h,
                                                int a6, int a7)
{
    assert(a6 == -1 && a7 == 0);
    return add(F_BOARD, cursor, colour, x, y, w, h, 0, 0, 0);
}

DotrLifeSetWord ExSortPrim_Pipe(DotrLifeSetWord cursor, DotrLifeSetWord packet, int a2, int a3)
{
    assert(a2 == 0 && a3 == 0 && nsorted < 2);
    memcpy(&sorted[nsorted++], (const void *)packet, sizeof(sorted[0]));
    return add(F_SORT, cursor, 0, 0, 0, 0, 0, 0, 0, 0);
}

static void reset(int player, int wide, short life)
{
    ntrace = 0;
    nsorted = 0;
    freed = 0;
    next_cursor = 500;
    msg_width = 60;
    memset(&dotr_life_set_block, 0, sizeof(dotr_life_set_block));
    memset(sorted, 0, sizeof(sorted));
    dotr_life_set_block.bytes[2 + player] = 0x13;
    dotr_life_set_block.bytes[2 + (player ^ 1)] = 0x05;
    DOTR_LIFE_SET_WIDE = (unsigned char)wide;
    dotr_life_set_block.life[player] = life;
    dotr_life_set_block.life[player ^ 1] = 1;
    data_00317490_lifeset[0xc58] = 0x21;
}

static void check_color(const struct DotrLifeSetPoint *p, const struct DotrLifeSetSource *s)
{
    assert(p->color.r == s->r && p->color.g == s->g && p->color.b == s->b && p->color.a == s->a);
    assert(p->color.q == 1.0f);
}

static void test_player1_narrow(void)
{
    DotrLifeSetWord r;
    const struct DotrLifeSetSource *c = data_002914a0_lifeset.c;
    int cx = 10 + 0x114 / 2, w = (int)(8000.0f * 176.0f / 9999.0f);

    reset(1, 0, 8000);
    r = lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe(500, 1, 10, 20);
    assert(r == next_cursor && freed == 1);
    /* Title message 0x81 + item nibble 3. */
    assert(trace[0].fn == F_LOAD && trace[0].a[1] == 0x84 && trace[0].a[3] == 0x16 && trace[1].fn == F_FLUSH);
    assert(nth(F_FRAME, 0)->a[3] == cx - 0x59 && nth(F_FRAME, 0)->a[0] == 0xc);
    assert(nth(F_X, 0)->a[0] == cx - 30 && nth(F_X, 0)->a[1] == 25);
    assert(nth(F_FRAME, 1)->a[0] == 1 && nth(F_FRAME, 1)->a[5] == -0x114 && nth(F_FRAME, 1)->a[6] == -0x96);
    assert(nth(F_DIGIT, 0)->a[0] == cx - 0xe && nth(F_DIGIT, 0)->a[5] == 8000 && nth(F_DIGIT, 0)->a[4] == 4);
    assert(nth(F_ICON, 0)->p == &data_00290a1c_lifeset && nth(F_ICON, 0)->a[0] == 10 + 0x68a + 0x8a);
    /* Gauge: 176 * life / 9999 wide, two 8-high halves with the colours swapped. */
    assert(nsorted == 2);
    assert(sorted[0].points[0].xyz.x == (unsigned)((10 + 0x668 + 0x8a) << 4));
    assert(sorted[0].points[1].xyz.x == (unsigned)((10 + 0x668 + 0x8a + w) << 4));
    assert(sorted[0].points[0].xyz.y == (20 + 0x770) << 4 && sorted[0].points[2].xyz.y == (20 + 0x778) << 4);
    assert(sorted[1].points[0].xyz.y == (20 + 0x778) << 4 && sorted[1].points[3].xyz.y == (20 + 0x780) << 4);
    assert(sorted[0].points[0].xyz.z == 0x21 && sorted[1].points[3].xyz.fog == 0);
    check_color(&sorted[0].points[0], &c[0]);
    check_color(&sorted[0].points[1], &c[0]);
    check_color(&sorted[0].points[2], &c[1]);
    check_color(&sorted[1].points[0], &c[1]);
    check_color(&sorted[1].points[3], &c[0]);
    assert(((struct DotrLifeSetTag *)sorted[0].tag)->count == 8 && ((struct DotrLifeSetTag *)sorted[0].tag)->flags == 0x14);
    assert(((struct DotrLifeSetTag *)sorted[1].tag)->registers == 0x41414141);
    /* Gauge frame drawn before (colour 0xd) and after (-1) the gauge. */
    assert(nth(F_FRAME, 3)->a[0] == 0xd && nth(F_FRAME, 4)->a[0] == -1 && nth(F_FRAME, 4)->a[3] == cx - 0x58);
    assert(nth(F_BOARD, 0)->a[0] == 1 && nth(F_BOARD, 0)->a[3] == -(0x114 - 0x24));
    /* Hints: the 0x2bf05b caption ("CHANGE" in the executable) at a quarter and the
     * 0x2bef93 caption ("BACK") at a half of width - 0x24; the stub strings are placeholders. */
    assert(nth(F_FONT, 0)->p == data_002bf05b_lifeset && nth(F_FONT, 0)->a[2] == 4);
    assert(nth(F_FONT, 0)->a[0] == 10 + 0x12 + (0x114 - 0x24) / 4 && nth(F_FONT, 0)->a[1] == 20 + 0x9c);
    assert(nth(F_FONT, 1)->p == data_002bef93_lifeset && nth(F_FONT, 1)->a[0] == 10 + 0x12 + (0x114 - 0x24) / 2);
    assert(nth(F_ICON, 5)->p == &data_00290a6c_lifeset);
}

static void test_player0_wide(void)
{
    const struct DotrLifeSetSource *c = data_002914a0_lifeset.c;

    reset(0, 1, 9999);
    lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe(500, 0, 0, 0);
    assert(nth(F_FRAME, 1)->a[5] == -0x244);
    assert(nth(F_FRAME, 0)->a[3] == 0x244 / 2 - 0x59);
    /* Full gauge 176; player 0 clears green and blue. */
    assert(sorted[0].points[1].xyz.x - sorted[0].points[0].xyz.x == 176 << 4);
    assert(sorted[0].points[0].color.r == c[0].r && sorted[0].points[0].color.g == 0 && sorted[0].points[0].color.b == 0);
    assert(sorted[0].points[2].color.r == c[1].r && sorted[0].points[2].color.g == 0 && sorted[0].points[2].color.a == c[1].a);
    assert(trace[0].a[1] == 0x84);
    assert(nth(F_DIGIT, 0)->a[5] == 9999);
}

int main(void)
{
    test_player1_narrow();
    test_player0_wide();
    return 0;
}
