/* Synthetic host tests for the camp_leader_model group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_leader_model.h"

#define PI_F 3.1415927f
#define TAIL "DRSPOAF"

struct DotrLeaderModelWork dotr_camp_leader_model_work;
struct DotrLeaderModelSpot data_00298ba0_leadermodel[22];
struct DotrLeaderModelBoard data_00299e10_leadermodel;
signed char data_00299a30_leadermodel[16];

static struct DotrLeaderModelWork *const work = &dotr_camp_leader_model_work;

static char trace[24];
static int n;
static float atan_args[4][2];
static float atan_results[4];
static int atan_calls;
static float conv_args[4];
static int conv_results[4];
static int conv_calls;
static struct DotrLeaderModelVec screen_in, screen_out, view_point, hit;
static float screen_x, screen_y;
static struct DotrLeaderModelRot rot;
static int alpha;

static void step(char c)
{
    assert(n < 22);
    trace[n++] = c;
    trace[n] = 0;
}
/* The library routines 0x1a6c18 and 0x1a22c0 are not part of this project: the stubs hand out
 * scripted results and record what they were asked. */
float func_001a6c18(float a, float b)
{
    assert(atan_calls < 4);
    atan_args[atan_calls][0] = a;
    atan_args[atan_calls][1] = b;
    step('a');
    return atan_results[atan_calls++];
}
int func_001a22c0(float value)
{
    assert(conv_calls < 4);
    conv_args[conv_calls] = value;
    step('c');
    return conv_results[conv_calls++];
}
int SzData_GetCurrentDeck(int side)
{
    assert(side == 0);
    step('k');
    return 0x10005;     /* only the low halfword may reach SzData_GetLeader */
}
int SzData_GetLeader(int side, int deck)
{
    assert(side == 0 && deck == 5);
    step('l');
    return 7;
}
void func_00205cb0_leadermodel(struct DotrLeaderModelVec *out, float x, float y)
{
    screen_in = *out;
    screen_x = x;
    screen_y = y;
    *out = screen_out;
    step('g');
}
void *SzView_GetSP(int index)
{
    assert(index == 0);
    step('s');
    return &view_point;
}
void func_00206430_leadermodel(float *plane, struct DotrLeaderModelVec *p1, struct DotrLeaderModelVec *p2,
                               struct DotrLeaderModelVec *out)
{
    assert(plane == work->plane);
    assert(memcmp(p1, &screen_out, sizeof *p1) == 0);
    assert(memcmp(p2, &view_point, sizeof *p2) == 0);   /* the point built on the stack is replaced */
    *out = hit;
    step('i');
}
int SzModel_SetDeform(int index, int value)
{
    assert(index == 30 && value == 1);
    step('D');
    return 0;
}
int SzModel_SetDeformRatio(int index, int value)
{
    assert(index == 30 && value == 32);
    step('R');
    return 0;
}
int func_001f25d0_leadermodel(int index, struct DotrLeaderModelScale *in)
{
    assert(index == 30 && in->x == 0.5f && in->y == 0.5f && in->z == 0.5f);
    step('S');
    return 0;
}
int func_001f22b0_leadermodel(int index, float *in)
{
    assert(index == 30 && in == work->pos);
    step('P');
    return 0;
}
int func_001f24c0_leadermodel(int index, struct DotrLeaderModelRot *in)
{
    assert(index == 30);
    rot.x = in->x;
    rot.y = in->y;
    rot.z = in->z;
    step('O');
    return 0;
}
int SzModel_SetAlpha(int index, int value)
{
    assert(index == 30);
    assert(work->angle == rot.y - 32768);   /* the stored angle is still there */
    alpha = value;
    step('A');
    return 0;
}
int SzModel_SetDrawFlag(int index, int value)
{
    assert(index == 30 && value == 1);
    assert(work->angle == 0);               /* and cleared before the last call */
    step('F');
    return 0;
}

/* Route [0][b]: eight points, of which `count` are announced. Segment k has dx = 10 * (k + 1)
 * and dz = 4 * (k + 1); column b is told apart by its offset. Location i is at
 * (64 i, i + 0.25, 32 i + 8). The leader's height byte is 3. */
static const float xs[8] = { 0.0f, 10.0f, 30.0f, 60.0f, 100.0f, 150.0f, 210.0f, 280.0f };
static const float zs[8] = { 0.0f, 4.0f, 12.0f, 24.0f, 40.0f, 60.0f, 84.0f, 112.0f };
#define BX(b) (1000.0f * (float)(b))
#define BZ(b) (2000.0f * (float)(b))

static void setup(int count, int idx, int t, int state, int show)
{
    int i, j, k;

    memset(work, 0, sizeof *work);
    for (j = 0; j < 16; j++) {
        for (k = 0; k < 8; k++) {
            work->route[0][j].x[k] = BX(j) + xs[k];
            work->route[0][j].z[k] = BZ(j) + zs[k];
            work->route[1][j].x[k] = -999.0f;   /* row 1 is never used */
            work->route[1][j].z[k] = -999.0f;
        }
        work->route[0][j].n = (short)count;
        work->route[1][j].n = 1;
    }
    for (i = 0; i < 22; i++) {
        memset(&data_00298ba0_leadermodel[i], 0x55, sizeof data_00298ba0_leadermodel[i]);
        data_00298ba0_leadermodel[i].x = 64.0f * (float)i;
        data_00298ba0_leadermodel[i].y = (float)i + 0.25f;
        data_00298ba0_leadermodel[i].z = 32.0f * (float)i + 8.0f;
        data_00298ba0_leadermodel[i].w = -1.0f;
    }
    memset(data_00299a30_leadermodel, 99, sizeof data_00299a30_leadermodel);
    data_00299a30_leadermodel[7] = 3;
    work->idx = (short)idx;
    work->t = (short)t;
    work->state = (short)state;
    work->show = (signed char)show;
    work->angle = 5;
    work->alpha = 200;
    work->mx = 400;
    work->my = 300;
    work->pos[0] = work->pos[1] = work->pos[2] = -7.0f;
    memset(atan_results, 0, sizeof atan_results);
    conv_results[0] = 1000;
    conv_results[1] = 2000;
    conv_results[2] = conv_results[3] = 0;
    screen_out.x = 1.0f;
    screen_out.y = 2.0f;
    screen_out.z = 3.0f;
    screen_out.w = 4.0f;
    view_point.x = 9.0f;
    view_point.y = 8.0f;
    view_point.z = 7.0f;
    view_point.w = 6.0f;
    hit.x = 164.0f;
    hit.y = 9.0f;
    hit.z = 240.0f;
    hit.w = 0.0f;
}
/* One call for the pair (from, to); checks what every call does. */
static void run(int from, int to, const char *head)
{
    char expected[24];

    memset(&data_00299e10_leadermodel, 0x11, sizeof data_00299e10_leadermodel);
    data_00299e10_leadermodel.from = (signed char)from;
    data_00299e10_leadermodel.to = (signed char)to;
    n = 0;
    trace[0] = 0;
    atan_calls = conv_calls = 0;
    alpha = -1;
    rot.x = rot.y = rot.z = -1;
    assert(Taku_Camp_3D_LeaderModel(0x12345) == 0x12345);
    strcpy(expected, head);
    strcat(expected, TAIL);
    if (strcmp(trace, expected) != 0) {
        printf("from %d to %d: trace %s, expected %s\n", from, to, trace, expected);
        assert(0);
    }
    assert(rot.x == 0 && rot.z == 0 && alpha == 200 && work->angle == 0);
    assert(data_00299e10_leadermodel.from == from && data_00299e10_leadermodel.to == to);
}
static int at(float x, float y, float z)
{
    return work->pos[0] == x && work->pos[1] == y && work->pos[2] == z;
}
static int asked(int call, float a, float b)
{
    return atan_args[call][0] == a && atan_args[call][1] == b;
}
static int left(int state, int t, int idx)
{
    return work->state == state && work->t == t && work->idx == idx;
}

struct pair {
    signed char from, to, b, rev;
};
static const struct pair pairs[15] = {
    { 1, 2, 1, 0 }, { 1, 8, 2, 0 }, { 2, 3, 3, 0 }, { 2, 5, 4, 0 }, { 2, 7, 12, 0 },
    { 3, 2, 3, 1 }, { 5, 2, 4, 1 }, { 5, 6, 10, 1 }, { 5, 7, 13, 0 }, { 6, 5, 10, 0 },
    { 7, 11, 14, 0 }, { 8, 6, 5, 0 }, { 8, 7, 11, 0 }, { 9, 8, 6, 1 }, { 11, 7, 14, 1 },
};
static int has_route(int from, int to)
{
    int i;

    for (i = 0; i < 15; i++) {
        if (pairs[i].from == from && pairs[i].to == to) {
            return 1;
        }
    }
    return 0;
}

static void check_layout(void)
{
    assert(sizeof(struct DotrLeaderModelRoute) == 516 && sizeof(struct DotrLeaderModelSpot) == 52);
    assert(offsetof(struct DotrLeaderModelWork, mx) == 0 && offsetof(struct DotrLeaderModelWork, my) == 2);
    assert(offsetof(struct DotrLeaderModelWork, plane) == 0x190);
    assert(offsetof(struct DotrLeaderModelWork, state) == 472 && offsetof(struct DotrLeaderModelWork, t) == 474);
    assert(offsetof(struct DotrLeaderModelWork, idx) == 478 && offsetof(struct DotrLeaderModelWork, pos) == 496);
    assert(offsetof(struct DotrLeaderModelWork, angle) == 512 && offsetof(struct DotrLeaderModelWork, route) == 0x22c);
    assert(offsetof(struct DotrLeaderModelWork, show) == 21363 && offsetof(struct DotrLeaderModelWork, alpha) == 21628);
    assert(offsetof(struct DotrLeaderModelBoard, from) == 199 && offsetof(struct DotrLeaderModelBoard, to) == 200);
    assert(sizeof(struct DotrLeaderModelVec) == 16 && sizeof(struct DotrLeaderModelRot) == 16);
    assert(sizeof(struct DotrLeaderModelScale) == 16);
}

/* Every (from, to) pair of 0..12 x 0..12: the fifteen listed pairs use their column and
 * direction, every other pair goes in a straight line. Mid-route with t = 0 the forward
 * position is point 1 and the reverse position point 2 of the column. */
static void check_pairs(void)
{
    int from, to, i;

    for (i = 0; i < 15; i++) {
        const struct pair *p = &pairs[i];

        setup(5, 1, 0, 2, 1);
        run(p->from, p->to, "klaacac");
        if (p->rev) {
            assert(at(BX(p->b) + 30.0f, 3.25f, BZ(p->b) + 12.0f));
            assert(asked(1, -20.0f, -8.0f));
        } else {
            assert(at(BX(p->b) + 10.0f, 3.25f, BZ(p->b) + 4.0f));
            assert(asked(1, 20.0f, 8.0f));
        }
        assert(left(2, 256, 1));
    }
    for (from = 0; from <= 12; from++) {
        for (to = 0; to <= 12; to++) {
            if (has_route(from, to)) {
                continue;
            }
            setup(5, 1, 0, 2, 1);
            run(from, to, "klac");
            assert(asked(0, 64.0f * (float)(to - from), 32.0f * (float)(to - from)));
            assert(at(64.0f * (float)from, 3.25f + (float)from, 32.0f * (float)from + 8.0f));
            if (from == to) {
                assert(left(3, 0, 0));
            } else {
                assert(left(2, 16, 1));
            }
        }
    }
}

static void check_forward(void)
{
    /* Mid-route (idx < n - 1): the heading is the blend of this segment and the next by
     * t / 1024; the conversion's result wraps by % 65536 and half a turn is added for the
     * model. The first 0x1a6c18 result (next segment) is discarded. */
    setup(5, 1, 256, 2, 1);
    atan_results[0] = 77.0f;
    atan_results[1] = PI_F;
    atan_results[2] = -PI_F / 2.0f;
    conv_results[1] = 65536 + 100;
    run(1, 2, "klaacac");
    assert(asked(0, 30.0f, 12.0f) && asked(1, 20.0f, 8.0f) && asked(2, 22.5f, 9.0f));
    assert(conv_args[0] == 32768.0f && conv_args[1] == -16384.0f);
    assert(at(1015.0f, 3.25f, 2006.0f));
    assert(rot.y == 100 + 32768);
    assert(left(2, 512, 1));

    /* Last segment (idx == n - 1): this segment's angle is kept, here negative (% keeps the
     * sign). t = 768: the step reaches 1024 and idx reaches n: arrived. */
    setup(5, 4, 768, 2, 1);
    conv_results[0] = -70000;
    run(1, 8, "klaac");
    assert(asked(0, 60.0f, 24.0f) && asked(1, 50.0f, 20.0f));
    assert(at(2137.5f, 3.25f, 4055.0f));
    assert(rot.y == -4464 + 32768);
    assert(left(3, 0, 5));

    /* At the end (idx == n): the previous segment's angle from a third call; nothing steps.
     * State 1 is not special in the forward direction. */
    setup(5, 5, 0, 1, 1);
    conv_results[1] = -5;
    run(2, 3, "klaacac");
    assert(asked(0, 70.0f, 28.0f) && asked(1, 60.0f, 24.0f) && asked(2, 50.0f, 20.0f));
    assert(at(3150.0f, 3.25f, 6060.0f));
    assert(rot.y == 32763);
    assert(left(1, 0, 5));

    /* idx beyond n takes the same branch */
    setup(4, 5, 0, 2, 1);
    run(2, 3, "klaacac");
    assert(asked(2, 50.0f, 20.0f) && left(2, 0, 5));

    /* State 3: the position is the "to" location with the height byte added; with t = 0 the
     * blend is this segment alone; the forward step is still taken (no state test). */
    setup(5, 1, 0, 3, 1);
    run(7, 11, "klaacac");
    assert(asked(2, 20.0f, 8.0f));
    assert(at(704.0f, 14.25f, 360.0f));
    assert(rot.y == 2000 + 32768);
    assert(left(3, 256, 1));

    /* t == 1024 still moves the model (to the next point) but does not step; the blend is the
     * next segment alone. */
    setup(5, 1, 1024, 2, 1);
    run(8, 6, "klaacac");
    assert(asked(2, 30.0f, 12.0f));
    assert(at(5030.0f, 3.25f, 10012.0f));
    assert(left(2, 1024, 1));

    /* The step reaching 1024 mid-route: next point, t cleared, still moving. */
    setup(5, 1, 768, 2, 1);
    run(2, 5, "klaacac");
    assert(at(4025.0f, 3.25f, 8010.0f));
    assert(left(2, 0, 2));
}

static void check_reverse(void)
{
    /* State 1 starts at the last segment (idx = n - 1) and becomes 2; the segments run
     * backwards; idx == n - 1 keeps this segment's angle. */
    setup(5, 0, 0, 1, 1);
    conv_results[0] = 65536 * 2 + 9;
    run(3, 2, "klaac");
    assert(asked(0, -60.0f, -24.0f) && asked(1, -50.0f, -20.0f));
    assert(at(3150.0f, 3.25f, 6060.0f));
    assert(rot.y == 9 + 32768);
    assert(left(2, 256, 4));

    /* Mid-route: position = point idx + 1 plus the backward segment * t / 1024; the blend
     * weighs the other segment by 1024 - t and this one by t. */
    setup(5, 1, 256, 2, 1);
    run(5, 2, "klaacac");
    assert(asked(0, -30.0f, -12.0f) && asked(1, -20.0f, -8.0f) && asked(2, -27.5f, -11.0f));
    assert(at(4025.0f, 3.25f, 8010.0f));
    assert(rot.y == 2000 + 32768);
    assert(left(2, 512, 1));

    /* The step reaching 1024 at idx 0: idx would become -1: arrived, idx = 0. */
    setup(5, 0, 768, 2, 1);
    run(9, 8, "klaacac");
    assert(asked(0, -20.0f, -8.0f) && asked(1, -10.0f, -4.0f) && asked(2, -12.5f, -5.0f));
    assert(at(6002.5f, 3.25f, 12001.0f));
    assert(left(3, 0, 0));

    /* The step reaching 1024 further up: one point back, still moving. */
    setup(5, 2, 768, 2, 1);
    run(5, 2, "klaacac");
    assert(left(2, 0, 1));

    /* idx == n: the third angle is that of point idx - point idx - 1 (not reversed). */
    setup(5, 5, 0, 2, 1);
    run(11, 7, "klaacac");
    assert(asked(0, -70.0f, -28.0f) && asked(1, -60.0f, -24.0f) && asked(2, 50.0f, 20.0f));
    assert(at(14210.0f, 3.25f, 28084.0f));
    assert(left(2, 256, 5));

    /* State 3: at the "to" location, and unlike the forward direction nothing steps. */
    setup(5, 0, 0, 3, 1);
    run(5, 6, "klaacac");
    assert(asked(2, -20.0f, -8.0f));
    assert(at(384.0f, 9.25f, 200.0f));
    assert(left(3, 0, 0));

    /* t == 1024: placed at point idx, no step. */
    setup(5, 1, 1024, 2, 1);
    run(5, 2, "klaacac");
    assert(at(4010.0f, 3.25f, 8004.0f));
    assert(left(2, 1024, 1));
}

static void check_straight(void)
{
    /* No route for 4 > 10: from the "from" location towards "to" by t / 1024, 16 per call. */
    setup(5, 9, 512, 1, 1);
    run(4, 10, "klac");
    assert(asked(0, 384.0f, 192.0f));
    assert(at(448.0f, 7.25f, 232.0f));
    assert(rot.y == 1000 + 32768);
    assert(left(1, 528, 9));

    /* t reaching 1024: arrived, idx cleared and the position replaced by "to". */
    setup(5, 9, 1008, 1, 1);
    run(4, 10, "klac");
    assert(at(640.0f, 13.25f, 328.0f));
    assert(left(3, 0, 0));
    /* one step earlier it does not wrap */
    setup(5, 9, 1007, 1, 1);
    run(4, 10, "klac");
    assert(left(1, 1023, 9));
    /* t == 1024 on entry is still inside the gate and wraps as well */
    setup(5, 9, 1024, 2, 1);
    run(4, 10, "klac");
    assert(at(640.0f, 13.25f, 328.0f) && left(3, 0, 0));

    /* State 3: stands at "to"; t still counts. */
    setup(5, 9, 0, 3, 1);
    run(4, 10, "klac");
    assert(at(640.0f, 13.25f, 328.0f));
    assert(left(3, 16, 9));

    /* from == to: arrived at once. */
    setup(5, 9, 100, 1, 1);
    run(2, 2, "klac");
    assert(asked(0, 0.0f, 0.0f));
    assert(at(128.0f, 5.25f, 72.0f));
    assert(left(3, 0, 0));

    /* A "from" value above the switch's range (the table has 12 entries). */
    setup(5, 9, 512, 1, 1);
    run(21, 13, "klac");
    assert(asked(0, -512.0f, -256.0f));
    assert(at(1088.0f, 24.25f, 552.0f));
}

static void check_idle(void)
{
    static const int gates[5][3] = {
        /* t, state, show */
        { 1025, 2, 1 }, { 0, 0, 1 }, { 0, 2, 0 }, { 0, 2, -1 }, { 1025, 1, 1 },
    };
    int i, from;

    /* Each failed gate, for a forward pair, a reverse pair and a pair without a route: the
     * model stands at "from" and turns towards the cursor's point on the plane; the angle is
     * ADDED to the stored one (5 in the fixture); state, t and idx are left alone (a reverse
     * route in state 1 is not started). */
    for (i = 0; i < 5; i++) {
        for (from = 1; from <= 4; from += (from == 1 ? 2 : 1)) {
            setup(5, 1, gates[i][0], gates[i][1], gates[i][2]);
            run(from, 2, "klgsiac");
            assert(screen_in.x == 80.0f && screen_in.y == 76.0f && screen_in.z == 1.0f && screen_in.w == 1.0f);
            assert(screen_x == 80.0f && screen_y == 76.0f);
            assert(at(64.0f * (float)from, 3.25f + (float)from, 32.0f * (float)from + 8.0f));
            assert(asked(0, 164.0f - 64.0f * (float)from, 240.0f - (32.0f * (float)from + 8.0f)));
            assert(rot.y == 5 + 1000 + 32768);
            assert(left(gates[i][1], gates[i][0], 1));
        }
    }
    /* A cursor left of and above the centre; location 0; a negative converted angle. */
    setup(5, 1, 0, 0, 1);
    work->mx = 0;
    work->my = 0;
    conv_results[0] = -65536 - 25;
    run(0, 0, "klgsiac");
    assert(screen_x == -320.0f && screen_y == -224.0f);
    assert(at(0.0f, 3.25f, 8.0f));
    assert(asked(0, 164.0f, 232.0f));
    assert(rot.y == 5 - 25 + 32768);
}

int main(void)
{
    check_layout();
    check_pairs();
    check_forward();
    check_reverse();
    check_straight();
    check_idle();
    puts("camp_leader_model: ok");
    return 0;
}
