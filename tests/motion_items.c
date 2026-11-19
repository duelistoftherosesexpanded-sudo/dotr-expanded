/* Synthetic host tests for the motion_items group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/motion_items.h"

struct DotrMotionPad dotr_motion_pad;

/* Five keys at times 0, 10 .. 40 and a sentinel: the search reads the time of the key after
 * the last one. Values are linear in time, so an interpolated value is known exactly. */
static struct DotrMotionKey pos_keys[6], scale_keys[6];
static struct DotrMotionAngleKey rot_keys[6];
static int angle_calls, complex_calls;
static int *complex_args[3];
static float complex_rates[2];

void *RhMem_CopyMemory16(void *destination, void *source, int n)
{
    memcpy(destination, source, (size_t)n * 16);
    return destination;
}

int *RhMem_CopyMemory4(int *destination, int *source, int n)
{
    memcpy(destination, source, (size_t)n * 4);
    return destination;
}

int RhGeom_RotationInterpolate(float first_rate, float second_rate, int first, int second)
{
    angle_calls++;
    return (int)(first_rate * (float)first + second_rate * (float)second);
}

void RhGeom_RoationComplexInterpolate(int *out, int *first, int *second, float first_rate, float second_rate)
{
    complex_calls++;
    complex_args[0] = out; complex_args[1] = first; complex_args[2] = second;
    complex_rates[0] = first_rate; complex_rates[1] = second_rate;
}

static void fill_keys(void)
{
    int i, j;

    for (i = 0; i < 6; i++) {
        pos_keys[i].time = scale_keys[i].time = rot_keys[i].time = i < 5 ? (unsigned int)i * 10 : 1000;
        for (j = 0; j < 3; j++) {
            pos_keys[i].value[j] = (float)(i * 10 + j);
            scale_keys[i].value[j] = (float)(i * 20 + j);
            rot_keys[i].value[j] = i * 100 + j;
        }
    }
}

/* One call with all three tracks present and `count` keys each; `at` is the key time the
 * result must correspond to (the frame when interpolated, a key's own time when copied). */
static void check_item(float frame, unsigned int count, float at, int interpolated)
{
    struct DotrMotionItem item;
    float position[3], scale[3];
    int rotation[3], j;

    item.keys[0] = pos_keys; item.keys[1] = (struct DotrMotionKey *)rot_keys; item.keys[2] = scale_keys;
    item.counts[0] = item.counts[1] = item.counts[2] = count;
    dotr_motion_pad.flags = 4;
    angle_calls = 0;
    assert(lRhModel_GetInterpolateMotionItem(&item, frame, 0xff, position, rotation, scale, 2.0f) == 0xf8);
    assert(angle_calls == (interpolated ? 3 : 0));
    for (j = 0; j < 3; j++) {
        assert(position[j] == (at + (float)j) * 2.0f); /* only the position is multiplied by the rate */
        assert(rotation[j] == (int)at * 10 + j);
        assert(scale[j] == at * 2.0f + (float)j);
    }
}

static void check_items(void)
{
    struct DotrMotionItem item;
    struct DotrMotionShortItem brief;
    float position[3] = { -1.0f, -1.0f, -1.0f }, scale[3] = { -1.0f, -1.0f, -1.0f };
    int rotation[3] = { -1, -1, -1 };

    check_item(25.0f, 5, 25.0f, 1); /* first probe: below the next key, not below this one */
    check_item(5.0f, 5, 5.0f, 1);   /* upper bound moves down twice; the loop ends on low == mid */
    check_item(35.0f, 5, 35.0f, 1); /* lower bound moves up once */
    check_item(45.0f, 5, 40.0f, 0); /* past the last key: no following key, the last one is copied */
    check_item(25.0f, 2, 0.0f, 0);  /* fewer than three keys: the first one is copied */
    check_item(25.0f, 3, 20.0f, 0); /* three keys: the search ends on the last one, which is copied */

    /* no tracks: flags and outputs stay */
    memset(&item, 0, sizeof(item));
    dotr_motion_pad.flags = 4;
    assert(lRhModel_GetInterpolateMotionItem(&item, 25.0f, 0xff, position, rotation, scale, 2.0f) == 0xff);
    assert(position[0] == -1.0f && rotation[0] == -1 && scale[0] == -1.0f);

    /* pad bit 2 clear: counts follow two pointers and the scale is not touched */
    brief.keys[0] = pos_keys; brief.keys[1] = (struct DotrMotionKey *)rot_keys;
    brief.counts[0] = 5; brief.counts[1] = 2;
    dotr_motion_pad.flags = 0xfffb;
    angle_calls = 0;
    assert(lRhModel_GetInterpolateMotionItem(&brief, 25.0f, 0xff, position, rotation, scale, 1.0f) == 0xfc);
    assert(position[0] == 25.0f && position[2] == 27.0f && rotation[0] == 0 && rotation[2] == 2);
    assert(angle_calls == 0 && scale[0] == -1.0f && scale[2] == -1.0f);
}

/* 0x2118f0, same fixture as check_item: `at` is the key time the result must correspond to. */
static void check_pulse_item(unsigned int frame, unsigned int count, float at, int interpolated)
{
    struct DotrMotionItem item;
    float position[3], scale[3];
    int rotation[3], j;

    item.keys[0] = pos_keys; item.keys[1] = (struct DotrMotionKey *)rot_keys; item.keys[2] = scale_keys;
    item.counts[0] = item.counts[1] = item.counts[2] = count;
    dotr_motion_pad.flags = 4;
    angle_calls = 0;
    assert(lRhModel_GetPulseMotionItem(&item, frame, 0xff, position, rotation, scale, 2.0f) == 0xf8);
    assert(angle_calls == (interpolated ? 3 : 0));
    for (j = 0; j < 3; j++) {
        assert(position[j] == (at + (float)j) * 2.0f); /* only the position is multiplied by the rate */
        assert(rotation[j] == (int)at * 10 + j);
        assert(scale[j] == at * 2.0f + (float)j);
    }
}

static void check_pulse_items(void)
{
    struct DotrMotionItem item;
    struct DotrMotionShortItem brief;
    float position[3] = { -1.0f, -1.0f, -1.0f }, scale[3] = { -1.0f, -1.0f, -1.0f };
    int rotation[3] = { -1, -1, -1 };

    check_pulse_item(25, 5, 25.0f, 1); /* first probe: below the next key, not below this one */
    check_pulse_item(5, 5, 5.0f, 1);   /* upper bound moves down twice; the loop ends on low == mid */
    check_pulse_item(35, 5, 35.0f, 1); /* lower bound moves up once */
    check_pulse_item(20, 5, 20.0f, 0); /* exactly on a key with a following key: copied */
    check_pulse_item(45, 5, 40.0f, 0); /* past the last key: no following key, the last one is copied */
    check_pulse_item(25, 2, 0.0f, 0);  /* fewer than three keys: the first one is copied */
    check_pulse_item(25, 3, 20.0f, 0); /* three keys: the search ends on the last one, which is copied */

    /* no tracks: flags and outputs stay */
    memset(&item, 0, sizeof(item));
    dotr_motion_pad.flags = 4;
    assert(lRhModel_GetPulseMotionItem(&item, 25, 0xff, position, rotation, scale, 2.0f) == 0xff);
    assert(position[0] == -1.0f && rotation[0] == -1 && scale[0] == -1.0f);

    /* pad bit 2 clear: counts follow two pointers and the scale is not touched */
    brief.keys[0] = pos_keys; brief.keys[1] = (struct DotrMotionKey *)rot_keys;
    brief.counts[0] = 5; brief.counts[1] = 2;
    dotr_motion_pad.flags = 0xfffb;
    angle_calls = 0;
    assert(lRhModel_GetPulseMotionItem(&brief, 25, 0xff, position, rotation, scale, 1.0f) == 0xfc);
    assert(position[0] == 25.0f && position[2] == 27.0f && rotation[0] == 0 && rotation[2] == 2);
    assert(angle_calls == 0 && scale[0] == -1.0f && scale[2] == -1.0f);
}

/* Branch coverage for 0x2106e0 through the real 0x210cf0: bit 2 of the result clear (both
 * items have a scale track) and set (neither has). */
static void check_multi(int scaled)
{
    struct DotrMotionItem first, second;
    float position[3], scale[3] = { -1.0f, -1.0f, -1.0f };
    int rotation[4], i;

    memset(&first, 0, sizeof(first));
    memset(&second, 0, sizeof(second));
    first.keys[0] = pos_keys; first.counts[0] = 5;
    second.keys[0] = scale_keys; second.counts[0] = 5;
    if (scaled) {
        first.keys[2] = scale_keys; first.counts[2] = 5;
        second.keys[2] = pos_keys; second.counts[2] = 5;
    }
    for (i = 0; i < 4; i++) {
        dotr_motion_pad.initial.scale[i] = dotr_motion_pad.initial.position[i] = 9.0f;
        dotr_motion_pad.initial.rotation[i] = 7;
    }
    memset(&dotr_motion_pad.first, 0, sizeof(dotr_motion_pad.first));
    memset(&dotr_motion_pad.second, 0, sizeof(dotr_motion_pad.second));
    dotr_motion_pad.flags = 4;
    complex_calls = 0;
    assert(lRhModel_GetMultiMotionItem(&first, &second, 25.0f, 2.0f, 5.0f, 0.5f, 0x27, position, rotation, scale, 1.0f)
           == (scaled ? 0x22u : 0x26u));
    /* first item at frame 25 gives 25 + j, second at frame 5 gives 10 + j */
    assert(position[0] == 55.0f && position[1] == 57.5f && position[2] == 60.0f);
    /* no rotation tracks: both work sets still hold the copied initial rotation */
    assert(dotr_motion_pad.first.rotation[0] == 7 && dotr_motion_pad.second.rotation[3] == 7);
    assert(complex_calls == 1 && complex_args[0] == rotation);
    assert(complex_args[1] == dotr_motion_pad.first.rotation && complex_args[2] == dotr_motion_pad.second.rotation);
    assert(complex_rates[0] == 2.0f && complex_rates[1] == 0.5f);
    if (scaled) {
        assert(scale[0] == 102.5f && scale[1] == 105.0f && scale[2] == 107.5f);
    } else {
        assert(scale[0] == -1.0f && scale[1] == -1.0f && scale[2] == -1.0f);
        assert(dotr_motion_pad.first.scale[0] == 9.0f && dotr_motion_pad.second.scale[2] == 9.0f);
    }
}

int main(void)
{
    fill_keys();
    check_items();
    check_pulse_items();
    check_multi(1);
    check_multi(0);
    puts("motion_items: ok");
    return 0;
}
