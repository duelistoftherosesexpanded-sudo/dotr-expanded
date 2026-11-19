/* Synthetic host tests for the cell_drops group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cell_drops.h"

int gSzBattle_bl[8];
struct DotrDropWords data_002921d0[2];

static struct DotrDropBlock block;
static struct DotrDropBlock *block_v;
static int log_v[96], log_n;
static int want_x, want_y, want_word, want_object, want_word1;

static void note(int id, int a, int b, int c, int d)
{
    assert(log_n + 5 <= 96);
    log_v[log_n++] = id;
    log_v[log_n++] = a;
    log_v[log_n++] = b;
    log_v[log_n++] = c;
    log_v[log_n++] = d;
}

struct DotrDropBlock *func_001df540_drop(int a, int b)
{
    note(0x1df540, a, b, 0, 0);
    return block_v;
}

int SzEffect_ClearRefID2(int packed, int value)
{
    note(0x1f56d0, packed, value, 0, 0);
    return 0;
}

int SzDuel_SetBusyFlag(int a, int b, int flag)
{
    note(0x1e3850, a, b, flag, 0);
    return 0;
}

int func_001f25d0_drop(int index, struct DotrDropVector *in)
{
    note(0x1f25d0, index, in == NULL, 0, 0);
    return 0;
}

int func_001f2430_drop(int index, struct DotrDropWords *in)
{
    /* the template words, with word 1 replaced */
    assert(in->words[0] == 11 && in->words[1] == want_word1 && in->words[2] == 13 && in->words[3] == 14);
    note(0x1f2430, index, 0, 0, 0);
    return 0;
}

int func_001f1fd0_drop(int index, struct DotrDropVector *out)
{
    note(0x1f1fd0, index, 0, 0, 0);
    out->x = 1.0f;
    out->y = 2.0f;
    out->z = 3.0f;
    out->w = 4.0f;
    return 0;
}

void func_001e10f0_drop(int x, int y, int z, struct DotrDropVector *v)
{
    note(0x1e10f0, x, y, z, 0);
    v->x = 10.0f;
    v->y = 20.0f;
    v->z = 30.0f;
    v->w = 40.0f;
}

int func_001f22b0_drop(int index, struct DotrDropVector *in)
{
    /* x and z from 0x1e10f0, y and w as 0x1f1fd0 left them */
    assert(in->x == 10.0f && in->y == 2.0f && in->z == 30.0f && in->w == 4.0f);
    note(0x1f22b0, index, 0, 0, 0);
    return 0;
}

int SzModel_SetDeform(int index, int value)
{
    note(0x1f29e0, index, value, 0, 0);
    return 0;
}

int SzModel_SetDeformRatio(int index, int value)
{
    note(0x1f2ad0, index, value, 0, 0);
    return 0;
}

int SzDuel_SetUnitCtlRateM(int a, int b, int pair, int value)
{
    note(0x1e1c10, a, b, pair, value);
    return 0;
}

int SzDuel_SetUnitCtlGoalM(int a, int b, int value)
{
    note(0x1e19d0, a, b, value, 0);
    return 0;
}

int SzModel_SetAnimDirect(int index, int slot)
{
    note(0x1f1830, index, slot, 0, 0);
    return 0;
}

int SzModel_SetAnimDir(int index, int value)
{
    note(0x1f1a00, index, value, 0, 0);
    return 0;
}

int func_001f26e0_drop(int index, int value, void *in)
{
    note(0x1f26e0, index, value, in == NULL, 0);
    return 0;
}

int func_001f2860_drop(int index, int value, void *in)
{
    note(0x1f2860, index, value, in == NULL, 0);
    return 0;
}

void SzModel_CancelMotionBlur(int value)
{
    note(0x1edf20, value, 0, 0, 0);
}

int SzSound_StopSE(int index, int value)
{
    /* the list entry is still the packed word here */
    assert(gSzBattle_bl[index - 2] == want_word);
    note(0x2178c0, index, value, 0, 0);
    return 0;
}

static void run(int index, int mode)
{
    int head[] = {
        0x1df540, want_x, want_y, 0, 0,
        0x1f56d0, want_word, 1, 0, 0,
        0x1e3850, want_x, want_y, 0, 0,
        0x1f25d0, want_object, 1, 0, 0,
        0x1f2430, want_object, 0, 0, 0,
        0x1f1fd0, want_object, 0, 0, 0,
        0x1e10f0, 0x21, 0x22, 0, 0,
        0x1f22b0, want_object, 0, 0, 0,
        0x1f29e0, want_object, 1, 0, 0,
        0x1f2ad0, want_object, 32, 0, 0,
    };
    int with_mode[] = {
        0x1e1c10, want_x, want_y, 0, 0,
        0x1e19d0, want_x, want_y, 0, 0,
    };
    int without_mode[] = {
        0x1f1830, want_object, 0, 0, 0,
        0x1f1a00, want_object, 1, 0, 0,
    };
    int tail[] = {
        0x1f26e0, want_object, 0, 1, 0,
        0x1f2860, want_object, 1, 1, 0,
        0x1edf20, want_object, 0, 0, 0,
        0x2178c0, index + 2, -1, 0, 0,
    };

    log_n = 0;
    SzBattle_Return(index, mode);
    assert(log_n == 80);
    assert(memcmp(log_v, head, sizeof(head)) == 0);
    assert(memcmp(log_v + 50, mode ? with_mode : without_mode, sizeof(with_mode)) == 0);
    assert(memcmp(log_v + 60, tail, sizeof(tail)) == 0);
    assert(gSzBattle_bl[index] == -1);
}

/* SzBattle_Return (branch coverage: null and non-null block, x zero and non-zero
 * for word 1, mode zero and non-zero, first and last list entry). */
static void check_drop(void)
{
    assert(offsetof(struct DotrDropBlock, modelId) == 0x46 && sizeof(struct DotrDropWords) == 16);
    memset(&block, 0, sizeof(block));
    block.locCol = 0x21;
    block.locRow = 0x22;
    block.modelId = 9;
    data_002921d0[0].words[0] = 11;
    data_002921d0[0].words[1] = 12;
    data_002921d0[0].words[2] = 13;
    data_002921d0[0].words[3] = 14;
    want_object = 9;

    /* null block: only 0x1df540 is called and the entry is kept */
    memset(gSzBattle_bl, 0, sizeof(gSzBattle_bl));
    gSzBattle_bl[3] = 0x0507;
    block_v = NULL;
    log_n = 0;
    SzBattle_Return(3, 1);
    assert(log_n == 5 && log_v[0] == 0x1df540 && log_v[1] == 5 && log_v[2] == 7);
    assert(gSzBattle_bl[3] == 0x0507);

    /* x != 0: word 1 becomes 0; mode set */
    block_v = &block;
    want_word = 0x0507;
    want_x = 5;
    want_y = 7;
    want_word1 = 0;
    run(3, 1);
    /* x == 0: word 1 becomes 0x8000; mode clear; first entry */
    gSzBattle_bl[0] = want_word = 0x0006;
    want_x = 0;
    want_y = 6;
    want_word1 = 0x8000;
    run(0, 0);
    /* last entry, x != 0 without mode */
    gSzBattle_bl[7] = want_word = 0x0304;
    want_x = 3;
    want_y = 4;
    want_word1 = 0;
    run(7, 0);
    assert(gSzBattle_bl[1] == 0 && gSzBattle_bl[6] == 0);
    /* the template itself is not written */
    assert(data_002921d0[0].words[1] == 12);
}

int main(void)
{
    check_drop();
    return 0;
}
