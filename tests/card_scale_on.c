/* Synthetic host tests for the card_scale_on group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/card_scale_on.h"

static int standby, standby_card, standby_calls;
int func_001f5af0_card(int packed)
{
    standby_calls++;
    standby_card = packed;
    return standby;
}

/* The first call sees the vector before the function stores anything in it; the stub then
 * puts the "previous stack contents" there, so the host run is defined. */
static struct DotrCardScaleVec stack_contents, written[2];
static struct DotrCardScaleVec *seen[2];
static int scale_cards[2], scales;
int func_001f5d10_card(int packed, struct DotrCardScaleVec *v)
{
    assert(scales < 2);
    scale_cards[scales] = packed;
    seen[scales] = v;
    if (scales == 0) {
        *v = stack_contents;
    }
    written[scales++] = *v;
    return 1;
}

static void run(int ready, float x, float y, float z, float step)
{
    standby = ready;
    standby_calls = 0;
    scales = 0;
    stack_contents.x = x;
    stack_contents.y = y;
    stack_contents.z = z;
    stack_contents.w = 7.0f;
    assert(CARD_SCL_ON_1(0x205, step) == 0);
    assert(standby_calls == 1 && standby_card == 0x205);
}

int main(void)
{
    /* Not standing by: no scale calls. */
    run(0, 0.0f, 0.0f, 0.0f, 0.01f);
    assert(scales == 0);

    /* All components below the limit: each grows by step; w untouched. */
    run(1, 0.02f, 0.05f, 0.07f, 0.01f);
    assert(scales == 2 && seen[0] == seen[1] && scale_cards[0] == 0x205 && scale_cards[1] == 0x205);
    assert(written[1].x == 0.02f + 0.01f && written[1].y == 0.05f + 0.01f && written[1].z == 0.07f + 0.01f);
    assert(written[1].w == 7.0f);

    /* Each component above 0.1 alone is clamped to 0.1f. */
    run(-1, 0.095f, 0.0f, 0.0f, 0.01f);
    assert(written[1].x == 0.1f && written[1].y == 0.01f && written[1].z == 0.01f);
    run(1, 0.0f, 0.2f, 0.0f, 0.01f);
    assert(written[1].x == 0.01f && written[1].y == 0.1f && written[1].z == 0.01f);
    run(1, 0.0f, 0.0f, 0.3f, 0.01f);
    assert(written[1].x == 0.01f && written[1].y == 0.01f && written[1].z == 0.1f);

    /* Boundary: a sum of exactly 0.1f ends as 0.1f; a sum below it stays. */
    run(1, 0.1f, 0.09f, 0.0f, 0.0f);
    assert(written[1].x == 0.1f && written[1].y == 0.09f && written[1].z == 0.0f);

    /* A negative step shrinks without a lower limit. */
    run(1, 0.0f, 0.5f, 0.05f, -0.2f);
    assert(written[1].x == -0.2f && written[1].y == 0.1f && written[1].z == 0.05f - 0.2f);

    puts("card_scale_on: ok");
    return 0;
}
