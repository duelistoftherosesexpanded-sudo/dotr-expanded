/* Synthetic tests of the fan-out wrappers; every called routine is a stub. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/index_fanout.h"

unsigned char data_0031bea0[3];

static int calls, log_target[4], log_unknown[4], log_index[4], log_value[4];

static void record(int target, int unknown, int index, int value)
{
    assert(calls < 4);
    log_target[calls] = target;
    log_unknown[calls] = unknown;
    log_index[calls] = index;
    log_value[calls] = value;
    calls++;
}

void SzView_SetFogColor(int unknown, int index, int value) { record(0, unknown, index, value); }
void SzView_SetFogParamZ(int unknown, int index, int value) { record(1, unknown, index, value); }
void SzView_SetFogParamY(int unknown, int index, int value) { record(2, unknown, index, value); }
void SzView_SetStraightLight(int unknown, int index, int value) { record(3, unknown, index, value); }
void SzView_SetPointLight(int unknown, int index, int value) { record(4, unknown, index, value); }
void SzEffect_SetIntensity(int index, int value) { record(5, 0, index, value); }

static const struct {
    void (*wrapper)(int);
    int target;
    int count;
    int indices[3];
} cases[] = {
    {SzEffect_SetFogColorForMonster, 0, 2, {4, 5}},
    {SzEffect_SetFogColorForDuel, 0, 3, {0, 1, 8}},
    {SzEffect_SetFogColorForBattle, 0, 2, {2, 3}},
    {SzEffect_SetFogParamZForMonster, 1, 2, {4, 5}},
    {SzEffect_SetFogParamZForDuel, 1, 3, {0, 1, 8}},
    {SzEffect_SetFogParamZForBattle, 1, 2, {2, 3}},
    {SzEffect_SetFogParamYForMonster, 2, 2, {4, 5}},
    {SzEffect_SetFogParamYForDuel, 2, 3, {0, 1, 8}},
    {SzEffect_SetFogParamYForBattle, 2, 2, {2, 3}},
    {SzEffect_SetStraightLightForMonster, 3, 2, {4, 5}},
    {SzEffect_SetStraightLightForDuel, 3, 3, {0, 1, 8}},
    {SzEffect_SetStraightLightForBattle, 3, 2, {2, 3}},
    {SzEffect_SetPointLightForMonster, 4, 2, {4, 5}},
    {SzEffect_SetPointLightForDuel, 4, 3, {0, 1, 8}},
    {SzEffect_SetPointLightForBattle, 4, 2, {2, 3}},
    {SzEffect_SetIntensityForMonster, 5, 2, {4, 5}},
    {SzEffect_SetIntensityForDuel, 5, 3, {0, 1, 8}},
    {SzEffect_SetIntensityForBattle, 5, 2, {2, 3}},
};

static int step_case(int b0, int b1, int step, int dir)
{
    data_0031bea0[0] = b0;
    data_0031bea0[1] = b1;
    data_0031bea0[2] = 0x77;
    return TaTuto_ControlFade(step, dir);
}

static void check_step(void)
{
    /* dir > 0: each byte below 128 gains step (truncated to a byte). */
    assert(step_case(127, 0, 3, 1) == 0 && data_0031bea0[0] == 130 && data_0031bea0[1] == 3);
    assert(step_case(128, 127, 0x1ff, 5) == 0 && data_0031bea0[0] == 128 && data_0031bea0[1] == 126);
    assert(step_case(10, 128, 1, 1) == 0 && data_0031bea0[0] == 11 && data_0031bea0[1] == 128);
    assert(step_case(128, 255, 1, 1) == 1 && data_0031bea0[0] == 128 && data_0031bea0[1] == 255);
    /* dir < 0: each non-zero byte loses step, clamped at 0. */
    assert(step_case(5, 3, 4, -1) == 0 && data_0031bea0[0] == 1 && data_0031bea0[1] == 0);
    assert(step_case(4, 200, 4, -9) == 0 && data_0031bea0[0] == 0 && data_0031bea0[1] == 196);
    assert(step_case(0, 1, 4, -1) == 0 && data_0031bea0[0] == 0 && data_0031bea0[1] == 0);
    assert(step_case(1, 0, 1, -1) == 0 && data_0031bea0[0] == 0 && data_0031bea0[1] == 0);
    assert(step_case(0, 0, 4, -1) == 1 && data_0031bea0[0] == 0 && data_0031bea0[1] == 0);
    /* dir == 0 touches nothing and reports 0. */
    assert(step_case(0, 128, 4, 0) == 0 && data_0031bea0[0] == 0 && data_0031bea0[1] == 128);
    assert(data_0031bea0[2] == 0x77);
}

int main(void)
{
    static const int values[] = {0, 1, -1, 0x12345678};
    size_t c, v;
    int i;
    for (c = 0; c < sizeof(cases) / sizeof(cases[0]); c++) {
        for (v = 0; v < sizeof(values) / sizeof(values[0]); v++) {
            calls = 0;
            cases[c].wrapper(values[v]);
            /* Same target each time, indices in the listed order, the value
             * forwarded unchanged and a zero first argument. */
            assert(calls == cases[c].count);
            for (i = 0; i < calls; i++) {
                assert(log_target[i] == cases[c].target && log_unknown[i] == 0);
                assert(log_index[i] == cases[c].indices[i] && log_value[i] == values[v]);
            }
        }
    }
    memset(data_0031bea0, 0, sizeof(data_0031bea0));
    SzEffect_SetBlackOutForMonster(0x111);
    assert(data_0031bea0[0] == 0x11 && data_0031bea0[1] == 0 && data_0031bea0[2] == 0);
    SzEffect_SetBlackOutForDuel(0x222);
    assert(data_0031bea0[0] == 0x11 && data_0031bea0[1] == 0x22 && data_0031bea0[2] == 0);
    SzEffect_SetBlackOutForBattle(0xff);
    assert(data_0031bea0[0] == 0x11 && data_0031bea0[1] == 0x22 && data_0031bea0[2] == 0xff);
    assert(SzEffect_GetBlackOutForMonster() == 0x11 && SzEffect_GetBlackOutForDuel() == 0x22 && SzEffect_GetBlackOutForBattle() == 0xff);
    check_step();
    puts("index_fanout: ok");
    return 0;
}
