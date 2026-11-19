/* Synthetic host tests for the camp_sinema_cut group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_sinema_cut.h"

struct DotrSinemaWork dotr_camp_sinema_cut_work;
struct DotrSinemaQuad data_0029aec0;
struct DotrSinemaQuad tex_sinema_brack[2];

static struct {
    int n;
    DotrTikeiWord cursor[2];
    struct DotrSinemaQuad quad[2];
    int dx[2], dy[2];
} g4;

/* Stub: records the cursor, a copy of the 38-byte quad and the offsets of each call. */
DotrTikeiWord Taku_G4_Set(DotrTikeiWord cursor, struct DotrTikeiItem *quad, short dx, short dy)
{
    assert(g4.n < 2);
    g4.cursor[g4.n] = cursor;
    memcpy(&g4.quad[g4.n], quad, sizeof g4.quad[0]);
    g4.dx[g4.n] = dx;
    g4.dy[g4.n] = dy;
    g4.n++;
    return cursor + 1;
}

#define W dotr_camp_sinema_cut_work

static void run(int fade, int mode, int state, int hold, int c0, int c1, int c2)
{
    int i, j;

    memset(&W, 0, sizeof W);
    W.mode = (short)mode;
    W.state = (short)state;
    W.hold = (short)hold;
    W.cut0 = (short)c0;
    W.cut1 = (short)c1;
    W.cut2 = (short)c2;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            tex_sinema_brack[i].w[j] = (short)(10 * (j + 1) + i);
        }
        for (j = 0; j < 16; j++) {
            tex_sinema_brack[i].col[j] = (unsigned char)(100 + 20 * i + j);
        }
        tex_sinema_brack[i].flag = (signed char)(5 + i);
        tex_sinema_brack[i].unknown_21 = (char)(7 + i);
        tex_sinema_brack[i].z = (short)(300 + i);
        tex_sinema_brack[i].code = (short)(500 + i);
    }
    tex_sinema_brack[0].w[1] = 20;
    tex_sinema_brack[1].w[1] = 400;
    for (j = 0; j < 8; j++) {
        data_0029aec0.w[j] = (short)(-1 - j);
    }
    for (j = 0; j < 16; j++) {
        data_0029aec0.col[j] = (unsigned char)(j + 1);
    }
    data_0029aec0.flag = 9;
    data_0029aec0.unknown_21 = 3;
    data_0029aec0.z = 77;
    data_0029aec0.code = 88;
    memset(&g4, 0, sizeof g4);
    /* both bars are always drawn, the cursor is threaded through and offsets are 0 */
    assert(Taku_Camp_SINEMA_CUT(100, (unsigned short)fade) == 102);
    assert(g4.n == 2 && g4.cursor[0] == 100 && g4.cursor[1] == 101);
    assert(g4.dx[0] == 0 && g4.dy[0] == 0 && g4.dx[1] == 0 && g4.dy[1] == 0);
}

/* The counters, mode and state after the call. */
static int now(int mode, int state, int c0, int c1, int c2)
{
    return W.mode == mode && W.state == state && W.cut0 == c0 && W.cut1 == c1 && W.cut2 == c2;
}

/* The two bars as drawn with fade == 0 for a total cut height `sum`. */
static void check_bars(int sum)
{
    int i, j;

    for (i = 0; i < 2; i++) {
        struct DotrSinemaQuad *q = &g4.quad[i];

        for (j = 0; j < 8; j++) {
            int want = tex_sinema_brack[i].w[j];

            if (i == 0 && (j == 5 || j == 7)) {
                want = 20 + sum;
            }
            if (i == 1 && (j == 1 || j == 3)) {
                want = 400 - sum;
            }
            assert(q->w[j] == want);
        }
        for (j = 0; j < 16; j++) {
            assert(q->col[j] == (j % 4 == 3 ? 128 : 0));
        }
        assert(q->flag == 0 && q->unknown_21 == 7 + i && q->z == 300 + i && q->code == 500 + i);
    }
}

/* The two bars as drawn with fade != 0: the copy of 0x29aec0 with the template's coordinates
 * and the four alpha bytes. */
static void check_fade_bars(int alpha)
{
    int i, j;

    for (i = 0; i < 2; i++) {
        struct DotrSinemaQuad *q = &g4.quad[i];

        for (j = 0; j < 8; j++) {
            assert(q->w[j] == tex_sinema_brack[i].w[j]);
        }
        for (j = 0; j < 16; j++) {
            assert(q->col[j] == (j % 4 == 3 ? alpha : j + 1));
        }
        assert(q->flag == 9 && q->unknown_21 == 3 && q->z == 77 && q->code == 88);
    }
}

static void check_cut(void)
{
    /* Mode 0: all three counters go down by 8 to 0; a negative counter is left alone and not
     * counted. */
    run(0, 0, 1, 0, 8, 8, 8);
    assert(now(0, 2, 0, 0, 0));
    check_bars(0);
    run(0, 0, 1, 0, 0, 0, 0);
    assert(now(0, 2, 0, 0, 0));
    run(0, 0, 1, 0, 16, 8, 0);
    assert(now(0, 1, 8, 0, 0));
    check_bars(8);
    run(0, 0, 1, 0, 20, 4, 9);
    assert(now(0, 1, 12, 0, 1));
    check_bars(13);
    run(0, 0, 1, 0, -1, 0, 4);
    assert(now(0, 1, -1, 0, 0));
    check_bars(-1);
    run(0, 0, 1, 0, 0, -3, 0);
    assert(now(0, 1, 0, -3, 0));
    run(0, 0, 1, 0, 0, 0, -3);
    assert(now(0, 1, 0, 0, -3));

    /* Mode 1: cut0 up to 64 (untouched above 64), the others down. */
    run(0, 1, 1, 0, 56, 8, 8);
    assert(now(1, 2, 64, 0, 0));
    check_bars(64);
    run(0, 1, 1, 0, 64, 0, 0);
    assert(now(1, 2, 64, 0, 0));
    run(0, 1, 1, 0, 65, 0, 0);
    assert(now(1, 1, 65, 0, 0));
    run(0, 1, 1, 0, 48, 16, 0);
    assert(now(1, 1, 56, 8, 0));
    check_bars(64);
    run(0, 1, 1, 0, 60, 0, 24);
    assert(now(1, 1, 64, 0, 16));
    check_bars(80);
    run(0, 1, 1, 0, 64, -1, -1);
    assert(now(1, 1, 64, -1, -1));

    /* Mode 2: cut0 up to 64 first; only once it is there, cut1 up to 64; cut2 down. */
    run(0, 2, 1, 0, 56, 56, 0);
    assert(now(2, 1, 64, 56, 0));
    check_bars(120);
    run(0, 2, 1, 0, 0, 0, -5);
    assert(now(2, 1, 8, 0, -5));
    check_bars(3);
    run(0, 2, 1, 0, 64, 56, 0);
    assert(now(2, 2, 64, 64, 0));
    check_bars(128);
    run(0, 2, 1, 0, 64, 48, 0);
    assert(now(2, 1, 64, 56, 0));
    run(0, 2, 1, 0, 100, 65, 8);
    assert(now(2, 1, 64, 65, 0));
    run(0, 2, 1, 0, 70, 64, 16);
    assert(now(2, 1, 64, 64, 8));
    check_bars(136);

    /* Mode 3: cut0, then cut1 up to 64, then cut2 up to 116. */
    run(0, 3, 1, 0, 48, 0, 0);
    assert(now(3, 1, 56, 0, 0));
    run(0, 3, 1, 0, 56, 0, 0);
    assert(now(3, 1, 64, 0, 0));
    run(0, 3, 1, 0, 64, 48, 0);
    assert(now(3, 1, 64, 56, 0));
    run(0, 3, 1, 0, 80, 56, 0);
    assert(now(3, 1, 64, 64, 0));
    run(0, 3, 1, 0, 64, 64, 100);
    assert(now(3, 1, 64, 64, 108));
    check_bars(236);
    run(0, 3, 1, 0, 70, 90, 108);
    assert(now(3, 2, 64, 64, 116));
    check_bars(244);
    run(0, 3, 1, 0, 64, 64, 116);
    assert(now(3, 2, 64, 64, 116));
    run(0, 3, 1, 0, 64, 64, 200);
    assert(now(3, 2, 64, 64, 116));

    /* Any other mode, a state other than 1 or a non-zero hold: nothing steps, the bars are
     * still drawn from the counters as they are. */
    run(0, 4, 1, 0, 5, 6, 7);
    assert(now(4, 1, 5, 6, 7));
    check_bars(18);
    run(0, -1, 1, 0, 5, 6, 7);
    assert(now(-1, 1, 5, 6, 7));
    run(0, 0, 0, 0, 8, 8, 8);
    assert(now(0, 0, 8, 8, 8));
    check_bars(24);
    run(0, 0, 2, 0, 8, 8, 8);
    assert(now(0, 2, 8, 8, 8));
    run(0, 0, 1, 1, 8, 8, 8);
    assert(now(0, 1, 8, 8, 8) && W.hold == 1);
    check_bars(24);
    run(0, 3, 1, -1, 64, 64, 108);
    assert(now(3, 1, 64, 64, 108));
}

static void check_fade(void)
{
    /* Mode 0: cut0 up by 32 to 255, then mode 1 and state 0. The hold and the other two
     * counters play no part. */
    run(1, 0, 1, 1, 200, 5, 6);
    assert(now(0, 1, 232, 5, 6));
    check_fade_bars(232);
    run(1, 0, 1, 0, 222, 0, 0);
    assert(now(0, 1, 254, 0, 0));
    check_fade_bars(254);
    run(1, 0, 1, 0, 223, 0, 0);
    assert(now(1, 0, 255, 0, 0));
    check_fade_bars(255);
    run(1, 0, 1, 0, 254, 0, 0);
    assert(now(1, 0, 255, 0, 0));
    run(1, 0, 1, 0, 255, 0, 0);
    assert(now(1, 0, 255, 0, 0));
    run(1, 0, 1, 0, 300, 0, 0);
    assert(now(1, 0, 255, 0, 0));
    check_fade_bars(255);

    /* Any other mode: cut0 down by 32 to 0, then mode 0 and state 0. */
    run(1, 1, 1, 0, 64, 5, 6);
    assert(now(1, 1, 32, 5, 6));
    check_fade_bars(32);
    run(1, 3, 1, 0, 33, 0, 0);
    assert(now(3, 1, 1, 0, 0));
    check_fade_bars(1);
    run(1, 1, 1, 0, 32, 0, 0);
    assert(now(0, 0, 0, 0, 0));
    check_fade_bars(0);
    run(1, 2, 1, 0, 20, 0, 0);
    assert(now(0, 0, 0, 0, 0));
    run(1, 1, 1, 0, 0, 0, 0);
    assert(now(0, 0, 0, 0, 0));
    run(1, 1, 1, 0, -5, 0, 0);
    assert(now(0, 0, 0, 0, 0));

    /* State not 1: nothing steps; the alpha is the low byte of cut0. Any non-zero fade value
     * selects this path. */
    run(1, 0, 0, 0, 200, 0, 0);
    assert(now(0, 0, 200, 0, 0));
    check_fade_bars(200);
    run(0x8000, 1, 2, 0, 64, 0, 0);
    assert(now(1, 2, 64, 0, 0));
    check_fade_bars(64);
    run(0x100, 0, 2, 0, 0x123, 0, 0);
    assert(now(0, 2, 0x123, 0, 0));
    check_fade_bars(0x23);
}

int main(void)
{
    assert(sizeof(struct DotrSinemaQuad) == 38 && sizeof(struct DotrSinemaQuad) == sizeof(struct DotrTikeiItem));
    assert(offsetof(struct DotrSinemaWork, mode) == 0x1a && offsetof(struct DotrSinemaWork, cut0) == 0x1e);
    assert(offsetof(struct DotrSinemaWork, hold) == 0x6c);
    check_cut();
    check_fade();
    puts("camp_sinema_cut: ok");
    return 0;
}
