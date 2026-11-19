/* Synthetic dependency results; does not execute the original lookup/scorers. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/state_compare.h"

static struct DotrStateCompareReference references[2];
static int null_first, null_second, calls, getter_calls;
static int expected_a0, scores[3];

struct DotrStateCompareReference *SzDuel_GetUnit(int first, int second)
{
    int which = calls++;
    assert(which == 0 || which == 1);
    assert(first == (which == 0 ? expected_a0 : 22));
    assert(second == (which == 0 ? 11 : 33));
    return (which == 0 ? null_first : null_second) ? 0 : &references[which];
}

int Ta_GetParamOfs(struct DotrStateCompareReference *reference,
                  int first, int second, int mode)
{
    int which = calls++ - 2;
    assert(which >= 0 && which < 3);
    assert(reference == &references[which == 0 ? 0 : 1]);
    assert(first == 44 && second == 55 && mode == (which < 2));
    return scores[which] - 10;
}

int SzDuel_GetUnitAP2(struct DotrStateCompareReference *reference, int unknown)
{
    int which = calls++ - 5;
    assert(which == 0 || which == 1);
    assert(reference == &references[which] && unknown == 0);
    return 10;
}

int SzDuel_GetUnitDP2(struct DotrStateCompareReference *reference, int unknown)
{
    assert(calls++ == 7 && reference == &references[1] && unknown == 0);
    return 10;
}

int SzDuel_GetTurnOwner(void)
{
    assert(calls++ == 8);
    getter_calls++;
    return 7;
}

static int bounded(int value)
{
    if (value < 0) return 0;
    if (value > 9999) return 9999;
    return value;
}

int main(void)
{
    static const int values[] = {-10000, -1, 0, 1, 9998, 9999, 10000, 20000};
    unsigned char before[sizeof(references)];
    size_t ix, iy, iz;
    int flag, different_index, guard, expected, x, y, z;
    assert(offsetof(struct DotrStateCompareReference, unknown_57) == 0x57);
    memset(references, 0xa5, sizeof(references));
    expected_a0 = 7;
    for (null_first = 0; null_first < 2; null_first++) {
        for (null_second = 0; null_second < 2; null_second++) {
            if (!null_first && !null_second) continue;
            calls = getter_calls = 0;
            memcpy(before, references, sizeof(before));
            assert(SzDuel_GetPlayMotion(7, 11, 22, 33, 44, 55, 0) == 0);
            assert(calls == 2 && getter_calls == 0);
            assert(memcmp(before, references, sizeof(before)) == 0);
        }
    }
    null_first = null_second = 0;
    for (ix = 0; ix < sizeof(values) / sizeof(values[0]); ix++) {
        for (iy = 0; iy < sizeof(values) / sizeof(values[0]); iy++) {
            for (iz = 0; iz < sizeof(values) / sizeof(values[0]); iz++) {
                scores[0] = values[ix]; scores[1] = values[iy]; scores[2] = values[iz];
                x = bounded(scores[0]); y = bounded(scores[1]); z = bounded(scores[2]);
                for (flag = 0; flag < 256; flag++) {
                    ((unsigned char *)&references[1])[0x57] = (unsigned char)flag;
                    for (different_index = 0; different_index < 2; different_index++) {
                        expected_a0 = different_index ? 8 : 7;
                        for (guard = 0; guard < 2; guard++) {
                            int should_query = !(flag & 8) && x == y && guard == 0;
                            expected = (flag & 8) ? (z >= x ? 6 : 4)
                                : y > x || (y == x && guard == 0 && !different_index) ? 3 : 4;
                            calls = getter_calls = 0;
                            memcpy(before, references, sizeof(before));
                            assert(SzDuel_GetPlayMotion(expected_a0, 11, 22, 33, 44, 55, guard) == expected);
                            assert(calls == 8 + should_query && getter_calls == should_query);
                            assert(memcmp(before, references, sizeof(before)) == 0);
                        }
                    }
                }
            }
        }
    }
    puts("state_compare: ok");
    return 0;
}
