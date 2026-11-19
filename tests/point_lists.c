/* Synthetic host tests for the point_lists group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/point_lists.h"

struct DotrPointBlock dotr_point_block_01ed0000;
struct DotrPointList root_no[2][16];

static int calls;

static int count_for(int n)
{
    /* list 0 is empty, list 1 has one point, list 31 is full */
    return n == 31 ? 16 : n % 7;
}

void Taku_spline2(float *a, float *b, short *c, float (*points)[2], float *weights, int count, short e, float scale)
{
    struct DotrPointOutput *output = &dotr_point_block_01ed0000.outputs[calls / 16][calls % 16];
    int k;

    assert(a == output->unknown_000 && b == output->unknown_100 && c == &output->unknown_200);
    assert(count == count_for(calls) && e == 1000 + calls && scale == 4.0f);
    for (k = 0; k < count; k++) {
        assert(points[k][0] == (float)(calls * 100 + k) && points[k][1] == (float)-(calls * 100 + k));
        assert(weights[k] == 1.0f);
    }
    calls++;
}

/* Loop coverage: every one of the 32 calls is checked in order, with counts of 0, 1 and 16. */
static void check_lists(void)
{
    int n, k;

    assert(sizeof(struct DotrPointList) == 76 && sizeof(struct DotrPointOutput) == 516);
    assert(offsetof(struct DotrPointBlock, outputs) == 556);
    for (n = 0; n < 32; n++) {
        struct DotrPointList *list = &root_no[n / 16][n % 16];

        list->count = (short)count_for(n);
        list->unknown_42 = (short)(1000 + n);
        for (k = 0; k < 16; k++) {
            list->points[k][0] = (short)(n * 100 + k);
            list->points[k][1] = (short)-(n * 100 + k);
        }
    }
    Taku_spline_init();
    assert(calls == 32);
}

int main(void)
{
    check_lists();
    puts("point_lists: ok");
    return 0;
}
