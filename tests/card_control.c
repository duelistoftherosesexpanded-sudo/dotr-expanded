/* Synthetic host tests for the card_control group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/card_control.h"

static float a22c0_args[16];
static int a22c0_calls;
int func_001a22c0(float value)
{
    a22c0_args[a22c0_calls++] = value;
    return (int)value;
}

static void fill_control(struct DotrCardControl *c)
{
    int i;

    memset(c, 0, sizeof(*c));
    c->total = 4;
    c->count = 1;
    for (i = 0; i < 4; i++) {
        c->pos[0][i] = 8.0f * (i + 1); c->pos[1][i] = 16.0f * (i + 1);
        c->scale[0][i] = 4.0f * (i + 1); c->scale[1][i] = -4.0f * (i + 1);
        c->color[0][i] = 16 * (i + 1); c->color[1][i] = 200 + 4 * i;
    }
    /* below, at and above the 16-bit range, and an exact negative multiple of 0x10000 */
    c->rot[0][0] = 16; c->rot[1][0] = 32;
    c->rot[0][1] = 0x10000; c->rot[1][1] = 0x10000;
    c->rot[0][2] = 0x20000; c->rot[1][2] = 0x20010;
    c->rot[0][3] = -0x10000; c->rot[1][3] = -0x10000;
    c->rot2[0][0] = 0; c->rot2[1][0] = 0xfffc;
    c->rot2[0][1] = 0x30004; c->rot2[1][1] = 0x30004;
    c->rot2[0][2] = 400; c->rot2[1][2] = 800;
    c->rot2[0][3] = -0x20000; c->rot2[1][3] = -0x20000;
}

/* 0x1cdeb0 branch coverage: null target, each channel flag alone, none and all; the modulo with values
 * below, at and above 0x10000 and at negative multiples (a negative non-multiple is not asserted: the
 * original machine code and host C differ there, see the source comment); a full-range step count. */
static void check_card_control(void)
{
    struct DotrCardControl c;
    struct DotrCardControlOut out, want;
    int mask, i;

    fill_control(&c);
    c.do_pos = c.do_rot = c.do_rot2 = c.do_scale = c.do_color = -1;
    a22c0_calls = 0;
    assert(SzDuel_DoCardControl0(&c) == 0 && a22c0_calls == 0);
    for (mask = 0; mask < 32; mask++) {
        if (mask != 0 && mask != 31 && (mask & (mask - 1)) != 0) continue;
        fill_control(&c);
        c.out = &out;
        c.do_pos = (mask & 1) ? -1 : 0;
        c.do_rot = (mask & 2) ? -1 : 0;
        c.do_rot2 = (mask & 4) ? -1 : 0;
        c.do_scale = (mask & 8) ? -1 : 0;
        c.do_color = (mask & 16) ? -1 : 0;
        memset(&out, 0x5a, sizeof(out));
        want = out;
        a22c0_calls = 0;
        assert(SzDuel_DoCardControl0(&c) == 1);
        if (mask & 1) for (i = 0; i < 4; i++) want.pos[i] = 8.0f * (i + 1) * 0.75f + 16.0f * (i + 1) * 0.25f;
        if (mask & 2) { want.rot[0] = 20; want.rot[1] = 0; want.rot[2] = 4; want.rot[3] = 0; }
        if (mask & 4) { want.rot2[0] = 0x3fff; want.rot2[1] = 4; want.rot2[2] = 500; want.rot2[3] = 0; }
        if (mask & 8) for (i = 0; i < 4; i++) want.scale[i] = 4.0f * (i + 1) * 0.75f + -4.0f * (i + 1) * 0.25f;
        if (mask & 16) for (i = 0; i < 4; i++) want.color[i] = (unsigned char)(12 * (i + 1) + 50 + i);
        assert(memcmp(&out, &want, sizeof(out)) == 0);
        assert(a22c0_calls == ((mask & 2) ? 4 : 0) + ((mask & 4) ? 4 : 0) + ((mask & 16) ? 4 : 0));
        if (mask == 2) assert(a22c0_args[0] == 20.0f && a22c0_args[2] == 131076.0f && a22c0_args[3] == -65536.0f);
        if (mask == 16) {
            /* the bytes are converted in the order 2, 1, 0, 3 */
            assert(a22c0_args[0] == 88.0f && a22c0_args[1] == 75.0f && a22c0_args[2] == 62.0f && a22c0_args[3] == 101.0f);
        }
    }
    /* the step counts are unsigned halfwords: 0xffff of 0xffff is the end value */
    fill_control(&c);
    c.out = &out;
    c.total = 0xffff; c.count = 0xffff;
    c.do_pos = -1;
    assert(SzDuel_DoCardControl0(&c) == 1);
    for (i = 0; i < 4; i++) assert(out.pos[i] == 16.0f * (i + 1));
}

int main(void)
{
    check_card_control();
    puts("card_control: ok");
    return 0;
}
