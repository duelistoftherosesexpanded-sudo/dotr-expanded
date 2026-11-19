/* Synthetic host tests for the camp_3d_spots group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics.
 * Taku_Camp_3D's switch on the halfword at 0x1ed01b2 has no default and reads its three picks
 * uninitialised for other values, as the original does: every call here keeps that halfword
 * within 0-4 (run() asserts it). */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_3d_spots.h"

struct DotrCamp3dWork dotr_camp_3d_spots_work;
struct DotrCamp3dBoard data_00299e10_camp3d;
struct DotrCamp3dSpot data_00298ba0_camp3d[22];

#define W dotr_camp_3d_spots_work
#define BOARD data_00299e10_camp3d
#define SPOT data_00298ba0_camp3d

static char trace[96];
static int steps;
static struct {
    int n;
    int cursor[64], index[64];
    void *pos[64];
    float x[64], y[64], z[64];
} draw;
static struct {
    int n;
    int spot[32];
} effect;
static int in_use, in_use_second, in_use_calls;
static int bou_cursor, ship_cursor, ship_a, ship_b, ship_model, leader_cursor;

static void step(char c)
{
    assert(steps < 94);
    trace[steps++] = c;
    trace[steps] = 0;
}
/* Stub for SzModel_Draw_Pipe: records the cursor, the index and (when given) the position. */
int func_001ee4b0_camp3d(int cursor, int index, void *pos, int d, int e, int f, int g)
{
    assert(draw.n < 64 && d == 0 && e == 0 && f == 0 && g == 0);
    draw.cursor[draw.n] = cursor;
    draw.index[draw.n] = index;
    draw.pos[draw.n] = pos;
    if (pos != 0) {
        float v[3];

        memcpy(v, pos, sizeof v);
        draw.x[draw.n] = v[0];
        draw.y[draw.n] = v[1];
        draw.z[draw.n] = v[2];
    }
    draw.n++;
    step('d');
    return cursor + 1;
}
int SzEffect_Entry(int a, int b, int c, int d, int e)
{
    assert(a == 0 && b == 404 && d == 0 && e == 0 && effect.n < 32);
    effect.spot[effect.n++] = c;
    step('e');
    return 0;
}
int SzEffect_IsUse2(int a, int b)
{
    assert(a == -1 && b == -1);
    step('u');
    return in_use_calls++ == 0 ? in_use : in_use_second;
}
int Taku_Camp_3D_BOU(int cursor)
{
    bou_cursor = cursor;
    step('B');
    return cursor + 10;
}
int func_0022cc40_camp3d(int cursor, short a, short b, short model)
{
    ship_cursor = cursor;
    ship_a = a;
    ship_b = b;
    ship_model = model;
    step('F');
    return cursor + 100;
}
int Taku_Camp_3D_LeaderModel(int cursor)
{
    leader_cursor = cursor;
    step('L');
    return 7777;
}

/* Location i rests at (10 + i, 100 + i, 200 + i); its two models are 2 i and 2 i + 1. Nothing
 * is seen or rising; the flag chain holds; ticks 50. */
static void setup(int all, int mode)
{
    int i;

    memset(&W, 0, sizeof W);
    memset(&BOARD, 0, sizeof BOARD);
    memset(SPOT, 0, sizeof SPOT);
    for (i = 0; i < 22; i++) {
        SPOT[i].x = (float)(10 + i);
        SPOT[i].y = (float)(100 + i);
        SPOT[i].z = (float)(200 + i);
        SPOT[i].w = -1.0f;
        SPOT[i].m0 = (short)(2 * i);
        SPOT[i].m1 = (short)(2 * i + 1);
    }
    W.all = (signed char)all;
    BOARD.mode = (signed char)mode;
    BOARD.ticks = 50;
    W.byte_5481 = 1;
    W.byte_5376 = 1;
    W.byte_5482 = 0;
    W.byte_5372 = 0;
    W.pick_ok = 9;
    W.ship_large = 9;
    W.ship_state = 5;
    W.ship_case = 0;
    in_use = in_use_second = 0;
}
/* Runs the function with cursor 1000 and checks what every call has in common: the result is
 * the last callee's, the fixed draws, the cursor threaded through every callee. Returns the
 * number of location draws (after the eight fixed ones, before the two last). */
static int run(void)
{
    static const int fixed[8] = { 34, 35, 36, 37, 38, 39, 22 + 34, 23 + 34 };
    int i;

    assert(W.ship_case >= 0 && W.ship_case <= 4);   /* see the note at the top */
    memset(&draw, 0, sizeof draw);
    memset(&effect, 0, sizeof effect);
    steps = 0;
    trace[0] = 0;
    in_use_calls = 0;
    bou_cursor = ship_cursor = leader_cursor = -1;
    assert(Taku_Camp_3D(1000) == 7777);
    assert(draw.n >= 12);
    for (i = 0; i < 8; i++) {
        assert(draw.index[i] == fixed[i]);
        assert(draw.pos[i] == (i < 6 ? (void *)0 : (void *)&SPOT[11]));
    }
    for (i = 0; i < draw.n - 2; i++) {
        assert(draw.cursor[i] == 1000 + i);
    }
    assert(bou_cursor == 1000 + draw.n - 2 && ship_cursor == bou_cursor + 10);
    assert(draw.index[draw.n - 2] == 41 && draw.pos[draw.n - 2] == 0 && draw.cursor[draw.n - 2] == ship_cursor + 100);
    assert(draw.index[draw.n - 1] == 40 && draw.pos[draw.n - 1] == 0 && draw.cursor[draw.n - 1] == ship_cursor + 101);
    assert(leader_cursor == ship_cursor + 102);
    return draw.n - 10;
}
/* Draw pair k (0 = the first location pair) is location `spot` at (x, y, z); all pairs point
 * into one array of 16-byte positions. */
static void check_pair(int k, int spot, float x, float y, float z)
{
    int i = 8 + 2 * k;

    assert(draw.index[i] == 2 * spot + 34 && draw.index[i + 1] == 2 * spot + 35);
    assert(draw.pos[i] == draw.pos[i + 1]);
    assert((char *)draw.pos[i] - (char *)draw.pos[8] == 16 * spot);
    assert(draw.x[i] == x && draw.y[i] == y && draw.z[i] == z);
    assert(draw.x[i + 1] == x && draw.y[i + 1] == y && draw.z[i + 1] == z);
}
/* Pair index of a location when everything is drawn (0, 1-10, 12-21). */
static int pair_of(int spot)
{
    assert(spot != 11);
    return spot < 11 ? spot : spot - 1;
}
static void check_spot(int spot, float dx, float dy)
{
    check_pair(pair_of(spot), spot, (float)(10 + spot) + dx, (float)(100 + spot) + dy, (float)(200 + spot));
}

/* The call order: eight fixed draws, `mid` (effects, the in-use question), `pairs` location
 * pairs, then the pole, the ship, two draws and the leader model. */
static int trace_is(const char *mid, int pairs)
{
    char want[96];
    int i, k = 0;

    for (i = 0; i < 8; i++) {
        want[k++] = 'd';
    }
    for (i = 0; mid[i] != 0; i++) {
        want[k++] = mid[i];
    }
    for (i = 0; i < 2 * pairs; i++) {
        want[k++] = 'd';
    }
    memcpy(want + k, "BFddL", 6);
    return strcmp(trace, want) == 0;
}

static void check_positions(void)
{
    int i;

    /* Nothing seen, nothing rising: every location is sunk by 12, location 16 by 18. */
    setup(1, 0);
    assert(run() == 42);
    for (i = 0; i < 22; i++) {
        if (i != 11) {
            check_spot(i, 0.0f, i == 16 ? -18.0f : -12.0f);
        }
        assert(BOARD.seen[i] == 0 && W.rise[i] == 0 && W.count[i] == 0 && W.started[i] == 0);
    }
    assert(effect.n == 0 && W.ship_state == 5);
    assert(trace_is("", 21));

    /* A mix, mode 0. */
    setup(1, 0);
    BOARD.seen[3] = 1;                                  /* seen: at rest */
    BOARD.seen[2] = 1; W.rise[2] = 1; W.count[2] = 8;   /* seen and still counting: at rest, no effect */
    W.rise[5] = 1; W.count[5] = 64;                     /* rising, effect not started yet */
    W.rise[6] = 1; W.count[6] = 4; W.started[6] = 1;    /* rising, odd quarter step, effect started */
    W.rise[7] = 1;                                      /* rising from count 0: no effect yet */
    W.rise[11] = 1; W.count[11] = 8;                    /* location 11 never starts the effect */
    W.rise[16] = 1; W.count[16] = 128;                  /* the deeper one */
    W.rise[9] = 3; W.count[9] = 248;                    /* one step short of the end */
    W.rise[22] = 1; W.count[22] = 8;                    /* beyond the 22 locations: untouched */
    assert(run() == 42);
    check_spot(3, 0.0f, 0.0f);
    check_spot(2, 0.0f, 0.0f);
    check_spot(4, 0.0f, -12.0f);
    check_spot(5, 0.0f, -12.0f + 3.0f);
    check_spot(6, 0.4f * 1.0f, -12.0f + 12.0f * 4.0f / 256.0f);
    check_spot(7, 0.0f, -12.0f);
    check_spot(16, 0.0f, -18.0f + 9.0f);
    check_spot(9, 0.0f, -12.0f + 12.0f * 248.0f / 256.0f);
    assert(effect.n == 3 && effect.spot[0] == 5 && effect.spot[1] == 9 && effect.spot[2] == 16);
    assert(W.started[5] == 1 && W.started[6] == 1 && W.started[7] == 0 && W.started[9] == 1 && W.started[11] == 0 && W.started[16] == 1);
    assert(W.started[2] == 0 && W.started[22] == 0);
    assert(W.count[2] == 12 && W.count[5] == 68 && W.count[6] == 8 && W.count[7] == 4 && W.count[11] == 12 && W.count[16] == 132);
    assert(W.count[9] == 252 && W.rise[9] == 3 && BOARD.seen[9] == 0);
    assert(W.count[22] == 8 && W.rise[22] == 1 && BOARD.seen[22] == 0);
    assert(W.ship_state == 5 && W.pick_ok == 1);
    /* the effects come while the positions are built, before any location is drawn */
    assert(trace_is("eee", 21));

    /* The counter reaching 256: cleared with the rise flag, the location marked seen and the
     * ship state cleared. The position of this frame is still the rising one. */
    setup(1, 0);
    W.rise[8] = 1; W.count[8] = 252; W.started[8] = 1;
    assert(run() == 42);
    check_spot(8, 0.4f * 1.0f, -12.0f + 12.0f * 252.0f / 256.0f);
    assert(W.count[8] == 0 && W.rise[8] == 0 && BOARD.seen[8] == 1 && W.ship_state == 0 && W.started[8] == 1);
    assert(effect.n == 0);
    /* above 252 as well */
    setup(1, 0);
    W.rise[8] = 1; W.count[8] = 300; W.started[8] = 1;
    run();
    assert(W.count[8] == 0 && W.rise[8] == 0 && BOARD.seen[8] == 1 && W.ship_state == 0);
    /* a seen location that still has its rise flag also finishes */
    setup(1, 0);
    BOARD.seen[2] = 1; W.rise[2] = 1; W.count[2] = 252;
    run();
    check_spot(2, 0.0f, 0.0f);
    assert(W.count[2] == 0 && W.rise[2] == 0 && BOARD.seen[2] == 1 && W.ship_state == 0);

    /* Mode 0: location 16 counts with any non-zero rise flag. */
    setup(1, 0);
    W.rise[16] = 1; W.count[16] = 252; W.started[16] = 1;
    run();
    assert(W.count[16] == 0 && W.rise[16] == 0 && BOARD.seen[16] == 1 && W.ship_state == 0);

    /* Mode not 0: location 16 counts only with rise flag 2 (it is still drawn rising and
     * still starts its effect); the others as in mode 0. */
    setup(1, 1);
    W.rise[16] = 1; W.count[16] = 128;
    W.rise[5] = 1; W.count[5] = 64;
    W.rise[15] = 2; W.count[15] = 252;
    run();
    check_spot(16, 0.0f, -18.0f + 9.0f);
    assert(effect.n == 3 && effect.spot[0] == 5 && effect.spot[1] == 15 && effect.spot[2] == 16);
    assert(W.count[16] == 128 && W.rise[16] == 1 && W.count[5] == 68);
    assert(W.count[15] == 0 && W.rise[15] == 0 && BOARD.seen[15] == 1 && W.ship_state == 0);
    setup(1, 2);
    W.rise[16] = 3; W.count[16] = 252;
    run();
    assert(W.count[16] == 252 && W.rise[16] == 3 && BOARD.seen[16] == 0 && W.ship_state == 5);
    setup(1, 1);
    W.rise[16] = 2; W.count[16] = 128;
    run();
    assert(W.count[16] == 132 && W.rise[16] == 2 && BOARD.seen[16] == 0 && W.ship_state == 5);
    setup(1, 1);
    W.rise[16] = 2; W.count[16] = 252;
    run();
    assert(W.count[16] == 0 && W.rise[16] == 0 && BOARD.seen[16] == 1 && W.ship_state == 0);
}

/* The byte at 0x1ed006e for a given tick count; `calls` is how often SzEffect_IsUse2 ran. */
static int pick(int mode, int ticks, int calls)
{
    BOARD.mode = (signed char)mode;
    BOARD.ticks = (short)ticks;
    W.pick_ok = 9;
    run();
    assert(in_use_calls == calls);
    return W.pick_ok;
}

static void check_pick_ok(void)
{
    /* The byte at 0x1ed018c wins over everything, without asking SzEffect_IsUse2. */
    setup(1, 0);
    W.byte_5481 = 0;
    assert(pick(0, 10000, 0) == 1);
    assert(pick(1, 100, 0) == 1);

    /* ticks >= 10000. */
    setup(0, 0);
    assert(pick(0, 10000, 0) == 0);
    assert(pick(0, 10001, 0) == 0);
    assert(pick(1, 32767, 0) == 0);
    assert(pick(0, 9999, 1) == 1);

    /* Nothing rising and the chain holds: mode 0 refuses ticks 100 and 101, other modes 104,
     * 105 and 111. */
    setup(0, 0);
    assert(pick(0, 99, 1) == 1);
    assert(pick(0, 100, 1) == 0);
    assert(pick(0, 101, 1) == 0);
    assert(pick(0, 102, 1) == 1);
    assert(pick(0, 104, 1) == 1);
    assert(pick(0, 105, 1) == 1);
    assert(pick(0, 111, 1) == 1);
    assert(pick(0, 0, 1) == 1);
    assert(pick(1, 100, 1) == 1);
    assert(pick(1, 101, 1) == 1);
    assert(pick(1, 103, 1) == 1);
    assert(pick(1, 104, 1) == 0);
    assert(pick(1, 105, 1) == 0);
    assert(pick(1, 106, 1) == 1);
    assert(pick(1, 110, 1) == 1);
    assert(pick(1, 111, 1) == 0);
    assert(pick(1, 112, 1) == 1);
    assert(pick(2, 111, 1) == 0);
    assert(pick(1, 0, 1) == 1);
    /* the byte at 0x1ed5372 refuses in both */
    W.byte_5372 = 1;
    assert(pick(0, 50, 1) == 0);
    assert(pick(1, 50, 1) == 0);
    assert(pick(0, 0, 1) == 0);

    /* Each link of the chain failing, with ticks 50 and with ticks 0 (the second chain is the
     * same one, so it fails as well; SzEffect_IsUse2 is asked again only when it was reached). */
    setup(0, 0);
    W.byte_5481 = 0;
    assert(pick(0, 50, 0) == 0);
    assert(pick(0, 0, 0) == 0);
    setup(0, 0);
    in_use = in_use_second = 1;
    assert(pick(0, 50, 1) == 0);
    assert(pick(0, 0, 2) == 0);
    setup(0, 0);
    W.byte_5376 = 0;
    assert(pick(0, 50, 1) == 0);
    assert(pick(0, 0, 2) == 0);
    setup(0, 0);
    W.byte_5482 = 1;
    assert(pick(0, 50, 1) == 0);
    assert(pick(0, 0, 2) == 0);
    /* in use at the first question, free at the second: the ticks == 0 arm accepts */
    setup(0, 0);
    in_use = 1;
    in_use_second = 0;
    assert(pick(0, 0, 2) == 1);
    assert(pick(0, 50, 1) == 0);

    /* Something rising: only ticks == 0 with the chain accepts. (count 0 and a rise flag that
     * the mode does not step for location 16 keep the fixture rising across calls.) */
    setup(0, 1);
    W.rise[16] = 1;
    assert(pick(1, 50, 0) == 0);
    assert(pick(1, 100, 0) == 0);
    assert(pick(1, 0, 1) == 1);
    assert(pick(1, 10000, 0) == 0);
    W.byte_5372 = 1;                    /* not looked at in this arm */
    assert(pick(1, 0, 1) == 1);
    W.byte_5481 = 0;
    assert(pick(1, 0, 0) == 0);
    W.byte_5481 = 1;
    in_use = 1;
    assert(pick(1, 0, 1) == 0);
    in_use = 0;
    W.byte_5376 = 0;
    assert(pick(1, 0, 1) == 0);
    W.byte_5376 = 1;
    W.byte_5482 = 1;
    assert(pick(1, 0, 1) == 0);
    W.byte_5482 = 0;
    assert(pick(1, 0, 1) == 1);
    assert(W.rise[16] == 1 && W.count[16] == 0);
    /* a location that is seen does not count as rising, even with its rise flag */
    setup(0, 1);
    W.rise[16] = 1;
    BOARD.seen[16] = 1;
    assert(pick(1, 50, 1) == 1);
}

static void check_selection(void)
{
    int i;

    /* 0x1ed018c set: location 0, then 1-10, then 12-21, whatever the mode. */
    setup(1, 7);
    assert(run() == 42);
    for (i = 0; i < 22; i++) {
        if (i != 11) {
            check_spot(i, 0.0f, i == 16 ? -18.0f : -12.0f);
        }
    }
    /* Mode 0: location 0 and 1-10. */
    setup(0, 0);
    assert(run() == 22);
    for (i = 0; i <= 10; i++) {
        check_pair(i, i, (float)(10 + i), (float)(88 + i), (float)(200 + i));
    }
    assert(trace_is("u", 11));
    /* Mode 1: location 0 and 12-21. */
    setup(0, 1);
    assert(run() == 22);
    check_pair(0, 0, 10.0f, 88.0f, 200.0f);
    for (i = 12; i <= 21; i++) {
        check_pair(i - 11, i, (float)(10 + i), (float)(i == 16 ? 82 + i : 88 + i), (float)(200 + i));
    }
    /* Any other mode: location 0 only. */
    setup(0, 2);
    assert(run() == 2);
    check_pair(0, 0, 10.0f, 88.0f, 200.0f);
    assert(trace_is("u", 1));
    setup(0, -1);
    assert(run() == 2);
}

static void check_ship(void)
{
    /* Switch values 0-4 only: the function has no default (see the note at the top). */
    static const int want[5][4] = {
        { 0, 15, 27, 0 }, { 0, 0, 27, 0 }, { 1, 4, 30, 1 }, { 1, 5, 30, 1 }, { 1, 11, 27, 0 },
    };
    int i;

    for (i = 0; i < 5; i++) {
        setup(0, 2);
        W.ship_case = (short)i;
        run();
        assert(ship_a == want[i][0] && ship_b == want[i][1] && ship_model == want[i][2]);
        assert(W.ship_large == want[i][3] && W.ship_case == i);
    }
}

int main(void)
{
    assert(sizeof(struct DotrCamp3dSpot) == 52 && sizeof(struct DotrCamp3dVec) == 16);
    assert(offsetof(struct DotrCamp3dBoard, seen) == 3 && offsetof(struct DotrCamp3dBoard, ticks) == 194);
    assert(offsetof(struct DotrCamp3dWork, pick_ok) == 0x6e && offsetof(struct DotrCamp3dWork, all) == 0x18c);
    assert(offsetof(struct DotrCamp3dWork, ship_state) == 0x1a4 && offsetof(struct DotrCamp3dWork, ship_large) == 0x1ac);
    assert(offsetof(struct DotrCamp3dWork, ship_case) == 0x1b2 && offsetof(struct DotrCamp3dWork, rise) == 0x52ec);
    assert(offsetof(struct DotrCamp3dWork, count) == 0x532c && offsetof(struct DotrCamp3dWork, byte_5372) == 0x5372);
    assert(offsetof(struct DotrCamp3dWork, byte_5376) == 0x5376 && offsetof(struct DotrCamp3dWork, started) == 0x5448);
    assert(offsetof(struct DotrCamp3dWork, byte_5481) == 0x5481 && offsetof(struct DotrCamp3dWork, byte_5482) == 0x5482);
    check_positions();
    check_selection();
    check_pick_ok();
    check_ship();
    puts("camp_3d_spots: ok");
    return 0;
}
