/* Synthetic host tests for the model_exsort group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/model_exsort.h"

int gSzModel_pfhCache;
int gSzModel_nBlurMode;

static int blur_calls, blur_index, draw_cursor, draw_args[6], clamp_out, center_answer, maxot_answer;
static int radius_calls, sort_calls, sort_args[4], conv_calls;
static long clamp_value;
static float sample_dz, z_answer;
static struct DotrExSortVec *center_p, *sample_p;

int ExSetAnchor_Pipe(int cursor) { return cursor + 1; }
int ExDitherMatrixEz_Pipe(int cursor, int index) { return cursor + 10 * index; }
int RhBase_SetZBufferMode(int cursor, int mode) { return cursor + 100 + mode; }
int SzModel_DrawMotionBlur_Pipe(int cursor, int index) { blur_calls++, blur_index = index; return cursor + 1000; }
int SzModel_Draw_Pipe(int cursor, int index, int c, int d, int e, int f, int g)
{
    draw_cursor = cursor;
    draw_args[0] = index, draw_args[1] = c, draw_args[2] = d, draw_args[3] = e, draw_args[4] = f, draw_args[5] = g;
    return cursor + 10000;
}
long ExGetRegVal(int a, int b) { assert(a == 0 && b == 4); return 0x55; }
int ExClampMode_Pipe(int cursor, long value) { clamp_out = cursor, clamp_value = value; return 777777; }
int SzModel_GetCenter(int index, struct DotrExSortVec *v) { assert(index == 9); center_p = v; return center_answer; }
int SzModel_GetMaxOT(int index) { assert(index == 9); return maxot_answer; }
float SzModel_GetRadius(int index) { assert(index == 9); radius_calls++; return 8.0f; }
float Ex3d_GetSampleW(struct DotrExSortVec *v, float dz) { sample_p = v, sample_dz = dz; return z_answer; }
int ExGetMyOT(void) { return 4242; }
int func_001a23b0(float value) { conv_calls++; return (int)value; }
int ExSortAnchor_Pipe(int packet, int cursor, int chain, int depth)
{
    sort_calls++;
    sort_args[0] = packet, sort_args[1] = cursor, sort_args[2] = chain, sort_args[3] = depth;
    return 999;
}

static int run(int mode, int center, int maxot, float z)
{
    gSzModel_nBlurMode = mode, center_answer = center, maxot_answer = maxot, z_answer = z;
    gSzModel_pfhCache = 77;
    blur_calls = radius_calls = sort_calls = conv_calls = 0;
    sample_dz = -1.0f;
    return SzModel_ExSort_Pipe(5, 9, 11, 12, 13, 14, 15);
}

/* SzModel_ExSort_Pipe (branch coverage): blur mode 0 / 1 / 2, no centre, negative maximum depth,
 * sample below and at or above the maximum depth. */
static void check_exsort(void)
{
    /* no centre: the caller's cursor comes back; mode 0 draws the blur first */
    assert(run(0, 0, 3, 0.0f) == 5 && gSzModel_pfhCache == 0 && blur_calls == 1 && blur_index == 9);
    assert(draw_cursor == 1227 && draw_args[0] == 9 && draw_args[1] == 11 && draw_args[2] == 12);
    assert(draw_args[3] == 13 && draw_args[4] == 14 && draw_args[5] == 15);
    assert(clamp_out == 11357 && clamp_value == 0x55 && sort_calls == 0 && center_p != NULL);
    /* mode 1: no blur, no radius; sample below the maximum depth keeps it */
    assert(run(1, 1, 30, 29.5f) == 999 && blur_calls == 0 && draw_cursor == 127 && clamp_out == 10257);
    assert(radius_calls == 0 && sample_dz == 0.0f && sample_p == center_p && conv_calls == 0);
    assert(sort_calls == 1 && sort_args[0] == 777777 && sort_args[1] == 5 && sort_args[2] == 4242 && sort_args[3] == 30);
    /* sample equal to the maximum depth is converted */
    assert(run(1, 1, 30, 30.0f) == 999 && conv_calls == 1 && sort_args[3] == 30);
    assert(run(1, 1, 30, 45.75f) == 999 && conv_calls == 1 && sort_args[3] == 45);
    /* negative maximum depth counts as 0 */
    assert(run(1, 1, -4, -1.0f) == 999 && conv_calls == 0 && sort_args[3] == 0);
    assert(run(1, 1, -4, 2.5f) == 999 && conv_calls == 1 && sort_args[3] == 2);
    /* mode 2: the sample is moved by half the radius */
    assert(run(2, 1, 30, 1.0f) == 999 && blur_calls == 0 && radius_calls == 1 && sample_dz == 4.0f && sort_args[3] == 30);
}

int main(void)
{
    check_exsort();
    return 0;
}
