/* Synthetic host tests for the shoki_obj group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/shoki_obj.h"

/* A work block followed by room for four more slots. */
static struct {
    struct DotrShokiWork w;
    struct DotrShokiObj more[4];
} area;

static int clears;
static void *cleared;

void func_00195108(void *a, int b)
{
    clears++;
    cleared = a;
    assert(b == 192);
    memset(a, 0, (size_t)b);
}

static struct DotrShokiObj *slot(int i)
{
    return (struct DotrShokiObj *)((char *)&area.w + 3168 + 192 * i);
}

static void fill(void)
{
    int i;

    memset(&area, 0x5a, sizeof area);
    for (i = 0; i < 5; i++) {
        slot(i)->id = (short)(100 + i);
    }
    clears = 0;
}

int main(void)
{
    struct DotrShokiObj *o;

    assert(sizeof(struct DotrShokiObj) == 192 && offsetof(struct DotrShokiObj, id) == 164);
    assert(offsetof(struct DotrShokiWork, obj) == 3168 && sizeof area == 3168 + 5 * 192);

    /* skip set: nothing is taken even with free slots. */
    fill();
    slot(0)->id = 0;
    assert(SHOKI_OBJ(&area.w, 9, 5, 1) == NULL && clears == 0 && slot(0)->id == 0);

    /* The first free slot is cleared and numbered. */
    fill();
    slot(2)->id = 0;
    slot(4)->id = 0;
    o = SHOKI_OBJ(&area.w, 9, 5, 0);
    assert(o == slot(2) && clears == 1 && cleared == slot(2));
    assert(o->id == 3 && o->unknown_00[0] == 0 && o->unknown_a6[25] == 0);
    assert(slot(4)->id == 0 && slot(3)->id == 103 && slot(3)->unknown_00[0] == 0x5a);
    o = SHOKI_OBJ(&area.w, 9, 5, 0);
    assert(o == slot(4) && o->id == 5 && clears == 2);

    /* All used, beyond count, or count 0: 0. */
    assert(SHOKI_OBJ(&area.w, 9, 5, 0) == NULL && clears == 2);
    fill();
    slot(3)->id = 0;
    assert(SHOKI_OBJ(&area.w, 9, 3, 0) == NULL && clears == 0);
    assert(SHOKI_OBJ(&area.w, 9, 0, 0) == NULL && SHOKI_OBJ(&area.w, 9, -1, 0) == NULL);
    o = SHOKI_OBJ(&area.w, 9, 4, 0);
    assert(o == slot(3) && o->id == 4);
    puts("shoki_obj: ok");
    return 0;
}
