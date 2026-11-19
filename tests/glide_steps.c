/* Synthetic host tests for the glide_steps group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/glide_steps.h"

/* Host objects standing in for the fixed-address storage. */
struct DotrGlideHead dotr_glide_head_01ed0000;
struct DotrGlideState dotr_glide_state_01ed0040;
struct DotrGlideTarget dotr_glide_target_01ed01b8;
struct DotrGlideFlags dotr_glide_flags_01ed5370;
short dotr_glide_limit_01ed536c;
struct DotrGlideBoard data_00299e10_glide;
struct DotrGlideSpot data_00298ba0_glide[22];
struct DotrGlideView dotr_glide_view_01ed0000;
struct DotrGlideBank dotr_glide_bank_01ed0000;
struct DotrGlideList data_00299020_glide[2][16];
struct DotrGlideFrame { int unused; } prim_data_glide;

static int frame_calls, look_calls, project_calls, order;
static void *frame_arg, *look_from, *look_to;
static int look_index, look_order;
static struct DotrGlideVector4 look_from_value, look_to_value;
static int *project_x[22], *project_y[22];
static struct DotrGlideVector3 project_in[22];
void Taku_Coord_Set(struct DotrGlideFrame *frame) { frame_calls++; frame_arg = frame; assert(order++ == 0); }
int SzView_SetCamera(int index, struct DotrGlideVector4 *from, struct DotrGlideVector4 *in)
{
    look_calls++; look_index = index; look_from = from; look_to = in;
    look_from_value = *from; look_to_value = *in; look_order = order++;
    return 0;
}
/* TkCamp_Act2 mode: a call on the block's arrays answers the spot's screen pair, the
 * following call on locals answers a point act2_reach to the right of it. */
static int act2_on, act2_calls, act2_last, act2_px[22], act2_py[22], act2_reach[22];
static unsigned act2_mask;
static struct DotrGlideVector3 act2_in[2];
void Taku_3D_2D_Set(int *x, int *y, void *a)
{
    if (act2_on) {
        struct DotrCampInitBlock *w = DOTR_CAMP_INIT;
        act2_calls++;
        if (x >= &w->unknown_8c[0] && x < &w->unknown_8c[22]) {
            act2_last = (int)(x - w->unknown_8c);
            assert(y == &w->unknown_10c[act2_last]);
            act2_mask |= 1u << act2_last;
            *x = act2_px[act2_last]; *y = act2_py[act2_last];
            if (act2_last == 3) act2_in[0] = *(struct DotrGlideVector3 *)a;
        } else {
            *x = act2_px[act2_last] + act2_reach[act2_last]; *y = -77;
            if (act2_last == 3) act2_in[1] = *(struct DotrGlideVector3 *)a;
        }
        return;
    }
    project_x[project_calls] = x; project_y[project_calls] = y;
    project_in[project_calls++] = *(struct DotrGlideVector3 *)a;
}

static void fill(int step)
{
    memset(&dotr_glide_state_01ed0040, 0, sizeof(dotr_glide_state_01ed0040));
    memset(&dotr_glide_flags_01ed5370, 7, sizeof(dotr_glide_flags_01ed5370));
    dotr_glide_state_01ed0040.x = 256.0f;
    dotr_glide_state_01ed0040.y = -512.0f;
    dotr_glide_state_01ed0040.from_x = 1024.0f;
    dotr_glide_state_01ed0040.from_y = 2048.0f;
    dotr_glide_state_01ed0040.step = (short)step;
    dotr_glide_target_01ed01b8.x = 512.0f;
    dotr_glide_target_01ed01b8.y = 768.0f;
}

static void expect_untouched(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        assert(((unsigned char *)&dotr_glide_flags_01ed5370)[i] == 7);
    }
}

/* Taku_Tyumoku_ido (branch coverage). */
static void check_glide(void)
{
    assert(offsetof(struct DotrGlideState, step) == 0x20 && offsetof(struct DotrGlideFlags, second) == 5);
    /* step 0: the current pair becomes the start; one sixteenth of the way */
    fill(0);
    assert(Taku_Tyumoku_ido() == 0);
    assert(dotr_glide_state_01ed0040.step == 16);
    assert(dotr_glide_state_01ed0040.from_x == 256.0f && dotr_glide_state_01ed0040.from_y == -512.0f);
    assert(dotr_glide_state_01ed0040.x == 256.0f * 240.0f / 256.0f + 512.0f * 16.0f / 256.0f);
    assert(dotr_glide_state_01ed0040.y == -512.0f * 240.0f / 256.0f + 768.0f * 16.0f / 256.0f);
    expect_untouched();
    /* step not 0: the remembered start is used and kept */
    fill(16);
    assert(Taku_Tyumoku_ido() == 0);
    assert(dotr_glide_state_01ed0040.step == 32);
    assert(dotr_glide_state_01ed0040.from_x == 1024.0f && dotr_glide_state_01ed0040.from_y == 2048.0f);
    assert(dotr_glide_state_01ed0040.x == 1024.0f * 224.0f / 256.0f + 512.0f * 32.0f / 256.0f);
    assert(dotr_glide_state_01ed0040.y == 2048.0f * 224.0f / 256.0f + 768.0f * 32.0f / 256.0f);
    expect_untouched();
    /* 224 -> 240 is the last step that answers 0 */
    fill(224);
    assert(Taku_Tyumoku_ido() == 0 && dotr_glide_state_01ed0040.step == 240);
    assert(dotr_glide_state_01ed0040.x == 1024.0f * 16.0f / 256.0f + 512.0f * 240.0f / 256.0f);
    expect_untouched();
    /* 240 -> 256: on the target, step cleared, new start, both bytes cleared and nothing beside them */
    fill(240);
    assert(Taku_Tyumoku_ido() == 1 && dotr_glide_state_01ed0040.step == 0);
    assert(dotr_glide_state_01ed0040.x == 512.0f && dotr_glide_state_01ed0040.y == 768.0f);
    assert(dotr_glide_state_01ed0040.from_x == 512.0f && dotr_glide_state_01ed0040.from_y == 768.0f);
    assert(dotr_glide_flags_01ed5370.first == 0 && dotr_glide_flags_01ed5370.second == 0);
    assert(dotr_glide_flags_01ed5370.unknown_00[1] == 7 && dotr_glide_flags_01ed5370.stage == 7);
    assert(dotr_glide_flags_01ed5370.unknown_04 == 7 && dotr_glide_flags_01ed5370.unknown_06[0] == 7);
    /* 250 -> 266 is limited to 256 */
    fill(250);
    assert(Taku_Tyumoku_ido() == 1 && dotr_glide_state_01ed0040.step == 0);
    assert(dotr_glide_state_01ed0040.x == 512.0f && dotr_glide_state_01ed0040.from_y == 768.0f);
    /* 239 -> 255 stays below the limit */
    fill(239);
    assert(Taku_Tyumoku_ido() == 0 && dotr_glide_state_01ed0040.step == 255);
    assert(dotr_glide_state_01ed0040.from_x == 1024.0f);
    expect_untouched();
}

static void phase_fill(int phase)
{
    memset(&dotr_glide_head_01ed0000, 7, sizeof(dotr_glide_head_01ed0000));
    memset(&dotr_glide_state_01ed0040, 7, sizeof(dotr_glide_state_01ed0040));
    memset(&dotr_glide_target_01ed01b8, 7, sizeof(dotr_glide_target_01ed01b8));
    memset(&dotr_glide_flags_01ed5370, 7, sizeof(dotr_glide_flags_01ed5370));
    memset(&data_00299e10_glide, 7, sizeof(data_00299e10_glide));
    memset(data_00298ba0_glide, 0, sizeof(data_00298ba0_glide));
    data_00298ba0_glide[5].x = 12.5f;
    data_00298ba0_glide[5].y = -3.25f;
    dotr_glide_flags_01ed5370.phase = (signed char)phase;
}

/* Phase reached from phase 1 with the wait word at 256. */
static int after_wait(int ticks, int mode, int kind, int extra)
{
    phase_fill(1);
    dotr_glide_state_01ed0040.wait = 256;
    data_00299e10_glide.ticks = (short)ticks;
    data_00299e10_glide.mode = (signed char)mode;
    data_00299e10_glide.kind[0] = (signed char)kind;
    data_00299e10_glide.unknown_c9 = (signed char)extra;
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.picked == 5);
    assert(dotr_glide_flags_01ed5370.unknown_0b == 1 && dotr_glide_flags_01ed5370.unknown_0d == 1);
    assert(dotr_glide_flags_01ed5370.fade == 0);
    return dotr_glide_flags_01ed5370.phase;
}

/* Taku_Tyumoku_ido_katimake (branch coverage). */
static void check_phase(void)
{
    static const signed char set_a[12] = { 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1 };
    int kind;

    assert(offsetof(struct DotrGlideState, wait) == 0x2c && offsetof(struct DotrGlideState, unknown_30) == 0x30);
    assert(offsetof(struct DotrGlideTarget, unknown_20) == 0x20 && offsetof(struct DotrGlideTarget, unknown_26) == 0x26);
    assert(offsetof(struct DotrGlideFlags, phase) == 0xa && offsetof(struct DotrGlideFlags, unknown_3b) == 0x3b);
    assert(offsetof(struct DotrGlideFlags, picked) == 0x50 && offsetof(struct DotrGlideFlags, unknown_96) == 0x96);
    assert(offsetof(struct DotrGlideFlags, countdown) == 0xd0 && offsetof(struct DotrGlideFlags, fading) == 0x102);
    assert(offsetof(struct DotrGlideFlags, total) == 0x104 && offsetof(struct DotrGlideFlags, fade) == 0x108);
    assert(offsetof(struct DotrGlideBoard, seen) == 3 && offsetof(struct DotrGlideBoard, ticks) == 0xc2);
    assert(offsetof(struct DotrGlideBoard, kind) == 0xc6 && offsetof(struct DotrGlideBoard, unknown_c9) == 0xc9);
    assert(sizeof(struct DotrGlideSpot) == 52 && offsetof(struct DotrGlideSpot, y) == 8);

    /* phase 0, pick not seen yet: marked, everything reset, phase 1 */
    phase_fill(0);
    data_00299e10_glide.seen[5] = 0;
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.picked == 5 && data_00299e10_glide.seen[5] == 1);
    assert(data_00299e10_glide.seen[4] == 7 && data_00299e10_glide.seen[6] == 7);
    assert(dotr_glide_flags_01ed5370.second == 1 && dotr_glide_flags_01ed5370.first == 7);
    assert(dotr_glide_state_01ed0040.x == 12.5f && dotr_glide_state_01ed0040.y == -3.25f);
    assert(dotr_glide_head_01ed0000.width == 320 && dotr_glide_head_01ed0000.height == 190);
    assert(dotr_glide_state_01ed0040.unknown_30 == 0 && dotr_glide_state_01ed0040.wait == 0);
    assert(dotr_glide_state_01ed0040.unknown_2e == 0x0707 && dotr_glide_state_01ed0040.step == 0x0707);
    assert(dotr_glide_flags_01ed5370.unknown_0b == 0 && dotr_glide_flags_01ed5370.unknown_0d == 0);
    assert(dotr_glide_flags_01ed5370.unknown_0c == 7 && dotr_glide_flags_01ed5370.countdown == 100);
    assert(data_00299e10_glide.kind[0] == 5 && data_00299e10_glide.kind[1] == 5 && data_00299e10_glide.kind[2] == 5);
    assert(data_00299e10_glide.unknown_c9 == 7 && data_00299e10_glide.ticks == 0x0707);
    assert(dotr_glide_target_01ed01b8.unknown_20 == 0 && dotr_glide_target_01ed01b8.unknown_22 == 0);
    assert(dotr_glide_target_01ed01b8.unknown_24 == 0x0707 && dotr_glide_target_01ed01b8.unknown_26 == 0);
    assert(dotr_glide_flags_01ed5370.unknown_96 == 0 && dotr_glide_flags_01ed5370.unknown_3b == 0);
    assert(dotr_glide_flags_01ed5370.fading == 0 && dotr_glide_flags_01ed5370.phase == 1);
    assert(dotr_glide_flags_01ed5370.total == 0x07070707 && dotr_glide_flags_01ed5370.fade == 0x0707);
    /* phase 0, pick already seen: the mark is kept */
    phase_fill(0);
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(data_00299e10_glide.seen[5] == 7 && dotr_glide_flags_01ed5370.phase == 1);

    /* phase 1: 255 waits, 256 goes on */
    phase_fill(1);
    dotr_glide_state_01ed0040.wait = 255;
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.picked == 5 && dotr_glide_flags_01ed5370.phase == 1);
    assert(dotr_glide_flags_01ed5370.unknown_0b == 7 && dotr_glide_flags_01ed5370.fade == 0x0707);
    /* fewer than 300 ticks: phase 3 whatever the kind; 300 is enough */
    assert(after_wait(299, 0, 2, 0) == 3);
    assert(after_wait(300, 0, 2, 0) == 2);
    /* mode 0: the jump-table set, kind 11 only with a zero extra byte */
    for (kind = 0; kind < 12; kind++) {
        assert(after_wait(300, 0, kind, 0) == (set_a[kind] ? 2 : 3));
    }
    assert(after_wait(300, 0, 11, 1) == 3);
    assert(after_wait(300, 0, 12, 0) == 3);
    assert(after_wait(300, 0, -1, 0) == 3);
    /* mode not 0: kinds 0, 12-14 and 18-21 */
    for (kind = -1; kind < 24; kind++) {
        int want = kind == 0 || (kind >= 12 && kind <= 14) || (kind >= 18 && kind <= 21);

        assert(after_wait(300, 1, kind, 0) == (want ? 2 : 3));
    }

    /* phase 2: a positive countdown waits */
    phase_fill(2);
    dotr_glide_flags_01ed5370.countdown = 1;
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.fading == 7 && dotr_glide_flags_01ed5370.total == 0x07070707);
    /* countdown 0, fade below the limit after the step */
    phase_fill(2);
    dotr_glide_flags_01ed5370.countdown = 0;
    dotr_glide_flags_01ed5370.total = 10;
    dotr_glide_flags_01ed5370.fade = 4063;
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.fading == 1 && dotr_glide_flags_01ed5370.total == 4106);
    assert(dotr_glide_flags_01ed5370.fade == 4095 && dotr_glide_flags_01ed5370.phase == 2);
    /* the step reaches or passes the limit: clamped, phase 3, fading cleared */
    phase_fill(2);
    dotr_glide_flags_01ed5370.countdown = -1;
    dotr_glide_flags_01ed5370.fade = 4064;
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.fade == 4096 && dotr_glide_flags_01ed5370.phase == 3);
    assert(dotr_glide_flags_01ed5370.fading == 0);
    phase_fill(2);
    dotr_glide_flags_01ed5370.countdown = 0;
    dotr_glide_flags_01ed5370.fade = 4095;
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.fade == 4096 && dotr_glide_flags_01ed5370.phase == 3);
    /* fade already at the limit: only the flag and the total change */
    phase_fill(2);
    dotr_glide_flags_01ed5370.countdown = 0;
    dotr_glide_flags_01ed5370.fade = 4096;
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.fading == 1 && dotr_glide_flags_01ed5370.fade == 4096);
    assert(dotr_glide_flags_01ed5370.phase == 2);

    /* any other phase: only the pick is stored */
    phase_fill(3);
    assert(Taku_Tyumoku_ido_katimake(99, 5) == 0);
    assert(dotr_glide_flags_01ed5370.picked == 5 && dotr_glide_flags_01ed5370.phase == 3);
    assert(dotr_glide_flags_01ed5370.fading == 7 && dotr_glide_state_01ed0040.wait == 0x0707);
}

/* Taku_Screen_Init (loop coverage: the fixed 22-pass loop is checked at its first and last pass and
 * by its call count; every stored value and forwarded argument is checked). The host
 * view object is separate from the DotrGlideState object that shares 0x1ed0040. */
static void check_project(void)
{
    struct DotrGlideView *b = &dotr_glide_view_01ed0000;
    int i;

    memset(b, 0, sizeof(*b));
    for (i = 0; i < 22; i++) {
        data_00298ba0_glide[i].x = (float)(i * 4);
        data_00298ba0_glide[i].unknown_04 = (float)(i * 4 + 1);
        data_00298ba0_glide[i].y = (float)(i * 4 + 2);
    }
    data_00299e10_glide.kind[0] = 3;
    data_00299e10_glide.kind[1] = 5;
    b->to.y = 9.0f;
    b->to.w = 7.0f;
    order = frame_calls = look_calls = project_calls = 0;
    Taku_Screen_Init();
    assert(frame_calls == 1 && frame_arg == &prim_data_glide);
    assert(look_calls == 1 && look_order == 1 && look_index == 0);
    assert(look_from == &b->from && look_to == &b->to);
    assert(look_to_value.x == 20.0f && look_to_value.y == 0.0f && look_to_value.z == 22.0f && look_to_value.w == 7.0f);
    assert(look_from_value.x == 20.0f && look_from_value.y == 96.0f && look_from_value.z == 118.0f && look_from_value.w == 7.0f);
    assert(project_calls == 22);
    assert(project_x[0] == &b->unknown_8c[0] && project_y[0] == &b->unknown_10c[0]);
    assert(project_x[21] == &b->unknown_8c[21] && project_y[21] == &b->unknown_10c[21]);
    assert(project_in[0].x == 0.0f && project_in[0].y == 9.65f && project_in[0].z == 2.0f);
    assert(project_in[21].x == 84.0f && project_in[21].y == 8.65f + 85.0f && project_in[21].z == 86.0f);
    assert(b->width == 320 && b->height == 190);
}

static int movie_calls;
void Taku_Camp_Set_Go_Duel(void) { movie_calls++; }

static void stage_fill(int stage, int kind)
{
    memset(&dotr_glide_state_01ed0040, 0, sizeof(dotr_glide_state_01ed0040));
    memset(&dotr_glide_flags_01ed5370, 0, sizeof(dotr_glide_flags_01ed5370));
    memset(&dotr_glide_target_01ed01b8, 0, sizeof(dotr_glide_target_01ed01b8));
    memset(&data_00299e10_glide, 0, sizeof(data_00299e10_glide));
    memset(data_00298ba0_glide, 0, sizeof(data_00298ba0_glide));
    dotr_glide_flags_01ed5370.stage = (signed char)stage;
    dotr_glide_flags_01ed5370.unknown_52 = -9;
    dotr_glide_flags_01ed5370.unknown_0b = 1;
    dotr_glide_limit_01ed536c = 40;
    data_00299e10_glide.kind[0] = (signed char)kind;
    data_00299e10_glide.kind[2] = 21;
    data_00299e10_glide.ticks = 5;
    data_00299e10_glide.seen[16] = 1;
    data_00298ba0_glide[kind].x = 512.0f;
    data_00298ba0_glide[kind].y = -256.0f;
    dotr_glide_state_01ed0040.x = 64.0f;
    dotr_glide_state_01ed0040.y = 32.0f;
    dotr_glide_head_01ed0000.width = 300;
    dotr_glide_head_01ed0000.height = 180;
    dotr_glide_target_01ed01b8.unknown_22 = 3;
    dotr_glide_target_01ed01b8.unknown_26 = 4;
    movie_calls = 0;
}

/* Taku_Tyumoku_ido_kettei (branch coverage). */
static void check_stage(void)
{
    assert(offsetof(struct DotrGlideFlags, stage) == 3 && offsetof(struct DotrGlideFlags, hold) == 0x30);
    assert(offsetof(struct DotrGlideFlags, unknown_52) == 0x52 && offsetof(struct DotrGlideFlags, unknown_0b) == 0xb);
    assert(offsetof(struct DotrGlideState, saved_width) == 0x36 && offsetof(struct DotrGlideState, saved_height) == 0x38);

    /* gate: first flag set and neither board condition: nothing happens */
    stage_fill(1, 4);
    dotr_glide_flags_01ed5370.first = 1;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && dotr_glide_flags_01ed5370.stage == 1);
    /* gate: limit above the stored halfword */
    stage_fill(1, 4);
    assert(Taku_Tyumoku_ido_kettei(0, 0, 41) == 0 && dotr_glide_flags_01ed5370.stage == 1);
    /* gate: byte 0x1ed537b clear */
    stage_fill(1, 4);
    dotr_glide_flags_01ed5370.unknown_0b = 0;
    data_00299e10_glide.seen[4] = 1;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && dotr_glide_flags_01ed5370.stage == 1);
    /* gate: seen byte 2 opens it although the first flag is set */
    stage_fill(1, 4);
    dotr_glide_flags_01ed5370.first = 1;
    data_00299e10_glide.seen[4] = 2;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 99) == 0 && dotr_glide_flags_01ed5370.stage == 2);
    /* gate: kind 15 with seen[16] set stays closed, clear opens it */
    stage_fill(1, 15);
    dotr_glide_flags_01ed5370.first = 1;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && dotr_glide_flags_01ed5370.stage == 1);
    data_00299e10_glide.seen[16] = 0;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && dotr_glide_flags_01ed5370.stage == 2);

    /* stage 1 through the limit gate (limit equal to the halfword): every store */
    stage_fill(1, 4);
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0);
    assert(data_00299e10_glide.kind[2] == 4 && data_00299e10_glide.unknown_c9 == -9);
    assert(dotr_glide_state_01ed0040.from_x == 64.0f && dotr_glide_state_01ed0040.from_y == 32.0f);
    assert(dotr_glide_state_01ed0040.saved_width == 300 && dotr_glide_state_01ed0040.saved_height == 180);
    assert(dotr_glide_state_01ed0040.step == 0 && dotr_glide_flags_01ed5370.stage == 2);
    assert(dotr_glide_target_01ed01b8.unknown_22 == 0 && dotr_glide_target_01ed01b8.unknown_26 == 0);
    assert(dotr_glide_target_01ed01b8.unknown_20 == 1 && movie_calls == 0);

    /* stage 2, step 0 -> 8: one thirty-second of the way, not finished */
    stage_fill(2, 4);
    dotr_glide_state_01ed0040.from_x = 0.0f;
    dotr_glide_state_01ed0040.from_y = 256.0f;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0);
    assert(dotr_glide_state_01ed0040.step == 8 && dotr_glide_flags_01ed5370.stage == 2);
    assert(dotr_glide_state_01ed0040.x == 16.0f && dotr_glide_state_01ed0040.y == 240.0f);
    /* stage 2, step 240 -> 248: still below the bound */
    stage_fill(2, 4);
    dotr_glide_state_01ed0040.step = 240;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && dotr_glide_state_01ed0040.step == 248);
    /* stage 2, step 248 -> 256: finished, pair is the spot, ticks untouched (kind 4) */
    stage_fill(2, 4);
    dotr_glide_state_01ed0040.step = 248;
    dotr_glide_state_01ed0040.from_x = 3.0f;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 1);
    assert(dotr_glide_state_01ed0040.step == 0 && dotr_glide_flags_01ed5370.stage == 3);
    assert(dotr_glide_state_01ed0040.x == 512.0f && dotr_glide_state_01ed0040.y == -256.0f);
    assert(data_00299e10_glide.ticks == 5);
    /* stage 2, pair already equal to the spot: finished at once */
    stage_fill(2, 4);
    dotr_glide_state_01ed0040.from_x = 512.0f;
    dotr_glide_state_01ed0040.from_y = -256.0f;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 1 && dotr_glide_flags_01ed5370.stage == 3);
    /* stage 2, only x equal: not finished */
    stage_fill(2, 4);
    dotr_glide_state_01ed0040.from_x = 512.0f;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && dotr_glide_state_01ed0040.step == 8);
    /* stage 2 finishing with kind 15: ticks set only when seen[16] is clear */
    stage_fill(2, 15);
    dotr_glide_state_01ed0040.step = 248;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 1 && data_00299e10_glide.ticks == 5);
    stage_fill(2, 15);
    dotr_glide_state_01ed0040.step = 248;
    data_00299e10_glide.seen[16] = 0;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 1 && data_00299e10_glide.ticks == 12040);

    /* stage 3: the call needs hold <= 0 and the target word 3 */
    stage_fill(3, 4);
    dotr_glide_target_01ed01b8.unknown_20 = 3;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && movie_calls == 1);
    dotr_glide_flags_01ed5370.hold = 1;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && movie_calls == 1);
    dotr_glide_flags_01ed5370.hold = -1;
    dotr_glide_target_01ed01b8.unknown_20 = 2;
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && movie_calls == 1);
    /* any other stage: nothing */
    stage_fill(0, 4);
    assert(Taku_Tyumoku_ido_kettei(0, 0, 40) == 0 && dotr_glide_flags_01ed5370.stage == 0 && movie_calls == 0);
}

static void reset_cells(int value)
{
    struct DotrGlideBank *bank = &dotr_glide_bank_01ed0000;
    int s, n, j;

    memset(bank, 0, sizeof(*bank));
    memset(&data_00299e10_glide, 0, sizeof(data_00299e10_glide));
    memset(data_00298ba0_glide, 0, sizeof(data_00298ba0_glide));
    memset(&dotr_glide_flags_01ed5370, 7, sizeof(dotr_glide_flags_01ed5370));
    dotr_glide_state_01ed0040.step = 5;
    dotr_glide_target_01ed01b8.unknown_20 = 5;
    dotr_glide_target_01ed01b8.unknown_22 = 5;
    dotr_glide_target_01ed01b8.unknown_24 = 5;
    dotr_glide_target_01ed01b8.unknown_26 = 5;
    for (j = 0; j < 32; j++) {
        bank->unknown_52ec[j] = (short)value;
        bank->unknown_532c[j] = (short)value;
    }
    for (s = 0; s < 2; s++) {
        for (n = 0; n < 16; n++) {
            data_00299020_glide[s][n].cell = (short)(12 + n);
            data_00299020_glide[s][n].other_cell = (short)(11 + n);
            bank->records[s][n].unknown_202 = (short)value;
            bank->unknown_42ac[s][n] = (short)value;
            for (j = 0; j < 64; j++) {
                bank->unknown_42ec[s][n][j] = (short)value;
            }
        }
    }
    data_00299e10_glide.ticks = 300;
}

static void expect_entry(int side, int n, int cell, int value)
{
    struct DotrGlideBank *bank = &dotr_glide_bank_01ed0000;

    assert(bank->unknown_52ec[cell] == value && bank->unknown_532c[cell] == value);
    assert(bank->records[side][n].unknown_202 == value && bank->unknown_42ac[side][n] == value);
    assert(bank->unknown_42ec[side][n][0] == value && bank->unknown_42ec[side][n][63] == value);
}

static void check_cells(void)
{
    struct DotrGlideBank *bank = &dotr_glide_bank_01ed0000;
    struct DotrGlideBoard *board = &data_00299e10_glide;
    short list[3] = { 1, 2, 3 };
    int m;

    assert(offsetof(struct DotrGlideFlags, unknown_fc) == 0xfc && offsetof(struct DotrGlideFlags, total) == 0x104);
    assert(sizeof(struct DotrGlideSpot) == 52 && offsetof(struct DotrGlideSpot, marks) == 38);
    assert(sizeof(struct DotrGlideRecord) == 516 && sizeof(struct DotrGlideList) == 76);
    assert(offsetof(struct DotrGlideBank, unknown_42ac) == 0x42ac && offsetof(struct DotrGlideBank, saved_ticks) == 0x5370);

    /* Side 1, count 0: fixed stores only; no post-loop count for side 1. */
    reset_cells(9);
    for (m = 2; m <= 10; m++) board->seen[m] = 3;
    Taku_Dodai_Root_init(1, 4, 6, list, 0);
    assert(dotr_glide_state_01ed0040.step == 0);
    assert(dotr_glide_flags_01ed5370.first == 1 && dotr_glide_flags_01ed5370.stage == 0);
    assert(dotr_glide_flags_01ed5370.unknown_0b == 0 && dotr_glide_flags_01ed5370.hold == 0);
    assert(dotr_glide_flags_01ed5370.second == 1 && dotr_glide_flags_01ed5370.unknown_fc == 0);
    assert(dotr_glide_flags_01ed5370.unknown_04 == 7 && dotr_glide_flags_01ed5370.unknown_fd[0] == 7);
    assert(dotr_glide_target_01ed01b8.unknown_20 == 0 && dotr_glide_target_01ed01b8.unknown_22 == 0);
    assert(dotr_glide_target_01ed01b8.unknown_26 == 0 && dotr_glide_target_01ed01b8.unknown_24 == 5);
    assert(board->kind[1] == 4 && board->kind[0] == 0 && board->seen[4] == 6);
    assert(board->ticks == 300 && bank->saved_ticks == 0);
    expect_entry(1, 1, 13, 9);

    /* Side 1: the mark is ignored (cell from +0x46); seen cell 14 is skipped;
     * count bounds the list (entry 3 untouched). */
    reset_cells(9);
    data_00298ba0_glide[4].marks[0] = 2;
    board->seen[14] = 1;
    Taku_Dodai_Root_init(1, 4, 6, list, 2);
    expect_entry(1, 1, 13, 0);
    assert(bank->unknown_52ec[12] == 9);
    expect_entry(1, 2, 14, 9);
    expect_entry(1, 3, 15, 9);
    assert(bank->records[0][1].unknown_202 == 9 && bank->unknown_42ac[0][1] == 9 && bank->unknown_42ec[0][1][0] == 9);
    assert(board->seen[13] == 0 && board->seen[14] == 1);

    /* Side 0: mark 2 picks the other cell (+0x4a) for that position only.
     * Record 12's halfword is non-zero, so no count. */
    reset_cells(9);
    data_00298ba0_glide[4].marks[0] = 2;
    data_00298ba0_glide[4].marks[1] = 1;
    for (m = 2; m <= 10; m++) board->seen[m] = 3;
    Taku_Dodai_Root_init(0, 4, 3, list, 2);
    assert(bank->unknown_52ec[12] == 0 && bank->unknown_532c[12] == 0 && bank->unknown_52ec[13] == 9);
    assert(bank->records[0][1].unknown_202 == 0 && bank->unknown_42ac[0][1] == 0);
    assert(bank->unknown_42ec[0][1][0] == 0 && bank->unknown_42ec[0][1][63] == 0 && bank->unknown_42ec[0][2][0] == 0);
    expect_entry(0, 2, 14, 0);
    expect_entry(1, 1, 31, 9);
    assert(board->ticks == 300 && bank->saved_ticks == 0);

    /* Side 0, record 12 clear: seven cells at 3 or more are not enough. */
    reset_cells(0);
    for (m = 2; m <= 8; m++) board->seen[m] = 3;
    board->seen[9] = 2; board->seen[10] = 2; board->seen[11] = 3; board->seen[1] = 3;
    Taku_Dodai_Root_init(0, 12, 2, list, 0);
    assert(board->ticks == 300 && bank->saved_ticks == 0);

    /* Eight: the ticks are saved and replaced once. */
    reset_cells(0);
    for (m = 2; m <= 10; m++) board->seen[m] = 3;
    board->seen[5] = 2;
    Taku_Dodai_Root_init(0, 12, 2, list, 0);
    assert(board->ticks == 11120 && bank->saved_ticks == 300);

    /* Nine: the ninth repeats the copy with the new value. */
    reset_cells(0);
    for (m = 2; m <= 10; m++) board->seen[m] = 4;
    Taku_Dodai_Root_init(0, 12, 2, list, 0);
    assert(board->ticks == 11120 && bank->saved_ticks == 11120);
}

/* Taku_Tyumoku_ido1 (branch coverage). Taku_Dodai_Root_init is the real one. */
static void check_row_glide(void)
{
    struct DotrGlideBoard *board = &data_00299e10_glide;

    /* flag 0, limit clear: the row is recorded with value 3 and the limit counted */
    reset_cells(9);
    dotr_glide_flags_01ed5370.first = 0;
    dotr_glide_limit_01ed536c = 0;
    assert(Taku_Tyumoku_ido1(1, 4) == 0);
    assert(dotr_glide_limit_01ed536c == 1);
    assert(board->kind[1] == 4 && board->seen[4] == 3 && dotr_glide_flags_01ed5370.first == 1);
    expect_entry(1, 1, 13, 9);
    /* flag 0, limit set: nothing */
    reset_cells(9);
    dotr_glide_flags_01ed5370.first = 0;
    dotr_glide_limit_01ed536c = 1;
    assert(Taku_Tyumoku_ido1(1, 4) == 0);
    assert(dotr_glide_limit_01ed536c == 1 && board->seen[4] != 3 && dotr_glide_flags_01ed5370.first == 0);
    /* flag 2 and any other value: nothing */
    fill(16);
    dotr_glide_flags_01ed5370.first = 2;
    dotr_glide_limit_01ed536c = 0;
    assert(Taku_Tyumoku_ido1(1, 4) == 0);
    assert(dotr_glide_state_01ed0040.step == 16 && dotr_glide_limit_01ed536c == 0);
    dotr_glide_flags_01ed5370.first = 3;
    assert(Taku_Tyumoku_ido1(1, 4) == 0);
    assert(dotr_glide_state_01ed0040.step == 16 && dotr_glide_limit_01ed536c == 0);
    /* flag 1, step 0: the current pair becomes the start; the pair is set with the old step */
    memset(data_00298ba0_glide, 0, sizeof(data_00298ba0_glide));
    data_00298ba0_glide[4].x = 512.0f;
    data_00298ba0_glide[4].y = 768.0f;
    fill(0);
    dotr_glide_flags_01ed5370.first = 1;
    assert(Taku_Tyumoku_ido1(1, 4) == 0);
    assert(dotr_glide_state_01ed0040.step == 16);
    assert(dotr_glide_state_01ed0040.from_x == 256.0f && dotr_glide_state_01ed0040.from_y == -512.0f);
    assert(dotr_glide_state_01ed0040.x == 256.0f && dotr_glide_state_01ed0040.y == -512.0f);
    assert(dotr_glide_flags_01ed5370.first == 1 && dotr_glide_flags_01ed5370.second == 7);
    /* step not 0: the remembered start is kept */
    fill(16);
    dotr_glide_flags_01ed5370.first = 1;
    assert(Taku_Tyumoku_ido1(1, 4) == 0);
    assert(dotr_glide_state_01ed0040.step == 32 && dotr_glide_state_01ed0040.from_x == 1024.0f);
    assert(dotr_glide_state_01ed0040.x == 1024.0f * 240.0f / 256.0f + 16.0f * 512.0f / 256.0f);
    assert(dotr_glide_state_01ed0040.y == 2048.0f * 240.0f / 256.0f + 16.0f * 768.0f / 256.0f);
    /* 240 -> 256 is not above 256: still 0 */
    fill(240);
    dotr_glide_flags_01ed5370.first = 1;
    assert(Taku_Tyumoku_ido1(1, 4) == 0 && dotr_glide_state_01ed0040.step == 256);
    assert(dotr_glide_state_01ed0040.from_x == 1024.0f && dotr_glide_flags_01ed5370.first == 1);
    /* 256 -> 272: on the spot, step cleared, new start, both bytes cleared */
    fill(256);
    dotr_glide_flags_01ed5370.first = 1;
    assert(Taku_Tyumoku_ido1(1, 4) == 1 && dotr_glide_state_01ed0040.step == 0);
    assert(dotr_glide_state_01ed0040.x == 512.0f && dotr_glide_state_01ed0040.y == 768.0f);
    assert(dotr_glide_state_01ed0040.from_x == 512.0f && dotr_glide_state_01ed0040.from_y == 768.0f);
    assert(dotr_glide_flags_01ed5370.first == 0 && dotr_glide_flags_01ed5370.second == 0);
    assert(dotr_glide_flags_01ed5370.stage == 7 && dotr_glide_flags_01ed5370.unknown_04 == 7);
}

int ACT2_Rock;

/* One call of 0x228150 on a board with kind[2] = kind, the given seen value for it and
 * marks 4 / 5 of every spot set to 400 + n / 500 + n; answers the ticks. */
static int score(int side, int kind, int seen, int mode, int extra, int both)
{
    struct DotrGlideBoard *board = &data_00299e10_glide;
    int n;

    memset(board, 0, sizeof(*board));
    for (n = 0; n < 22; n++) {
        data_00298ba0_glide[n].marks[4] = (short)(400 + n);
        data_00298ba0_glide[n].marks[5] = (short)(500 + n);
    }
    board->mode = (signed char)mode;
    board->unknown_01[0] = 1;
    board->unknown_01[1] = (signed char)both;
    board->kind[0] = 1; board->kind[1] = 1;
    board->kind[2] = (signed char)kind;
    board->seen[kind] = (signed char)seen;
    board->unknown_c9 = (signed char)extra;
    board->ticks = 77;
    board->unknown_c4 = 31;
    board->wins[0] = 5; board->wins[1] = 9998;
    board->row_wins[0][31] = 9998; board->row_wins[1][31] = 6;
    board->streak = 3;
    ACT2_Rock = 0;
    Taku_Camp_What_KATIMAKE(side);
    assert(ACT2_Rock == 1);
    assert(board->kind[0] == kind && board->kind[1] == kind && board->kind[2] == kind);
    assert(board->streak == 4 && board->unknown_c4 == 31);
    return board->ticks;
}

/* Taku_Camp_What_KATIMAKE (branch coverage). */
static void check_score(void)
{
    struct DotrGlideBoard *board = &data_00299e10_glide;

    assert(offsetof(struct DotrGlideBoard, wins) == 0x3e && offsetof(struct DotrGlideBoard, row_wins) == 0x42);
    assert(offsetof(struct DotrGlideBoard, ticks) == 0xc2 && offsetof(struct DotrGlideBoard, streak) == 0xca);
    /* side not 0: first counters, mark 4; the other side's counters are untouched */
    assert(score(1, 7, 2, 0, 0, 0) == 407);
    assert(board->wins[0] == 6 && board->row_wins[0][31] == 9999);
    assert(board->wins[1] == 9998 && board->row_wins[1][31] == 6 && board->row_wins[0][30] == 0);
    /* side 0: second counters, mark 5; 9998 -> 9999 */
    assert(score(0, 7, 2, 0, 0, 0) == 507);
    assert(board->wins[1] == 9999 && board->row_wins[1][31] == 7);
    assert(board->wins[0] == 5 && board->row_wins[0][31] == 9998);
    /* the counters stay at 9999 */
    board->wins[0] = 9999; board->row_wins[0][31] = 9999; board->streak = 4;
    Taku_Camp_What_KATIMAKE(1);
    assert(board->wins[0] == 9999 && board->row_wins[0][31] == 9999 && board->streak == 5);
    board->wins[1] = 9999; board->row_wins[1][31] = 9999;
    Taku_Camp_What_KATIMAKE(0);
    assert(board->wins[1] == 9999 && board->row_wins[1][31] == 9999 && board->streak == 5);
    board->streak = 100;
    Taku_Camp_What_KATIMAKE(0);
    assert(board->streak == 5);
    /* both bytes set: ticks cleared whatever the seen byte */
    assert(score(1, 7, 2, 0, 0, 1) == 0);
    /* seen 3 or more: cleared, except mode 0 with kind 11 and extra 1 */
    assert(score(1, 7, 3, 0, 1, 0) == 0);
    assert(score(1, 11, 3, 1, 1, 0) == 0);
    assert(score(1, 11, 3, 0, 0, 0) == 0);
    assert(score(1, 11, 3, 0, 2, 0) == 0);
    assert(score(1, 11, 3, 0, 1, 0) == 313);
    assert(score(0, 11, 3, 0, 1, 0) == 213);
    /* kind 11 below 3: the mark when extra is clear, the fixed value otherwise */
    assert(score(1, 11, 2, 0, 0, 0) == 411);
    assert(score(1, 11, 2, 1, 2, 0) == 313);
    assert(score(0, 11, 2, 0, 0, 0) == 511);
    assert(score(0, 11, 2, 1, 2, 0) == 213);
    /* a side whose low byte is 0 is still "not 0" */
    assert(score(0x100, 7, 2, 0, 0, 0) == 407);
}

/* 0x233f50: one assertion per mode 0..3 and for the modes outside the switch. */
static void check_mode_row(void)
{
    static const short want[4][3] = { { 0, 0, 0 }, { 64, 0, 0 }, { 64, 64, 0 }, { 64, 64, 116 } };
    struct DotrGlideHead *head = &dotr_glide_head_01ed0000;
    int mode;

    for (mode = -1; mode <= 4; mode++) {
        memset(head, 7, sizeof(*head));
        Taku_Camp_SINEMA_Init((signed char)mode);
        assert(head->unknown_1c == 0 && head->width == 0x0707 && head->height == 0x0707);
        if (mode < 0 || mode > 3) {
            assert(head->unknown_1a == 0x0707 && head->unknown_1e == 0x0707);
            assert(head->unknown_20 == 0x0707 && head->unknown_22 == 0x0707);
        } else {
            assert(head->unknown_1a == mode && head->unknown_1e == want[mode][0]);
            assert(head->unknown_20 == want[mode][1] && head->unknown_22 == want[mode][2]);
        }
    }
}

static char pipe_log[4];
static int pipe_n;
int Taku_Camp_3D(int cursor) { pipe_log[pipe_n++] = 'D'; return cursor + 1; }
int Taku_Camp_STON(int cursor) { pipe_log[pipe_n++] = 'S'; return cursor + 16; }
int Taku_Camp_Kumo(int cursor) { pipe_log[pipe_n++] = 'K'; return cursor + 256; }

/* TkCamp_Draw_Pipe (branch coverage): every case value with both sides of its limit test, the
 * default on both sides of 11000 and the count on both sides of 10000. */
static void check_draw_pipe(void)
{
    static const struct { short ticks, limit; const char *log; int add; } rows[] = {
        { 9999, 0, "D", 1 }, { 10000, 0, "SK", 272 }, { 10999, 9, "SK", 272 },
        { 11000, 0, "SK", 272 }, { 11001, 0, "DK", 257 }, { 12157, 0, "DK", 257 },
        { 11182, 0, "SK", 272 }, { 11184, 0, "SK", 272 }, { 12000, 0, "SK", 272 },
        { 12130, 0, "SK", 272 }, { 12152, 0, "SK", 272 }, { 12154, 0, "SK", 272 },
        { 12156, 0, "SK", 272 },
        { 11160, 1, "DK", 257 }, { 11160, 2, "SK", 272 },
        { 11162, 8, "SK", 272 }, { 11162, 9, "DK", 257 },
        { 11180, 1, "DK", 257 }, { 11180, 2, "SK", 272 },
        { 12132, 9, "SK", 272 }, { 12132, 10, "DK", 257 },
        { 12150, 1, "DK", 257 }, { 12150, 2, "SK", 272 },
    };
    unsigned i;

    for (i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
        memset(pipe_log, 0, sizeof(pipe_log));
        pipe_n = 0;
        data_00299e10_glide.ticks = rows[i].ticks;
        dotr_glide_limit_01ed536c = rows[i].limit;
        assert(TkCamp_Draw_Pipe(0x1000) == 0x1000 + rows[i].add);
        assert(strcmp(pipe_log, rows[i].log) == 0);
    }
    data_00299e10_glide.ticks = 0;
    dotr_glide_limit_01ed536c = 0;
}

struct DotrCampInitBlock dotr_camp_init_01ed0000;
struct DotrCampSave dotr_camp_save_01e301e0;
unsigned char HELP_flg[1];
unsigned short DeckTab_halves[16];
struct DotrCampRow data_00299d00[32];
static struct DotrCampSave init_save_out;
static void *init_clear_ptr;
static int init_clear_size, init_get_calls, init_get_arg;
static float init_surface[3][3], *init_surface_out;
void func_00195108(void *a, int b) { init_clear_ptr = a; init_clear_size = b; }
void Ex3d_GetSurfaceC(float *a, float *b, float *c, float *out)
{
    memcpy(init_surface[0], a, sizeof(init_surface[0]));
    memcpy(init_surface[1], b, sizeof(init_surface[1]));
    memcpy(init_surface[2], c, sizeof(init_surface[2]));
    init_surface_out = out;
}
void *SzData_GetData(int value) { init_get_calls++; init_get_arg = value; return &init_save_out; }

/* Taku_Camp_Data_Init (branch coverage): the save-flag byte set and clear, every loop's first and
 * last element with the storage just past it left alone (the clearing callee is a
 * recording stub, so untouched bytes keep the fill), two sums for the remainder loop and
 * the three 5-bit picks. */
static void check_data_init(void)
{
    static const unsigned short want_pick[2][3] = { { 3, 17, 31 }, { 0, 0, 0 } };
    static const unsigned short want_low[2][3] = { { 1, 0x3ff, 0 }, { 0x123, 0x123, 0x123 } };
    struct DotrCampInitBlock *b = &dotr_camp_init_01ed0000;
    unsigned short *words = (unsigned short *)&data_00299e10_glide;
    int pass, i, j, k;

    for (pass = 0; pass < 2; pass++) {
        memset(b, 0x55, sizeof(*b));
        memset(&data_00299e10_glide, 0, sizeof(data_00299e10_glide));
        memset(&init_save_out, 0, sizeof(init_save_out));
        memset(DeckTab_halves, 0, sizeof(DeckTab_halves));
        memset(data_00299d00, 0, sizeof(data_00299d00));
        for (i = 0; i < 16360; i++) {
            dotr_camp_save_01e301e0.words[i] = i * 3 + 1;
        }
        data_00299e10_glide.ticks = 12345;
        words[17] = 0xf;
        words[30] = 0xf;
        for (i = 0; i < 12; i++) {
            words[18 + i] = (unsigned short)(0x8100 + i + 1);
        }
        if (pass == 1) {
            words[18] = 0x10f;
        }
        DeckTab_halves[14] = 3 | (17 << 5) | (31 << 10);
        data_00299d00[3].unknown_00 = 0xfc01;
        data_00299d00[17].unknown_00 = 0x07ff;
        data_00299d00[31].unknown_00 = 0x0400;
        data_00299d00[0].unknown_00 = 0x123;
        HELP_flg[0] = (unsigned char)(pass == 0);
        look_calls = 0;
        init_get_calls = 0;
        init_clear_ptr = 0;
        Taku_Camp_Data_Init();
        assert(init_clear_ptr == b && init_clear_size == 21680);
        assert(init_surface[0][0] == 0.0f && init_surface[0][1] == 8.65f && init_surface[0][2] == 100.0f);
        assert(init_surface[1][0] == 0.0f && init_surface[1][1] == 8.65f && init_surface[1][2] == 0.0f);
        assert(init_surface[2][0] == 100.0f && init_surface[2][1] == 8.65f && init_surface[2][2] == 0.0f);
        assert(init_surface_out == b->surface);
        for (i = 0; i < 12; i++) {
            assert(b->unknown_53cf[i] == 0 && b->unknown_53db[i] == 6 && b->unknown_53e7[i] == 1);
        }
        assert(b->unknown_537b[0x53] == 0x55 && b->unknown_53f3[0] == 0x55);
        assert(b->unknown_547c == 128 && b->width == 320 && b->height == 190 && b->unknown_04 == 0);
        assert(b->unknown_1a == 0 && b->unknown_1c == 0 && b->unknown_1e == 0);
        assert(b->unknown_536e == 12345 && b->limit == 0 && b->unknown_5372 == 0 && b->unknown_6e == 0);
        assert(b->unknown_5370 == 0x5555);
        assert(b->to.x == 0.0f && b->to.y == 0.0f && b->to.z == 0.0f && b->to.w == 0.0f);
        assert(b->from.x == 0.0f && b->from.y == 96.0f && b->from.z == 96.0f && b->from.w == 0.0f);
        assert(look_calls == 1 && look_index == 0 && look_from == &b->from && look_to == &b->to);
        for (i = 0; i < 32; i++) {
            assert(b->unknown_8c[i] == 0 && b->unknown_10c[i] == 0);
            assert(b->unknown_52ec[i] == 0 && b->unknown_532c[i] == 0);
        }
        assert(b->unknown_82[9] == 0x55 && b->unknown_18c == 0x55555555);
        assert(b->unknown_1a6 == 0 && b->unknown_1ae == 0 && b->unknown_1b0 == 0 && b->unknown_1b2 == 0);
        assert(b->unknown_224 == 128 && b->unknown_226 == 128 && b->unknown_228 == 128);
        assert(b->unknown_22a == 0x5555);
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 16; j++) {
                assert(b->records[i][j].unknown_202 == 0 && ((unsigned char *)&b->records[i][j])[513] == 0x55);
                assert(b->unknown_42ac[i][j] == 0);
                for (k = 0; k < 64; k++) {
                    assert(b->unknown_42ec[i][j][k] == 0);
                }
            }
        }
        if (pass == 0) {
            assert(init_get_calls == 1 && init_get_arg == 0 && HELP_flg[0] == 0);
            assert(memcmp(&init_save_out, &dotr_camp_save_01e301e0, sizeof(init_save_out)) == 0);
        } else {
            assert(init_get_calls == 0 && HELP_flg[0] == 0 && init_save_out.words[0] == 0);
        }
        assert(b->unknown_53f6 == (pass == 0 ? 14 : 12));
        for (i = 0; i < 3; i++) {
            assert(b->unknown_53f8[i] == want_pick[pass][i] && b->unknown_53fe[i] == want_low[pass][i]);
        }
        assert(b->unknown_5404[0] == 0x55);
    }
    memset(&data_00299e10_glide, 0, sizeof(data_00299e10_glide));
}

static int ido0(int first, int step, int side, short *list, int count)
{
    dotr_glide_flags_01ed5370.first = (signed char)first;
    dotr_glide_flags_01ed5370.second = 9;
    dotr_glide_state_01ed0040.step = (short)step;
    data_00299e10_glide.ticks = 77;
    return Taku_Tyumoku_ido0((short)side, 3, list, (short)count);
}

static void check_ido0(void)
{
    struct DotrGlideState *s = &dotr_glide_state_01ed0040;
    struct DotrGlideTarget *t = &dotr_glide_target_01ed01b8;
    short list[3] = { 4, 5, 6 };
    int side, n;

    memset(data_00298ba0_glide, 0, sizeof(data_00298ba0_glide));
    memset(s, 0, sizeof(*s));
    memset(t, 0, sizeof(*t));
    data_00298ba0_glide[3].x = 512.0f;
    data_00298ba0_glide[3].y = 256.0f;
    data_00298ba0_glide[3].marks[0] = 2;
    data_00298ba0_glide[3].marks[1] = 1;
    data_00298ba0_glide[3].marks[2] = 2;
    data_00298ba0_glide[10].x = 64.0f;
    data_00298ba0_glide[10].y = 32.0f;
    data_00298ba0_glide[11].x = 128.0f;
    data_00298ba0_glide[11].y = 16.0f;
    data_00298ba0_glide[12].x = 8.0f;
    data_00298ba0_glide[12].y = 4.0f;
    data_00298ba0_glide[7].x = 4096.0f;
    data_00298ba0_glide[7].y = 4096.0f;
    for (side = 0; side < 2; side++) {
        for (n = 0; n < 16; n++) {
            data_00299020_glide[side][n].cell = 12;
            data_00299020_glide[side][n].other_cell = 12;
        }
    }

    /* Flag 0 (and 3): nothing moves, 0. */
    s->x = 256.0f; s->y = 512.0f; s->from_x = 1.0f; s->from_y = 2.0f;
    assert(ido0(0, 5, 0, list, 3) == 0 && s->step == 5 && s->x == 256.0f && s->from_x == 1.0f);
    assert(ido0(3, 5, 0, list, 3) == 0 && s->step == 5 && dotr_glide_flags_01ed5370.first == 3);
    /* Flag 1 from step 0: the pair is saved first, the step becomes 16 before the blend. */
    assert(ido0(1, 0, 0, list, 3) == 0 && s->step == 16 && s->from_x == 256.0f && s->from_y == 512.0f);
    assert(s->x == 240.0f + 32.0f && s->y == 480.0f + 16.0f && dotr_glide_flags_01ed5370.first == 1);
    /* From a non-zero step the saved pair is kept. */
    assert(ido0(1, 16, 0, list, 3) == 0 && s->step == 32 && s->from_x == 256.0f);
    assert(s->x == 224.0f + 64.0f && s->y == 448.0f + 32.0f);
    /* 224 -> 240 does not finish; 240 -> 256 and 250 -> 266 (limited to 256) finish: flag 2. */
    assert(ido0(1, 224, 0, list, 3) == 0 && s->step == 240 && dotr_glide_flags_01ed5370.first == 1);
    assert(ido0(1, 240, 0, list, 3) == 0 && s->step == 0 && dotr_glide_flags_01ed5370.first == 2);
    assert(s->x == 512.0f && s->y == 256.0f && s->from_x == 512.0f && s->from_y == 256.0f);
    s->from_x = 256.0f; s->from_y = 512.0f;
    assert(ido0(1, 250, 0, list, 3) == 0 && s->step == 0 && dotr_glide_flags_01ed5370.first == 2);
    assert(s->x == 512.0f && s->y == 256.0f && data_00299e10_glide.ticks == 77);

    /* Flag 2, step 0, side 0: mark 2 takes the other cell (10), mark 1 the cell (11), and the
     * third entry's other cell is 7, which is left out and lowers the divisor to 2. */
    data_00299020_glide[0][4].other_cell = 10; data_00299020_glide[0][4].cell = 7;
    data_00299020_glide[0][5].other_cell = 7; data_00299020_glide[0][5].cell = 11;
    data_00299020_glide[0][6].other_cell = 7; data_00299020_glide[0][6].cell = 12;
    s->x = 100.0f; s->y = 200.0f;
    assert(ido0(2, 0, 0, list, 3) == 0 && s->step == 8 && s->from_x == 100.0f && s->from_y == 200.0f);
    assert(t->unknown_10[0] == (64.0f + 128.0f) / 2.0f && t->unknown_10[2] == (32.0f + 16.0f) / 2.0f);
    assert(s->x == 100.0f && s->y == 200.0f && dotr_glide_flags_01ed5370.first == 2);
    /* Side 1 never takes the other cell; no cell 7, so the divisor stays 3. */
    data_00299020_glide[1][4].other_cell = 7; data_00299020_glide[1][4].cell = 10;
    data_00299020_glide[1][5].other_cell = 7; data_00299020_glide[1][5].cell = 11;
    data_00299020_glide[1][6].other_cell = 7; data_00299020_glide[1][6].cell = 12;
    assert(ido0(2, 0, 1, list, 3) == 0 && s->step == 8);
    assert(t->unknown_10[0] == (64.0f + 128.0f + 8.0f) / 3.0f && t->unknown_10[2] == (32.0f + 16.0f + 4.0f) / 3.0f);
    /* A single entry: the divisor is 1. */
    assert(ido0(2, 0, 1, list, 1) == 0 && t->unknown_10[0] == 64.0f && t->unknown_10[2] == 32.0f);
    /* Non-zero step: the target is not recomputed; halfway blend, step + 8. */
    t->unknown_10[0] = 300.0f; t->unknown_10[2] = 400.0f;
    s->from_x = 100.0f; s->from_y = 200.0f;
    assert(ido0(2, 128, 0, list, 3) == 0 && s->step == 136 && t->unknown_10[0] == 300.0f);
    assert(s->x == 200.0f && s->y == 300.0f);
    /* 248 -> 256 is not above 256: still 0. 256 -> 264 finishes. */
    assert(ido0(2, 248, 0, list, 3) == 0 && s->step == 256 && dotr_glide_flags_01ed5370.first == 2);
    assert(dotr_glide_flags_01ed5370.second == 9 && data_00299e10_glide.ticks == 77);
    assert(ido0(2, 256, 0, list, 3) == 1 && s->step == 0 && s->x == 300.0f && s->y == 400.0f);
    assert(s->from_x == 300.0f && s->from_y == 400.0f && dotr_glide_flags_01ed5370.first == 0);
    assert(dotr_glide_flags_01ed5370.second == 0 && data_00299e10_glide.ticks == 0);
    memset(s, 0, sizeof(*s));
    memset(t, 0, sizeof(*t));
    memset(&dotr_glide_flags_01ed5370, 0, sizeof(dotr_glide_flags_01ed5370));
    memset(data_00298ba0_glide, 0, sizeof(data_00298ba0_glide));
    memset(data_00299020_glide, 0, sizeof(data_00299020_glide));
    memset(&data_00299e10_glide, 0, sizeof(data_00299e10_glide));
}

/* Taku_Camp_3D_BOU (branch coverage). */
static int bou_calls, bou_cursor[8], bou_index[8];
static struct DotrGlideVector3 bou_pos[8];
int SzModel_Draw_Pipe(int cursor, int index, struct DotrGlideVector3 *position, void *d, int e, int f, int g)
{
    assert(bou_calls < 8 && d == NULL && e == 0 && f == 0 && g == 0);
    bou_cursor[bou_calls] = cursor; bou_index[bou_calls] = index; bou_pos[bou_calls++] = *position;
    return cursor + 1;
}

static struct DotrGlideBank *bou_reset(void)
{
    struct DotrGlideBank *bank = &dotr_glide_bank_01ed0000;
    int s, n, i;

    memset(bank, 0, sizeof(*bank));
    memset(data_00299020_glide, 0, sizeof(data_00299020_glide));
    for (s = 0; s < 2; s++) {
        for (n = 0; n < 16; n++) {
            for (i = 0; i < 64; i++) {
                bank->records[s][n].x[i] = (float)(1000 * s + 100 * n + i);
                bank->records[s][n].y[i] = (float)-(1000 * s + 100 * n + i);
            }
        }
    }
    data_00298ba0_glide[0].unknown_04 = 100.0f;
    bou_calls = 0;
    return bank;
}

static void bou_drawn(int call, int cursor, int index, float x, float y)
{
    assert(bou_cursor[call] == cursor && bou_index[call] == index);
    assert(bou_pos[call].x == x && bou_pos[call].y == y && bou_pos[call].z == -x);
}

static void check_bou(void)
{
    struct DotrGlideBank *bank;

    assert(offsetof(struct DotrGlideRecord, count) == 0x200 && offsetof(struct DotrGlideBank, idle) == 0x5376);

    /* Nothing to draw: state 0, an unknown state, the skipped rows of each side, and a
     * side-1 row past its twelve. The byte is set and the cursor comes back. */
    bank = bou_reset();
    bank->records[0][0].unknown_202 = 1; bank->records[0][15].unknown_202 = 3;
    bank->records[0][7].unknown_202 = 4; bank->records[1][7].unknown_202 = 4;
    bank->records[1][4].unknown_202 = 1; bank->records[1][5].unknown_202 = 2;
    bank->records[1][11].unknown_202 = 3; bank->records[1][12].unknown_202 = 3;
    assert(Taku_Camp_3D_BOU(5) == 5 && bou_calls == 0 && bank->idle == 1);
    assert(bank->unknown_42ac[1][5] == 0 && bank->records[1][4].unknown_202 == 1);

    /* State 3 on both sides: points 0..count at model height; the byte stays set. */
    bank = bou_reset();
    bank->records[0][2].unknown_202 = 3; bank->records[0][2].count = 2;
    bank->records[1][0].unknown_202 = 3;
    assert(Taku_Camp_3D_BOU(10) == 14 && bou_calls == 4 && bank->idle == 1);
    bou_drawn(0, 10, 62, 200.0f, 100.0f); bou_drawn(1, 11, 62, 201.0f, 100.0f);
    bou_drawn(2, 12, 62, 202.0f, 100.0f); bou_drawn(3, 13, 63, 1000.0f, 100.0f);

    /* State 1, growing point below 64: drawn low, grows by 16, reach stays. */
    bank = bou_reset();
    bank->records[0][1].unknown_202 = 1; bank->records[0][1].count = 1;
    bank->unknown_42ec[0][1][0] = 48;
    assert(Taku_Camp_3D_BOU(0) == 1 && bou_calls == 1 && bank->idle == 0);
    bou_drawn(0, 0, 62, 100.0f, 91.875f);
    assert(bank->unknown_42ec[0][1][0] == 64 && bank->unknown_42ac[0][1] == 0 && bank->records[0][1].unknown_202 == 1);
    /* At 64 the reach advances, so the next point is drawn in the same call. */
    bou_calls = 0;
    assert(Taku_Camp_3D_BOU(0) == 2 && bou_calls == 2);
    bou_drawn(0, 0, 62, 100.0f, 92.5f); bou_drawn(1, 1, 62, 101.0f, 90.0f);
    assert(bank->unknown_42ec[0][1][0] == 80 && bank->unknown_42ec[0][1][1] == 16 && bank->unknown_42ac[0][1] == 1);

    /* State 1 finishing on side 1: a full point at model height, the last one clamped to
     * 256, reach held at the count, then the cell marked and the row reset to state 3. */
    bank = bou_reset();
    bank->records[1][2].unknown_202 = 1; bank->records[1][2].count = 1;
    bank->unknown_42ac[1][2] = 1;
    bank->unknown_42ec[1][2][0] = 256; bank->unknown_42ec[1][2][1] = 250; bank->unknown_42ec[1][2][63] = 5;
    data_00299020_glide[1][2].cell = 9; data_00299020_glide[1][2].other_cell = 10;
    assert(Taku_Camp_3D_BOU(0) == 2 && bou_calls == 2 && bank->idle == 0);
    bou_drawn(0, 0, 63, 1200.0f, 100.0f); bou_drawn(1, 1, 63, 1201.0f, 90.0f + 10.0f * 250.0f / 256.0f);
    assert(bank->unknown_52ec[9] == 1 && bank->unknown_52ec[10] == 0 && bank->records[1][2].unknown_202 == 3);
    assert(bank->unknown_42ac[1][2] == 0 && bank->unknown_42ec[1][2][0] == 0 && bank->unknown_42ec[1][2][1] == 0);
    assert(bank->unknown_42ec[1][2][63] == 0 && bank->unknown_42ec[1][3][0] == 0);

    /* State 2 on side 0: reach starts at the count, points are drawn downwards and the
     * reach retreats once the point there has grown to 64. */
    bank = bou_reset();
    bank->records[0][3].unknown_202 = 2; bank->records[0][3].count = 2;
    bank->unknown_42ec[0][3][2] = 64;
    assert(Taku_Camp_3D_BOU(0) == 2 && bou_calls == 2 && bank->idle == 0);
    bou_drawn(0, 0, 62, 302.0f, 92.5f); bou_drawn(1, 1, 62, 301.0f, 90.0f);
    assert(bank->unknown_42ac[0][3] == 1 && bank->unknown_42ec[0][3][2] == 80 && bank->unknown_42ec[0][3][1] == 16);
    assert(bank->unknown_42ec[0][3][0] == 0 && bank->records[0][3].unknown_202 == 2);

    /* State 2 on side 1: the reach is not reset to the count; at reach 0 with point 0
     * reaching 256 the other cell is marked and the row reset. */
    bank = bou_reset();
    bank->records[1][0].unknown_202 = 2; bank->records[1][0].count = 3;
    bank->unknown_42ec[1][0][0] = 250;
    data_00299020_glide[1][0].cell = 9; data_00299020_glide[1][0].other_cell = 12;
    assert(Taku_Camp_3D_BOU(0) == 4 && bou_calls == 4 && bank->idle == 0);
    bou_drawn(0, 0, 63, 1003.0f, 90.0f); bou_drawn(3, 3, 63, 1000.0f, 90.0f + 10.0f * 250.0f / 256.0f);
    assert(bank->unknown_52ec[12] == 1 && bank->unknown_52ec[9] == 0 && bank->records[1][0].unknown_202 == 3);
    assert(bank->unknown_42ac[1][0] == 0 && bank->unknown_42ec[1][0][3] == 0 && bank->unknown_42ec[1][0][0] == 0);
}

struct DotrTikeiItem camp_sys_icon[4];
float func_001a6c48(float value, float power) { assert(power == 2.0f); return value * value; }
float func_001a6c60(float value) { float r = 0.0f; while (r * r < value) r += 1.0f; return r; }

/* Runs TkCamp_Act2 with every spot at screen (10 * i, 300) and no reach unless set. */
static void act2_reset(void)
{
    struct DotrCampInitBlock *w = DOTR_CAMP_INIT;
    int i;
    memset(w, 0, sizeof(*w));
    memset(&data_00299e10_glide, 0, sizeof(data_00299e10_glide));
    memset(camp_sys_icon, 0, sizeof(camp_sys_icon));
    for (i = 0; i < 4; i++) { camp_sys_icon[i].w[0] = 1000; camp_sys_icon[i].w[1] = 1000; }
    for (i = 0; i < 22; i++) {
        data_00298ba0_glide[i].x = (float)i; data_00298ba0_glide[i].unknown_04 = 1.0f;
        data_00298ba0_glide[i].y = (float)(2 * i);
        act2_px[i] = 10 * i; act2_py[i] = 300; act2_reach[i] = 0;
    }
    ACT2_Rock = 0;
    w->width = 500; w->height = 200; w->unknown_04 = 5;
    w->unknown_224 = w->unknown_226 = w->unknown_228 = 112;
}
static void act2_run(int force)
{
    order = frame_calls = act2_calls = 0; act2_mask = 0; act2_on = 1;
    TkCamp_Act2(force);
    act2_on = 0;
    assert(frame_calls == 1 && frame_arg == &prim_data_glide);
}
static void check_act2(void)
{
    struct DotrCampInitBlock *w = DOTR_CAMP_INIT;
    struct DotrGlideBoard *b = &data_00299e10_glide;
    int i;

    /* Mode 0: spots 0 and 11 always, 1..10 by their seen byte, 12.. never. No spot pointed at. */
    act2_reset();
    b->seen[1] = b->seen[3] = b->seen[10] = b->seen[12] = b->seen[15] = 1;
    w->unknown_6a = w->unknown_7e = 9;
    act2_run(0);
    assert(act2_mask == ((1u << 0) | (1u << 1) | (1u << 3) | (1u << 10) | (1u << 11)) && act2_calls == 10);
    assert(act2_in[0].x == 3.0f && act2_in[0].y == 1.0f + 8.65f && act2_in[0].z == 6.0f);
    assert(act2_in[1].x == 8.0f && act2_in[1].y == 1.0f + 8.65f && act2_in[1].z == 6.0f);
    assert(w->unknown_8c[3] == 30 && w->unknown_10c[3] == 300 && w->unknown_8c[2] == 0);
    assert(w->unknown_6a == 0 && w->unknown_7e == 0 && w->unknown_04 == 0 && b->kind[0] == 0);
    assert(w->width == 500 && w->height == 200);
    /* Height 200: +0x224 up, +0x226 up; width 500: +0x228 down. */
    assert(w->unknown_224 == 120 && w->unknown_226 == 120 && w->unknown_228 == 104);

    /* Other modes: spots 0 and 11 always, 12..21 by their seen byte, 1..11 never. */
    act2_reset();
    b->mode = 1;
    b->seen[1] = b->seen[10] = b->seen[12] = b->seen[15] = b->seen[21] = 1;
    act2_run(0);
    assert(act2_mask == ((1u << 0) | (1u << 11) | (1u << 12) | (1u << 15) | (1u << 21)));

    /* The force argument or the byte at +0x18c shows every spot. */
    act2_reset();
    act2_run(1);
    assert(act2_mask == 0x3fffff && act2_calls == 44);
    act2_reset();
    b->mode = 1; *(signed char *)&w->unknown_18c = 1;
    act2_run(0);
    assert(act2_mask == 0x3fffff);

    /* Head +4 of 1: only the two bytes are cleared, whatever was pointed at. */
    act2_reset();
    ACT2_Rock = 1; w->unknown_04 = 1; w->unknown_6a = w->unknown_7e = 9;
    act2_run(0);
    assert(w->unknown_6a == 0 && w->unknown_7e == 0 && w->unknown_04 == 1);

    /* ACT2_Rock: pointed at without a distance test. Spot 11 with seen 0..5 arms. */
    for (i = 0; i <= 6; i++) {
        act2_reset();
        ACT2_Rock = 1; b->kind[0] = 11; b->seen[11] = (signed char)i; w->unknown_7e = 9;
        act2_run(0);
        assert(w->unknown_6a == (i <= 5) && w->unknown_7e == (i <= 5 ? 0 : 9));
        assert(w->unknown_04 == (i <= 5 ? 2 : 5) && w->width == 500);
    }
    act2_reset();
    ACT2_Rock = 1; b->kind[0] = 11; b->seen[11] = -1;
    act2_run(0);
    assert(w->unknown_6a == 0 && w->unknown_04 == 5);
    /* The byte at +0x72 blocks it unless 0x1ed537a is set. */
    act2_reset();
    ACT2_Rock = 1; b->kind[0] = 11; w->unknown_72 = 1;
    act2_run(0);
    assert(w->unknown_6a == 0 && w->unknown_04 == 5);
    act2_reset();
    ACT2_Rock = 1; b->kind[0] = 11; w->unknown_72 = 1; w->unknown_537a = 1;
    act2_run(0);
    assert(w->unknown_6a == 1 && w->unknown_04 == 2);
    /* Another spot arms with seen 1..3 only. */
    for (i = 0; i <= 4; i++) {
        act2_reset();
        ACT2_Rock = 1; b->kind[0] = 4; b->seen[4] = (signed char)i; w->unknown_7e = 9;
        act2_run(0);
        assert(w->unknown_6a == (i >= 1 && i <= 3) && w->unknown_7e == (i >= 1 && i <= 3 ? 0 : 9));
        assert(w->unknown_04 == (i >= 1 && i <= 3 ? 2 : 5));
    }
    act2_reset();
    ACT2_Rock = 1; b->kind[0] = 4; b->seen[4] = 2; w->unknown_72 = 1;
    act2_run(0);
    assert(w->unknown_6a == 0 && w->unknown_04 == 5);
    act2_reset();
    ACT2_Rock = 1; b->kind[0] = 4; b->seen[4] = 2; w->unknown_72 = 1; w->unknown_537a = 1;
    act2_run(0);
    assert(w->unknown_6a == 1 && w->unknown_04 == 2);

    /* Distance test: head (33, 304) is 5 from spot 3 at (30, 300). Reach 6 picks it. */
    act2_reset();
    b->seen[3] = 2; act2_reach[3] = 6; w->width = 33; w->height = 304;
    act2_run(0);
    assert(b->kind[0] == 3 && w->unknown_6a == 1 && w->unknown_04 == 2);
    assert(w->width == 33 && w->height == 304);
    /* Reach 5 is not above the distance. */
    act2_reset();
    b->seen[3] = 2; act2_reach[3] = 5; w->width = 33; w->height = 304;
    act2_run(0);
    assert(b->kind[0] == 0 && w->unknown_6a == 0 && w->unknown_04 == 0);
    /* The halfword at +0x6c keeps kind[0]: another spot is then not pointed at ... */
    act2_reset();
    b->seen[3] = 2; act2_reach[3] = 6; w->width = 33; w->height = 304; w->unknown_6c = 1;
    act2_run(0);
    assert(b->kind[0] == 0 && w->unknown_04 == 0);
    /* ... the kept one is ... */
    act2_reset();
    b->seen[3] = 2; act2_reach[3] = 6; w->width = 33; w->height = 304; w->unknown_6c = 1; b->kind[0] = 3;
    act2_run(0);
    assert(b->kind[0] == 3 && w->unknown_6a == 1 && w->unknown_04 == 2);
    /* ... and the byte 0x1ed5373 lifts the hold and moves the head pair to spot kind[0]:
     * first to spot 0 at (0, 300), 30 from spot 3 and below its reach of 31, then to spot 3. */
    act2_reset();
    b->seen[3] = 2; act2_reach[3] = 31; w->width = 33; w->height = 304; w->unknown_6c = 1;
    w->unknown_5373 = 1;
    act2_run(0);
    assert(b->kind[0] == 3 && w->width == 30 && w->height == 300 && w->unknown_04 == 2);
    /* Same with a reach of 30: the head pair stays on spot 0, which is pointed at. */
    act2_reset();
    b->seen[3] = 2; act2_reach[3] = 30; act2_reach[0] = 1; w->width = 33; w->height = 304;
    w->unknown_5373 = 1;
    act2_run(0);
    assert(b->kind[0] == 0 && w->width == 0 && w->height == 300 && w->unknown_04 == 5);

    /* Icons: each edge of rectangle 2 (40..60, 50..70) widened by 2, inside and outside. */
    {
        static const short at[9][3] = {
            { 38, 48, 1 }, { 62, 72, 1 }, { 37, 60, 0 }, { 63, 60, 0 }, { 50, 47, 0 },
            { 50, 73, 0 }, { 38, 72, 1 }, { 62, 48, 1 }, { 50, 60, 1 },
        };
        for (i = 0; i < 9; i++) {
            act2_reset();
            camp_sys_icon[2].w[0] = 40; camp_sys_icon[2].w[2] = 60;
            camp_sys_icon[2].w[1] = 50; camp_sys_icon[2].w[3] = 70;
            w->width = at[i][0]; w->height = at[i][1]; w->unknown_6a = 9; w->unknown_80 = -1;
            act2_run(0);
            if (at[i][2]) {
                assert(w->unknown_80 == 2 && w->unknown_547a == 1 && w->unknown_7e == 1);
                assert(w->unknown_6a == 0 && w->unknown_04 == 2);
            } else {
                assert(w->unknown_80 == -1 && w->unknown_547a == 0 && w->unknown_7e == 0);
                assert(w->unknown_6a == 0 && w->unknown_04 == 0);
            }
        }
    }
    /* An icon wins over a spot; with head +4 of 1 only the loop's stores remain. */
    act2_reset();
    ACT2_Rock = 1; camp_sys_icon[0].w[0] = 490; camp_sys_icon[0].w[2] = 510;
    camp_sys_icon[0].w[1] = 190; camp_sys_icon[0].w[3] = 210; w->unknown_04 = 1; w->unknown_80 = -1;
    act2_run(0);
    assert(w->unknown_80 == 0 && w->unknown_547a == 1 && w->unknown_7e == 0 && w->unknown_04 == 1);

    /* The three halfwords: thresholds and both limits. */
    act2_reset();
    w->height = 119; w->width = 455; w->unknown_224 = 105; w->unknown_226 = 119; w->unknown_228 = 120;
    act2_run(0);
    assert(w->unknown_224 == 97 && w->unknown_226 == 127 && w->unknown_228 == 128);
    act2_reset();
    w->height = 119; w->width = 455; w->unknown_224 = 104; w->unknown_226 = 128; w->unknown_228 = 127;
    act2_run(0);
    assert(w->unknown_224 == 96 && w->unknown_226 == 128 && w->unknown_228 == 128);
    act2_reset();
    w->height = 120; w->width = 456; w->unknown_224 = 119; w->unknown_226 = 120; w->unknown_228 = 105;
    act2_run(0);
    assert(w->unknown_224 == 127 && w->unknown_226 == 128 && w->unknown_228 == 97);
    act2_reset();
    w->height = 339; w->width = 456; w->unknown_224 = 128; w->unknown_226 = 96; w->unknown_228 = 96;
    act2_run(0);
    assert(w->unknown_224 == 128 && w->unknown_226 == 104 && w->unknown_228 == 96);
    act2_reset();
    w->height = 340; w->width = 0; w->unknown_224 = 120; w->unknown_226 = 105; w->unknown_228 = 96;
    act2_run(0);
    assert(w->unknown_224 == 128 && w->unknown_226 == 97 && w->unknown_228 == 104);
    act2_reset();
    w->height = 340; w->width = 0; w->unknown_224 = 96; w->unknown_226 = 104; w->unknown_228 = 96;
    act2_run(0);
    assert(w->unknown_224 == 104 && w->unknown_226 == 96);
}


/* Taku_Nagare2_set (branch coverage; the three callees are the group's real functions, kept
 * idle by a zero flag block and board). */
static void check_nagare2(void)
{
    struct DotrGlideBank *b = &dotr_glide_bank_01ed0000;
    struct DotrGlideHead *h = &dotr_glide_head_01ed0000;
    short list[3] = { 4, 5, 6 };
    int n;

#define NAGARE_RESET(limit_, state_) \
    memset(b, 0, sizeof(*b)); memset(h, 0, sizeof(*h)); \
    memset(&dotr_glide_flags_01ed5370, 0, sizeof(dotr_glide_flags_01ed5370)); \
    memset(&data_00299e10_glide, 0, sizeof(data_00299e10_glide)); \
    memset(data_00298ba0_glide, 0, sizeof(data_00298ba0_glide)); \
    data_00298ba0_glide[3].marks[0] = 7; data_00298ba0_glide[3].marks[1] = 8; \
    data_00298ba0_glide[3].marks[2] = 9; \
    for (n = 0; n < 16; n++) { b->records[0][n].unknown_202 = 20; b->records[1][n].unknown_202 = 20; } \
    dotr_glide_limit_01ed536c = (limit_); h->unknown_1c = (state_); h->unknown_1a = 5

    /* Limit 0: both halfwords set, the reset call (value 3 marks the row seen), limit 1;
     * the fall into case 1 sees state 1 and leaves the records alone. */
    NAGARE_RESET(0, 2);
    Taku_Nagare2_set(0, 3, list, 0);
    assert(h->unknown_1c == 1 && h->unknown_1a == 0 && dotr_glide_limit_01ed536c == 1);
    assert(data_00299e10_glide.seen[3] == 3);
    assert(b->records[0][4].unknown_202 == 20 && b->records[1][4].unknown_202 == 20);
    /* Limit 1, state not 2: nothing. */
    NAGARE_RESET(1, 1);
    Taku_Nagare2_set(0, 3, list, 3);
    assert(dotr_glide_limit_01ed536c == 1 && h->unknown_1c == 1 && h->unknown_1a == 5);
    assert(b->records[0][4].unknown_202 == 20 && data_00299e10_glide.seen[3] == 0);
    /* Limit 1, state 2, side 0: each listed record gets the row's mark unless it holds 3. */
    NAGARE_RESET(1, 2);
    b->records[0][5].unknown_202 = 3;
    Taku_Nagare2_set(0, 3, list, 3);
    assert(b->records[0][4].unknown_202 == 7 && b->records[0][5].unknown_202 == 3);
    assert(b->records[0][6].unknown_202 == 9 && b->records[0][7].unknown_202 == 20);
    assert(b->records[1][4].unknown_202 == 20 && dotr_glide_limit_01ed536c == 2);
    /* Side 0 with a count of 0: no record, the limit still moves. */
    NAGARE_RESET(1, 2);
    Taku_Nagare2_set(0, 3, list, 0);
    assert(b->records[0][4].unknown_202 == 20 && dotr_glide_limit_01ed536c == 2);
    /* Side 1: only the first listed record, 1 unless it holds 3. */
    NAGARE_RESET(1, 2);
    Taku_Nagare2_set(1, 3, list, 3);
    assert(b->records[1][4].unknown_202 == 1 && b->records[1][5].unknown_202 == 20);
    assert(b->records[0][4].unknown_202 == 20 && dotr_glide_limit_01ed536c == 2);
    NAGARE_RESET(1, 2);
    b->records[1][4].unknown_202 = 3;
    Taku_Nagare2_set(1, 3, list, 3);
    assert(b->records[1][4].unknown_202 == 3 && dotr_glide_limit_01ed536c == 2);
    /* Any other limit: no case runs. */
    NAGARE_RESET(2, 2);
    Taku_Nagare2_set(0, 3, list, 3);
    assert(b->records[0][4].unknown_202 == 20 && dotr_glide_limit_01ed536c == 2 && h->unknown_1a == 5);
#undef NAGARE_RESET
}

int main(void)
{
    check_mode_row();
    check_stage();
    check_project();
    check_glide();
    check_phase();
    check_cells();
    check_row_glide();
    check_score();
    check_draw_pipe();
    check_data_init();
    check_ido0();
    check_bou();
    check_act2();
    check_nagare2();
    return 0;
}
