/* Synthetic host tests for the unit_turn group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/unit_turn.h"

static struct DotrStateItem unit, other;
static struct DotrStateItem *unit_answer, *map_answer;
static int unit_args[2], map_args[2], dir1_args[2], dir1_calls, atan_calls;
static float atan_args[2], atan_result;

struct DotrStateItem *SzDuel_GetUnit(int a, int b)
{
    unit_args[0] = a, unit_args[1] = b;
    return unit_answer;
}
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    map_args[0] = x, map_args[1] = y;
    return map_answer;
}
/* Truncates; the game's routine is in the library block. */
int func_001a22c0(float value)
{
    return (int)value;
}
float func_001a6c18(float a, float b)
{
    atan_args[0] = a, atan_args[1] = b;
    atan_calls++;
    return atan_result;
}
int SzDuel_TrimUnitDir1(int a, int b)
{
    dir1_args[0] = a, dir1_args[1] = b;
    dir1_calls++;
    return 77;
}

/* One call with the unit on cell (3, 3), offset (10, -20), toward cell (5, 1); `turn` is
 * the stubbed angle as a fraction of a turn (powers of two, so the scaling is exact). */
static int run(float turn, int flip, int dir)
{
    memset(&unit, 0, sizeof(unit));
    unit.unknown_30 = dir;
    unit.locCol = 3;
    unit.locRow = 3;
    unit.team = flip;
    DOTR_UNIT_OFFSET_X(&unit) = 10;
    DOTR_UNIT_OFFSET_Y(&unit) = -20;
    unit_answer = &unit;
    atan_result = 6.2831855f * turn;
    atan_calls = dir1_calls = 0;
    return SzDuel_TrimUnitDir2(6, 7, 5, 1);
}

/* SzDuel_TrimUnitDir2 branch coverage: the same unit on the cell, a missing unit, the cell
 * empty and occupied, each side of every bound of the difference (-32767, 0x8000, 0, 2048,
 * -2048), the final wrap above 0x8000, the half-turn byte and the rounding step. */
int main(void)
{
    /* the unit itself stands there (also when both are missing): forwarded */
    unit_answer = map_answer = &unit;
    assert(SzDuel_TrimUnitDir2(6, 7, 5, 1) == 77 && dir1_calls == 1 && dir1_args[0] == 6 && dir1_args[1] == 7);
    assert(unit_args[0] == 6 && unit_args[1] == 7 && map_args[0] == 5 && map_args[1] == 1);
    unit_answer = map_answer = 0;
    assert(SzDuel_TrimUnitDir2(1, 2, 3, 4) == 77 && dir1_calls == 2);
    /* no unit: nothing to turn */
    map_answer = &other;
    dir1_calls = atan_calls = 0;
    assert(SzDuel_TrimUnitDir2(6, 7, 5, 1) == 0 && dir1_calls == 0 && atan_calls == 0);

    /* empty cell: the target is the cell centre (100, -100); the unit is at (10, -20) */
    map_answer = 0;
    assert(run(0.25f, 0, 0) == 1 && atan_calls == 1 && atan_args[0] == -80.0f && atan_args[1] == 90.0f);
    /* a quarter turn against direction 0: difference -32768 wraps to 32768, step 2048 */
    assert(unit.unknown_30 == 2048 && dir1_calls == 0);
    /* occupied cell: its unit's offset moves the target */
    DOTR_UNIT_OFFSET_X(&other) = 1;
    DOTR_UNIT_OFFSET_Y(&other) = 2;
    map_answer = &other;
    assert(run(0.25f, 0, 0) == 1 && atan_args[0] == -78.0f && atan_args[1] == 91.0f);
    map_answer = 0;
    /* a difference above 0x8000 (49152) wraps to -16384: step -2048, and the wrap of the sum */
    assert(run(0.0f, 0, -0x20000) == 1 && unit.unknown_30 == -2048);
    /* already there */
    assert(run(0.25f, 0, -32768) == 0 && unit.unknown_30 == -32768);
    /* 1024 and -1024 are taken whole; 32768 is kept, not wrapped */
    assert(run(0.25f, 0, -33792) == 1 && unit.unknown_30 == 32768);
    assert(run(0.25f, 0, -31744) == 1 && unit.unknown_30 == 32768);
    /* a result above 0x8000 comes back by a whole turn */
    assert(run(0.25f, 0, 40000) == 1 && unit.unknown_30 == 37952 - 0x10000);
    assert(run(0.0f, 0, 0) == 1 && unit.unknown_30 == -2048);
    /* the byte at +0x44 adds half a turn */
    assert(run(0.0f, 1, 0) == 1 && unit.unknown_30 == 2048);
    /* 1/64 turn (1024) rounds up to 2048, 1/128 turn (512) down to 0 */
    assert(run(1.0f / 64, 0, -19432) == 1 && unit.unknown_30 == -18432);
    assert(run(1.0f / 128, 0, -19432) == 1 && unit.unknown_30 == -17384);
    return 0;
}
