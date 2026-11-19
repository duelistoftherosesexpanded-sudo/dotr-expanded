/* Synthetic host tests for the lk_frames group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/lk_frames.h"

static int step;
static float *world_out, *local_out, *mul_m, *mul_other;
static float *seen_scale, *seen_pos;
static int *seen_rot;
static int seen_rot_words[3];
static float seen_pos_values[3];
static int look[3];
static float *point_out;
static float point_x, point_y;

void RhView_GetWorldScreenMatrix(float *out)
{
    int i;

    for (i = 0; i < 16; i++)
        out[i] = (float)(i + 1);
    world_out = out;
    step = step * 10 + 1;
}

void RhGeom_GetScaleRotXYZTransMatrix(float *m, float *scale, int *rot, float *pos)
{
    m[0] = 5.0f;
    local_out = m;
    seen_scale = scale;
    seen_rot = rot;
    seen_pos = pos;
    memcpy(seen_rot_words, rot, sizeof(seen_rot_words));
    memcpy(seen_pos_values, pos, sizeof(seen_pos_values));
    step = step * 10 + 2;
}

void RhGeom_MulMatrix(float *m, float *other)
{
    assert(other[0] == 5.0f);
    m[15] = 99.0f;
    mul_m = m;
    mul_other = other;
    step = step * 10 + 3;
}

void Ex3d_GetPointOnScreen(float *out, float x, float y)
{
    out[0] = 10.0f;
    out[1] = 20.0f;
    out[2] = 30.0f;
    point_out = out;
    point_x = x;
    point_y = y;
    step = step * 10 + 4;
}

float Ex3d_GetLookVector(int *out)
{
    out[0] = look[0];
    out[1] = look[1];
    out[2] = look[2];
    step = step * 10 + 5;
    return 0.0f;
}

static void check_frame_call(struct DotrLkFrame *frame)
{
    assert(world_out == frame->matrix && mul_m == frame->matrix);
    assert(local_out == mul_other && local_out != frame->matrix);
    assert(seen_scale == frame->scale && seen_rot == frame->rot && seen_pos == frame->pos);
}

/* LkRotSF (branch coverage): the % 65536 outcomes (non-negative, negative with a
 * remainder, negative multiple) and the unreduced result. */
static void check_rot(void)
{
    struct DotrLkFrame frame;

    memset(&frame, 0, sizeof(frame));
    frame.rot[3] = 7;
    look[0] = 11; look[1] = 65536 + 5; look[2] = 33;
    step = 0;
    assert(LkRotSF(&frame) == 65536 + 5);
    assert(step == 5123);
    assert(frame.rot[0] == 11 && frame.rot[1] == 5 && frame.rot[2] == 33 && frame.rot[3] == 7);
    assert(seen_rot_words[0] == 11 && seen_rot_words[1] == 5 && seen_rot_words[2] == 33);
    assert(frame.matrix[15] == 99.0f);
    check_frame_call(&frame);

    look[1] = -5;
    assert(LkRotSF(&frame) == -5);
    assert(frame.rot[1] == -5);
    look[1] = -65536 - 5;
    assert(LkRotSF(&frame) == -65536 - 5);
    assert(frame.rot[1] == -5);
    look[1] = -65536;
    assert(LkRotSF(&frame) == -65536);
    assert(frame.rot[1] == 0);
    look[1] = 65535;
    assert(LkRotSF(&frame) == 65535);
    assert(frame.rot[1] == 65535);
}

/* LkChange2D (straight-line coverage): forwarded floats, call order, the three stored
 * position floats (the fourth untouched) and the result. */
static void check_change(void)
{
    struct DotrLkFrame frame;

    memset(&frame, 0, sizeof(frame));
    frame.pos[3] = 4.0f;
    look[0] = 1; look[1] = 2; look[2] = 3;
    step = 0;
    assert(LkChange2D(&frame, 1.5f, 2.5f) == 0);
    /* point, then LkRotSF's own four calls, then the matrix again */
    assert(step == 45123123);
    assert(point_x == 1.5f && point_y == 2.5f && (void *)point_out != (void *)frame.pos);
    assert(frame.pos[0] == 10.0f && frame.pos[1] == 20.0f && frame.pos[2] == 30.0f && frame.pos[3] == 4.0f);
    assert(seen_pos_values[0] == 10.0f && seen_pos_values[2] == 30.0f);
    assert(frame.rot[0] == 1 && frame.rot[1] == 2 && frame.rot[2] == 3);
    check_frame_call(&frame);
}

/* S_I_P_IN (straight-line coverage): forwarded pointers, call order and all 64 copied bytes. */
static void check_copy(void)
{
    struct DotrLkMatrix out[2];
    float scale[4] = {0}, pos[4] = {0};
    int rot[4] = {0};
    int i;

    memset(out, 0, sizeof(out));
    step = 0;
    S_I_P_IN(&out[0], scale, rot, pos);
    assert(step == 123);
    assert(seen_scale == scale && seen_rot == rot && seen_pos == pos);
    assert(world_out == mul_m && local_out == mul_other && world_out != out[0].m);
    for (i = 0; i < 15; i++)
        assert(out[0].m[i] == (float)(i + 1));
    assert(out[0].m[15] == 99.0f && out[1].m[0] == 0.0f);
}

/* Branch coverage: step taken, the boundary where the step lands on the limit, and the clamp. */
static void check_steps(void)
{
    short value;

    value = 10;
    assert(LkIncrSHORT(&value, 20, 4) == 1 && value == 14);
    value = 16;
    assert(LkIncrSHORT(&value, 20, 4) == 1 && value == 20);
    value = 17;
    assert(LkIncrSHORT(&value, 20, 4) == 0 && value == 20);
    value = 10;
    assert(LkDecrSHORT(&value, 0, 4) == 1 && value == 6);
    value = 4;
    assert(LkDecrSHORT(&value, 0, 4) == 1 && value == 0);
    value = 3;
    assert(LkDecrSHORT(&value, 0, 4) == 0 && value == 0);
}

static float angle_args[2];

static float angle_log[4][2], angle_results[4], root_arg;
static int angle_calls, angle_scripted, root_calls;

float func_001a6c18(float a, float b)
{
    angle_args[0] = a;
    angle_args[1] = b;
    if (angle_scripted) {
        angle_log[angle_calls][0] = a;
        angle_log[angle_calls][1] = b;
        return angle_results[angle_calls++];
    }
    return 3.1415927f;
}

float func_001a6c60(float value)
{
    root_arg = value;
    root_calls++;
    return 5.0f;
}

int func_001a22c0(float value)
{
    return (int)value;
}

/* Straight-line coverage: forwarded arguments and the result (stub angle pi gives 180 + 90). */
static void check_angle(void)
{
    float vector[2] = { 2.0f, 5.0f };

    assert(LkSetAngleXY_m90(vector) == 270);
    assert(angle_args[0] == 5.0f && angle_args[1] == 2.0f);
}

/* Branch coverage: above/below/equal, each with the half-turn boundary on both sides. */
static void check_rot_border(void)
{
    assert(LkRegulateRotBorder(10, 5) == 5);
    assert(LkRegulateRotBorder(0x8000, 0) == 0x8000);
    assert(LkRegulateRotBorder(0x8001, 0) == -0x7fff);
    assert(LkRegulateRotBorder(0x9000, 0x100) == -0x7100);
    assert(LkRegulateRotBorder(5, 10) == -5);
    assert(LkRegulateRotBorder(0, 0x8000) == -0x8000);
    assert(LkRegulateRotBorder(0, 0x8001) == 0x7fff);
    assert(LkRegulateRotBorder(0x100, 0x9000) == 0x7100);
    assert(LkRegulateRotBorder(77, 77) == 0);
}

static struct DotrLkVector *norm_arg[2], norm_seen[2], *outer_args[3], *scalor_arg, *rot_axis;
static int norm_calls, rot_phase;
static float *rot_m, asin_arg;

void RhGeom_NormalizeVector(struct DotrLkVector *v)
{
    assert(norm_calls < 2);
    norm_arg[norm_calls] = v;
    norm_seen[norm_calls] = *v;
    norm_calls++;
}

void RhGeom_OuterProduct(struct DotrLkVector *out, struct DotrLkVector *a, struct DotrLkVector *b)
{
    outer_args[0] = out;
    outer_args[1] = a;
    outer_args[2] = b;
}

float RhGeom_GetVectorScalor(struct DotrLkVector *v)
{
    scalor_arg = v;
    return 0.5f;
}

float func_001a6c00(float value)
{
    asin_arg = value;
    return 6.2831855f / 4.0f;
}

void RhGeom_GetRotVectorMatrix(float *m, struct DotrLkVector *axis, int phase)
{
    rot_m = m;
    rot_axis = axis;
    rot_phase = phase;
}

/* Straight-line coverage: every forwarded argument and the return value. */
static void check_vec2mat(void)
{
    struct DotrLkVector v = { 1.0f, 2.0f, 3.0f, 4.0f };
    float m[16];

    assert(LkVec2Mat(&v, m) == 0);
    assert(norm_calls == 2 && norm_arg[0] != &v && norm_arg[1] != norm_arg[0]);
    assert(norm_seen[0].x == 1.0f && norm_seen[0].y == 2.0f && norm_seen[0].z == 3.0f && norm_seen[0].w == 4.0f);
    assert(norm_seen[1].x == 0.0f && norm_seen[1].y == 1.0f && norm_seen[1].z == 0.0f);
    assert(outer_args[1] == norm_arg[1] && outer_args[2] == norm_arg[0]);
    assert(scalor_arg == outer_args[0] && asin_arg == 0.5f);
    assert(rot_m == m && rot_axis == outer_args[0] && rot_phase == 0x4000);
    assert(v.x == 1.0f && v.y == 2.0f && v.z == 3.0f && v.w == 4.0f);
}

/* Straight-line coverage: every forwarded argument pair in order, every stored angle and the
 * result. Angles are whole binary fractions of a turn so the conversion is exact; two turns
 * show the reduction to 16 bits (non-negative values only). */
static void check_vec2rot(void)
{
    float v[3] = { 3.0f, 4.0f, 12.0f };
    int rot[4] = { -1, -1, -1, -1 };

    angle_scripted = 1;
    angle_calls = 0;
    angle_results[0] = 6.2831855f * 0.25f;
    angle_results[1] = 6.2831855f * 0.125f;
    angle_results[2] = 6.2831855f * 2.0f;
    assert(LkVec2Rot(v, rot) == 0 && angle_calls == 3);
    assert(angle_log[0][0] == 3.0f && angle_log[0][1] == 12.0f && angle_log[1][0] == 12.0f && angle_log[1][1] == 4.0f);
    assert(angle_log[2][0] == 4.0f && angle_log[2][1] == 3.0f);
    assert(rot[1] == 0x4000 && rot[0] == 0x2000 && rot[2] == 0 && rot[3] == 0);

    rot[0] = rot[1] = rot[2] = rot[3] = -1;
    angle_calls = root_calls = 0;
    angle_results[0] = 6.2831855f * 2.0f;
    angle_results[1] = 6.2831855f * 0.5f;
    assert(LkVec2RotXY(v, rot) == 0 && angle_calls == 2 && root_calls == 1);
    assert(angle_log[0][0] == 3.0f && angle_log[0][1] == 12.0f && root_arg == 153.0f);
    assert(angle_log[1][0] == 5.0f && angle_log[1][1] == 4.0f);
    assert(rot[1] == 0 && rot[0] == 0x8000 && rot[2] == 0 && rot[3] == 0);
    angle_scripted = 0;
}

int main(void)
{
    assert(sizeof(struct DotrLkFrame) == 112 && sizeof(struct DotrLkMatrix) == 64);
    check_rot();
    check_change();
    check_copy();
    check_steps();
    check_angle();
    check_rot_border();
    check_vec2mat();
    check_vec2rot();
    return 0;
}
