/* Synthetic host tests for the item_needs group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/item_needs.h"

static struct DotrNeedItem item;
static int mask_v, mask_calls, have_v, have_calls;

int SzGame_GetMenuNum(int unknown)
{
    assert(unknown == 28);
    mask_calls++;
    return mask_v;
}

int SzData_GetRank(int side, int id)
{
    assert(side == item.team && id == item.cardNo);
    have_calls++;
    return have_v;
}

static void set_half(int which, unsigned int half)
{
    unsigned int *word = &item.leaderAbilities[which / 2];

    if (which & 1) {
        *word = (*word & 0xffff0000u) | half;
    } else {
        *word = (*word & 0xffffu) | (half << 16);
    }
}

/* SzDuel_GetUnitLeaderAbility3 (branch coverage: every early exit, every case with the 0xffff
 * halfword and both sides of its comparison, null and non-null out, the
 * out-of-range selectors). */
static void check_need(void)
{
    /* cases whose need is the low byte and value the high byte */
    static const char swapped[17] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0 };
    int which, out, need, value;

    assert(offsetof(struct DotrNeedItem, cardNo) == 0x3c && offsetof(struct DotrNeedItem, team) == 0x44);
    memset(&item, 0, sizeof(item));
    item.cardNo = 853;
    item.team = 3;
    mask_v = 8;
    set_half(0, 5);

    /* null item: out is cleared first, no callee */
    out = 77;
    mask_calls = have_calls = 0;
    assert(SzDuel_GetUnitLeaderAbility3(NULL, 0, &out, 0) == 0 && out == 0 && mask_calls == 0);
    assert(SzDuel_GetUnitLeaderAbility3(NULL, 0, NULL, 0) == 0);
    /* id bound (853 passes, 854 does not) and the excluded id 671 */
    assert(SzDuel_GetUnitLeaderAbility3(&item, 0, NULL, 0) == 1 && mask_calls == 1);
    item.cardNo = 854;
    assert(SzDuel_GetUnitLeaderAbility3(&item, 0, NULL, 0) == 0 && mask_calls == 1);
    item.cardNo = 671;
    assert(SzDuel_GetUnitLeaderAbility3(&item, 0, NULL, 0) == 0 && mask_calls == 1);
    item.cardNo = 670;
    /* mask bit clear: 0 unless the flag skips the test */
    mask_v = ~8;
    assert(SzDuel_GetUnitLeaderAbility3(&item, 0, NULL, 0) == 0 && mask_calls == 2);
    assert(SzDuel_GetUnitLeaderAbility3(&item, 0, NULL, 1) == 1 && mask_calls == 2);
    mask_v = 8;
    assert(have_calls == 0);

    /* cases 0 and 1: 0xffff and 0 answer 0, anything else 1; out stays cleared, 0x215d30 is not asked */
    for (which = 0; which < 2; which++) {
        memset(item.leaderAbilities, 0, sizeof(item.leaderAbilities));
        out = 77;
        assert(SzDuel_GetUnitLeaderAbility3(&item, which, &out, 0) == 0 && out == 0);
        set_half(which, 0xffff);
        assert(SzDuel_GetUnitLeaderAbility3(&item, which, &out, 0) == 0);
        set_half(which, 0xfffe);
        assert(SzDuel_GetUnitLeaderAbility3(&item, which, &out, 0) == 1 && out == 0);
        /* the other halfword of the same word is not read */
        set_half(which, 0);
        set_half(which ^ 1, 0x1234);
        assert(SzDuel_GetUnitLeaderAbility3(&item, which, &out, 0) == 0);
    }
    assert(have_calls == 0);

    for (which = 2; which < 17; which++) {
        /* every other halfword 0xffff: only the selected one decides */
        memset(item.leaderAbilities, 0xff, sizeof(item.leaderAbilities));
        have_calls = 0;
        out = 77;
        assert(SzDuel_GetUnitLeaderAbility3(&item, which, &out, 0) == 0 && out == 0 && have_calls == 0);
        set_half(which, 0x2a07);
        need = swapped[which] ? 0x07 : 0x2a;
        value = swapped[which] ? 0x2a : which == 2 ? 0x07 : 0;
        have_v = need;
        out = 77;
        assert(SzDuel_GetUnitLeaderAbility3(&item, which, &out, 0) == 1 && out == value && have_calls == 1);
        have_v = need - 1;
        out = 77;
        assert(SzDuel_GetUnitLeaderAbility3(&item, which, &out, 0) == 0 && out == value && have_calls == 2);
        /* null out on the compared path */
        have_v = need + 1;
        assert(SzDuel_GetUnitLeaderAbility3(&item, which, NULL, 1) == 1 && have_calls == 3);
    }
    /* selectors outside 0..16 */
    out = 77;
    have_calls = 0;
    assert(SzDuel_GetUnitLeaderAbility3(&item, 17, &out, 0) == 0 && out == 0);
    assert(SzDuel_GetUnitLeaderAbility3(&item, -1, &out, 0) == 0 && have_calls == 0);
}

int main(void)
{
    check_need();
    return 0;
}
