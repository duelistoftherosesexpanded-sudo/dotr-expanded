/* Synthetic host tests for the kind_pairs group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/kind_pairs.h"

static void check_relation(void)
{
    /* For each kind: the `other` that gives -1 and the one that gives 1. */
    static const int lose[6] = {1, 5, 4, 2, 0, 3};
    static const int win[6] = {4, 0, 3, 5, 2, 1};
    int kind, other;

    for (kind = 0; kind < 6; kind++) {
        for (other = -1; other <= 6; other++) {
            int want = other == lose[kind] ? -1 : other == win[kind] ? 1 : 0;
            assert(ChkAttrAdv(kind, other) == want);
        }
    }
    /* Outside 0-5 (6 is the bound; negative values compare unsigned): 0 for every other. */
    for (other = -1; other <= 6; other++) {
        assert(ChkAttrAdv(6, other) == 0);
        assert(ChkAttrAdv((unsigned int)-1, other) == 0);
    }
}

/* Stubs for Ta_IsVisualAttrAdv's callees: assumed semantics, arguments checked. The first cell is
 * (1, 2), the second (3, 4), except where a test passes a zero b or d. */
static int side_result, cell_side, range_first, range_second, veto_result, bit_result, count_result;
static int kind_first, kind_second, veto_calls, bit_calls, count_calls;
static int item_fixture;

int SzDuel_GetTurnOwner(void)
{
    return side_result;
}

int Ta_GetCurrentSide(int a, int b)
{
    assert(a == 3 && (b == 4 || b == 0));
    return cell_side;
}

int SzDuel_GetUnitKind(int a, int b)
{
    if (a == 1) {
        assert(b == 2 || b == 0);
        return range_first;
    }
    assert(a == 3 && (b == 4 || b == 0));
    return range_second;
}

struct DotrStateItem *SzDuel_GetUnit(int a, int b)
{
    assert(a == 3 && b == 4);
    return (struct DotrStateItem *)&item_fixture;
}

int Ta_IsUnitHide2(struct DotrStateItem *item)
{
    assert(item == (struct DotrStateItem *)&item_fixture);
    veto_calls++;
    return veto_result;
}

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 3 && b == 4 && bit == 4);
    bit_calls++;
    return bit_result;
}

int SzDuel_GetUnitAttr(int a, int b)
{
    if (a == 1) {
        assert(b == 2);
        return kind_first;
    }
    assert(a == 3 && b == 4);
    return kind_second;
}

int Ta_SearchUncTypeNaturalEffectUserNum(int kind)
{
    assert(kind == 61);
    count_calls++;
    return count_result;
}

static void reset_cells(void)
{
    side_result = 0;
    cell_side = 1;
    range_first = 31;
    range_second = 31;
    veto_result = 0;
    bit_result = 1;
    count_result = 0;
    kind_first = 0;
    kind_second = 1;
    veto_calls = bit_calls = count_calls = 0;
}

static void check_cell_relation(void)
{
    /* Bit set: relation of kinds (0, 1) is -1, plus one. */
    reset_cells();
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == 0);
    assert(veto_calls == 1 && bit_calls == 1 && count_calls == 0);
    kind_second = 4;
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == 2);
    kind_second = 2;
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == 1);

    /* Same side: nothing further is asked. */
    reset_cells();
    cell_side = 0;
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == -1);
    assert(veto_calls == 0);

    /* Either range value at 32 (the boundary) rejects; 31 passed above. */
    reset_cells();
    range_first = 32;
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == -1);
    reset_cells();
    range_second = 32;
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == -1);
    assert(veto_calls == 0);

    /* A zero b or d rejects. */
    reset_cells();
    assert(Ta_IsVisualAttrAdv(1, 0, 3, 4) == -1);
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 0) == -1);
    assert(veto_calls == 0);

    /* The item veto. */
    reset_cells();
    veto_result = 1;
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == -1);
    assert(veto_calls == 1 && bit_calls == 0);

    /* Bit clear: the count of kind 61 decides; 0 rejects, 1 gives the relation. */
    reset_cells();
    bit_result = 0;
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == -1);
    assert(count_calls == 1);
    count_result = 1;
    kind_second = 4;
    assert(Ta_IsVisualAttrAdv(1, 2, 3, 4) == 2);
    assert(count_calls == 2);
}

int main(void)
{
    check_relation();
    check_cell_relation();
    return 0;
}
