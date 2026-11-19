/* Synthetic host tests for the tick_bytes group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/tick_bytes.h"

/* 0x1f53a0 answers 0x53a0 and 0x1f5380 answers 0x5380; 0x1f5620 records its
 * arguments and counts its calls. */
static int send_calls, send_packed, send_value;
static struct DotrObjectVector4 *send_v;

int SzEffect_GetFrom(void) { return 0x53a0; }
int SzEffect_GetTarget(void) { return 0x5380; }
int SzEffect_GetRef2(int packed, int value, struct DotrObjectVector4 *v)
{
    send_calls++;
    send_packed = packed, send_value = value, send_v = v;
    return 0x77;
}

/* Branch coverage: one assertion per outcome and per bound, for both copies. */
static void check_step(int (*step)(unsigned char *))
{
    unsigned char p[4];

    p[0] = 0, p[1] = 5, p[2] = 9, p[3] = 0xaa;
    assert(step(p) == 0 && p[0] == 0 && p[1] == 5);
    p[0] = 3, p[1] = 2;
    assert(step(p) == 0 && p[0] == 3 && p[1] == 1);
    assert(step(p) == 0 && p[0] == 3 && p[1] == 0);
    assert(step(p) == 1 && p[0] == 2 && p[1] == 0);
    p[0] = 1;
    assert(step(p) == 1 && p[0] == 0 && p[1] == 0);
    assert(step(p) == 0 && p[0] == 0);
    p[0] = 0xff, p[1] = 0xff;
    assert(step(p) == 0 && p[0] == 0xff && p[1] == 0xfe);
    assert(p[2] == 9 && p[3] == 0xaa);
}

/* Straight-line coverage: every stored value in one call. */
static void check_set(void)
{
    unsigned char p[4] = { 1, 2, 3, 0xaa };

    ShFx_initPartsTimer(p, 0x134, 0x256);
    assert(p[0] == 0x34 && p[1] == 0x56 && p[2] == 0x34 && p[3] == 0xaa);
}

/* Branch coverage: each mode, and a mode outside the switch. */
static void check_send(void)
{
    struct DotrTickSource source;
    struct DotrObjectVector4 *v = (struct DotrObjectVector4 *)&source;

    memset(&source, 0, sizeof source);
    source.value = 0xc8;
    send_calls = 0;
    source.mode = 0;
    ShFx_AdjustPoint(&source, v);
    assert(send_calls == 0);
    source.mode = 1;
    ShFx_AdjustPoint(&source, v);
    assert(send_calls == 1 && send_packed == 0x53a0 && send_value == 0xc8 && send_v == v);
    source.mode = 2;
    ShFx_AdjustPoint(&source, v);
    assert(send_calls == 2 && send_packed == 0x5380 && send_value == 0xc8 && send_v == v);
    source.mode = 3;
    ShFx_AdjustPoint(&source, v);
    source.mode = 0xff;
    ShFx_AdjustPoint(&source, v);
    assert(send_calls == 2);
}

/* Loop coverage: no entry, first, middle and last hit, no hit, stride. */
static void check_find(void)
{
    unsigned char p[12];

    memset(p, 1, sizeof p);
    assert(ShFx_GetTempIndex(p, 1, 0) == -1);
    assert(ShFx_GetTempIndex(p, 1, -1) == -1);
    assert(ShFx_GetTempIndex(p, 1, 12) == -1);
    p[0] = 0;
    assert(ShFx_GetTempIndex(p, 1, 12) == 0);
    assert(ShFx_GetTempIndex(p, 1, 0) == -1);
    p[0] = 1, p[11] = 0;
    assert(ShFx_GetTempIndex(p, 1, 12) == 11);
    assert(ShFx_GetTempIndex(p, 1, 11) == -1);
    p[11] = 1, p[6] = 0, p[7] = 0;
    assert(ShFx_GetTempIndex(p, 1, 12) == 6);
    assert(ShFx_GetTempIndex(p, 3, 4) == 2);
    assert(ShFx_GetTempIndex(p, 4, 3) == -1);
    assert(ShFx_GetTempIndex(p, 7, 2) == 1);
}

int main(void)
{
    check_step(ShFx_isTimer);
    check_step(ShFx_isPartsTimer);
    check_set();
    check_send();
    check_find();
    return 0;
}
