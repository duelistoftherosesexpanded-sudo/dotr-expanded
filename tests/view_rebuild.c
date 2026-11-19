/* Synthetic host tests for the view_rebuild group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/view_rebuild.h"

struct DotrViewEntry gRhView_avl[3];
int gRhView_nStackIndex[1];

static int order[8], count;
static void *d5e0_args[3], *d8e0_m, *copy_args[2][2], *bc00_args[2];
static int d5e0_word, copy_counts[2], copies, d40_arg_null;
static float d8e0_f[9];

int RhBase_GetScreenCenter(int *a, int *b) { order[count++] = 1; *a = 10; *b = 20; return 0; }
int RhBase_GetZMinMax(int *a, int *b) { order[count++] = 2; *a = 640; *b = 448; return 0; }
void RhGeom_CreateCameraMatrix2(struct DotrViewMatrix *out, float *a, float *b, int c)
{
    order[count++] = 3;
    d5e0_args[0] = out; d5e0_args[1] = a; d5e0_args[2] = b; d5e0_word = c;
}
float RhBase_GetAspectY(int *out) { order[count++] = 4; d40_arg_null = out == 0; return 0.5f; }
void RhGeom_GetScreenMatrix(struct DotrViewMatrix *m, float a, float b, float c, float d, float e, float f, float g, float h, float i)
{
    order[count++] = 5;
    d8e0_m = m;
    d8e0_f[0] = a; d8e0_f[1] = b; d8e0_f[2] = c; d8e0_f[3] = d; d8e0_f[4] = e;
    d8e0_f[5] = f; d8e0_f[6] = g; d8e0_f[7] = h; d8e0_f[8] = i;
}
void *RhMem_CopyMemory16(void *destination, void *source, int n)
{
    order[count++] = 6;
    copy_args[copies][0] = destination; copy_args[copies][1] = source; copy_counts[copies] = n;
    copies++;
    return destination;
}
void RhGeom_MulMatrix(struct DotrViewMatrix *m, struct DotrViewMatrix *other) { order[count++] = 7; bc00_args[0] = m; bc00_args[1] = other; }

/* lRhView_ConstructMatrix (straight-line coverage): one call checking every forwarded argument, the
 * call order and the return value. */
static void check_rebuild(void)
{
    struct DotrViewEntry *e = &gRhView_avl[2];
    static const int want[8] = { 1, 2, 3, 4, 5, 6, 7, 6 };
    int word = 0x1234567, i;

    gRhView_nStackIndex[0] = 2;
    e->unknown_150 = 100.0f;
    e->unknown_154 = 200.0f;
    e->unknown_158 = 3.0f;
    e->unknown_15c = 8.0f;
    e->unknown_160 = 5.0f;
    e->unknown_164 = 64.0f;
    e->unknown_168 = 32.0f;
    e->unknown_16c = 7.0f;
    e->unknown_170 = 9.0f;
    memcpy((char *)&e->unknown_0c0 + 0x80, &word, 4);
    assert(lRhView_ConstructMatrix() == 1 && count == 8);
    for (i = 0; i < 8; i++) {
        assert(order[i] == want[i]);
    }
    assert(d5e0_args[0] == (void *)&e->unknown_040 && d5e0_args[1] == (void *)((char *)&e->unknown_0c0 + 0x40));
    assert(d5e0_args[2] == (void *)((char *)&e->unknown_0c0 + 0x50) && d5e0_word == word && d40_arg_null);
    assert(d8e0_m == (void *)&e->unknown_080 && d8e0_f[0] == 5.0f && d8e0_f[1] == 3.0f && d8e0_f[2] == 4.0f);
    assert(d8e0_f[3] == 78.0f && d8e0_f[4] == 204.0f && d8e0_f[5] == 640.0f && d8e0_f[6] == 448.0f);
    assert(d8e0_f[7] == 7.0f && d8e0_f[8] == 18.0f);
    assert(copy_args[0][0] == (void *)e && copy_args[0][1] == (void *)&e->unknown_080 && copy_counts[0] == 4);
    assert(bc00_args[0] == (void *)e && bc00_args[1] == (void *)&e->unknown_040);
    assert(copy_args[1][0] == (void *)&e->unknown_0c0 && copy_args[1][1] == (void *)e && copy_counts[1] == 4);
}

int main(void)
{
    check_rebuild();
    puts("view_rebuild: ok");
    return 0;
}
