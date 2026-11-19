/* Synthetic host tests for the position_rules group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/position_rules.h"

static int gate_value, lookup_result, lookup_id, lookup_key, lookup_calls;
static int near_table[8], near_calls;
static int cell_result, cell_row, cell_column;
static int total_value, count_value, count_side;
static int side_value, level_value, level_side;
static int pick_calls, pick_side, pick_slot;

int TaHand_GetCardKind(void) { return gate_value; }
int Ta_GetHandEnchant(int id, int key)
{
    lookup_calls++;
    lookup_id = id;
    lookup_key = key;
    return lookup_result;
}
int TaLogic_GetCyclePoint(int index)
{
    assert(index >= 0 && index < 8);
    near_calls++;
    return near_table[index];
}
/* With word_on set (0x13a9a0) the answer comes from a per-column table. */
static int word_on, word_side, word_present[8], word_cell[8];
/* The 0x154eb0 cluster reads the first four (row << 8 | column) requests. */
static int cell_log[4], cell_log_n;
/* With cell_zero_on set (0x131f80) a request for column 0 answers cell_zero unrecorded. */
static int cell_zero_on, cell_zero, cell_zero_row;
int SzDuel_GetUnitLoc(int row, int column)
{
    if (cell_zero_on && column == 0) {
        cell_zero_row = row;
        return cell_zero;
    }
    cell_row = row;
    cell_column = column;
    if (cell_log_n < 4) {
        cell_log[cell_log_n++] = (row << 8) | column;
    }
    if (word_on) {
        assert(row == word_side && column >= 1 && column <= 5 && word_present[column]);
        return word_cell[column];
    }
    return cell_result;
}
int func_001df540_word(int a, int b)
{
    assert(a == word_side && b >= 1 && b <= 5);
    return word_present[b];
}
int SzDuel_GetLP(void) { return total_value; }
int SzDuel_GetUnitMonsterCount(int side) { count_side = side; return count_value; }
int SzDuel_GetTurnOwner(void) { return side_value; }
int SzDuel_GetUnitCount(int side) { level_side = side; return level_value; }
/* Both pickers answer 0x100 * (slot + 1) + slot so each slot is a distinct position. */
int TaLogic_GetSommonPiecePreference__0014da30(int side, int slot)
{
    pick_calls++; pick_side = side; pick_slot = slot;
    return 0x100 * (slot + 1) + slot;
}
int TaLogic_GetSommonPiecePreference__001604a0(int side, int slot)
{
    pick_calls++; pick_side = side; pick_slot = slot;
    return 0x100 * (slot + 1) + slot;
}

/* Stubs for the 0x118dd0 tests: an answer per index, a cell table, a key per index. */
static int grid_cell[8][8], grid_calls, flag_value, flag_calls, kind_by[8], kind_calls;
static int band_value, band_side;
static char cell_full[8][8], cell_fixture;
static int key_by[7], key_c, key_calls;
/* With key_alt_on set (the 0x154eb0 cluster) a zero c answers from key_alt. */
static int key_alt_on, key_alt[7];
static int pair_row, pair_column, pair_calls;

/* 0x118ce0's cells: grid_cell[x][y], -1 for none. The 0x250360 stub answers
 * the cell itself (a << 8 | b), so a cell "matches" a key equal to it. */
/* wide_mode (0x11d4e0 only): 0x1e3790 answers wide_cell for any cell of the
 * 8x8 grid and the 0x2792c0 stub records up to eight calls. */
static int wide_mode, wide_cell[8][8], wide_cost[8], wide_calls, wide_x[8], wide_y[8], wide_id[8], wide_on[4];
int SzDuel_GetMapUnitID(int x, int y)
{
    if (wide_mode) {
        assert(x >= 0 && x < 8 && y >= 0 && y < 8);
        return wide_cell[x][y];
    }
    assert(x >= 0 && x < 8 && y >= 1 && y <= 5);
    grid_calls++;
    return grid_cell[x][y];
}
int SzDuel_GetUnitKind(int a, int b)
{
    assert(a == pair_row && b == pair_column);
    flag_calls++;
    return flag_value;
}
int SzDuel_GetField(int x, int y)
{
    assert(y == 2 && (x == 2 || x == 5));
    kind_calls++;
    return kind_by[x];
}
static int third_value, third_side, third_index, third_calls;
int Ta_GetSommonPiecePreference(int a, int i)
{
    third_side = a;
    third_index = i;
    third_calls++;
    return third_value;
}
int TaLogic010_GetEmptyLine(int side)
{
    band_side = side;
    return band_value;
}
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    assert(x >= 0 && x < 8 && y >= 0 && y < 8);
    return cell_full[x][y] ? (struct DotrStateItem *)&cell_fixture : 0;
}
int TaLogic017_GetLine(int i, int c)
{
    assert(i >= 0 && i < 7 && i <= key_calls);
    key_c = c;
    key_calls++;
    if (key_alt_on && c == 0) {
        return key_alt[i];
    }
    return key_by[i];
}
/* With pair_row_on set (0x15e2e0) the answer is the row alone. */
static int pair_row_on, pair_zero_on;
int func_00250360_pair(int a, int b)
{
    pair_row = a;
    pair_column = b;
    pair_calls++;
    if (pair_zero_on) {
        return 0;
    }
    if (pair_row_on) {
        return a;
    }
    return (a << 8) | b;
}

static void check_id_rule(void)
{
    /* gate >> 5 non-zero: rejected before the lookup */
    gate_value = 0x20; lookup_calls = 0; lookup_result = 0;
    assert(TaLogic002_IsRefuseEmptyCombo(0x18e) == 0);
    assert(lookup_calls == 0);
    /* gate boundary 0x1f passes; lookup >= 0 accepts */
    gate_value = 0x1f; lookup_result = 0;
    assert(TaLogic002_IsRefuseEmptyCombo(7) == 1);
    assert(lookup_calls == 1 && lookup_id == 7 && lookup_key == 0x31e);
    /* lookup negative: only id 0x18e is accepted */
    lookup_result = -1;
    assert(TaLogic002_IsRefuseEmptyCombo(0x18e) == 1);
    assert(TaLogic002_IsRefuseEmptyCombo(0x18d) == 0);
    assert(TaLogic002_IsRefuseEmptyCombo(0x18f) == 0);
}

static void check_near(void)
{
    int i;

    /* every entry above the argument in both bytes: 2 + 1 calls per byte, plus the final one */
    for (i = 0; i < 8; i++)
        near_table[i] = 0x505 + i;
    near_calls = 0;
    assert(TaLogic005_GetNearCyclePoint(2, 2) == 0x50c);
    assert(near_calls == 8 * 7);
    /* every entry below the argument in both bytes: 1 + 2 calls per byte */
    near_calls = 0;
    assert(TaLogic005_GetNearCyclePoint(0x40, 0x40) == 0x50c);
    assert(near_calls == 8 * 7);
    /* equal row (2 + 2 calls), column above */
    near_calls = 0;
    assert(TaLogic005_GetNearCyclePoint(2, 5) == 0x50c);
    assert(near_calls == 8 * 8);
    /* equal row and equal column for entry 0 only */
    near_calls = 0;
    assert(TaLogic005_GetNearCyclePoint(5, 5) == 0x50c);
    assert(near_calls == 9 + 7 * 8);
    /* the result is the last entry whatever the distance (the sum is never below -1) */
    near_table[7] = 0x0102;
    assert(TaLogic005_GetNearCyclePoint(0xff, 0x7f) == 0x0102);
}

static void check_constants(void)
{
    static const int five[] = { 0x403, 0x402, 0x404, 0x401, 0x405 };
    static const int masked[] = { 0x203, 0x301, 0x305, 0x202, 0x204 };
    int i;

    assert(TaLogic005_GetDefensePoint(0) == 0x303);
    assert(TaLogic005_GetDefensePoint(1) == 0x304);
    assert(TaLogic005_GetDefensePoint(2) == 0x302);
    assert(TaLogic005_GetDefensePoint(3) == 0x302);
    assert(TaLogic005_GetDefensePoint(-1) == 0x302);

    for (i = 0; i < 5; i++)
        assert(TaLogic013_GetDefensePoint(i) == five[i]);
    assert(TaLogic013_GetDefensePoint(5) == -1);
    assert(TaLogic013_GetDefensePoint(-1) == -1);

    /* 0x147f40: every cell full, then each of the five empty in turn (the
     * earlier ones full), then all empty. */
    memset(cell_full, 1, sizeof cell_full);
    assert(TaLogic013_GetEmptyLine() == -1);
    for (i = 4; i >= 0; i--) {
        cell_full[five[i] & 0xff][five[i] >> 8] = 0;
        assert(TaLogic013_GetEmptyLine() == five[i]);
    }
    memset(cell_full, 0, sizeof cell_full);
    assert(TaLogic013_GetEmptyLine() == 0x403);

    for (i = 0; i < 5; i++) {
        assert(TaLogic020_GetLinePiece(i) == masked[i]);
        assert(TaLogic020_GetLinePiece(0x700 + i) == masked[i]);
    }
    assert(TaLogic020_GetLinePiece(5) == 0x203);
    assert(TaLogic020_GetLinePiece(0xff) == 0x203);
}

static void check_order(void)
{
    struct DotrPositionOrder order;

    /* cell >> 8 below 3 */
    memset(&order, 0, sizeof order);
    order.position = 0x8234;
    order.flags.word = 0xffff1234;
    cell_result = 0x2ff;
    TaLogic006_CommandInvadeUnit(&order);
    assert(cell_row == 0x82 && cell_column == 0x34);
    assert(order.position == 0x8234 && order.kind == 0x6ff);
    assert(order.flags.word == 0x03011234);
    /* boundary 3 and above */
    order.flags.word = 0xffff0001;
    cell_result = 0x300;
    TaLogic006_CommandInvadeUnit(&order);
    assert(order.flags.word == 0x05020001);
    assert(order.unknown_02 == 0 && order.unknown_06 == 0);
}

static void check_limits(void)
{
    /* total below 0x4b0: rejected without the count */
    total_value = 0x4af; count_value = 9; count_side = -1;
    assert(TaLogic011_IsUseBraveHourGlass(1) == 0);
    assert(count_side == -1);
    /* total at the boundary: count decides at 4 */
    total_value = 0x4b0; count_value = 4;
    assert(TaLogic011_IsUseBraveHourGlass(1) == 1);
    assert(count_side == 1);
    count_value = 3;
    assert(TaLogic011_IsUseBraveHourGlass(0) == 0);
    assert(count_side == 0);

    side_value = 1; level_value = 2; level_side = -1;
    assert(TaLogic015_IsActiveMode() == 0);
    assert(level_side == 1);
    level_value = 3;
    assert(TaLogic015_IsActiveMode() == 1);
}

static void check_pickers(void)
{
    static const int slots3[] = { 0, 3, 4 };
    static const int slots4[] = { 0, 3, 4, 5 };
    int i;

    side_value = 1;
    for (i = 0; i < 3; i++) {
        pick_calls = 0;
        assert(TaLogic014_GetLine(i) == 0x100 * (slots3[i] + 1) + slots3[i]);
        assert(pick_calls == 1 && pick_side == 1 && pick_slot == slots3[i]);
    }
    pick_calls = 0;
    assert(TaLogic014_GetLine(3) == -1);
    assert(TaLogic014_GetLine(-1) == -1);

    /* 0x14f3a0: no empty cell, then each of the three empty first with a
     * positive, zero and negative 0x24e410 answer. */
    {
        static const int asked[] = { 0, 3, 4 };
        int spot;
        int picks = pick_calls;

        memset(cell_full, 1, sizeof cell_full);
        side_value = 9; third_calls = 0;
        assert(TaLogic014_IsActiveMode() == 1 && third_calls == 0);
        for (i = 2; i >= 0; i--) {
            spot = TaLogic014_GetLine(i);
            cell_full[spot & 0xff][spot >> 8] = 0;
            third_calls = 0; third_value = 5;
            assert(TaLogic014_IsActiveMode() == 0 && third_calls == 1 && third_side == 9 && third_index == asked[i]);
            third_value = 0;
            assert(TaLogic014_IsActiveMode() == 0);
            third_value = -1;
            assert(TaLogic014_IsActiveMode() == 1 && third_calls == 3);
        }
        memset(cell_full, 0, sizeof cell_full);
        side_value = 1; pick_calls = picks;
    }
    assert(pick_calls == 0);

    side_value = 0;
    for (i = 0; i < 4; i++) {
        pick_calls = 0;
        assert(TaLogic019_GetLine(i) == 0x100 * (slots4[i] + 1) + slots4[i]);
        assert(pick_calls == 1 && pick_side == 0 && pick_slot == slots4[i]);
    }
    pick_calls = 0;
    assert(TaLogic019_GetLine(4) == -1);
    assert(TaLogic019_GetLine(-1) == -1);
    assert(pick_calls == 0);
}

static void check_membership(void)
{
    /* 0x147e50 over the five constants of 0x147eb0: first, last, absent */
    assert(TaLogic013_IsDefensePoint(3, 4) == 1);
    assert(TaLogic013_IsDefensePoint(5, 4) == 1);
    assert(TaLogic013_IsDefensePoint(6, 4) == 0);
    assert(TaLogic013_IsDefensePoint(3, 3) == 0);

    /* 0x14c970 over slots 0, 3, 4 of the stubbed picker: 0x100, 0x403, 0x504 */
    pick_calls = 0;
    assert(TaLogic014_IsLinePoint(0, 1) == 1);
    assert(pick_calls == 1);
    pick_calls = 0;
    assert(TaLogic014_IsLinePoint(4, 5) == 1);
    assert(pick_calls == 3);
    pick_calls = 0;
    assert(TaLogic014_IsLinePoint(5, 6) == 0);
    assert(pick_calls == 3);

    /* 0x15ed30 over slots 0, 3, 4, 5: the fourth (0x605) is reached, a fifth is not */
    pick_calls = 0;
    assert(TaLogic019_IsLinePoint(0, 1) == 1);
    assert(pick_calls == 1);
    pick_calls = 0;
    assert(TaLogic019_IsLinePoint(5, 6) == 1);
    assert(pick_calls == 4);
    pick_calls = 0;
    assert(TaLogic019_IsLinePoint(6, 7) == 0);
    assert(pick_calls == 4);
}

static void check_tier(int (*f)(int), int limit, int need, int many, int few, int low)
{
    /* total at the limit: the low value, the count is not asked */
    total_value = limit; count_value = need; count_side = -1;
    assert(f(7) == low && count_side == -1);
    /* total above the limit: the count decides, boundary at need */
    total_value = limit + 1;
    assert(f(7) == many && count_side == 7);
    count_value = need - 1;
    assert(f(7) == few);
}

static void check_tiers(void)
{
    check_tier(TaLogic_GetWarnHideLvl__0011fa80, 1000, 3, 500, 1000, 2000);
    check_tier(TaLogic_GetWarnHideLvl__001254a0, 1000, 3, 500, 1000, 2000);
    check_tier(TaLogic_GetWarnHideLvl__0012b1b0, 1200, 4, 1000, 2000, 2400);
    check_tier(TaLogic_GetWarnHideLvl__00130780, 2000, 4, 750, 1500, 2400);
    check_tier(TaLogic_GetWarnHideLvl__00134ae0, 1000, 4, 750, 1500, 1700);
    check_tier(TaLogic_GetWarnHideLvl__0014b330, 1500, 4, 750, 1500, 2000);
    check_tier(TaLogic_GetWarnHideLvl__0014f9b0, 1200, 5, 750, 1500, 2400);
    check_tier(TaLogic_GetWarnHideLvl__00153ed0, 1200, 4, 500, 1000, 1500);
    check_tier(TaLogic_GetWarnHideLvl__00158780, 1200, 5, 750, 1500, 2400);
    check_tier(TaLogic_GetWarnHideLvl__00162580, 1200, 4, 500, 1000, 2000);
    check_tier(TaLogic_GetWarnHideLvl__0010eda0, 1500, 4, 0, 0, 0);
    check_tier(TaLogic_GetWarnHideLvl__0011aa10, 1500, 3, 0, 0, 0);
    check_tier(TaLogic_GetWarnHideLvl__00141b30, 2000, 4, 0, 0, 0);
    check_tier(TaLogic_GetWarnHideLvl__001461c0, 2000, 4, 0, 0, 0);
    check_tier(TaLogic_GetWarnHideLvl__0013da70, 1000, 5, 0, 0, 0);
    check_tier(TaLogic_GetWarnHideLvl__00172310, 1200, 4, 1000, 2000, 2400);
    /* 0x1676d0 asks nothing and returns 0. */
    total_value = 5000; count_side = -1;
    assert(TaLogic_GetWarnHideLvl__001676d0(7) == 0 && count_side == -1);
}

static void clear_grid(void)
{
    int x, y;

    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            grid_cell[x][y] = -1;
        }
    }
    flag_value = 0;
}

static void check_column_match(void)
{
    static const int column[6] = { 2, 4, 0, 6, 6, 2 };
    static const int ks[6] = { 0, 1, 2, 3, 4, 0x100 };
    int i;

    /* column by k (read as a byte), a matching cell in the first and in the last row */
    for (i = 0; i < 6; i++) {
        clear_grid();
        grid_calls = 0;
        assert(TaLogic003_IsEmptyAtkLine(0x0305, (unsigned char)ks[i]) == 1 && grid_calls == 5);
        grid_cell[column[i]][5] = 0x0305;
        grid_calls = 0;
        assert(TaLogic003_IsEmptyAtkLine(0x0305, (unsigned char)ks[i]) == 0 && grid_calls == 5);
        grid_cell[column[i]][1] = 0x0305;
        grid_calls = 0;
        assert(TaLogic003_IsEmptyAtkLine(0x0305, (unsigned char)ks[i]) == 0 && grid_calls == 1);
    }
    /* another key: 0x1e0700 is not asked; flag bits at and above 0x20 reject */
    clear_grid();
    grid_cell[2][3] = 0x0306;
    flag_calls = 0;
    assert(TaLogic003_IsEmptyAtkLine(0x0305, 0) == 1 && flag_calls == 0);
    grid_cell[2][3] = 0x0305;
    flag_value = 0x20;
    assert(TaLogic003_IsEmptyAtkLine(0x0305, 0) == 1 && flag_calls == 1);
    flag_value = 0x1f;
    assert(TaLogic003_IsEmptyAtkLine(0x0305, 0) == 0 && flag_calls == 2);
}

static void check_first_answers(void)
{
    static const int column[4] = { 2, 4, 0, 6 };
    static const int on_hit[4] = { 0x102, 0x104, 0x100, 0x106 };
    static const int on_miss[4] = { 0x602, 0x604, 0x600, 0x606 };
    int i, j;

    /* through the real 0x118ce0, which answers 0 for a column holding the key */
    for (i = 0; i <= 4; i++) {
        /* 0x118dd0: the first column without the key */
        clear_grid();
        for (j = 0; j < i && j < 4; j++) {
            grid_cell[column[j]][1] = 0x0305;
        }
        assert(TaLogic003_GetEmptyAtkLineStart(0x0305) == (i < 4 ? on_hit[i] : -1));
        /* 0x118e90: the first column with it */
        clear_grid();
        if (i < 4) {
            grid_cell[column[i]][1] = 0x0305;
        }
        assert(TaLogic003_GetEmptyAtkLineEnd(0x0305) == (i < 4 ? on_miss[i] : -1));
    }
}

static void check_bands(void)
{
    struct DotrPositionOrder order;
    int i;

    /* 0x113b60: each test at its boundary */
    assert(TaLogic001_GetNextGrassPoint(0, 5) == 0x602 && TaLogic001_GetNextGrassPoint(9, 5) == 0x602);
    assert(TaLogic001_GetNextGrassPoint(5, 4) == 0x206 && TaLogic001_GetNextGrassPoint(4, 4) == 0x105);
    assert(TaLogic001_GetNextGrassPoint(3, 4) == 0x501 && TaLogic001_GetNextGrassPoint(4, 3) == 0x105);

    /* 0x163f90: rows 0, 1..5 and above, columns 2 and 3 */
    assert(TaLogic020_GetNextGrassPoint(3, 0) == 0x106 && TaLogic020_GetNextGrassPoint(2, 0) == 0x100);
    assert(TaLogic020_GetNextGrassPoint(3, 1) == 0x605 && TaLogic020_GetNextGrassPoint(2, 1) == 0x601);
    assert(TaLogic020_GetNextGrassPoint(3, 5) == 0x605 && TaLogic020_GetNextGrassPoint(2, 5) == 0x601);
    assert(TaLogic020_GetNextGrassPoint(3, 6) == 0x601 && TaLogic020_GetNextGrassPoint(2, 6) == 0x605);
    assert(TaLogic020_GetNextGrassPoint(3, -1) == 0x605 && TaLogic020_GetNextGrassPoint(2, -1) == 0x601);

    /* 0x13d280 */
    side_value = 1; band_value = -1;
    assert(TaLogic010_IsActiveMode() == 1 && band_side == 1);
    band_value = 0;
    assert(TaLogic010_IsActiveMode() == 0);

    /* 0x136060: cell (2, 2) free and not kind 8, else cell (5, 2) the same way */
    memset(cell_full, 0, sizeof cell_full);
    kind_by[2] = 7; kind_by[5] = 7; kind_calls = 0;
    assert(TaLogic009_GetEmptyDefensePoint() == 0x202 && kind_calls == 1);
    kind_by[2] = 8; kind_calls = 0;
    assert(TaLogic009_GetEmptyDefensePoint() == 0x205 && kind_calls == 2);
    kind_by[5] = 8;
    assert(TaLogic009_GetEmptyDefensePoint() == -1);
    kind_by[2] = 7; kind_by[5] = 7; kind_calls = 0;
    cell_full[2][2] = 1;
    assert(TaLogic009_GetEmptyDefensePoint() == 0x205 && kind_calls == 1);
    cell_full[5][2] = 1; kind_calls = 0;
    assert(TaLogic009_GetEmptyDefensePoint() == -1 && kind_calls == 0);

    /* 0x136010 through the real 0x136060: -1, then 0x202 */
    memset(&order, 0, sizeof order);
    order.flags.word = 0xffff1234;
    TaLogic009_CommandDefenseUnit(&order);
    assert(order.kind == 0x5ff && order.flags.word == 0xffff1234);
    cell_full[2][2] = 0;
    TaLogic009_CommandDefenseUnit(&order);
    assert(order.kind == 0x3ff && order.flags.word == 0x02021234);
    assert(order.position == 0 && order.unknown_02 == 0 && order.unknown_06 == 0);

    /* 0x1555f0: the first, the last and no free cell; cells are (key & 0xff, key >> 8) */
    for (i = 0; i < 7; i++) {
        key_by[i] = 0x100 + i;
    }
    memset(cell_full, 0, sizeof cell_full);
    key_calls = 0;
    assert(TaLogic017_GetEmptyLine(0x55) == 0x100 && key_c == 0x55);
    for (i = 0; i < 6; i++) {
        cell_full[i][1] = 1;
    }
    key_calls = 0;
    assert(TaLogic017_GetEmptyLine(0x55) == 0x106);
    cell_full[6][1] = 1;
    key_calls = 0;
    assert(TaLogic017_GetEmptyLine(0x55) == -1 && key_calls == 7);

    /* 0x13a9a0: floor of 2, entries without a word skipped, only bits 8..15 compared */
    word_on = 1; word_side = 4;
    memset(word_present, 0, sizeof word_present);
    memset(word_cell, 0, sizeof word_cell);
    assert(TaLogic010_GetLineHeight(4) == 2);
    word_present[1] = 1; word_cell[1] = 0x1ff;
    assert(TaLogic010_GetLineHeight(4) == 2);
    word_present[5] = 1; word_cell[5] = 0x10300;
    assert(TaLogic010_GetLineHeight(4) == 3);
    word_present[3] = 1; word_cell[3] = 0x2ff;
    assert(TaLogic010_GetLineHeight(4) == 3);
    word_cell[3] = 0xff00;
    assert(TaLogic010_GetLineHeight(4) == 0xff);
    word_on = 0;
}

static void check_position_tables(void)
{
    static const int first[6] = { 0x204, 0x202, 0, 6, 6, 6 };
    static const int second[6] = { 0x402, 0x404, 0x600, 0x606, 0x606, 0x606 };
    static const int keys[6] = { 0, 1, 2, 3, 4, -1 };
    int i;

    for (i = 0; i < 6; i++) {
        assert(TaLogic004_GetAtkLineStart(99, keys[i]) == first[i]);
        assert(TaLogic004_GetAtkLineEnd(99, keys[i]) == second[i]);
    }
}

static void check_key_scan(void)
{
    int i;

    for (i = 0; i < 7; i++) {
        key_by[i] = 0x1000 + i;
    }
    /* first and last entry, then no entry after seven calls */
    key_calls = 0;
    assert(TaLogic017_IsLinePoint(0x00, 0x10, 0x55) == 1 && key_calls == 1 && key_c == 0x55);
    key_calls = 0;
    assert(TaLogic017_IsLinePoint(0x06, 0x10, 0x55) == 1 && key_calls == 7);
    key_calls = 0;
    assert(TaLogic017_IsLinePoint(0x07, 0x10, 0x55) == 0 && key_calls == 7);
    key_calls = 0;
    assert(TaLogic017_IsLinePoint(0x00, 0x11, 0x55) == 0 && key_calls == 7);
}

static void check_free_cells(void)
{
    /* 0x156f70: cells (2, 3) and (4, 3) in that order */
    memset(cell_full, 0, sizeof cell_full);
    assert(TaLogic017_GetEmptyTrapPiece() == 0x302);
    cell_full[2][3] = 1;
    assert(TaLogic017_GetEmptyTrapPiece() == 0x304);
    cell_full[4][3] = 1;
    assert(TaLogic017_GetEmptyTrapPiece() == -1);
    cell_full[2][3] = 0;
    assert(TaLogic017_GetEmptyTrapPiece() == 0x302);

    /* 0x15ee40: the picker stub gives slots 0, 3, 4, 5 the positions 0x100,
     * 0x403, 0x504, 0x605, so the cells asked are (0, 1), (3, 4), (4, 5), (5, 6). */
    memset(cell_full, 0, sizeof cell_full);
    side_value = 1;
    assert(TaLogic019_GetEmptyLine() == 0x100 && pick_side == 1 && pick_slot == 0);
    cell_full[0][1] = 1;
    assert(TaLogic019_GetEmptyLine() == 0x403 && pick_slot == 3);
    cell_full[3][4] = 1;
    assert(TaLogic019_GetEmptyLine() == 0x504 && pick_slot == 4);
    cell_full[4][5] = 1;
    assert(TaLogic019_GetEmptyLine() == 0x605 && pick_slot == 5);
    cell_full[5][6] = 1;
    assert(TaLogic019_GetEmptyLine() == -1);

    /* 0x14ca60: the same over slots 0, 3, 4 of the 0x14da30 stub. */
    memset(cell_full, 0, sizeof cell_full);
    side_value = 1;
    pick_calls = 0;
    assert(TaLogic014_GetEmptyLine() == 0x100 && pick_side == 1 && pick_slot == 0 && pick_calls == 2);
    cell_full[0][1] = 1;
    assert(TaLogic014_GetEmptyLine() == 0x403 && pick_slot == 3);
    cell_full[3][4] = 1;
    pick_calls = 0;
    assert(TaLogic014_GetEmptyLine() == 0x504 && pick_slot == 4 && pick_calls == 4);
    cell_full[4][5] = 1;
    pick_calls = 0;
    assert(TaLogic014_GetEmptyLine() == -1 && pick_calls == 3);
}

static void check_order_word(void)
{
    struct DotrPositionOrder order;

    memset(&order, 0, sizeof order);
    order.position = 0x8234;
    order.flags.word = 0xffff1234;
    cell_result = 0x0907;
    pair_calls = 0;
    TaLogic020_CommandInvadeUnit(&order);
    assert(pair_calls == 1 && pair_row == 0x82 && pair_column == 0x34);
    assert(cell_row == 0x82 && cell_column == 0x34);
    assert(order.position == 0x8234 && order.kind == 0x6ff);
    /* through the real 0x163f90: (7, 9) is 0x601 */
    assert(order.flags.word == 0x06011234);
    assert(order.unknown_02 == 0 && order.unknown_06 == 0);
}

static int cost_by[3], cost_calls, cost_x[3], cost_id[3];
int func_002792c0_spot(int word, struct DotrSpotNibbles *bytes, int column, int row, int x, int y, unsigned short id, int n)
{
    int i;

    if (wide_mode) {
        assert(word == 0x4321 && column == 6 && row == 2 && n == 7);
        for (i = 0; i < 12; i++)
            assert(bytes[i].low == 15 && bytes[i].high == 15);
        assert(wide_calls < 8);
        wide_x[wide_calls] = x;
        wide_y[wide_calls] = y;
        wide_id[wide_calls] = id;
        return wide_cost[wide_calls++];
    }
    assert(word == 0x4321 && column == 6 && row == 2 && y == 3 && n == 7);
    for (i = 0; i < 12; i++)
        assert(bytes[i].low == 15 && bytes[i].high == 15);
    assert(cost_calls < 3);
    cost_x[cost_calls] = x;
    cost_id[cost_calls] = id;
    return cost_by[cost_calls++];
}

static void spot_best(struct DotrSpotBest *best, int c0, int c1, int c2)
{
    cost_by[0] = c0; cost_by[1] = c1; cost_by[2] = c2;
    cost_calls = 0;
    best->spot = 0x7777; best->cost = 0x11;
    TaLogic005_GetNearDefensePoint(0x4321, 6, 2, best);
}

/* TaLogic005_IsEmptyDefensePoint (branch coverage: every index case with the cell empty and full)
 * and TaLogic005_GetNearDefensePoint (branch and loop coverage) on the real 0x123670. */
static void check_spot_best(void)
{
    static const int spots[] = { 0x303, 0x304, 0x302, 0x302, 0x302 };
    static const int index[] = { 0, 1, 2, 3, -1 };
    struct DotrSpotBest best;
    int i;

    for (i = 0; i < 5; i++) {
        memset(cell_full, 1, sizeof cell_full);
        assert(TaLogic005_IsEmptyDefensePoint(index[i]) == 0);
        cell_full[spots[i] & 0xff][spots[i] >> 8] = 0;
        assert(TaLogic005_IsEmptyDefensePoint(index[i]) == 1);
    }

    grid_cell[3][3] = 0x0123; grid_cell[4][3] = -1; grid_cell[2][3] = 0;
    /* no empty cell: only the opening stores */
    memset(cell_full, 1, sizeof cell_full);
    spot_best(&best, 5, 5, 5);
    assert(cost_calls == 0 && best.spot == -1 && best.cost == 0xff);
    /* all empty: the first cost ends the walk (0 and 0xfe are accepted) */
    memset(cell_full, 0, sizeof cell_full);
    spot_best(&best, 0, 5, 5);
    assert(cost_calls == 1 && best.spot == 0x303 && best.cost == 0 && cost_x[0] == 3 && cost_id[0] == 0x0123);
    spot_best(&best, 0xfe, 5, 5);
    assert(cost_calls == 1 && best.spot == 0x303 && best.cost == 0xfe);
    /* a negative cost and one not below 0xff go on; a negative id is passed as 0xffff */
    spot_best(&best, -1, 0xff, 7);
    assert(cost_calls == 3 && best.spot == 0x302 && best.cost == 7);
    assert(cost_x[1] == 4 && cost_id[1] == 0xffff && cost_x[2] == 2 && cost_id[2] == 0);
    spot_best(&best, -1, 0x100, -5);
    assert(cost_calls == 3 && best.spot == -1 && best.cost == 0xff);
    /* a full first cell is skipped */
    cell_full[3][3] = 1;
    spot_best(&best, 9, 5, 5);
    assert(cost_calls == 1 && best.spot == 0x304 && best.cost == 9 && cost_x[0] == 4);
    memset(cell_full, 0, sizeof cell_full);
}

/* Stubs for the 0x154eb0 cluster. The order under test is always at (1, 3). */
static int side_pair_value, side_pair_calls, code_value;
static int total_side_value, total_side_arg;
static int spot_list[2];
/* With list2_on set (0x14fa10) a request with d == 2 answers a second list. */
static int list2_on, list2_value, list2_count;
static int list_value, list_count, list_words[4], list_hits[4], list_args[6], hit_calls;
int func_0024de70_pair(int a, int b)
{
    assert(a == 1 && b == 3);
    side_pair_calls++;
    return side_pair_value;
}
int SzDuel_GetUnitCardNo(int a, int b)
{
    assert(a == 1 && b == 3);
    return code_value;
}
/* With total_by_on set (0x13ecf0) the answer depends on the side asked. */
static int total_by_on, total_by[3];
int func_001df470_side(int side)
{
    total_side_arg = side;
    if (total_by_on) {
        assert(side >= 0 && side < 3);
        return total_by[side];
    }
    return total_side_value;
}
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    list_args[0] = a; list_args[1] = b; list_args[2] = c;
    list_args[3] = d; list_args[4] = e; list_args[5] = f;
    /* 0x15e2e0's two keys answer from spot_list (zero unless a test sets it). */
    if (e == 0x33b001c) {
        return spot_list[0];
    }
    if (e == 0x2ed001c) {
        return spot_list[1];
    }
    if (list2_on && d == 2) {
        return list2_value;
    }
    return list_value;
}
int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    if (list2_on && value == (unsigned)list2_value) {
        return list2_count;
    }
    assert(value == (unsigned)list_value);
    return list_count;
}
int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    if (list2_on) {
        assert(word == list2_value && n >= 0 && n < list2_count);
        return list_words[n];
    }
    assert(word == list_value && n >= 0 && n < list_count);
    return list_words[n];
}
int TaLogic_IsJudgeUnitDanger(int a, int b, int skip)
{
    assert(skip == 0 && hit_calls < (list2_on ? list2_count : list_count));
    assert(((a << 8) | b) == list_words[hit_calls]);
    return list_hits[hit_calls++];
}

/* Fixtures for the body shared by 0x154eb0 and 0x155230: the seven keys are
 * ((i + 1) << 8 | i), the first key's cell is taken, the order's cell is
 * cell_result, and a non-zero level is assumed (key_alt answers level 0). */
enum { SCENE_OWN, SCENE_LISTED, SCENE_NEAR, SCENE_FAR, SCENE_ALT, SCENE_SECOND, SCENE_NONE, SCENE_COUNT };
static const unsigned scene_high[SCENE_COUNT] = { 0x0507, 0x0201, 0x0607, 0x0201, 0x0201, 0x0307, 0x0503 };

static void order_scene(int scene)
{
    int i;

    clear_grid();
    memset(cell_full, 0, sizeof cell_full);
    for (i = 0; i < 7; i++) {
        key_by[i] = ((i + 1) << 8) | i;
        key_alt[i] = key_by[i];
    }
    key_alt_on = 1;
    cell_full[0][1] = 1;
    cell_result = 0x0507;
    switch (scene) {
    case SCENE_OWN: /* 0x155670 finds the order's own position in the first cell */
        grid_cell[0][1] = 0x0103;
        break;
    case SCENE_LISTED: /* the cell is one of the level's keys */
        cell_result = 0x0100;
        break;
    case SCENE_NEAR: /* in neither list, row 5 */
        break;
    case SCENE_FAR: /* in neither list, row 6 */
        cell_result = 0x0607;
        break;
    case SCENE_ALT: /* only in the level 0 list */
        key_alt[2] = 0x0507;
        break;
    case SCENE_SECOND: /* no free position for the level, one for level 0 */
    case SCENE_NONE:
        cell_result = 0x0607;
        for (i = 0; i < 7; i++) {
            cell_full[i][i + 1] = 1;
        }
        if (scene == SCENE_SECOND) {
            key_alt[3] = 0x0307;
        }
        break;
    }
}

static void order_reset(struct DotrPositionOrder *order)
{
    memset(order, 0, sizeof *order);
    order->position = 0x0103;
    order->flags.word = 0xffff1234;
    word_on = 0;
    word_side = 1;
    memset(word_present, 0, sizeof word_present);
    word_present[3] = 1;
    pair_row = 1;
    pair_column = 3;
    key_calls = 0;
    flag_calls = 0;
    cell_log_n = 0;
    side_pair_calls = 0;
    side_pair_value = 0;
    hit_calls = 0;
}

static void order_is(const struct DotrPositionOrder *order, unsigned kind, unsigned high)
{
    assert(order->position == 0x0103 && order->kind == kind);
    assert(order->flags.word == ((high << 16) | 0x1234));
    assert(order->unknown_02 == 0 && order->unknown_06 == 0);
}

static void check_cell_scan(void)
{
    int i;

    /* 0x155670. First key's cell is the position; first key's cell is negative. */
    order_scene(SCENE_OWN);
    flag_value = 0x40;
    key_calls = 0; pair_calls = 0; cell_log_n = 0;
    assert(TaLogic017_IsLineTop(1, 3) == 1 && key_calls == 1 && key_c == 2);
    assert(pair_calls == 1 && cell_log_n == 1 && cell_log[0] == 0x0103);
    order_scene(SCENE_NEAR);
    flag_value = 0x40;
    key_calls = 0;
    assert(TaLogic017_IsLineTop(1, 3) == 0 && key_calls == 1);

    /* Keys (i, 1), all cells other positions: seventh key of the level's scan. */
    clear_grid();
    for (i = 0; i < 7; i++) {
        key_by[i] = 0x0100 + i;
        key_alt[i] = key_by[i];
        grid_cell[i][1] = 0x7777;
    }
    flag_value = 0x40;
    grid_cell[6][1] = 0x0103;
    key_calls = 0;
    assert(TaLogic017_IsLineTop(1, 3) == 1 && key_calls == 7 && key_c == 2);
    grid_cell[6][1] = 0x7777;
    /* Second scan asks level 0: its seventh key matches, is negative, differs. */
    key_alt[6] = 0x0207;
    grid_cell[7][2] = 0x0103;
    key_calls = 0;
    assert(TaLogic017_IsLineTop(1, 3) == 1 && key_calls == 14 && key_c == 0);
    grid_cell[7][2] = -1;
    key_calls = 0;
    assert(TaLogic017_IsLineTop(1, 3) == 0 && key_calls == 14);
    grid_cell[7][2] = 0x7777;
    key_calls = 0;
    assert(TaLogic017_IsLineTop(1, 3) == 0 && key_calls == 14 && key_c == 0);
    /* Level 1 (0x20) still asks level 0 second; level 0 (0x1f) asks level 1 second. */
    flag_value = 0x20;
    key_calls = 0;
    assert(TaLogic017_IsLineTop(1, 3) == 0 && key_calls == 14 && key_c == 0);
    flag_value = 0x1f;
    key_calls = 0;
    assert(TaLogic017_IsLineTop(1, 3) == 0 && key_calls == 14 && key_c == 1);
    key_alt_on = 0;
}

static void check_order_body(void)
{
    struct DotrPositionOrder order;
    int scene, deep;

    /* 0x154eb0 on the real 0x155670, 0x1554b0 and 0x1555f0, level 2. */
    for (scene = 0; scene < SCENE_COUNT; scene++) {
        order_reset(&order);
        order_scene(scene);
        flag_value = 0x40;
        TaLogic017_CommandBtlUnit(&order);
        order_is(&order, scene == SCENE_NONE ? 0x6ff : 0x7ff, scene_high[scene]);
        /* own cell, then column 0 of the other side's row */
        assert(side_pair_calls == 1 && cell_log[0] == 0x0103 && cell_log[1] == 0x0100);
    }
    /* a non-zero 0x24de70 answer asks row 0 */
    for (deep = 1; deep <= 2; deep++) {
        order_reset(&order);
        order_scene(SCENE_OWN);
        flag_value = 0x40;
        side_pair_value = deep;
        TaLogic017_CommandBtlUnit(&order);
        order_is(&order, 0x7ff, 0x0507);
        assert(cell_log[1] == 0x0000);
    }
    /* zero 0x1df540 word: nothing asked further, nothing written */
    order_reset(&order);
    order_scene(SCENE_OWN);
    word_present[3] = 0;
    TaLogic017_CommandBtlUnit(&order);
    assert(order.kind == 0 && order.flags.word == 0xffff1234);
    assert(side_pair_calls == 0 && cell_log_n == 0 && flag_calls == 0);

    /* 0x155230: levels 1, 2, 3 run the same body; the level is a byte. */
    for (deep = 1; deep <= 3; deep++) {
        for (scene = 0; scene < SCENE_COUNT; scene++) {
            order_reset(&order);
            order_scene(scene);
            flag_value = deep << 5;
            code_value = 820;
            pair_calls = 0;
            TaLogic017_CommandSpellUnit(&order);
            order_is(&order, scene == SCENE_NONE ? 0x6ff : 0x7ff, scene_high[scene]);
            assert(pair_calls >= 1 && cell_log[0] == 0x0103);
        }
    }
    order_reset(&order);
    order_scene(SCENE_NEAR);
    flag_value = 0x2040;
    TaLogic017_CommandSpellUnit(&order);
    order_is(&order, 0x7ff, 0x0607);
    /* levels 0 and 5: kind only */
    for (deep = 0; deep <= 5; deep += 5) {
        order_reset(&order);
        order_scene(SCENE_OWN);
        flag_value = deep == 0 ? 0x1f : 0xa0;
        TaLogic017_CommandSpellUnit(&order);
        assert(order.kind == 0x5ff && order.flags.word == 0xffff1234 && key_calls == 0);
    }
    /* level 4, kinds 820 and 823: a free position for the level, for level 0, none */
    for (deep = 820; deep <= 823; deep += 3) {
        static const int scenes[3] = { SCENE_FAR, SCENE_SECOND, SCENE_NONE };

        for (scene = 0; scene < 3; scene++) {
            order_reset(&order);
            order_scene(scenes[scene]);
            flag_value = 0x80;
            code_value = deep;
            TaLogic017_CommandSpellUnit(&order);
            order_is(&order, 0x6ff, scene_high[scenes[scene]]);
            assert(key_c == (scene == 0 ? 4 : 0));
        }
    }
    /* level 4, neighbouring kinds: the own cell, no key asked */
    for (deep = 819; deep <= 824; deep++) {
        if (deep == 820 || deep == 823) {
            continue;
        }
        order_reset(&order);
        order_scene(SCENE_FAR);
        flag_value = 0x80;
        code_value = deep;
        TaLogic017_CommandSpellUnit(&order);
        order_is(&order, 0x3ff, 0x0607);
        assert(key_calls == 0);
    }
    key_alt_on = 0;
}

static void check_order_kinds(void)
{
    static const int plain[6] = { 53, 58, 63, 65, 94, 96 };
    struct DotrPositionOrder order;
    int i;

    /* 0x155070 on the real 0x154eb0, which in SCENE_OWN writes 0x7ff and the
     * own cell after two 0x1e0700 requests. Kinds 54..57: no 0x154eb0. */
    for (i = 54; i <= 57; i++) {
        order_reset(&order);
        order_scene(SCENE_OWN);
        flag_value = 0x40;
        code_value = i;
        pair_calls = 0;
        TaLogic017_CommandEfcMonster(&order);
        order_is(&order, 0x6ff, 0x0507);
        assert(pair_calls == 1 && flag_calls == 0 && cell_log_n == 1 && cell_log[0] == 0x0103);
    }
    for (i = 0; i < 6; i++) {
        order_reset(&order);
        order_scene(SCENE_OWN);
        flag_value = 0x40;
        code_value = plain[i];
        TaLogic017_CommandEfcMonster(&order);
        order_is(&order, 0x7ff, 0x0507);
        assert(flag_calls == 2);
    }
    /* kind 95: total of the 0x250360 answer below 1200 */
    for (i = 1199; i <= 1200; i++) {
        order_reset(&order);
        order_scene(SCENE_OWN);
        flag_value = 0x40;
        code_value = 95;
        total_side_value = i;
        total_side_arg = -1;
        TaLogic017_CommandEfcMonster(&order);
        order_is(&order, i == 1199 ? 0x3ff : 0x7ff, 0x0507);
        assert(total_side_arg == 0x0103 && flag_calls == 2);
    }
    /* kind 64: at least two list entries for which 0x2781b0 answers exactly 1 */
    list_value = 0x4455;
    list_words[0] = 0x0a01; list_words[1] = 0x0b02; list_words[2] = 0x0c03;
    for (i = 0; i < 4; i++) {
        static const int counts[4] = { 0, 2, 2, 3 };
        static const int hits[4][3] = { { 0, 0, 0 }, { 1, 1, 0 }, { 1, 0, 0 }, { 1, 2, 1 } };
        static const unsigned kinds[4] = { 0x7ff, 0x3ff, 0x7ff, 0x3ff };

        order_reset(&order);
        order_scene(SCENE_OWN);
        flag_value = 0x40;
        code_value = 64;
        list_count = counts[i];
        list_hits[0] = hits[i][0]; list_hits[1] = hits[i][1]; list_hits[2] = hits[i][2];
        memset(list_args, -1, sizeof list_args);
        TaLogic017_CommandEfcMonster(&order);
        order_is(&order, kinds[i], 0x0507);
        assert(hit_calls == counts[i] && flag_calls == 2);
        assert(list_args[0] == 0 && list_args[1] == 0 && list_args[2] == 0x0103);
        assert(list_args[3] == 2 && list_args[4] == 3 && list_args[5] == 14);
    }
    key_alt_on = 0;
}

/* Stubs for the order writers that choose between try functions. The try
 * functions only log their identity and argument. */
enum { W_DDA0 = 1, W_E0E0, W_E030, W_E520, W_9D00 };
static int w_weight, w_test_by[3], w_try_c[4], w_try_n;
static int w_sent[4], w_sent_arg[4], w_sent_n;
static struct DotrPositionOrder *w_order;
static int w_bit, w_bit_calls, w_rule, w_rule_calls;
static unsigned *w_word;
static int w_near, w_near_calls, w_near_args[6];

static void w_send(int id, struct DotrPositionOrder *order, int arg)
{
    assert(order == w_order && w_sent_n < 4);
    w_sent[w_sent_n] = id;
    w_sent_arg[w_sent_n++] = arg;
}
static int w_test(int a, int b, int c)
{
    assert(a == 1 && b == 3 && (c == 1 || c == 2) && w_try_n < 4);
    w_try_c[w_try_n++] = c;
    return w_test_by[c];
}
int func_001e0290_try(int a, int b, int flag)
{
    assert(a == 1 && b == 3 && flag == 0);
    return w_weight;
}
int TaLogic022_IsEmptyCntrGuard(int a, int b, unsigned char c) { return w_test(a, b, c); }
int TaLogic019_IsEmptyCntrGuard(int a, int b, unsigned char c) { return w_test(a, b, c); }
void func_0016dda0_order(struct DotrPositionOrder *order) { w_send(W_DDA0, order, 0); }
void func_0016e0e0_order(struct DotrPositionOrder *order, unsigned char deep) { w_send(W_E0E0, order, deep); }
void func_0015e030_order(struct DotrPositionOrder *order) { w_send(W_E030, order, 0); }
void func_0015e520_order(struct DotrPositionOrder *order, unsigned char deep) { w_send(W_E520, order, deep); }
void func_00159d00_order(struct DotrPositionOrder *order) { w_send(W_9D00, order, 0); }
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 1 && b == 3 && bit == 4);
    w_bit_calls++;
    return w_bit;
}
int TaLogic_CommandGunnerUnit(struct DotrPositionOrder *order, int a)
{
    assert(order == w_order && a == 0);
    w_rule_calls++;
    return w_rule;
}
unsigned *func_00250740_word(int a, int b, int n)
{
    assert(a == 1 && b == 3 && n == 0);
    return w_word;
}
int TaLogic_GetLandChange(int a, int b, int c, int d, int e, int f)
{
    w_near_args[0] = a; w_near_args[1] = b; w_near_args[2] = c;
    w_near_args[3] = d; w_near_args[4] = e; w_near_args[5] = f;
    w_near_calls++;
    return w_near;
}

static void w_reset(struct DotrPositionOrder *order)
{
    order_reset(order);
    w_order = order;
    cell_result = 0x0507;
    word_present[3] = 0x77;
    w_try_n = 0;
    w_sent_n = 0;
    w_bit_calls = 0;
    w_rule_calls = 0;
    w_near_calls = 0;
    w_test_by[1] = 0;
    w_test_by[2] = 0;
    spot_list[0] = 0;
    spot_list[1] = 0;
}

/* One try function ran with this argument and the order was left alone. */
static void w_sent_is(const struct DotrPositionOrder *order, int id, int arg)
{
    assert(w_sent_n == 1 && w_sent[0] == id && w_sent_arg[0] == arg);
    assert(order->kind == 0 && order->flags.word == 0xffff1234);
}

static void check_try_choice(void)
{
    struct DotrPositionOrder order;
    int i;

    /* 0x16dca0 and 0x15ded0: zero word, weight at and below 2500, first test */
    for (i = 0; i < 2; i++) {
        void (*f)(struct DotrPositionOrder *) = i ? TaLogic019_CommandBtlUnit : TaLogic022_CommandBtlUnit;

        w_reset(&order);
        word_present[3] = 0;
        w_weight = 2500;
        f(&order);
        assert(w_sent_n == 0 && side_pair_calls == 0 && cell_log_n == 0);
        assert(order.kind == 0 && order.flags.word == 0xffff1234);

        w_reset(&order);
        w_weight = 2500;
        f(&order);
        w_sent_is(&order, i ? W_E030 : W_DDA0, 0);
        assert(w_try_n == 0 && side_pair_calls == 1);
        assert(cell_log_n == 2 && cell_log[0] == 0x0103 && cell_log[1] == 0x0100);

        w_reset(&order);
        side_pair_value = 1;
        w_weight = 2499;
        w_test_by[1] = 1;
        f(&order);
        w_sent_is(&order, i ? W_E520 : W_E0E0, 1);
        assert(w_try_n == 1 && w_try_c[0] == 1 && cell_log[1] == 0x0000);
    }
    /* 0x16dca0: first test 0 and 2 */
    for (i = 0; i <= 2; i += 2) {
        w_reset(&order);
        w_weight = 2499;
        w_test_by[1] = i;
        TaLogic022_CommandBtlUnit(&order);
        w_sent_is(&order, W_DDA0, 0);
        assert(w_try_n == 1);
    }
    /* 0x15ded0 on the real 0x15e2e0: its two positions; negative goes to the second test */
    w_reset(&order);
    w_weight = 2499;
    spot_list[0] = 1;
    grid_cell[0][5] = -1;
    TaLogic019_CommandBtlUnit(&order);
    order_is(&order, 0x6ff, 0x0500);
    assert(w_sent_n == 0 && w_try_n == 1);
    w_reset(&order);
    w_weight = 2499;
    w_test_by[1] = 2;
    spot_list[1] = 1;
    grid_cell[6][3] = -1;
    TaLogic019_CommandBtlUnit(&order);
    order_is(&order, 0x6ff, 0x0306);
    assert(w_sent_n == 0 && w_try_n == 1);
    w_reset(&order);
    w_weight = 2499;
    w_test_by[2] = 1;
    TaLogic019_CommandBtlUnit(&order);
    w_sent_is(&order, W_E520, 2);
    assert(w_try_n == 2 && w_try_c[0] == 1 && w_try_c[1] == 2);
    for (i = 0; i <= 2; i += 2) {
        w_reset(&order);
        w_weight = 2499;
        w_test_by[2] = i;
        TaLogic019_CommandBtlUnit(&order);
        w_sent_is(&order, W_E030, 0);
        assert(w_try_n == 2);
    }
}

static void check_kind_writers(void)
{
    static const int plain[6] = { 646, 648, 654, 656, 658, 660 };
    static const int other[6] = { 686, 688, 694, 696, 731, 733 };
    static unsigned word;
    struct DotrPositionOrder order;
    int i;

    /* 0x159fe0, kind 659: flag bit, then 0x27d720 answers 1, 0, 2 */
    for (i = 0; i < 4; i++) {
        w_reset(&order);
        code_value = 659;
        w_bit = i == 3;
        w_rule = i == 0 ? 1 : i == 1 ? 0 : i == 2 ? 2 : 1;
        pair_calls = 0;
        TaLogic018_CommandEfcMonster(&order);
        assert(pair_calls == 1 && w_bit_calls == 1 && w_rule_calls == (i == 3 ? 0 : 1));
        if (i == 0) {
            assert(w_sent_n == 0 && order.kind == 0 && order.flags.word == 0xffff1234);
        } else {
            w_sent_is(&order, W_9D00, 0);
        }
    }
    /* kind 647 */
    w_reset(&order);
    code_value = 647;
    w_rule = 1;
    TaLogic018_CommandEfcMonster(&order);
    assert(w_sent_n == 0 && w_bit_calls == 0 && order.kind == 0xbff && order.flags.word == 0xffff1234);
    for (i = 0; i <= 2; i += 2) {
        w_reset(&order);
        w_rule = i;
        TaLogic018_CommandEfcMonster(&order);
        w_sent_is(&order, W_9D00, 0);
    }
    /* kind 655: list of six and of five entries */
    list_value = 0x4455;
    w_reset(&order);
    code_value = 655;
    list_count = 6;
    memset(list_args, -1, sizeof list_args);
    TaLogic018_CommandEfcMonster(&order);
    order_is(&order, 0x3ff, 0x0507);
    assert(w_sent_n == 0 && cell_log_n == 1 && cell_log[0] == 0x0103);
    assert(list_args[0] == 0 && list_args[1] == 0 && list_args[2] == 0x0103);
    assert(list_args[3] == 2 && list_args[4] == 52 && list_args[5] == 14);
    w_reset(&order);
    list_count = 5;
    TaLogic018_CommandEfcMonster(&order);
    w_sent_is(&order, W_9D00, 0);
    for (i = 0; i < 6; i++) {
        w_reset(&order);
        code_value = plain[i];
        w_rule = 1;
        list_count = 6;
        TaLogic018_CommandEfcMonster(&order);
        w_sent_is(&order, W_9D00, 0);
        assert(w_rule_calls == 0 && w_bit_calls == 0);
    }

    /* 0x14c870 on the real 0x14f3a0: 1 with every cell taken, 0 with free
     * cells and a non-negative 0x24e410 answer. */
    w_reset(&order);
    memset(cell_full, 1, sizeof cell_full);
    w_word = 0;
    w_near = 0x0405;
    TaLogic014_CommandSpellUnit(&order);
    assert(order.kind == 0x5ff && order.flags.word == 0xffff1234 && w_near_calls == 0);
    assert(cell_log_n == 1 && cell_log[0] == 0x0103);
    w_word = &word;
    for (i = 0; i < 2; i++) {
        w_reset(&order);
        word = i ? 0x300 : 0x1ff;
        TaLogic014_CommandSpellUnit(&order);
        assert(order.kind == 0x5ff && order.flags.word == 0xffff1234 && w_near_calls == 0);
    }
    for (i = 0; i < 2; i++) {
        w_reset(&order);
        word = i ? 0x2ff : 0x200;
        w_near = i ? 0x0405 : 0;
        TaLogic014_CommandSpellUnit(&order);
        order_is(&order, 0x5ff, w_near);
        assert(w_near_calls == 1 && w_near_args[0] == 0x77 && w_near_args[1] == 7 && w_near_args[2] == 5);
        assert(w_near_args[3] == 4 && w_near_args[4] == 9 && w_near_args[5] == 2);
    }
    w_reset(&order);
    w_near = -1;
    TaLogic014_CommandSpellUnit(&order);
    assert(order.kind == 0x5ff && order.flags.word == 0xffff1234 && w_near_calls == 1);
    w_reset(&order);
    memset(cell_full, 0, sizeof cell_full);
    third_value = 5;
    w_near = 0x0405;
    TaLogic014_CommandSpellUnit(&order);
    assert(order.kind == 0x5ff && order.flags.word == 0xffff1234 && w_near_calls == 0);

    /* 0x1360e0, kind 695: rows 3 and 4, zero, negative */
    for (i = 0; i < 4; i++) {
        static const int near[4] = { 0x0305, 0x0405, 0, -1 };

        w_reset(&order);
        code_value = 695;
        w_near = near[i];
        pair_calls = 0;
        TaLogic009_CommandSpellUnit(&order);
        if (i == 0 || i == 2) {
            order_is(&order, 0x6ff, near[i]);
        } else {
            assert(order.kind == 0x5ff && order.flags.word == 0xffff1234);
        }
        assert(pair_calls == 1 && w_near_calls == 1 && w_near_args[0] == 0x77 && w_near_args[1] == 7);
        assert(w_near_args[2] == 5 && w_near_args[3] == 6 && w_near_args[4] == 5 && w_near_args[5] == 2);
    }
    for (i = 0; i < 2; i++) {
        w_reset(&order);
        order.flags.word = 0x00001234;
        code_value = i ? 732 : 687;
        TaLogic009_CommandSpellUnit(&order);
        order_is(&order, i ? 0xdff : 0xcff, 0xffff);
        assert(w_near_calls == 0);
    }
    for (i = 0; i < 6; i++) {
        w_reset(&order);
        code_value = other[i];
        w_near = 0x0305;
        TaLogic009_CommandSpellUnit(&order);
        assert(order.kind == 0x5ff && order.flags.word == 0xffff1234 && w_near_calls == 0);
    }
}

/* Stubs for the second set of order writers. The fallbacks log through w_send. */
enum { W_E7B0 = 10, W_C3B0, W_D110, W_77C0, W_C7F0, W_D490, W_DB00 };
static unsigned *m_word;
static int m_far_calls, m_far_args[3], m_side_spot, m_side_arg, m_scan;
static int m_buf_calls, m_gate[2], m_gate_arg;
static int m_try[4], m_try_n, m_other, m_r7, m_s6 = 1500;
static int m_bbc0, m_live, m_live_calls, m_step, m_block_byte, m_ab_calls;
static int m_weight, m_weight_calls, m_spot, m_spot_calls, m_be0_calls;
static struct DotrPositionBlock *m_block;

static int m_word_n;
unsigned *func_001e0b70_word(int a, int b, int n)
{
    assert(a == 1 && b == 3 && (n == 0 || n == 1));
    m_word_n = n;
    return m_word;
}
/* 0x131f80 runs for real: m_far_calls counts its 0x250370 requests, the
 * 0x27a130 stub answers from m_a130_ans in call order and keeps its arguments. */
static int m_far_side, m_score, m_score_calls, m_score_args[3];
static int m_a130_n, m_a130_ans[3], m_a130_args[3][8];
int func_00250370_word(int word)
{
    m_far_args[0] = word;
    m_far_calls++;
    return m_far_side;
}
int func_00250a60_spot(int word, int x, int y)
{
    m_score_args[0] = word; m_score_args[1] = x; m_score_args[2] = y;
    m_score_calls++;
    return m_score;
}
int func_0027a130_spot(int word, struct DotrSpotNibbles *bytes, int column, int row, int x, int y, int g, int h,
                       unsigned char i, unsigned short key)
{
    int n;
    assert(m_a130_n < 3 && word == m_far_args[0] && g == -1 && h == 0 && i == 7);
    for (n = 0; n < 12; n++) {
        assert(bytes[n].low == 0xf && bytes[n].high == 0xf);
    }
    m_far_args[1] = column; m_far_args[2] = row;
    m_a130_args[m_a130_n][0] = x; m_a130_args[m_a130_n][1] = y; m_a130_args[m_a130_n][2] = key;
    return m_a130_ans[m_a130_n++];
}
int func_001349a0_side(int side) { m_side_arg = side; return m_side_spot; }
int func_0016cbb0_word(int side, unsigned *record)
{
    assert(side == 0x0103 && record == w_word);
    return m_scan;
}
void func_0027e770_buf(void *buf)
{
    assert(buf != 0);
    m_buf_calls++;
}
int TaLogic011_IsUseLifeHourGlass(int side) { m_gate_arg = side; return m_gate[1]; }
void func_0013e7b0_order(struct DotrPositionOrder *order, int flag) { w_send(W_E7B0, order, flag); }
void func_0014c3b0_order(struct DotrPositionOrder *order) { w_send(W_C3B0, order, 0); }
int func_0027de20_order(struct DotrPositionOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7,
                        struct DotrPositionCell *cells, int s1, unsigned short *a, unsigned short *b, int n,
                        int s5, int s6, void *buf)
{
    int i;

    assert(order == w_order && r1 == 0 && r2 == 0x77 && r3 == 7 && r4 == 5);
    assert(r5 == m_other && r6 == m_try_n + 1 && r7 == m_r7 && m_try_n < 4);
    for (i = 0; i < 22; i++) {
        assert(cells[i].id == 0xffff && cells[i].kind == 0xff);
    }
    assert(s1 == 0 && a != b && *a == 0 && *b == 0 && n == 2 && s5 == 0 && s6 == m_s6 && buf != 0);
    return m_try[m_try_n++];
}
void func_0011d110_order(struct DotrPositionOrder *order, int flag) { w_send(W_D110, order, flag); }
void func_001277c0_order(struct DotrPositionOrder *order, int flag) { w_send(W_77C0, order, flag); }
void func_0012c7f0_pos(struct DotrPositionOrder *order, int flag, int deep) { w_send(W_C7F0, order, (flag << 8) | deep); }
void func_0010d490_pos(struct DotrPositionOrder *order, int flag, int deep) { w_send(W_D490, order, (flag << 8) | deep); }
void func_0010db00_pos(struct DotrPositionOrder *order, int one, int hit) { w_send(W_DB00, order, (one << 8) | hit); }
int TaLogic_IsAttackedNow(int a, int b, int c)
{
    assert(a == 1 && b == 3 && c == 0);
    return m_bbc0;
}
int SzDuel_InqHandPhase(int index)
{
    assert(index == 1);
    m_live_calls++;
    return m_live;
}
int func_0015a5f0_block(int side, struct DotrPositionBlock *block)
{
    assert(side == 1);
    m_block = block;
    block->unknown_3e = m_block_byte;
    return m_step;
}
int func_0015ab10_block(int side, unsigned char *item, struct DotrPositionBlock *block)
{
    assert(side == 1 && item != 0 && block == m_block);
    m_ab_calls++;
    return 0;
}
int func_001e02f0_block(struct DotrPositionBlock *block, int unknown)
{
    assert(block == m_block && unknown == 0);
    m_weight_calls++;
    return m_weight;
}
int TaLogic018_GetVirusMove(int a, int b)
{
    assert(a == 1 && b == 3);
    m_spot_calls++;
    return m_spot;
}
/* 0x159be0 runs for real: its list comes from here (m_be0_calls counts the requests). */
static int m_list_n, m_list_ret, m_list_a, m_list_b;
static unsigned short m_list[49];
int func_0015a120_count(int a, int b, unsigned short *out)
{
    int i;
    m_be0_calls++;
    m_list_a = a;
    m_list_b = b;
    for (i = 0; i < m_list_n; i++) {
        out[i] = m_list[i];
    }
    return m_list_ret;
}

static void m_reset(struct DotrPositionOrder *order)
{
    w_reset(order);
    pair_calls = 0;
    m_far_calls = 0;
    m_far_side = 0;
    m_score_calls = 0;
    m_a130_n = 0;
    m_a130_ans[0] = 0;
    m_a130_ans[1] = 0;
    m_a130_ans[2] = 0;
    cell_zero_on = 0;
    m_buf_calls = 0;
    m_try_n = 0;
    m_live_calls = 0;
    m_ab_calls = 0;
    m_weight_calls = 0;
    m_spot_calls = 0;
    m_be0_calls = 0;
    m_list_n = 0;
    m_list_ret = 0;
    m_gate[1] = 0;
    list_count = 0;
    w_rule = 0;
    w_bit = 0;
}
static void m_untouched(const struct DotrPositionOrder *order, unsigned kind)
{
    assert(order->kind == kind && order->flags.word == 0xffff1234 && w_sent_n == 0);
}

static void check_word_eight(void)
{
    static unsigned words[2];
    struct DotrPositionOrder order;
    int i, j;

    /* word 8: kind 0x2ff, bit 24 follows a zero 0x250360 answer; 0x131f80 is not run */
    for (i = 0; i < 2; i++) {
        m_reset(&order);
        words[0] = (8 << 8) | (i ? 0xff : 0);
        m_word = words;
        m_word_n = -1;
        pair_zero_on = i;
        TaLogic008_CommandInvadeUnit(&order);
        pair_zero_on = 0;
        order_is(&order, 0x2ff, i ? 0x100 : 0);
        assert(pair_calls == 1 && m_word_n == 1 && m_far_calls == 0 && w_sent_n == 0);
    }
    /* no word, and the neighbours of 8: the real 0x131f80 answers its first
     * position, its 0x1e1090 cell, or nothing */
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 3; i++) {
            m_reset(&order);
            words[0] = ((j == 1 ? 7 : j == 2 ? 9 : 0x10008) << 8) | 0xff;
            m_word = j == 0 ? NULL : words;
            m_a130_ans[0] = i == 0 ? 0 : -1;
            m_a130_ans[1] = -1;
            cell_zero_on = 1;
            cell_zero = i == 2 ? -1 : 0x0405;
            TaLogic008_CommandInvadeUnit(&order);
            if (i < 2) {
                order_is(&order, 0x6ff, i == 0 ? 0x0606 : 0x0405);
            } else {
                m_untouched(&order, 0x5ff);
            }
            assert(pair_calls == 1 && m_word_n == 1 && m_far_calls == (i == 0 ? 1 : 2));
        }
    }
}

static void check_level_writers(void)
{
    static const int far[3] = { 0x0405, 0, -1 };
    static const int plain[6] = { 1, 3, 36, 38, 71, 73 };
    static const int other[9] = { 1, 3, 5, 19, 21, 30, 32, 36, 38 };
    static unsigned words[2];
    struct DotrPositionOrder order;
    int i, j;

    /* 0x132280 on the real 0x131f80, level 3 and level 1 with word 37: its first
     * position, its 0x1e1090 cell and a negative answer */
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 3; i++) {
            m_reset(&order);
            flag_value = j ? 0x20 : 0x60;
            words[0] = (37 << 8) | 0xff;
            m_word = words;
            m_a130_ans[0] = i == 0 ? 0 : -1;
            m_a130_ans[1] = -1;
            cell_zero_on = 1;
            cell_zero = i == 2 ? -1 : 0x0405;
            TaLogic008_CommandSpellUnit(&order);
            if (i < 2) {
                order_is(&order, 0x3ff, i == 0 ? 0x0606 : 0x0405);
            } else {
                m_untouched(&order, 0x5ff);
            }
            assert(pair_calls == 1 && flag_calls == 1 && m_far_calls == (i == 0 ? 1 : 2));
            assert(m_far_args[0] == 0x77 && m_far_args[1] == 7 && m_far_args[2] == 5);
        }
    }
    /* level 1 (also with bits above the byte): null word, words 2 and 72, others */
    for (i = 0; i < 2; i++) {
        m_reset(&order);
        flag_value = i ? 0x2020 : 0x20;
        m_word = 0;
        TaLogic008_CommandSpellUnit(&order);
        m_untouched(&order, 0);
    }
    m_word = words;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 3; i++) {
            m_reset(&order);
            flag_value = 0x3f;
            words[0] = j ? 72 << 8 : (2 << 8) | 0xff;
            m_side_spot = far[i];
            m_side_arg = -1;
            TaLogic008_CommandSpellUnit(&order);
            order_is(&order, 0x6ff, i < 2 ? far[i] : 6);
            assert(m_side_arg == 0x0103 && m_far_calls == 0);
        }
    }
    for (i = 0; i < 6; i++) {
        m_reset(&order);
        flag_value = 0x20;
        words[0] = plain[i] << 8;
        m_side_spot = 0x0405;
        TaLogic008_CommandSpellUnit(&order);
        m_untouched(&order, 0x5ff);
        assert(m_far_calls == 0);
    }
    /* levels 0, 2 and 4 */
    for (i = 0; i <= 4; i += 2) {
        m_reset(&order);
        flag_value = i << 5;
        words[0] = 37 << 8;
        TaLogic008_CommandSpellUnit(&order);
        m_untouched(&order, 0x5ff);
        assert(m_far_calls == 0);
    }

    /* 0x16f240. Word 20; word 4 with the 0x16cbb0 answers and low bits */
    w_word = words;
    words[1] = 9;
    m_reset(&order);
    words[0] = (20 << 8) | 0xff;
    TaLogic022_CommandSpellUnit(&order);
    m_untouched(&order, 0xcff);
    assert(flag_calls == 1 && pair_calls == 1);
    for (i = 0; i < 4; i++) {
        static const int scan[4] = { -1, 0x0302, 0x0302, 0 };
        static const unsigned low[4] = { 0, 0, 1, 0 };
        static const unsigned high[4] = { 0, 0x0502, 0x0302, 0x0500 };

        m_reset(&order);
        words[0] = (4 << 8) | low[i];
        m_scan = scan[i];
        TaLogic022_CommandSpellUnit(&order);
        if (i == 0) {
            m_untouched(&order, 0x5ff);
        } else {
            order_is(&order, 0x6ff, high[i]);
        }
    }
    /* word 2: the 0x27c770 answers */
    for (i = 0; i < 3; i++) {
        m_reset(&order);
        words[0] = 2 << 8;
        w_near = far[i];
        TaLogic022_CommandSpellUnit(&order);
        if (i < 2) {
            order_is(&order, 0x6ff, far[i]);
        } else {
            m_untouched(&order, 0x5ff);
        }
        assert(w_near_calls == 1 && w_near_args[0] == 0x77 && w_near_args[1] == 7 && w_near_args[2] == 5);
        assert(w_near_args[3] == 9 && w_near_args[4] == 8 && w_near_args[5] == 2);
    }
    /* word 37: low bits 32..34; with 33 the 0x16dca0 outcomes */
    for (i = 32; i <= 34; i += 2) {
        m_reset(&order);
        words[0] = (37 << 8) | i;
        w_weight = 2500;
        TaLogic022_CommandSpellUnit(&order);
        m_untouched(&order, 0x5ff);
    }
    words[0] = (37 << 8) | 33;
    m_reset(&order);
    word_present[3] = 0;
    TaLogic022_CommandSpellUnit(&order);
    m_untouched(&order, 0);
    assert(side_pair_calls == 0);
    m_reset(&order);
    w_weight = 2500;
    TaLogic022_CommandSpellUnit(&order);
    w_sent_is(&order, W_DDA0, 0);
    assert(w_try_n == 0 && side_pair_calls == 1 && cell_log_n == 3 && cell_log[2] == 0x0100);
    for (i = 0; i < 3; i++) {
        m_reset(&order);
        w_weight = 2499;
        w_test_by[1] = i;
        TaLogic022_CommandSpellUnit(&order);
        w_sent_is(&order, i == 1 ? W_E0E0 : W_DDA0, i == 1);
        assert(w_try_n == 1 && w_try_c[0] == 1);
    }
    /* word 31: kinds 731..733 */
    for (i = 731; i <= 733; i++) {
        m_reset(&order);
        words[0] = 31 << 8;
        code_value = i;
        TaLogic022_CommandSpellUnit(&order);
        m_untouched(&order, i == 732 ? 0xdff : 0x5ff);
    }
    for (i = 0; i < 9; i++) {
        m_reset(&order);
        words[0] = (other[i] << 8) | 33;
        code_value = 732;
        m_scan = 0x0302;
        w_near = 0x0405;
        TaLogic022_CommandSpellUnit(&order);
        m_untouched(&order, 0x5ff);
        assert(w_near_calls == 0);
    }
}

static void check_fallback_writers(void)
{
    static const int sides[3] = { 2, 0x0a01, 0x0b02 };
    static const int plain_e[6] = { 376, 378, 385, 387, 389, 391 };
    static const int plain_c[8] = { 550, 552, 569, 571, 574, 576, 611, 613 };
    static const int plain_d[4] = { 495, 497, 508, 511 };
    struct DotrPositionOrder order;
    int i, j;

    /* 0x141f30: empty list; one entry answering 1, 0, 2; three entries with
     * two answers of 1; side equal to the first / second entry's 0x250360 */
    list_value = 0x4455;
    list_words[0] = 0x0a01; list_words[1] = 0x0b02; list_words[2] = 0x0c03;
    for (i = 0; i < 7; i++) {
        static const int counts[7] = { 0, 1, 1, 1, 3, 3, 3 };
        static const int hits[7][3] = { { 0, 0, 0 }, { 1, 0, 0 }, { 0, 0, 0 }, { 2, 0, 0 }, { 1, 0, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
        static const int side_at[7] = { 0, 0, 0, 0, 0, 1, 2 };
        static const int answers[7] = { 0, 1, 0, 0, 1, 0, 0 };
        static const int asked[7] = { 0, 1, 1, 1, 3, 0, 1 };

        hit_calls = 0;
        list_count = counts[i];
        list_hits[0] = hits[i][0]; list_hits[1] = hits[i][1]; list_hits[2] = hits[i][2];
        memset(list_args, -1, sizeof list_args);
        assert(TaLogic011_IsUseBindChain(sides[side_at[i]]) == answers[i] && hit_calls == asked[i]);
        assert(list_args[0] == 0 && list_args[1] == 0 && list_args[2] == sides[side_at[i]]);
        assert(list_args[3] == 3 && list_args[4] == 3 && list_args[5] == 14);
    }

    /* 0x13ecf0: zero word; kind 386 on the real 0x141f30 answering 1 and 0 */
    m_reset(&order);
    word_present[3] = 0;
    code_value = 386;
    TaLogic011_CommandEfcMonster(&order);
    m_untouched(&order, 0);
    assert(side_pair_calls == 0 && m_buf_calls == 0);
    for (i = 0; i < 2; i++) {
        m_reset(&order);
        side_pair_value = 2;
        list_count = 1;
        list_hits[0] = i == 0;
        TaLogic011_CommandEfcMonster(&order);
        if (i == 0) {
            order_is(&order, 0x6ff, 0x0507);
            assert(w_sent_n == 0);
        } else {
            w_sent_is(&order, W_E7B0, 1);
        }
        assert(list_args[2] == 2 && hit_calls == 1 && m_buf_calls == 1 && side_pair_calls == 1);
    }
    /* kind 388: own total below, equal and above, from side 0, 1 and 2 */
    total_by_on = 1;
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
            m_reset(&order);
            code_value = 388;
            side_pair_value = j;
            total_by[j ? 0 : 1] = 6;
            total_by[j] = 5 + i;
            TaLogic011_CommandEfcMonster(&order);
            if (i == 0) {
                order_is(&order, 0x6ff, 0x0507);
                assert(w_sent_n == 0);
            } else {
                w_sent_is(&order, W_E7B0, 1);
            }
            assert(total_side_arg == (j ? 0 : 1));
        }
    }
    total_by_on = 0;
    /* kind 377: 0x142040 answers 1, 0, 2 and 0x27d720 answers 1, 0, 2 */
    for (i = 0; i < 5; i++) {
        m_reset(&order);
        code_value = 377;
        m_gate[1] = i == 3 ? 0 : i == 4 ? 2 : 1;
        w_rule = i == 1 ? 0 : i == 2 ? 2 : 1;
        TaLogic011_CommandEfcMonster(&order);
        if (i == 0) {
            m_untouched(&order, 0xbff);
        } else {
            w_sent_is(&order, W_E7B0, 1);
        }
        assert(w_rule_calls == (i < 3));
    }
    /* kind 390 on the real 0x142220: total 1200 / 1199, count 4 / 3 */
    for (i = 0; i < 4; i++) {
        m_reset(&order);
        code_value = 390;
        side_pair_value = 1;
        total_value = i == 2 ? 1199 : 1200;
        count_value = i == 3 ? 3 : 4;
        w_rule = i != 1;
        TaLogic011_CommandEfcMonster(&order);
        if (i == 0) {
            m_untouched(&order, 0xbff);
            assert(count_side == 1);
        } else {
            w_sent_is(&order, W_E7B0, 1);
        }
        assert(w_rule_calls == (i < 2));
    }
    for (i = 0; i < 6; i++) {
        m_reset(&order);
        code_value = plain_e[i];
        list_count = 1;
        list_hits[0] = 1;
        m_gate[1] = 1;
        w_rule = 1;
        TaLogic011_CommandEfcMonster(&order);
        w_sent_is(&order, W_E7B0, 1);
        assert(w_rule_calls == 0 && m_buf_calls == 1 && hit_calls == 0);
    }

    /* 0x14c720 on the real 0x14fa10: kinds 612 and 551 with answers 1, 0, 0 */
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 3; i++) {
            m_reset(&order);
            code_value = j ? 551 : 612;
            list_value = 0x4455;
            hit_calls = 0;
            if (j == 0) {
                list_count = i == 0 ? 4 : 3;
            } else {
                list_count = i == 1;
                list2_on = 1;
                list2_value = 0x4466;
                list2_count = 2;
                list_words[0] = 0x0201;
                list_words[1] = 0x0302;
                list_hits[0] = 1;
                list_hits[1] = i == 0;
            }
            TaLogic014_CommandEfcMonster(&order);
            if (i == 0) {
                order_is(&order, 0x3ff, 0x0507);
                assert(w_sent_n == 0);
            } else {
                w_sent_is(&order, W_C3B0, 0);
            }
            assert(list_args[2] == 0x0103 && list_args[4] == (j ? 15 : 16) && pair_calls == 1);
            list2_on = 0;
        }
    }
    /* kinds 570 and 575 on the real TaLogic014_IsActiveMode (1 with every cell taken, 0
     * with free cells and a non-negative 0x24e410 answer) */
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 5; i++) {
            m_reset(&order);
            code_value = j ? 575 : 570;
            memset(cell_full, i != 3, sizeof cell_full);
            third_value = 5;
            w_rule = i == 1 ? 0 : i == 2 ? 2 : 1;
            w_bit = i == 4;
            TaLogic014_CommandEfcMonster(&order);
            if (i == 0 || (i == 4 && j == 0)) {
                m_untouched(&order, 0);
            } else {
                w_sent_is(&order, W_C3B0, 0);
            }
            assert(w_bit_calls == (j && i != 3));
            assert(w_rule_calls == (i < 3 || (i == 4 && j == 0)));
        }
    }
    memset(cell_full, 1, sizeof cell_full);
    for (i = 0; i < 8; i++) {
        m_reset(&order);
        code_value = plain_c[i];
        w_rule = 1;
        TaLogic014_CommandEfcMonster(&order);
        w_sent_is(&order, W_C3B0, 0);
        assert(w_rule_calls == 0);
    }
    memset(cell_full, 0, sizeof cell_full);

    /* 0x11d9b0 on the real TaLogic_GetWarnHideLvl__0011fa80 (500 here): zero word; kinds 496 and 510
     * with first answers 0..3, second answers 0 and 1, lists of 4 and 3 */
    total_value = 1001;
    count_value = 3;
    m_r7 = 500;
    list_value = 0x4455;
    m_reset(&order);
    word_present[3] = 0;
    code_value = 496;
    TaLogic_CommandEfcMonster__0011d9b0(&order);
    m_untouched(&order, 0);
    assert(m_try_n == 0 && m_buf_calls == 0);
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 8; i++) {
            static const int first[8] = { 0, 1, 2, 3, 4, 4, 4, 4 };
            static const int second[8] = { 0, 0, 0, 0, 0, 1, 4, -1 };

            m_reset(&order);
            code_value = j ? 510 : 496;
            side_pair_value = j;
            m_other = j ? 0 : 1;
            m_try[0] = first[i];
            m_try[1] = second[i];
            list_count = i == 6 ? 4 : 3;
            memset(list_args, -1, sizeof list_args);
            TaLogic_CommandEfcMonster__0011d9b0(&order);
            assert(m_try_n == (i < 4 ? 1 : 2) && m_buf_calls == 1 && count_side == j);
            if (i == 1 || i == 5) {
                m_untouched(&order, 0);
            } else if (i == 7) {
                w_sent_is(&order, W_D110, 0);
            } else {
                order_is(&order, 0x3ff, 0x0507);
                assert(w_sent_n == 0);
            }
            if (i >= 6) {
                assert(list_args[0] == 0 && list_args[1] == 0 && list_args[2] == j);
                assert(list_args[3] == 1 && list_args[4] == 14 && list_args[5] == 14);
            } else {
                assert(list_args[0] == -1);
            }
        }
    }
    /* kind 509: 0x27d720 answers 1, 0, 2 */
    for (i = 0; i < 3; i++) {
        m_reset(&order);
        code_value = 509;
        w_rule = i == 0 ? 1 : i == 1 ? 0 : 2;
        TaLogic_CommandEfcMonster__0011d9b0(&order);
        if (i == 0) {
            m_untouched(&order, 0xbff);
        } else {
            w_sent_is(&order, W_D110, 0);
        }
        assert(m_try_n == 0 && w_rule_calls == 1);
    }
    for (i = 0; i < 4; i++) {
        m_reset(&order);
        code_value = plain_d[i];
        w_rule = 1;
        list_count = 4;
        TaLogic_CommandEfcMonster__0011d9b0(&order);
        w_sent_is(&order, W_D110, 0);
        assert(m_try_n == 0 && w_rule_calls == 0);
    }

    /* 0x127b00 on the real TaLogic_GetWarnHideLvl__0012b1b0 (1000 here): zero word; kind 108 with first
     * answers 0..3, second answers 0, 1, 4 and -1 (the kind is stored after two tries) */
    total_value = 1201;
    count_value = 4;
    m_r7 = 1000;
    m_s6 = 2000;
    m_reset(&order);
    word_present[3] = 0;
    code_value = 108;
    TaLogic006_CommandEfcMonster(&order);
    m_untouched(&order, 0);
    assert(m_try_n == 0 && m_buf_calls == 0);
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 8; i++) {
            static const int first[8] = { 0, 1, 2, 3, 4, 4, 4, 4 };
            static const int second[8] = { 0, 0, 0, 0, 0, 1, 4, -1 };

            m_reset(&order);
            code_value = 108;
            side_pair_value = j;
            m_other = j ? 0 : 1;
            m_try[0] = first[i];
            m_try[1] = second[i];
            memset(list_args, -1, sizeof list_args);
            TaLogic006_CommandEfcMonster(&order);
            assert(m_try_n == (i < 4 ? 1 : 2) && m_buf_calls == 1 && count_side == j);
            if (i == 1 || i == 5) {
                m_untouched(&order, 0);
            } else {
                order_is(&order, 0x3ff, 0x0507);
            }
            assert(w_sent_n == 0 && list_args[0] == -1);
        }
    }
    /* kind 111: 0x27d720 answers 1, 0, 2; other kinds go to 0x1277c0 with 1 */
    for (i = 0; i < 3; i++) {
        m_reset(&order);
        code_value = 111;
        w_rule = i == 0 ? 1 : i == 1 ? 0 : 2;
        TaLogic006_CommandEfcMonster(&order);
        if (i == 0) {
            m_untouched(&order, 0xbff);
        } else {
            w_sent_is(&order, W_77C0, 1);
        }
        assert(m_try_n == 0 && w_rule_calls == 1);
    }
    for (i = 0; i < 4; i++) {
        static const int plain_k[4] = { 107, 109, 110, 112 };

        m_reset(&order);
        code_value = plain_k[i];
        w_rule = 1;
        TaLogic006_CommandEfcMonster(&order);
        w_sent_is(&order, W_77C0, 1);
        assert(m_try_n == 0 && w_rule_calls == 0);
    }
    m_s6 = 1500;

    /* TaLogic007_CommandEfcMonster (real 0x130780: 750 here) and
     * TaLogic_CommandEfcMonster__0010d880 (real 0x10eda0: 0): zero word; the two loop kinds with
     * first answers 0..3 and second answers 0, 1, 4, -1 */
    { int k; for (k = 0; k < 2; k++) {
        total_value = k ? 1501 : 2001;
        count_value = 4;
        m_r7 = k ? 0 : 750;
        m_s6 = k ? 2400 : 2500;
        m_reset(&order);
        word_present[3] = 0;
        code_value = k ? 402 : 298;
        if (k) {
            TaLogic_CommandEfcMonster__0010d880(&order, 0x55, 0x66);
        } else {
            TaLogic007_CommandEfcMonster(&order, 0x66);
        }
        m_untouched(&order, 0);
        assert(m_try_n == 0 && m_buf_calls == 0);
        for (j = 0; j < 2; j++) {
            for (i = 0; i < 8; i++) {
                static const int first[8] = { 0, 1, 2, 3, 4, 4, 4, 4 };
                static const int second[8] = { 0, 0, 0, 0, 0, 1, 4, -1 };

                m_reset(&order);
                code_value = k ? 402 + j : (j ? 322 : 298);
                side_pair_value = j;
                m_other = j ? 0 : 1;
                m_try[0] = first[i];
                m_try[1] = second[i];
                if (k) {
                    TaLogic_CommandEfcMonster__0010d880(&order, 0x55, 0x66);
                } else {
                    TaLogic007_CommandEfcMonster(&order, 0x66);
                }
                assert(m_try_n == (i < 4 ? 1 : 2) && m_buf_calls == 1 && count_side == j);
                if (i == 1 || i == 5) {
                    m_untouched(&order, 0);
                } else {
                    order_is(&order, 0x3ff, 0x0507);
                }
                assert(w_sent_n == 0);
            }
        }
    } }
    m_s6 = 1500;
    /* 0x12cbe0 kind 305: 0x1e2100 bit 4 clear stores 0xcff, set goes on; kind 326 stores
     * 0x3ff; other kinds go to 0x12c7f0 with 1 and the deep argument */
    for (i = 0; i < 2; i++) {
        m_reset(&order);
        code_value = 305;
        w_bit = i;
        TaLogic007_CommandEfcMonster(&order, 0x66);
        if (i == 0) {
            order_is(&order, 0xcff, 0x0507);
            assert(w_sent_n == 0);
        } else {
            w_sent_is(&order, W_C7F0, 0x166);
        }
        assert(m_try_n == 0 && w_bit_calls == 1);
    }
    w_bit = 0;
    m_reset(&order);
    code_value = 326;
    TaLogic007_CommandEfcMonster(&order, 0x66);
    order_is(&order, 0x3ff, 0x0507);
    assert(w_sent_n == 0 && m_try_n == 0);
    for (i = 0; i < 5; i++) {
        static const int plain_m[5] = { 297, 299, 304, 323, 327 };

        m_reset(&order);
        code_value = plain_m[i];
        TaLogic007_CommandEfcMonster(&order, 0x66);
        w_sent_is(&order, W_C7F0, 0x166);
        assert(m_try_n == 0);
    }
    /* 0x10d880 kind 417: 0x27d720 answers 1, 0, 2; kind 408: the side total 999 and 1000;
     * other kinds go to 0x10d490 with 1 and the third argument */
    for (i = 0; i < 3; i++) {
        m_reset(&order);
        code_value = 417;
        w_rule = i == 0 ? 1 : i == 1 ? 0 : 2;
        TaLogic_CommandEfcMonster__0010d880(&order, 0x55, 0x66);
        if (i == 0) {
            m_untouched(&order, 0);
            assert(w_sent_n == 0);
        } else {
            w_sent_is(&order, W_D490, 0x166);
        }
        assert(m_try_n == 0 && w_rule_calls == 1);
    }
    for (i = 0; i < 2; i++) {
        m_reset(&order);
        code_value = 408;
        side_pair_value = 1;
        total_by_on = 0;
        total_side_arg = -1;
        total_side_value = 999 + i;
        TaLogic_CommandEfcMonster__0010d880(&order, 0x55, 0x66);
        assert(total_side_arg == 1 && m_try_n == 0);
        if (i == 0) {
            assert(w_sent_n == 2 && w_sent[0] == W_DB00 && w_sent_arg[0] == 0);
            assert(w_sent[1] == W_D490 && w_sent_arg[1] == 0x166);
        } else {
            w_sent_is(&order, W_D490, 0x166);
        }
    }
    for (i = 0; i < 4; i++) {
        static const int plain_n[4] = { 401, 404, 409, 418 };

        m_reset(&order);
        code_value = plain_n[i];
        w_rule = 1;
        TaLogic_CommandEfcMonster__0010d880(&order, 0x55, 0x66);
        w_sent_is(&order, W_D490, 0x166);
        assert(m_try_n == 0 && w_rule_calls == 0);
    }

    /* 0x159760: 0x27bbc0 is 1 with 0x1de6d0 answers 1, 0, 2 */
    for (i = 0; i < 3; i++) {
        m_reset(&order);
        m_bbc0 = 1;
        m_live = i == 0 ? 1 : i == 1 ? 0 : 2;
        m_step = 0;
        TaLogic018_CommandLDUnit(&order);
        m_untouched(&order, i == 0 ? 0x50a : 0x5ff);
        assert(m_live_calls == 1 && m_spot_calls == 0 && cell_log_n == 1 && cell_log[0] == 0x0103);
    }
    /* negative 0x15a5f0 answer */
    for (i = 0; i < 3; i++) {
        m_reset(&order);
        m_bbc0 = i == 2 ? 2 : 0;
        m_live = i == 0;
        m_step = -1;
        m_spot = 0x0405;
        TaLogic018_CommandLDUnit(&order);
        order_is(&order, i == 0 ? 0x60a : 0x6ff, 0x0405);
        assert(m_live_calls == 1 && m_spot_calls == 1 && m_ab_calls == 0 && m_weight_calls == 0);
    }
    /* answers 0 and 1; block byte 0x1f and 0x20; weight 1501 and 1500 */
    for (i = 0; i < 5; i++) {
        m_reset(&order);
        m_bbc0 = 0;
        m_live = i & 1;
        m_step = i == 1 ? 1 : 0;
        m_block_byte = i == 2 ? 0x20 : 0x1f;
        m_weight = i == 3 ? 1500 : 1501;
        m_spot = 0x0405;
        TaLogic018_CommandLDUnit(&order);
        order_is(&order, (i & 1) ? 0x60a : 0x6ff, 0x0405);
        assert(m_ab_calls == (i != 1) && m_weight_calls == (i != 2));
        assert(m_be0_calls == (i != 2 && i != 3) && m_spot_calls == 1);
        assert(m_be0_calls == 0 || (cell_row == 0 && cell_column == 0));
        assert(m_live_calls == 1 + m_be0_calls);
    }
}

/* 0x159be0 on stubs: the list entries are (b << 8) | word_side with b in 1..5. */
static void check_nearest(void)
{
    struct DotrPositionOrder order;
    int i;

    m_reset(&order);
    word_on = 0;
    word_side = 9;
    cell_result = 0x0507;
    for (i = 0; i < 8; i++) {
        word_present[i] = 0;
    }
    /* empty list: a count byte of zero, sides 0 and 2 */
    m_list_ret = 0x100;
    assert(TaLogic018_GetSummonPoint(0, 9, 3) == -1);
    assert(cell_row == 1 && cell_column == 0 && m_list_a == 7 && m_list_b == 5 && m_be0_calls == 1);
    assert(TaLogic018_GetSummonPoint(2, 9, 3) == -1);
    assert(cell_row == 0 && cell_column == 0);
    /* three free entries: exact cell, tie keeps the first, each side of x and y */
    m_list[0] = 0x0109;
    m_list[1] = 0x0309;
    m_list[2] = 0x0509;
    m_list_n = 3;
    m_list_ret = 3;
    assert(TaLogic018_GetSummonPoint(1, 9, 3) == 0x0309);
    assert(TaLogic018_GetSummonPoint(1, 9, 4) == 0x0309);
    assert(TaLogic018_GetSummonPoint(1, 9, 5) == 0x0509);
    assert(TaLogic018_GetSummonPoint(1, 12, 0) == 0x0109);
    assert(TaLogic018_GetSummonPoint(1, 5, 6) == 0x0509);
    /* only the counted entries are read */
    m_list_ret = 0x102;
    assert(TaLogic018_GetSummonPoint(1, 9, 5) == 0x0309);
    /* an occupied entry is passed over; all occupied gives the cell */
    m_list_ret = 3;
    word_present[3] = 1;
    assert(TaLogic018_GetSummonPoint(1, 9, 3) == 0x0109);
    word_present[1] = 1;
    word_present[5] = 1;
    assert(TaLogic018_GetSummonPoint(1, 9, 3) == 0x0507);
    /* distance 13 is taken, 14 is not */
    word_present[1] = 0;
    m_list_ret = 1;
    assert(TaLogic018_GetSummonPoint(1, 22, 1) == 0x0109);
    assert(TaLogic018_GetSummonPoint(1, 23, 1) == 0x0507);
    assert(TaLogic018_GetSummonPoint(1, 9, 14) == 0x0109);
    assert(TaLogic018_GetSummonPoint(1, 9, 15) == 0x0507);
    for (i = 0; i < 8; i++) {
        word_present[i] = 0;
    }
}

/* 0x15e2e0 on stubs; the 0x250360 stub answers the row, so "same answer" is "same row". */
static void check_spot_stages(void)
{
    static const int cells[4] = { -1, 0x0103, 0x0204, 0x0104 };
    int i, stage;

    pair_row_on = 1;
    spot_list[0] = 0;
    spot_list[1] = 0;
    grid_calls = 0;
    assert(TaLogic019_IsEmptyTrapGuard(1, 3) == -1 && grid_calls == 0);
    assert(list_args[0] == 0 && list_args[1] == 0 && list_args[2] == 1 && list_args[3] == 1);
    assert(list_args[4] == 0x2ed001c && list_args[5] == 14);
    /* each stage alone: no cell, the own cell, another row, the same row */
    for (stage = 0; stage < 2; stage++) {
        int *cell = stage == 0 ? &grid_cell[0][5] : &grid_cell[6][3];
        int answer = stage == 0 ? 0x500 : 0x306;

        spot_list[stage] = 1;
        for (i = 0; i < 4; i++) {
            *cell = cells[i];
            grid_calls = 0;
            assert(TaLogic019_IsEmptyTrapGuard(1, 3) == (i == 3 ? -1 : answer) && grid_calls == 1);
        }
        *cell = -1;
        spot_list[stage] = 0;
    }
    /* both lists: the first stage passes and the second answers, or both pass */
    spot_list[0] = 1;
    spot_list[1] = 1;
    grid_cell[0][5] = 0x0104;
    grid_cell[6][3] = 0x0204;
    grid_calls = 0;
    assert(TaLogic019_IsEmptyTrapGuard(1, 3) == 0x306 && grid_calls == 2);
    grid_cell[6][3] = 0x0105;
    assert(TaLogic019_IsEmptyTrapGuard(1, 3) == -1);
    grid_cell[0][5] = -1;
    grid_cell[6][3] = -1;
    spot_list[0] = 0;
    spot_list[1] = 0;
    pair_row_on = 0;
}

/* 0x14fa10 on stubs: list_value / list_count is the d == 1 list, list2 the d == 2 list. */
static void check_list_gate(void)
{
    static const int other[5] = { 0, 550, 552, 611, 613 };
    static const int counts[6] = { 0, 1, 2, 2, 3, 3 };
    static const int hits[6][3] = { { 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 }, { 1, 2, 0 }, { 1, 0, 1 }, { 0, 2, 0 } };
    static const int want[6] = { 0, 0, 1, 0, 1, 0 };
    int i;

    list_value = 0x4455;
    list2_on = 0;
    for (i = 0; i < 5; i++) {
        list_args[5] = -1;
        assert(TaLogic014_IsUnitNaturalEffectUse(7, other[i]) == 0 && list_args[5] == -1);
    }
    /* kind 612: three and four entries */
    list_count = 3;
    assert(TaLogic014_IsUnitNaturalEffectUse(7, 612) == 0);
    assert(list_args[0] == 0 && list_args[1] == 0 && list_args[2] == 7);
    assert(list_args[3] == 1 && list_args[4] == 16 && list_args[5] == 14);
    list_count = 4;
    assert(TaLogic014_IsUnitNaturalEffectUse(7, 612) == 1);
    /* kind 551: the first list has an entry */
    list2_on = 1;
    list2_value = 0x4466;
    list2_count = 3;
    list_count = 1;
    hit_calls = 0;
    assert(TaLogic014_IsUnitNaturalEffectUse(7, 551) == 0 && hit_calls == 0);
    assert(list_args[2] == 7 && list_args[3] == 1 && list_args[4] == 15 && list_args[5] == 14);
    /* empty first list: second lists of 0..3 entries */
    list_count = 0;
    list_words[0] = 0x0201;
    list_words[1] = 0x0302;
    list_words[2] = 0x0403;
    for (i = 0; i < 6; i++) {
        list2_count = counts[i];
        list_hits[0] = hits[i][0]; list_hits[1] = hits[i][1]; list_hits[2] = hits[i][2];
        hit_calls = 0;
        assert(TaLogic014_IsUnitNaturalEffectUse(7, 551) == want[i] && hit_calls == counts[i]);
        assert(list_args[0] == 0 && list_args[1] == 0 && list_args[2] == 7);
        assert(list_args[3] == 2 && list_args[4] == 15 && list_args[5] == 14);
    }
    list2_on = 0;
    hit_calls = 0;
}

/* 0x131f80 on stubs and the real TaLogic_GetWarnHideLvl__00134ae0 (750 here). */
static int far_run(int column, int row, int a0, int a1, int a2)
{
    m_far_calls = 0;
    m_score_calls = 0;
    m_a130_n = 0;
    m_a130_ans[0] = a0; m_a130_ans[1] = a1; m_a130_ans[2] = a2;
    return TaLogic008_GetNextInvadePoint(0x77, column, row);
}
/* TaLogic001_CommandEfcMonster (branch and loop coverage): no word; code 83 both ways; code 137
 * with the first 0x277160 answer 0, the fifth 0x0204, and none, on both sides; codes 191, 377 and
 * 390 each way; an unlisted code. Every callee is a stub. */
enum { W_3760 = 20 };
static int t_side[4], t_gate[3], t_key, t_a_n, t_b_n, t_a_first[5], t_b_ans[5], t_flip;
/* t_one (0x164020): the value expected in the seventh 0x2770e0 and ninth 0x277160 argument. */
static int t_one;
enum { W_3C50 = 21 };
void TaLogic020_CommandBtlUnit(struct DotrPositionOrder *order, int flag) { w_send(W_3C50, order, flag); }
int TaLogic001_IsUseLordOfDragon(int side) { t_side[0] = side; return t_gate[0]; }
int TaLogic001_IsUseLifeHourGlass(int side) { t_side[1] = side; return t_gate[1]; }
int TaLogic001_IsUseBraveHourGlass(int side) { t_side[2] = side; return t_gate[2]; }
int TaLogic_GetWarnHideLvl__00117c30(int side) { t_side[3] = side; return t_key; }
void func_00113760_spot(struct DotrPositionOrder *order, int flag, int deep)
{
    assert(deep == 0x5d);
    w_send(W_3760, order, flag);
}
/* With u_on set (0x118f50, 0x11dc00) the 0x2770e0 stub answers through
 * u_cells: two passes of u_bound - 1 calls each. */
static int u_on, u_bound, u_width, u_key, u_side, u_a_n, u_c_n, u_d_n, u_c_ans[6], u_d_ans[9];
/* 0x12ce50 also asks 0x1e1580 and 0x1e1640 about a 0x277cd0 answer; u_gate picks which one refuses. */
static int u_gate, u_gate_n;
int SzDuel_GetUnitCurseRest(int a, int b)
{
    assert(u_on && a == 2 && b == 4);
    u_gate_n++;
    return u_gate == 1;
}
int SzDuel_GetUnitSpellRest(int a, int b)
{
    assert(u_on && a == 2 && b == 4 && u_gate != 1);
    u_gate_n += 0x10;
    return u_gate == 2;
}
static int u_cells(int a, int b, int c, int d, int e, int f, int g, int h, struct DotrPositionCell *cells,
    unsigned short *j, unsigned short *k, unsigned short *l, unsigned char *buf)
{
    int pass = u_a_n / (u_bound - 1), at = u_a_n % (u_bound - 1);

    assert(pass < 2 && (pass == 0 ? u_c_n == 2 * at && u_d_n == 0 : u_d_n == 3 * at));
    assert(a == (at ? 0x900 + u_a_n - 1 : 0) && b == 0x77 && c == 0x07 && d == 0x05);
    assert(e == (pass == 0 ? !u_side : u_side) && f == at + 1 && g == 0 && h == u_key);
    assert(cells[0].id == 0xffff && cells[0].kind == 0xff && cells[21].id == 0xffff && cells[21].kind == 0xff);
    assert(*j == (at ? 0x4240 + u_a_n - 1 : 0) && *k == 0 && *l == 0 && k != l && buf != 0);
    *j = (unsigned short)(0x4240 + u_a_n);
    return 0x900 + u_a_n++;
}
int TaLogic_GetTrapNearCard(int a, int b, int c, int d, struct DotrPositionCell *cells, int f, int g, int h, int i, int j,
    int k, unsigned short l, int m)
{
    assert(u_on && u_c_n < 2 * (u_bound - 1) && u_a_n == u_c_n / 2 + 1);
    assert(a == 1 && b == 3 && c == 0x07 && d == 0x05 && cells[0].id == 0xffff && f == u_width && g == 31);
    assert(h == (u_c_n % 2 == 0 ? 1 : 0) && i == 2 && j == 0 && k == u_key && l == 0x4240 + u_a_n - 1 && m == 1);
    return u_c_ans[u_c_n++];
}
int TaLogic_GetFollowNearUnit(int a, int b, struct DotrPositionCell *cells, int d, int e, int f, int g, unsigned short h)
{
    static const int want[3][2] = { { 34, 2 }, { 31, 1 }, { 31, 0 } };

    assert(u_on && u_d_n < 3 * (u_bound - 1) && u_a_n == u_bound - 1 + u_d_n / 3 + 1);
    assert(a == 0x07 && b == 0x05 && cells[0].id == 0xffff && d == u_width);
    assert(e == want[u_d_n % 3][0] && f == want[u_d_n % 3][1] && g == 2 && h == 0x4240 + u_a_n - 1);
    return u_d_ans[u_d_n++];
}
int func_002770e0_cells(int a, int b, int c, int d, int e, int f, int g, int h, struct DotrPositionCell *cells,
    unsigned short *j, unsigned short *k, unsigned short *l, unsigned char *buf)
{
    if (u_on) {
        return u_cells(a, b, c, d, e, f, g, h, cells, j, k, l, buf);
    }
    assert(t_a_n < 5 && t_b_n == t_a_n);
    assert(b == 0x77 && c == 0x07 && d == 0x05 && e == t_flip && f == t_a_n + 1 && g == t_one && h == t_key);
    assert(cells[0].id == 0xffff && cells[0].kind == 0xff && cells[21].id == 0xffff && cells[21].kind == 0xff);
    assert(*j == (t_a_n ? 0x4240 + t_a_n - 1 : 0) && *k == 0 && *l == 0 && k != l && buf != 0);
    t_a_first[t_a_n] = a;
    *j = (unsigned short)(0x4240 + t_a_n);
    return 0x900 + t_a_n++;
}
int TaLogic_GetNearUnitL3(int a, int b, int c, struct DotrPositionCell *cells, int e, int f, int g, int h, int i, int j,
    unsigned short k)
{
    assert(t_b_n < 5 && t_a_n == t_b_n + 1);
    assert(a == 0x77 && b == 0x07 && c == 0x05 && cells[0].id == 0xffff && e == 5 && f == 52 && g == 0 && h == 2);
    assert(i == t_one && j == t_key && k == 0x4240 + t_b_n);
    return t_b_ans[t_b_n++];
}

static void t_reset(struct DotrPositionOrder *order, int code)
{
    m_reset(order);
    code_value = code;
    t_side[0] = t_side[1] = t_side[2] = t_side[3] = -1;
    t_gate[0] = t_gate[1] = t_gate[2] = 0;
    t_a_n = t_b_n = 0;
    t_key = 0x9c4;
}

/* 0x118f50 / 0x11dc00: every return point at every loop index, both sides. */
static void check_two_pass(void (*f)(struct DotrPositionOrder *), int bound, int key)
{
    struct DotrPositionOrder order;
    int first = 2 * (bound - 1), all = 5 * (bound - 1);
    int side, hit, k;

    u_on = 1;
    u_bound = bound;
    u_width = bound;
    u_key = key;
    total_value = 0;

    t_reset(&order, 0);
    word_present[3] = 0;
    f(&order);
    m_untouched(&order, 0);
    assert(m_buf_calls == 0);

    for (side = 0; side < 2; side++) {
        for (hit = 0; hit <= all; hit++) {
            int value = hit == 0 ? 0 : 0x0204;

            t_reset(&order, 0);
            side_pair_value = side;
            u_side = side;
            u_a_n = u_c_n = u_d_n = 0;
            for (k = 0; k < 6; k++) {
                u_c_ans[k] = k == hit ? value : -1;
            }
            for (k = 0; k < 9; k++) {
                u_d_ans[k] = k + first == hit ? value : -1;
            }
            f(&order);
            assert(m_buf_calls == 1 && w_sent_n == 0);
            if (hit == all) {
                assert(u_c_n == first && u_d_n == all - first && u_a_n == 2 * (bound - 1));
                m_untouched(&order, 0x5ff);
            } else if (hit < first) {
                assert(u_c_n == hit + 1 && u_d_n == 0 && u_a_n == hit / 2 + 1);
                order_is(&order, 0xbff, value);
            } else {
                assert(u_c_n == first && u_d_n == hit - first + 1 && u_a_n == bound - 1 + (hit - first) / 3 + 1);
                order_is(&order, 0xbff, value);
            }
        }
    }
    u_on = 0;
}

/* TaLogic_CommandTrapUnit__0012ce50 (branch and loop coverage): no word; every return point at
 * every loop index on both sides with both gates open; each 0x277cd0 answer refused by 0x1e1580 and
 * by 0x1e1640, so the scan goes on; no answer at all stores the side in the top byte. 0x130780 is
 * real (2400 here). */
static void check_two_pass_gated(void)
{
    struct DotrPositionOrder order;
    int side, hit, gate, k;

    u_on = 1;
    u_bound = 3;
    u_width = 5;
    u_key = 2400;
    total_value = 0;

    t_reset(&order, 0);
    word_present[3] = 0;
    TaLogic_CommandTrapUnit__0012ce50(&order);
    m_untouched(&order, 0);
    assert(m_buf_calls == 0);

    for (side = 0; side < 2; side++) {
        for (gate = 0; gate < 3; gate++) {
            for (hit = 0; hit <= 10; hit++) {
                int refused = gate != 0 && hit < 4;

                if (gate != 0 && hit >= 4) {
                    break;
                }
                t_reset(&order, 0);
                side_pair_value = side;
                u_side = side;
                u_gate = gate;
                u_a_n = u_c_n = u_d_n = u_gate_n = 0;
                for (k = 0; k < 6; k++) {
                    u_c_ans[k] = k == hit ? 0x0204 : -1;
                }
                for (k = 0; k < 9; k++) {
                    u_d_ans[k] = k + 4 == hit ? 0x0204 : -1;
                }
                TaLogic_CommandTrapUnit__0012ce50(&order);
                assert(m_buf_calls == 1 && w_sent_n == 0);
                assert(u_gate_n == (hit >= 4 ? 0 : gate == 0 ? 0x11 : gate == 1 ? 1 : 0x11));
                if (hit == 10 || refused) {
                    assert(u_c_n == 4 && u_d_n == 6 && u_a_n == 4);
                    order_is(&order, 0xbff, side << 8);
                } else if (hit < 4) {
                    assert(u_c_n == hit + 1 && u_d_n == 0 && u_a_n == hit / 2 + 1);
                    order_is(&order, 0xbff, 0x0204);
                } else {
                    assert(u_c_n == 4 && u_d_n == hit - 4 + 1 && u_a_n == 2 + (hit - 4) / 3 + 1);
                    order_is(&order, 0xbff, 0x0204);
                }
            }
        }
    }
    u_on = 0;
}

static void check_spot_tries(void)
{
    struct DotrPositionOrder order;
    int i, j;

    t_reset(&order, 83);
    word_present[3] = 0;
    TaLogic001_CommandEfcMonster(&order, 0x5d);
    m_untouched(&order, 0);
    assert(m_buf_calls == 0 && t_side[0] == -1);

    for (i = 0; i < 3; i++) {
        t_reset(&order, 83);
        side_pair_value = 1;
        t_gate[0] = i;
        TaLogic001_CommandEfcMonster(&order, 0x5d);
        assert(t_side[0] == 1 && m_buf_calls == 1 && w_sent_n == 0);
        if (i == 1) {
            order_is(&order, 0x3ff, 0x0507);
        } else {
            m_untouched(&order, 0x5ff);
        }
    }

    for (j = 0; j < 2; j++) {
        for (i = 0; i < 3; i++) {
            static const int ans[3][5] = { { 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, 0x0204 }, { -1, -1, -1, -1, -1 } };
            int k, n = i == 0 ? 1 : 5;

            t_reset(&order, 137);
            side_pair_value = j;
            t_flip = j == 0 ? 1 : 0;
            memcpy(t_b_ans, ans[i], sizeof t_b_ans);
            TaLogic001_CommandEfcMonster(&order, 0x5d);
            assert(t_a_n == n && t_b_n == n && t_side[3] == j && m_buf_calls == 1 && t_a_first[0] == 0);
            for (k = 1; k < n; k++) {
                assert(t_a_first[k] == 0x900 + k - 1);
            }
            if (i == 2) {
                w_sent_is(&order, W_3760, 1);
            } else {
                order_is(&order, 0x2ff, i == 0 ? 0 : 0x0204);
                assert(w_sent_n == 0);
            }
        }
    }

    for (i = 0; i < 3; i++) {
        t_reset(&order, 191);
        w_rule = i;
        TaLogic001_CommandEfcMonster(&order, 0x5d);
        assert(w_rule_calls == 1);
        if (i == 1) {
            m_untouched(&order, 0xbff);
        } else {
            w_sent_is(&order, W_3760, 1);
        }
    }
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 5; i++) {
            /* gate 1 with rule 1; gate 0 and 2 with rule 1; gate 1 with rule 0 and 2 */
            static const int gate[5] = { 1, 0, 2, 1, 1 };
            static const int rule[5] = { 1, 1, 1, 0, 2 };

            t_reset(&order, j ? 390 : 377);
            t_gate[1 + j] = gate[i];
            w_rule = rule[i];
            TaLogic001_CommandEfcMonster(&order, 0x5d);
            assert(t_side[1 + j] == 0 && t_side[2 - j] == -1 && w_rule_calls == (gate[i] == 1));
            if (i == 0) {
                m_untouched(&order, 0xbff);
            } else {
                w_sent_is(&order, W_3760, 1);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        t_reset(&order, i ? 84 : 0);
        w_rule = 1;
        t_gate[0] = t_gate[1] = t_gate[2] = 1;
        TaLogic001_CommandEfcMonster(&order, 0x5d);
        w_sent_is(&order, W_3760, 1);
        assert(w_rule_calls == 0 && t_a_n == 0 && t_side[0] == -1 && m_buf_calls == 1);
    }
    code_value = 0;
}

/* TaLogic020_CommandEfcMonster (branch and loop coverage): no word; code 297 with the first 0x277160
 * answer 0, the fifth 0x0204, and none, on both sides; code 79 at 1999 and 2000;
 * an unlisted code. 0x1676d0 is the real function (answers 0). */
static void check_spot_tries_b(void)
{
    struct DotrPositionOrder order;
    int i, j;

    t_one = 1;
    t_reset(&order, 297);
    t_key = 0;
    word_present[3] = 0;
    TaLogic020_CommandEfcMonster(&order);
    m_untouched(&order, 0);
    assert(m_buf_calls == 0 && t_a_n == 0 && w_sent_n == 0);

    for (j = 0; j < 2; j++) {
        for (i = 0; i < 3; i++) {
            static const int ans[3][5] = { { 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, 0x0204 }, { -1, -1, -1, -1, -1 } };
            int k, n = i == 0 ? 1 : 5;

            t_reset(&order, 297);
            t_key = 0;
            side_pair_value = j;
            t_flip = j == 0 ? 1 : 0;
            memcpy(t_b_ans, ans[i], sizeof t_b_ans);
            TaLogic020_CommandEfcMonster(&order);
            assert(t_a_n == n && t_b_n == n && m_buf_calls == 1 && t_a_first[0] == 0);
            for (k = 1; k < n; k++) {
                assert(t_a_first[k] == 0x900 + k - 1);
            }
            if (i == 2) {
                w_sent_is(&order, W_3C50, 1);
            } else {
                order_is(&order, 0x2ff, i == 0 ? 0 : 0x0204);
                assert(w_sent_n == 0);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        t_reset(&order, 79);
        side_pair_value = 1;
        total_side_value = 1999 + i, total_side_arg = -1;
        TaLogic020_CommandEfcMonster(&order);
        assert(total_side_arg == 1 && t_a_n == 0 && m_buf_calls == 1 && w_sent_n == 1);
        if (i == 0) {
            assert(order.kind == 0x2ff && order.flags.word >> 16 == 0x0507);
        } else {
            w_sent_is(&order, W_3C50, 1);
        }
    }
    t_reset(&order, 80);
    TaLogic020_CommandEfcMonster(&order);
    w_sent_is(&order, W_3C50, 1);
    assert(t_a_n == 0 && m_buf_calls == 1);
    code_value = 0, t_one = 0, total_side_value = 0;
}

static void check_far_spot(void)
{
    static const int rows[4] = { 0, 1, 4, 5 };
    static const int first[4] = { 0x0206, 0x0206, 0x0606, 0x0606 };
    static const int second[4] = { 0x0200, 0x0200, 0x0600, 0x0600 };
    struct DotrPositionOrder order;
    int i;

    m_reset(&order);
    total_value = 1001;
    count_value = 4;
    cell_zero_on = 1;
    cell_zero = 0x0405;
    m_far_side = 0;
    /* zero word */
    assert(TaLogic008_GetNextInvadePoint(0, 3, 0) == -1 && m_far_calls == 1 && m_a130_n == 0);
    /* first position: column 3 with rows 0, 1, 4, 5; columns below 3 with rows 2 and 3 */
    for (i = 0; i < 4; i++) {
        count_side = -1;
        assert(far_run(3, rows[i], 0, -1, -1) == first[i] && m_a130_n == 1 && m_far_calls == 1);
        assert(m_far_args[1] == 3 && m_far_args[2] == rows[i] && count_side == 0);
        assert(m_a130_args[0][0] == 6 && m_a130_args[0][1] == first[i] >> 8 && m_a130_args[0][2] == 750);
    }
    assert(far_run(2, 2, 0, -1, -1) == 0x0400 && m_a130_n == 1 && m_score_calls == 0);
    assert(far_run(0, 3, 0, -1, -1) == 0x0400 && m_a130_n == 1);
    assert(m_a130_args[0][0] == 0 && m_a130_args[0][1] == 4);
    /* refused once: the same position is offered again, then the cell */
    assert(far_run(3, 0, -1, 0, -1) == 0x0206 && m_a130_n == 2 && m_far_calls == 1);
    assert(m_a130_args[1][0] == 6 && m_a130_args[1][1] == 2);
    assert(far_run(2, 2, -1, 0, -1) == 0x0400 && m_a130_n == 2 && m_score_calls == 0);
    assert(far_run(3, 0, -1, -1, 0) == 0x0405 && m_a130_n == 3 && m_far_calls == 2);
    assert(m_a130_args[2][0] == 5 && m_a130_args[2][1] == 4 && cell_zero_row == 1);
    assert(far_run(3, 0, -1, -1, -1) == 0x0403 && m_a130_n == 3);
    /* second position: columns below 3 with rows 0, 1, 4, 5 and answers 1499 / 1500 */
    for (i = 0; i < 4; i++) {
        m_score = 1499;
        assert(far_run(2, rows[i], 0, -1, -1) == second[i] && m_a130_n == 1 && m_score_calls == 1);
        assert(m_score_args[0] == 0x77 && m_score_args[1] == 0 && m_score_args[2] == (rows[i] + 1) / 2 * 2);
        assert(m_a130_args[0][0] == 0 && m_a130_args[0][1] == second[i] >> 8);
        m_score = 1500;
        assert(far_run(2, rows[i], 0, -1, -1) == 0x0405 && m_a130_n == 1 && m_score_calls == 1);
    }
    /* column 3 with row 2: answers 1499 / 1500; row 3 goes straight to the cell */
    m_score = 1499;
    assert(far_run(3, 2, 0, -1, -1) == 0x0406 && m_a130_n == 1 && m_score_calls == 1);
    assert(m_score_args[1] == 6 && m_score_args[2] == 3);
    assert(far_run(3, 2, -1, 0, -1) == 0x0405 && m_a130_n == 2);
    m_score = 1500;
    assert(far_run(3, 2, 0, -1, -1) == 0x0405 && m_a130_n == 1 && m_score_calls == 1);
    assert(far_run(3, 3, 0, -1, -1) == 0x0405 && m_a130_n == 1 && m_score_calls == 0 && m_far_calls == 2);
    assert(far_run(3, 3, -1, -1, -1) == 0x0403 && m_a130_n == 1);
    /* the cell: other side, a negative cell */
    m_far_side = 1;
    assert(far_run(3, 3, 0, -1, -1) == 0x0405 && cell_zero_row == 0);
    m_far_side = 0;
    cell_zero = -1;
    assert(far_run(3, 3, 0, -1, -1) == (-256 | 3) && m_a130_n == 0 && cell_zero_row == 1);
    m_reset(&order);
}

static int side_scan, side_scan_calls, side_scan_arg;
int TaLogic019_GetUseThunderBolt(int side)
{
    side_scan_calls++;
    side_scan_arg = side;
    return side_scan;
}

/* One 0x15ef40 run on position (1, 3): `level` is the 0x1e0700 answer, `code` the 0x1e1030 one. */
static void cw_run(struct DotrPositionOrder *order, int level, int code)
{
    m_reset(order);
    flag_value = level;
    code_value = code;
    side_scan_calls = 0;
    grid_calls = 0;
    TaLogic019_CommandSpellUnit(order);
    assert(flag_calls == 1 && order->position == 0x0103);
}

/* 0x15ef40 on the real 0x15e2e0. Branch coverage: the null word, every level,
 * every word case with each test both ways, and the column scans' ends. */
static void check_cell_writer(void)
{
    static const int other[8] = { 3, 5, 19, 21, 30, 32, 36, 38 };
    static unsigned words[2];
    struct DotrPositionOrder order;
    int i, level;

    memset(grid_cell, 0, sizeof grid_cell);
    spot_list[0] = 0;
    spot_list[1] = 0;
    flag_calls = 0;
    /* no word */
    w_word = NULL;
    flag_calls = 0;
    cw_run(&order, 1 << 5, 749);
    m_untouched(&order, 0x5ff);
    w_word = words;
    /* levels 0, 2, 5: nothing else is asked */
    words[0] = 20 << 8;
    for (i = 0; i < 3; i++) {
        static const int plain[3] = { 0, 2, 5 };

        flag_calls = 0;
        cw_run(&order, plain[i] << 5, 749);
        m_untouched(&order, 0x5ff);
        assert(w_rule_calls == 0 && grid_calls == 0);
    }
    /* level 1 (read as a byte: 0x13f gives 1). Word 20 */
    flag_calls = 0;
    cw_run(&order, 0x13f, 0);
    m_untouched(&order, 0xcff);
    /* word 4: negative, then a cell whose column-0 request answers 0x0902 */
    words[0] = (4 << 8) | 0x3f;
    side_scan = -1;
    flag_calls = 0;
    cw_run(&order, 1 << 5, 0);
    m_untouched(&order, 0x5ff);
    assert(side_scan_calls == 1 && side_scan_arg == 0x0103);
    for (i = 0; i < 2; i++) {
        side_scan = i ? 0x0300 : 0;
        cell_zero = 0x0902;
        m_reset(&order);
        cell_zero_on = 1;
        flag_value = 1 << 5;
        flag_calls = 0;
        TaLogic019_CommandSpellUnit(&order);
        cell_zero_on = 0;
        order_is(&order, 0x6ff, 0x0502);
        assert(cell_zero_row == (i ? 3 : 0) && w_sent_n == 0);
    }
    /* word 37: code 749; else the 0x27d720 answers */
    words[0] = 37 << 8;
    flag_calls = 0;
    cw_run(&order, 1 << 5, 749);
    order_is(&order, 0x6ff, 0x0406);
    assert(w_rule_calls == 0);
    for (i = 0; i < 3; i++) {
        m_reset(&order);
        flag_value = 1 << 5;
        code_value = 748 + 2 * (i & 1);
        w_rule = i;
        flag_calls = 0;
        TaLogic019_CommandSpellUnit(&order);
        m_untouched(&order, i == 1 ? 0xbff : 0x5ff);
        assert(w_rule_calls == 1);
    }
    /* word 31: codes 731..733 */
    words[0] = 31 << 8;
    for (i = 731; i <= 733; i++) {
        flag_calls = 0;
        cw_run(&order, 1 << 5, i);
        m_untouched(&order, i == 732 ? 0xdff : 0x5ff);
    }
    for (i = 0; i < 8; i++) {
        words[0] = other[i] << 8;
        flag_calls = 0;
        cw_run(&order, 1 << 5, 732);
        m_untouched(&order, 0x5ff);
        assert(side_scan_calls == 0 && w_rule_calls == 0);
    }

    /* levels 3 and 4; the word is not read */
    words[0] = 20 << 8;
    for (level = 3; level <= 4; level++) {
        flag_calls = 0;
        cw_run(&order, level << 5, 749);
        order_is(&order, 0x6ff, 0x0600);
        assert(grid_calls == 0);
        /* column 3 empty over rows 1..5 (rows 0 and 6 are not asked): TaLogic019_CommandLineUnit(order, 1) */
        memset(grid_cell, 0, sizeof grid_cell);
        pair_row_on = 1;
        grid_cell[0][3] = 0x0104;
        grid_cell[6][3] = 0x0104;
        flag_calls = 0;
        cw_run(&order, level << 5, 0);
        w_sent_is(&order, W_E520, 1);
        assert(grid_calls == 5);
        /* the order's own cell ends the scan; another side's cell does not */
        grid_cell[1][3] = 0x0204;
        grid_cell[2][3] = 0x0103;
        grid_cell[3][3] = 0x0104;
        flag_calls = 0;
        cw_run(&order, level << 5, 0);
        w_sent_is(&order, W_E520, 1);
        assert(grid_calls == 2);
        /* a same-side cell in the last row blocks column 3: 0x15e2e0, here its first position */
        memset(grid_cell, 0, sizeof grid_cell);
        grid_cell[5][3] = 0x0104;
        m_reset(&order);
        flag_value = level << 5;
        code_value = 0;
        spot_list[0] = 1;
        grid_cell[0][5] = -1;
        flag_calls = 0;
        TaLogic019_CommandSpellUnit(&order);
        spot_list[0] = 0;
        order_is(&order, 0x6ff, 0x0500);
        assert(w_sent_n == 0 && w_rule_calls == 0);
        /* 0x15e2e0 negative: column 4 empty sends TaLogic019_CommandLineUnit(order, 2) */
        flag_calls = 0;
        cw_run(&order, level << 5, 0);
        w_sent_is(&order, W_E520, 2);
        assert(grid_calls == 10 && w_rule_calls == 0);
        /* column 4 blocked too: the 0x27d720 answers */
        grid_cell[5][4] = 0x0105;
        for (i = 0; i < 3; i++) {
            m_reset(&order);
            flag_value = level << 5;
            code_value = 0;
            w_rule = i;
            flag_calls = 0;
            TaLogic019_CommandSpellUnit(&order);
            m_untouched(&order, i == 1 ? 0xbff : 0x5ff);
            assert(w_rule_calls == 1);
        }
        pair_row_on = 0;
    }
    memset(grid_cell, 0, sizeof grid_cell);
}

/* 0x15ec60 on the real TaLogic019_IsLinePoint, TaLogic019_GetEmptyLine and
 * TaLogic019_CommandBtlUnit (picker positions 0x100, 0x403, 0x504, 0x605 as in check_free_cells).
 * */
static void check_cell_order(void)
{
    struct DotrPositionOrder order;
    int cell;

    /* a member cell (first picker position) and a non-member cell: the same store; a free
     * picker cell replaces it (picker calls: the membership scan, then two in 0x15ee40) */
    for (cell = 0x0100; cell <= 0x0706; cell += 0x0606) {
        w_reset(&order);
        cell_result = cell;
        memset(cell_full, 0, sizeof cell_full);
        side_value = 1;
        pair_calls = 0;
        pick_calls = 0;
        TaLogic019_CommandLDGuardUnit(4, 2, &order);
        assert(pair_calls == 1 && pair_row == 4 && pair_column == 2 && cell_row == 4 && cell_column == 2);
        order_is(&order, 0x3ff, 0x100);
        assert(pick_calls == (cell == 0x0100 ? 1 : 4) + 2 && pick_slot == 0 && w_sent_n == 0 && w_try_n == 0);
        cell_full[0][1] = 1;
        TaLogic019_CommandLDGuardUnit(4, 2, &order);
        order_is(&order, 0x3ff, 0x403);
    }
    /* no free picker cell: the cell stays and 0x15ded0 runs (here to its 0x15e030 end) */
    for (cell = 0x0100; cell <= 0x0706; cell += 0x0606) {
        w_reset(&order);
        cell_result = cell;
        cell_full[0][1] = cell_full[3][4] = cell_full[4][5] = cell_full[5][6] = 1;
        w_weight = 2499;
        TaLogic019_CommandLDGuardUnit(4, 2, &order);
        order_is(&order, 0x3ff, cell);
        assert(w_sent_n == 1 && w_sent[0] == W_E030 && w_try_n == 2);
    }
    memset(cell_full, 0, sizeof cell_full);
}

/* TaLogic004_GetNearAtkLineEdge (branch and loop coverage) on the real 0x11d8d0 and 0x11d940;
 * 0x11d7b0 answers wide_on[kind]. */
int TaLogic004_IsEmptyAtkLine(int side, unsigned char kind)
{
    assert(side == 0 && kind < 4);
    return wide_on[kind];
}

static void wide_best(struct DotrSpotBest *best, int c0, int c1)
{
    int i;

    for (i = 0; i < 8; i += 2) {
        wide_cost[i] = c0;
        wide_cost[i + 1] = c1;
    }
    wide_calls = 0;
    best->spot = 0x7777; best->cost = 0x11;
    TaLogic004_GetNearAtkLineEdge(0x4321, 6, 2, best);
}

static void check_wide_best(void)
{
    static const int order[8] = { 0x204, 0x402, 0x202, 0x404, 0, 0x600, 6, 0x606 };
    struct DotrSpotBest best;
    int i;

    wide_mode = 1;
    memset(wide_cell, 0, sizeof wide_cell);
    wide_cell[4][2] = 0x0123;
    wide_cell[2][4] = -1;
    /* no kind answers 1: only the opening stores */
    wide_on[0] = 0; wide_on[1] = 2; wide_on[2] = 0; wide_on[3] = -1;
    wide_best(&best, 5, 3);
    assert(wide_calls == 0 && best.spot == -1 && best.cost == 0xff);
    /* kind 0 only: both spots are costed, the lower second cost wins */
    wide_on[0] = 1;
    wide_best(&best, 5, 3);
    assert(wide_calls == 2 && best.spot == 0x402 && best.cost == 3);
    assert(wide_x[0] == 4 && wide_y[0] == 2 && wide_id[0] == 0x0123);
    assert(wide_x[1] == 2 && wide_y[1] == 4 && wide_id[1] == 0xffff);
    /* an equal or higher second cost does not replace the first */
    wide_best(&best, 5, 5);
    assert(wide_calls == 2 && best.spot == 0x204 && best.cost == 5);
    wide_best(&best, 5, 6);
    assert(best.spot == 0x204 && best.cost == 5);
    /* bounds: 0 and 0xfe are accepted, a negative cost and 0xff are not */
    wide_best(&best, -1, 0xff);
    assert(wide_calls == 2 && best.spot == -1 && best.cost == 0xff);
    wide_best(&best, 0, -1);
    assert(best.spot == 0x204 && best.cost == 0);
    wide_best(&best, 0xff, 0xfe);
    assert(best.spot == 0x402 && best.cost == 0xfe);
    /* all four kinds: eight costs in order, no early return */
    wide_on[1] = wide_on[2] = wide_on[3] = 1;
    for (i = 0; i < 8; i++) wide_cost[i] = 8 - i;
    wide_calls = 0;
    best.spot = 0x7777; best.cost = 0x11;
    TaLogic004_GetNearAtkLineEdge(0x4321, 6, 2, &best);
    assert(wide_calls == 8 && best.spot == 0x606 && best.cost == 1);
    for (i = 0; i < 8; i++)
        assert(wide_x[i] == (order[i] & 0xff) && wide_y[i] == order[i] >> 8);
    wide_mode = 0;
}

int main(void)
{
    check_id_rule();
    check_near();
    check_constants();
    check_order();
    check_limits();
    check_pickers();
    check_membership();
    check_tiers();
    check_column_match();
    check_first_answers();
    check_bands();
    check_position_tables();
    check_key_scan();
    check_free_cells();
    check_order_word();
    check_spot_best();
    check_cell_scan();
    check_order_body();
    check_order_kinds();
    check_try_choice();
    check_kind_writers();
    check_level_writers();
    check_word_eight();
    check_fallback_writers();
    check_spot_tries();
    check_spot_tries_b();
    check_two_pass(TaLogic_CommandTrapUnit__00118f50, 3, 0);
    check_two_pass(TaLogic_CommandTrapUnit__0011dc00, 4, 2000);
    check_two_pass_gated();
    check_nearest();
    check_spot_stages();
    check_list_gate();
    check_far_spot();
    check_cell_order();
    check_cell_writer();
    check_wide_best();
    return 0;
}
