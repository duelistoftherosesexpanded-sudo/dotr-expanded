/* Synthetic host tests for the camp_ship_route group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_ship_route.h"

#define PI_F 3.1415927f

struct DotrShipWork dotr_camp_ship_route_work;
struct DotrShipRecord data_00299066_shiproute[2][16];
float data_00298ba4_shiproute;

static char trace[16];
static int n;
static float atan_args[4][2];
static float atan_results[4];
static int atan_calls;
static float conv_args[4];
static int conv_results[4];
static int conv_calls;
static int draw_index, draw_rot[3], draw_cursor;
static float *draw_pos;

static void step(char c)
{
    assert(n < 14);
    trace[n++] = c;
    trace[n] = 0;
}
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
int func_001ee4b0_shiproute(int cursor, int index, float *pos, int *rot, int e, int f, int g)
{
    assert(e == 0 && f == 0 && g == 0);
    draw_cursor = cursor;
    draw_index = index;
    draw_pos = pos;
    memcpy(draw_rot, rot, sizeof draw_rot);
    step('d');
    return cursor + 7;
}
int SzEffect_Entry(int a, int b, int c, int d, int e)
{
    assert(a == 0 && b == 403 && c == 0 && d == 0 && e == 0);
    step('e');
    return 0;
}

/* Route [1][3]: seven points, of which `count` are announced. Segment k has dx = 10 * (k + 1)
 * and dz = 4 * (k + 1). */
static const float xs[7] = { 0.0f, 10.0f, 30.0f, 60.0f, 100.0f, 150.0f, 210.0f };
static const float zs[7] = { 0.0f, 4.0f, 12.0f, 24.0f, 40.0f, 60.0f, 84.0f };

static void setup(int count, int idx, int t, int state, int show)
{
    struct DotrShipWork *w = &dotr_camp_ship_route_work;
    int i, j;

    memset(w, 0, sizeof *w);
    memset(data_00299066_shiproute, 0, sizeof data_00299066_shiproute);
    /* every other route would give other numbers and has no points */
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            w->route[i][j].x[0] = -999.0f;
            w->route[i][j].n = 0;
            data_00299066_shiproute[i][j].id = (short)(i * 16 + j + 40);
        }
    }
    memcpy(w->route[1][3].x, xs, sizeof xs);
    memcpy(w->route[1][3].z, zs, sizeof zs);
    w->route[1][3].n = (short)count;
    data_00299066_shiproute[1][3].id = 7;
    data_00298ba4_shiproute = 2.5f;
    w->idx = (short)idx;
    w->t = (short)t;
    w->state = (short)state;
    w->show = (signed char)show;
    w->angle = -1;
    n = 0;
    trace[0] = 0;
    atan_calls = conv_calls = 0;
    atan_results[0] = 99.0f;   /* discarded by the function */
    atan_results[1] = PI_F;    /* half a turn: 32768 before the conversion */
    atan_results[2] = -PI_F / 2.0f;
    conv_results[0] = 1000;
    conv_results[1] = 2000;
    draw_index = -1;
    draw_pos = 0;
}
static int flags_set(void)
{
    int i, count = 0;

    for (i = 0; i < 113; i++) {
        count += dotr_camp_ship_route_work.flag[i] != 0;
    }
    return count;
}

int main(void)
{
    struct DotrShipWork *w = &dotr_camp_ship_route_work;

    assert(sizeof(struct DotrShipRoute) == 516 && sizeof(struct DotrShipRecord) == 76);
    assert(offsetof(struct DotrShipWork, state) == 420 && offsetof(struct DotrShipWork, t) == 422);
    assert(offsetof(struct DotrShipWork, angle) == 424 && offsetof(struct DotrShipWork, idx) == 432);
    assert(offsetof(struct DotrShipWork, show) == 436 && offsetof(struct DotrShipWork, pos) == 440);
    assert(offsetof(struct DotrShipWork, route) == 556 && offsetof(struct DotrShipWork, flag) == 21228);
    assert(sizeof w->route[0] == 8256);

    /* Mid-route (idx < n - 1), shown, idle: the heading is the blend of this segment and the
     * next by t / 1024; the conversion's result wraps by % 65536. */
    setup(5, 1, 256, 0, 1);
    conv_results[1] = 65536 + 100;
    assert(Taku_Camp_3D_FUNE(50, 1, 3, 9) == 57);
    assert(strcmp(trace, "aacacd") == 0);
    assert(atan_args[0][0] == 30.0f && atan_args[0][1] == 12.0f);   /* next segment, result unused */
    assert(atan_args[1][0] == 20.0f && atan_args[1][1] == 8.0f);    /* this segment */
    assert(atan_args[2][0] == 22.5f && atan_args[2][1] == 9.0f);    /* 3/4 this + 1/4 next */
    assert(conv_args[0] == 32768.0f && conv_args[1] == -16384.0f);
    assert(w->pos[0] == 15.0f && w->pos[1] == 2.5f && w->pos[2] == 6.0f);
    assert(w->angle == 100);
    assert(draw_cursor == 50 && draw_index == 9 + 34 && draw_pos == w->pos);
    assert(draw_rot[0] == 0 && draw_rot[1] == 100 && draw_rot[2] == 0);
    assert(w->state == 0 && w->t == 256 && w->idx == 1 && flags_set() == 0);

    /* Last segment (idx == n - 1), hidden: this segment's angle, here negative (% keeps the
     * sign). Moving with t = 768: the step reaches 1024, idx reaches n: arrived. */
    setup(5, 4, 768, 2, 0);
    conv_results[0] = -70000;
    assert(Taku_Camp_3D_FUNE(50, 1, 3, 9) == 50);
    assert(strcmp(trace, "aac") == 0);
    assert(atan_args[0][0] == 60.0f && atan_args[0][1] == 24.0f);
    assert(atan_args[1][0] == 50.0f && atan_args[1][1] == 20.0f);
    assert(w->pos[0] == 137.5f && w->pos[2] == 55.0f);
    assert(w->angle == -4464);
    assert(w->state == 3 && w->t == 0 && w->idx == 5);
    assert(w->flag[7] == 1 && flags_set() == 1);

    /* At the end (idx == n): the previous segment's angle from a third call; state 1 does not
     * start anything because idx < n fails. */
    setup(5, 5, 0, 1, 1);
    conv_results[1] = -5;
    assert(Taku_Camp_3D_FUNE(3, 1, 3, 0) == 10);
    assert(strcmp(trace, "aacacd") == 0);
    assert(atan_args[2][0] == 50.0f && atan_args[2][1] == 20.0f);   /* point 5 - point 4 */
    assert(w->pos[0] == 150.0f && w->pos[2] == 60.0f);
    assert(w->angle == -5 && draw_rot[1] == -5 && draw_index == 34);
    assert(w->state == 1 && w->t == 0 && w->idx == 5 && flags_set() == 0);

    /* idx beyond n takes the same branch */
    setup(4, 5, 0, 0, 0);
    assert(Taku_Camp_3D_FUNE(3, 1, 3, 0) == 3);
    assert(strcmp(trace, "aacac") == 0 && w->angle == 2000);

    /* State 1 mid-route: the effect is entered after the draw, the state becomes 2 and the
     * first step is taken in the same call. With t = 0 the blend is this segment alone. */
    setup(5, 1, 0, 1, 1);
    assert(Taku_Camp_3D_FUNE(0, 1, 3, 1) == 7);
    assert(strcmp(trace, "aacacde") == 0);
    assert(atan_args[2][0] == 20.0f && atan_args[2][1] == 8.0f);
    assert(w->pos[0] == 10.0f && w->pos[2] == 4.0f && w->angle == 2000);
    assert(w->state == 2 && w->t == 256 && w->idx == 1 && flags_set() == 0);

    /* State 2 below 1024: only t moves. */
    setup(5, 1, 256, 2, 0);
    assert(Taku_Camp_3D_FUNE(0, 1, 3, 1) == 0);
    assert(strcmp(trace, "aacac") == 0);
    assert(w->state == 2 && w->t == 512 && w->idx == 1);

    /* State 2 reaching 1024 mid-route: next point, t cleared, still moving, no flag. */
    setup(5, 1, 768, 2, 0);
    assert(Taku_Camp_3D_FUNE(0, 1, 3, 1) == 0);
    assert(w->pos[0] == 25.0f && w->pos[2] == 10.0f);
    assert(w->state == 2 && w->t == 0 && w->idx == 2 && flags_set() == 0);

    /* t already 1024: nothing moves. */
    setup(5, 1, 1024, 2, 0);
    assert(Taku_Camp_3D_FUNE(0, 1, 3, 1) == 0);
    assert(w->pos[0] == 30.0f && w->pos[2] == 12.0f);
    assert(w->state == 2 && w->t == 1024 && w->idx == 1);

    /* State 3 with room left behaves like state 1 or 2 as far as the gate goes, but neither
     * branch acts. */
    setup(5, 1, 256, 3, 0);
    assert(Taku_Camp_3D_FUNE(0, 1, 3, 1) == 0);
    assert(strcmp(trace, "aacac") == 0 && w->state == 3 && w->t == 256 && w->idx == 1);

    puts("camp_ship_route: ok");
    return 0;
}
