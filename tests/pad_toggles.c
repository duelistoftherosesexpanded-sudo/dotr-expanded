/* Synthetic host tests for the pad_toggles group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/pad_toggles.h"

struct DotrStateControl data_00317490_control[1];
static int held[3], pad_seen, sound[8], sound_count, side_log[8][3], side_count;

int SzPad_Check(int unknown, int zero, int mask)
{
    pad_seen = unknown;
    assert(zero == 0 || zero == 2);
    return held[zero] & mask;
}
int SzSound_PlaySE(int index, int value) { assert(index == 0); sound[sound_count++] = value; return 0; }
int SzData_SetOption(int side, int which, int flag)
{
    side_log[side_count][0] = side; side_log[side_count][1] = which; side_log[side_count++][2] = flag;
    return 0;
}

static int run(int held2, int held0, int d35, int d36)
{
    struct DotrStateControl *c = data_00317490_control;

    held[2] = held2; held[0] = held0;
    c->unknown_d35 = (unsigned char)d35; c->unknown_d36 = (unsigned char)d36;
    sound_count = side_count = 0;
    pad_seen = 0;
    return SzDuel_ActOptionPopup(41);
}

struct DotrControlDrift data_00317490_drift;
struct DotrPadSteps gSzDuel_dt;
static int menu_calls;
void SzDebug_OutputFrameBuffer(void) { menu_calls++; }

/* Holds one mask under second argument `group` and returns the four words after
 * SzDuel_ActCardTest, starting from {0xfff0, 0x100, 0, last}. */
static int *step(int group, int mask, int last)
{
    static int out[4];
    struct DotrPadSteps *p = &gSzDuel_dt;

    memset(p, 0, sizeof(*p));
    p->unknown_5c = 0xfff0;
    p->unknown_60 = 0x100;
    p->unknown_70 = last;
    held[0] = held[1] = held[2] = 0;
    held[group] = mask;
    menu_calls = 0;
    SzDuel_ActCardTest();
    out[0] = p->unknown_5c; out[1] = p->unknown_60; out[2] = p->unknown_64; out[3] = p->unknown_70;
    return out;
}

static void check_steps(void)
{
    static const int cases[][6] = {
        /* mask, last, then the four words expected */
        {0, 853, 0xfff0, 0x100, 0, 853},
        {0x1000, 853, 0x3f0, 0x100, 0, 853},
        {0x4000, 853, 0xfbf0, 0x100, 0, 853},
        {0x2000, 853, 0xfff0, 0x500, 0, 853},
        {0x8000, 853, 0xfff0, 0xfd00, 0, 853},
        {8, 853, 0xfff0, 0x100, 0x400, 853},
        {4, 853, 0xfff0, 0x100, 0xfc00, 853},
        {2, 853, 0xfff0, 0x100, 0, 0},
        {2, 0, 0xfff0, 0x100, 0, 1},
        {1, 853, 0xfff0, 0x100, 0, 852},
        {1, 0, 0xfff0, 0x100, 0, 853},
        {16, 853, 0xfff0, 0x100, 0, 853}, /* mask 16 is asked under 0 only */
    };
    unsigned i;
    int *out;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        out = step(2, cases[i][0], cases[i][1]);
        assert(out[0] == cases[i][2] && out[1] == cases[i][3] && out[2] == cases[i][4] && out[3] == cases[i][5]);
        assert(menu_calls == 0 && pad_seen == 0);
    }
    out = step(0, 16, 5);
    assert(menu_calls == 1 && out[0] == 0xfff0 && out[1] == 0x100 && out[2] == 0 && out[3] == 5);
    out = step(0, 0xf00f, 5); /* the step masks are asked under 2 only */
    assert(menu_calls == 0 && out[0] == 0xfff0 && out[1] == 0x100 && out[2] == 0 && out[3] == 5);
}

static int movie_result, shown_result, movie_calls, shown_count, shown[4];
int SzGame_ActMemCardProc(void) { movie_calls++; return movie_result; }
int lSzDuel_ActTradeProc(int which) { shown[shown_count++] = which; return shown_result; }

/* Branch coverage. One run with the halfwords, steps and restart byte given. */
static struct DotrControlDrift *drift(int x, int y, int dx, int dy, int restart)
{
    struct DotrControlDrift *d = &data_00317490_drift;

    memset(d, 0, sizeof(*d));
    d->unknown_c86 = (unsigned short)x;
    d->unknown_c88 = (unsigned short)y;
    d->step_c86 = dx;
    d->step_c88 = dy;
    d->restart = (unsigned char)restart;
    ((unsigned char *)d)[0xd64] |= 0xa0;
    movie_calls = shown_count = 0;
    SzDuel_ActTrade();
    assert(movie_calls == 1 && (((unsigned char *)d)[0xd64] & 0xf0) == 0xa0);
    return d;
}

static void check_drift(void)
{
    struct DotrControlDrift *d;

    /* Inside the range: both halfwords move, steps kept; a non-negative first callee stops there. */
    movie_result = 0;
    shown_result = 0;
    d = drift(0x1000, 0x2000, 1, -1, 0);
    assert(d->unknown_c86 == 0x1200 && d->unknown_c88 == 0x1800 && d->step_c86 == 1 && d->step_c88 == -1);
    assert(shown_count == 0);

    /* Upper end: the sum is cut to 0x8000 and the step negated; 0x7fff stays below it. */
    d = drift(0x7f00, 0x7800, 1, 1, 0);
    assert(d->unknown_c86 == 0x8000 && d->unknown_c88 == 0x8000 && d->step_c86 == -1 && d->step_c88 == -1);
    d = drift(0x7dff, 0x77ff, 1, 1, 0);
    assert(d->unknown_c86 == 0x7fff && d->unknown_c88 == 0x7fff && d->step_c86 == 1 && d->step_c88 == 1);

    /* Lower end: a sum of 0 or less becomes 0 and the step is negated; 1 stays. */
    d = drift(0x100, 0x400, -1, -1, 0);
    assert(d->unknown_c86 == 0 && d->unknown_c88 == 0 && d->step_c86 == 1 && d->step_c88 == 1);
    d = drift(0x200, 0x800, -1, -1, 0);
    assert(d->unknown_c86 == 0 && d->unknown_c88 == 0 && d->step_c86 == 1 && d->step_c88 == 1);
    d = drift(0x201, 0x801, -1, -1, 0);
    assert(d->unknown_c86 == 1 && d->unknown_c88 == 1 && d->step_c86 == -1 && d->step_c88 == -1);

    /* A zero step at 0 negates to zero; the restart byte forces the first step to 1. */
    d = drift(0, 0, 0, 0, 0);
    assert(d->unknown_c86 == 0 && d->unknown_c88 == 0 && d->step_c86 == 0 && d->step_c88 == 0);
    d = drift(0x1000, 0x1000, -1, 0, 1);
    assert(d->unknown_c86 == 0x1200 && d->unknown_c88 == 0x1000 && d->step_c86 == 1 && d->restart == 1);

    /* Negative first callee: lSzDuel_ActTradeProc(0), then lSzDuel_ActTradeProc(1) only when that
     * returned 0. */
    movie_result = -1;
    drift(0x1000, 0x1000, 0, 0, 0);
    assert(shown_count == 2 && shown[0] == 0 && shown[1] == 1);
    shown_result = 1;
    drift(0x1000, 0x1000, 0, 0, 0);
    assert(shown_count == 1 && shown[0] == 0);
}

int main(void)
{
    struct DotrStateControl *c = data_00317490_control;
    int i;

    /* First gate (mask 0x5000, second argument 2): bit 0x1000 adds 1, else 3, modulo 2;
     * the byte always changes from 0 or 1, so sound 3 follows. Later gates are not asked. */
    assert(run(0x1000, 0xa0e0, 0, 0) == 0 && c->unknown_d35 == 1 && sound_count == 1 && sound[0] == 3);
    assert(pad_seen == 41 && c->unknown_d36 == 0 && side_count == 0);
    assert(run(0x4000, 0, 1, 0) == 0 && c->unknown_d35 == 0 && sound_count == 1 && sound[0] == 3);
    assert(run(0x4000, 0, 0, 0) == 0 && c->unknown_d35 == 1);

    /* Second gate (mask 0xa000): bit 0x8000 sets bit d35 + 1, else clears it;
     * sound 3 only when the byte changed. */
    assert(run(0, 0x8000, 1, 1) == 0 && c->unknown_d36 == 5 && sound_count == 1 && sound[0] == 3);
    assert(run(0, 0x8000, 1, 5) == 0 && c->unknown_d36 == 5 && sound_count == 0);
    assert(run(0, 0x2000, 0, 7) == 0 && c->unknown_d36 == 5 && sound_count == 1);
    assert(run(0, 0x2000, 0, 5) == 0 && c->unknown_d36 == 5 && sound_count == 0 && c->unknown_d35 == 0);

    /* Third gate (mask 0xc0): both sides get each of bits 0..2 of the byte, sound 4, 1. */
    assert(run(0, 0x40, 0, 5) == 1 && side_count == 6 && sound_count == 1 && sound[0] == 4);
    for (i = 0; i < 3; i++) {
        assert(side_log[2 * i][0] == 0 && side_log[2 * i][1] == i && side_log[2 * i][2] == (5 & (1 << i)));
        assert(side_log[2 * i + 1][0] == 1 && side_log[2 * i + 1][1] == i && side_log[2 * i + 1][2] == (5 & (1 << i)));
    }
    assert(run(0, 0x80, 0, 0) == 1 && side_count == 6);

    /* Fourth gate (mask 0x20): sound 5, 2. Nothing held: 0 and nothing happens. */
    assert(run(0, 0x20, 1, 3) == 2 && sound_count == 1 && sound[0] == 5 && side_count == 0);
    assert(run(0, 0, 1, 3) == 0 && sound_count == 0 && side_count == 0 && c->unknown_d35 == 1 && c->unknown_d36 == 3);
    check_steps();
    check_drift();
    return 0;
}
