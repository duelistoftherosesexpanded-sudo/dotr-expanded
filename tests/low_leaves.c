/* Synthetic host tests for the low_leaves group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/low_leaves.h"

static int poll_result;

int func_001a2e60(void) { return poll_result; }

/* 0x15a180: 0x1e22b0 answers from a 7x7 grid and rejects coordinates
 * outside it; 0x15a320 counts the entries that are not 0xffff and 0x250860
 * answers the larger of its two arguments (only limit 0 is used); 0x15a2e0
 * is the real function. */
static int fill_grid[7][7], fill_asked[7][7], fill_calls;

int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    fill_asked[x][y]++;
    fill_calls++;
    return fill_grid[x][y];
}

int TaLogic018_GetZoneSize(unsigned short *p)
{
    int i, n = 0;
    for (i = 0; i < 49; i++) {
        if (p[i] != 0xffff) n++;
    }
    return n;
}

int GetCap(int limit, int value)
{
    assert(limit == 0);
    return value < limit ? limit : value;
}

static void fill_reset(unsigned short *list)
{
    memset(fill_grid, 0, sizeof fill_grid);
    memset(fill_asked, 0, sizeof fill_asked);
    memset(list, 0xff, 49 * sizeof *list);
    fill_calls = 0;
}

static void check_fill(void)
{
    unsigned short list[49];

    /* already listed: no cell query, list unchanged */
    fill_reset(list);
    fill_grid[2][3] = 9;
    list[0] = 0x302;
    TaLogic018_GetZoneSide(2, 3, list);
    assert(fill_calls == 0 && list[0] == 0x302 && list[1] == 0xffff);

    /* cell is not 9: one query, nothing stored */
    fill_reset(list);
    fill_grid[2][3] = 8;
    TaLogic018_GetZoneSide(2, 3, list);
    assert(fill_calls == 1 && list[0] == 0xffff);

    /* lone cell: its four neighbours are asked, the diagonal ones are not */
    fill_reset(list);
    fill_grid[3][3] = 9;
    fill_grid[4][4] = 9;
    TaLogic018_GetZoneSide(3, 3, list);
    assert(list[0] == 0x303 && list[1] == 0xffff && fill_calls == 5);
    assert(fill_asked[2][3] == 1 && fill_asked[4][3] == 1 && fill_asked[3][2] == 1 && fill_asked[3][4] == 1);
    assert(fill_asked[4][4] == 0 && fill_asked[2][2] == 0 && fill_asked[2][4] == 0 && fill_asked[4][2] == 0);

    /* lower corner: the block starts at 0; visit order is x outer, y inner */
    fill_reset(list);
    fill_grid[0][0] = 9;
    fill_grid[0][1] = 9;
    fill_grid[1][0] = 9;
    TaLogic018_GetZoneSide(0, 0, list);
    assert(list[0] == 0x000 && list[1] == 0x100 && list[2] == 0x001 && list[3] == 0xffff);

    /* upper corner: the block stops at 6 */
    fill_reset(list);
    fill_grid[6][6] = 9;
    fill_grid[5][6] = 9;
    fill_grid[6][5] = 9;
    TaLogic018_GetZoneSide(6, 6, list);
    assert(list[0] == 0x606 && list[1] == 0x605 && list[2] == 0x506 && list[3] == 0xffff);
    assert(fill_asked[5][5] == 2);
}

static int weigh_calls;

int TaHand_GetCardSummonCost2(int value, int index)
{
    assert(index == 9);
    weigh_calls++;
    return value;
}

/* 0x155fb0 with the real 0x1562b0: halfword reads, word exchanges (as the bytes have it). */
static void check_sort(void)
{
    static int weights[4] = {5, 1, 7, 6};
    union {
        int words[2];
        unsigned short halves[4];
    } v;

    /* lo > hi and lo == hi: nothing read, nothing moved */
    v.halves[0] = 1; v.halves[1] = 2; v.halves[2] = 3; v.halves[3] = 0;
    weigh_calls = 0;
    TaLogic017_SortTempArray(v.halves, 1, 0, 9, weights);
    TaLogic017_SortTempArray(v.halves, 1, 1, 9, weights);
    assert(weigh_calls == 0);
    assert(v.halves[0] == 1 && v.halves[1] == 2 && v.halves[2] == 3 && v.halves[3] == 0);

    /* compare false: last stays at lo, both inner ranges are empty */
    v.halves[0] = 2; v.halves[1] = 1;
    TaLogic017_SortTempArray(v.halves, 0, 1, 9, weights);
    assert(weigh_calls == 2);
    assert(v.halves[0] == 2 && v.halves[1] == 1 && v.halves[2] == 3 && v.halves[3] == 0);

    /* compare true: words 0 and 1 exchanged, then the expanded copy runs once more
     * on [0, 1] (compare false there) and the upper range is empty */
    v.halves[0] = 1; v.halves[1] = 2;
    weigh_calls = 0;
    TaLogic017_SortTempArray(v.halves, 0, 1, 9, weights);
    assert(weigh_calls == 4);
    assert(v.halves[0] == 3 && v.halves[1] == 0 && v.halves[2] == 1 && v.halves[3] == 2);
}

/* Ta_IsSquareRange (branch coverage): inside, each edge, just outside each edge, both clamps, empty range. */
static void check_square_range(void)
{
    assert(Ta_IsSquareRange(3, 3, 3, 3, 2) == 1);
    assert(Ta_IsSquareRange(3, 3, 2, 2, 2) == 1 && Ta_IsSquareRange(3, 3, 4, 4, 3) == 1);
    assert(Ta_IsSquareRange(3, 3, 1, 3, 2) == 0 && Ta_IsSquareRange(3, 3, 5, 3, 2) == 0);
    assert(Ta_IsSquareRange(3, 3, 3, 1, 2) == 0 && Ta_IsSquareRange(3, 3, 3, 5, 2) == 0);
    assert(Ta_IsSquareRange(3, 3, 3, 3, 1) == 1 && Ta_IsSquareRange(3, 3, 3, 4, 1) == 0);
    /* lower bounds come from GetCap(0, ...): negative starts are clamped to 0 */
    assert(Ta_IsSquareRange(0, 0, 0, 0, 4) == 1 && Ta_IsSquareRange(0, 0, -1, 0, 4) == 0);
    assert(Ta_IsSquareRange(0, 0, 0, -1, 4) == 0);
    /* upper bounds are capped at 6 */
    assert(Ta_IsSquareRange(6, 6, 6, 6, 4) == 1 && Ta_IsSquareRange(6, 6, 7, 6, 4) == 0);
    assert(Ta_IsSquareRange(6, 6, 6, 7, 4) == 0 && Ta_IsSquareRange(5, 5, 6, 6, 2) == 1);
    /* a start above the capped end leaves nothing to scan */
    assert(Ta_IsSquareRange(9, 3, 9, 3, 0) == 0 && Ta_IsSquareRange(3, 9, 3, 9, 0) == 0);
}

static int ld_able, ld_kind, ld_level, ld_level_calls;
int Ta_IsLDAbl(int a, int b)
{
    assert(a == 8 && b == 1);
    return ld_able;
}
int SzDuel_GetUnitConstKind(int a, int b)
{
    assert(a == 1 && b == 0);
    return ld_kind;
}
int SzDuel_GetUnitLevel(int a, int b, int flag)
{
    assert(a == 1 && b == 0 && flag == 0);
    ld_level_calls++;
    return ld_level;
}

/* Ta_GetLDLvRev (branch coverage): ability off, every listed kind at both sides of its level
 * bound, kind 20, other kinds. */
static void check_ld_level(void)
{
    static const int seven[4] = { 0, 6, 12, 14 };
    static const int six[2] = { 4, 13 };
    int i;

    ld_able = 0;
    ld_kind = 20;
    ld_level = 9;
    assert(Ta_GetLDLvRev(1) == 0 && ld_level_calls == 0);
    ld_able = 1;
    assert(Ta_GetLDLvRev(1) == 1 && ld_level_calls == 0);
    for (i = 0; i < 4; i++) {
        ld_kind = seven[i] | 0x20; /* only the low five bits select */
        ld_level = 6;
        assert(Ta_GetLDLvRev(1) == 1);
        ld_level = 7;
        assert(Ta_GetLDLvRev(1) == 2);
    }
    for (i = 0; i < 2; i++) {
        ld_kind = six[i];
        ld_level = 5;
        assert(Ta_GetLDLvRev(1) == 1);
        ld_level = 6;
        assert(Ta_GetLDLvRev(1) == 2);
    }
    assert(ld_level_calls == 12);
    ld_level = 9;
    ld_kind = 1;
    assert(Ta_GetLDLvRev(1) == 0);
    ld_kind = 21;
    assert(Ta_GetLDLvRev(1) == 0 && ld_level_calls == 12);
}

static int load_answer, load_no, entry_answer, entry_calls;
static struct jp_11e6e *load_temp;
static unsigned int entry_args[5];
int TaEffect_IsLoadModule2(struct jp_11e6e *Temp, int eNO)
{
    load_temp = Temp;
    load_no = eNO;
    return load_answer;
}
int SzEffect_Entry(int a, int eNO, int dwPar, int dwTo, int dwFrom)
{
    entry_args[0] = a;
    entry_args[1] = eNO;
    entry_args[2] = dwPar;
    entry_args[3] = dwTo;
    entry_args[4] = dwFrom;
    entry_calls++;
    return entry_answer;
}

/* Ta_Do_Effect (branch coverage): module not loaded (0), load failed (-1), entry made, entry refused. */
static void check_do_effect(void)
{
    struct jp_11dc9 info = { 0x21, 0x1111, 0x2222, 0x3333 };
    struct jp_11e6e *temp = (struct jp_11e6e *)&info;
    load_answer = 0;
    assert(Ta_Do_Effect(&info, temp) == -2 && entry_calls == 0);
    assert(load_temp == temp && load_no == 0x21);
    load_answer = -1;
    assert(Ta_Do_Effect(&info, temp) == -3 && entry_calls == 0);
    load_answer = 1;
    entry_answer = 0;
    assert(Ta_Do_Effect(&info, temp) == 0 && entry_calls == 1);
    assert(entry_args[0] == 0 && entry_args[1] == 0x21 && entry_args[2] == 0x3333);
    assert(entry_args[3] == 0x2222 && entry_args[4] == 0x1111);
    entry_answer = 9;
    assert(Ta_Do_Effect(&info, temp) == 9 && entry_calls == 2);
    entry_answer = -5;
    assert(Ta_Do_Effect(&info, temp) == -1 && entry_calls == 3);
}

/* Ta_GetLDChgLand (branch coverage): every case label, the default, and the values next to each label. */
static void check_ld_chg_land(void)
{
    assert(Ta_GetLDChgLand(1) == 5 && Ta_GetLDChgLand(7) == 5);
    assert(Ta_GetLDChgLand(9) == 0 && Ta_GetLDChgLand(10) == 1);
    assert(Ta_GetLDChgLand(16) == 4 && Ta_GetLDChgLand(20) == 9);
    assert(Ta_GetLDChgLand(0) == 7 && Ta_GetLDChgLand(2) == 7 && Ta_GetLDChgLand(8) == 7);
    assert(Ta_GetLDChgLand(11) == 7 && Ta_GetLDChgLand(21) == 7 && Ta_GetLDChgLand(-1) == 7);
}

int main(void)
{
    check_fill();
    char bytes[5] = { 9, 9, 9, 9, 9 };
    int words[4] = { 10, 20, 30, 40 };
    unsigned short list[50];
    int i;

    /* straight-line: every stored byte */
    ShFx_initTimer(bytes, 1, 2, 3);
    assert(bytes[0] == 1 && bytes[1] == 2 && bytes[2] == 1 && bytes[3] == 3 && bytes[4] == 9);

    /* straight-line: both stores, untouched neighbours, same index */
    TaLogic017_SortSwap(words, 1, 3);
    assert(words[0] == 10 && words[1] == 40 && words[2] == 30 && words[3] == 20);
    TaLogic017_SortSwap(words, 2, 2);
    assert(words[2] == 30);

    /* first entry, last entry (index 48), entry 49 is outside the bound, absent */
    for (i = 0; i < 50; i++)
        list[i] = (unsigned short)(0x100 + i);
    assert(TaLogic018_IsZonePast(0x100, list) == 1);
    assert(TaLogic018_IsZonePast(0x130, list) == 1);
    assert(TaLogic018_IsZonePast(0x131, list) == 0);
    assert(TaLogic018_IsZonePast(0x10100, list) == 0);

    assert(TaLogic_GetWarnHideLvl__0016b0e0() == 0);

    poll_result = 1;
    assert(_dpfgt() == 1);
    poll_result = 0;
    assert(_dpfgt() == 0);
    poll_result = -1;
    assert(_dpfgt() == 0);
    poll_result = -1;
    assert(_dpflt() == 1);
    poll_result = 0;
    assert(_dpflt() == 0);
    poll_result = 1;
    assert(_dpflt() == 0);
    check_sort();
    check_square_range();
    check_ld_level();
    check_do_effect();
    check_ld_chg_land();
    return 0;
}
