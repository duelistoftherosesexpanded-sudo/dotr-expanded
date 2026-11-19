/* Synthetic host tests for the object_seeds group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/object_seeds.h"

DotrSeedRecord gMain_ModelData[84];
static int log_[4096], log_count, fail_at;
static void put(int v) { log_[log_count++] = v; }
int SzModel_Create(int index) { put(0xb20); put(index); return index != fail_at; }
int SzModel_SetPos(int index, struct DotrSeedBlock *in) { put(0x2b0); put(index); put((int)(in - &gMain_ModelData[0].vector)); return 0; }
int SzModel_SetRot(int index, struct DotrSeedBlock *in) { put(0x430); put(index); put((int)(in - &gMain_ModelData[0].words)); return 0; }
int SzModel_Preset(int index, int value, int unknown) { put(0xe70); put(index); put(value); put(unknown); return 0; }
int SzDuel_Xax2New(int value) { put(0x130); put(value); return value + 1000; }
void SzDuel_AddUnit(int a, int b, int c, int d, int e) { put(0x580); put(a); put(b); put(c); put(d); put(e); }
int SzDuel_SetUnitStatus(int a, int b, int bit, int value) { put(0x1e7); put(a); put(b); put(bit); put(value); return 0; }
int SzDuel_SetUnitOpenRate(int a, int b, int pair, int value) { put(0x1d7); put(a); put(b); put(pair); put(value); return 0; }
int SzDuel_SetUnitCtlRateM(int a, int b, int pair, int value) { put(0x1c1); put(a); put(b); put(pair); put(value); return 0; }

/* Finds the 0x580 call that follows the 0xb20 call for `index` and returns its five arguments. */
static const int *record_call(int index)
{
    int i;

    for (i = 0; i + 1 < log_count; i++) {
        if (log_[i] == 0xb20 && log_[i + 1] == index && log_[i + 12] == 0x130) {
            return &log_[i + 14];
        }
    }
    return 0;
}

static void check_seed_walk(void)
{
    const int *c;
    int i;

    for (i = 0; i < 84; i++) {
        gMain_ModelData[i].kind = 0;
        gMain_ModelData[i].value = i + 100;
        gMain_ModelData[i].unknown_08 = i + 200;
        gMain_ModelData[i].unknown_0c = i + 300;
    }
    gMain_ModelData[40].kind = 5;
    gMain_ModelData[24].kind = 4;
    gMain_ModelData[25].kind = 4;

    /* A zero from 0x1e7b20 stops the walk at once with 0. */
    fail_at = 0;
    log_count = 0;
    assert(lMain_InitModel2() == 0 && log_count == 2 && log_[0] == 0xb20 && log_[1] == 0);
    fail_at = 2;
    log_count = 0;
    assert(lMain_InitModel2() == 0 && log_count == 2 * 12 + 2);

    /* Full walk: 84 records of 12 log words, plus 23 more for each kind 4 / 5 record. */
    fail_at = -1;
    log_count = 0;
    assert(lMain_InitModel2() == 1 && log_count == 84 * 12 + 3 * 23);
    {
        static const int want[] = {0xb20, 0, 0x2b0, 0, 0, 0x430, 0, 0, 0xe70, 0, 0, 100, 0xb20, 1, 0x2b0, 1, 3};
        assert(memcmp(log_, want, sizeof(want)) == 0);
    }
    /* kind 5: first argument index - 30, last 1, then the three grid calls. */
    c = record_call(40);
    assert(c && c[0] == 0x580 && c[1] == 10 && c[2] == 240 && c[3] == 340 && c[4] == 1140 && c[5] == 1);
    {
        static const int want[] = {0x1e7, -1, 40, 4, 1, 0x1d7, -1, 40, 0, 255, 0x1c1, -1, 40, 1, 0};
        assert(memcmp(c + 6, want, sizeof(want)) == 0);
    }
    /* kind 4: first argument 0 for index 24 (index - 20 < 5), 1 for index 25; last 0. */
    c = record_call(24);
    assert(c && c[0] == 0x580 && c[1] == 0 && c[2] == 224 && c[3] == 324 && c[4] == 1124 && c[5] == 0);
    {
        static const int want[] = {0x1e7, -1, 24, 4, 1, 0x1d7, -1, 24, 0, 255, 0x1c1, -1, 24, 1, 0};
        assert(memcmp(c + 6, want, sizeof(want)) == 0);
    }
    c = record_call(25);
    assert(c && c[0] == 0x580 && c[1] == 1 && c[5] == 0);
    /* Other kinds make no further call. */
    assert(record_call(41) == 0);
}

int main(void)
{
    check_seed_walk();
    return 0;
}
