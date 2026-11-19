/* Synthetic host tests for the item_fills group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/item_fills.h"

static struct DotrFillSource sources[2];
static int source_calls, source_id;
struct DotrFillPair EffIdx_M_fill[4][5];
struct DotrFillPair EffIdx_S_fill[4];
struct DotrFillSource *func_001dd0e0_fill(int id)
{
    source_calls++; source_id = id;
    return &sources[id & 1];
}

static void expect_defaults(const struct DotrFillItem *item)
{
    int i;

    assert(item->cardModel == 0 && item->unknown_30 == 0);
    assert(item->xOffsetTerrain == 0 && item->yOffsetTerrain == 0 && item->apBoost == 0 && item->dpBoost == 0);
    for (i = 0; i < 4; i++) assert(item->unknown_40[i] == 7);
    assert(item->team == 2 && item->slot == 11 && item->modelId == 84);
    for (i = 0; i < 6; i++) assert(item->unknown_47[i] == 0);
    assert(item->cardHighlight == 128 && item->ctlModeC == 1 && item->unknown_4f == 7);
    for (i = 0; i < 3; i++) assert(item->unknown_50[i] == 128 && item->unknown_53[i] == 0);
    assert(*(const unsigned char *)&item->unknown_56 == 0 && *(const unsigned char *)&item->status == 0);
    assert(item->modelStatusFlags.flags.unknown_bit_0 == 0 && item->modelStatusFlags.flags.unknown_bit_1 == 0);
    assert(item->modelStatusFlags.flags.unknown_bit_2 == 0 && item->modelStatusFlags.flags.unknown_bit_3 == 0);
    assert(item->modelStatusFlags.wide.unknown_bits_4_9 == -1);
    /* the two bytes after the halfword are not written */
    assert(item->unknown_5a[0] == 0x55 && item->unknown_5a[1] == 0x55);
}

static void expect_pairs(const struct DotrFillItem *item, int from, int value)
{
    int i;

    for (i = from; i < 5; i++) {
        assert(item->pairs[i].unknown_words[0] == value && item->pairs[i].unknown_words[1] == value);
    }
}

static void run(struct DotrFillItem *item, int id)
{
    memset(item, 0x55, sizeof(*item));
    source_calls = 0;
    SzDuel_InitCardInfo(item, id);
}

/* SzDuel_InitCardInfo (branch coverage, with the id bounds). */
static void check_fill(void)
{
    static const int outside[] = { -1, 671, 854, 999 };
    struct DotrFillItem item;
    int i;

    for (i = 0; i < 4; i++) {
        EffIdx_S_fill[i].unknown_words[0] = 900 + i;
        EffIdx_S_fill[i].unknown_words[1] = 950 + i;
    }
    for (i = 0; i < 20; i++) {
        EffIdx_M_fill[i / 5][i % 5].unknown_words[0] = 100 + i;
        EffIdx_M_fill[i / 5][i % 5].unknown_words[1] = 200 + i;
    }
    sources[0].kind = 0x21; sources[0].unknown_02_low = 9; sources[0].unknown_02_high = 5;
    sources[0].effectId = 2;
    sources[1].kind = 0x43; sources[1].unknown_02_low = 3; sources[1].unknown_02_high = 15;
    sources[1].effectId = 0xffff;

    /* ids outside 0..853 and 671 (treated as 999): defaults only, no lookup */
    for (i = 0; i < 4; i++) {
        run(&item, outside[i]);
        assert(source_calls == 0 && item.source == 0 && item.id == 999);
        assert(item.kind == 255 && item.attr == 255);
        expect_pairs(&item, 0, -1);
        expect_defaults(&item);
    }
    /* lowest id: the five-pair row named by the source halfword */
    run(&item, 0);
    assert(source_calls == 1 && source_id == 0 && item.source == &sources[0] && item.id == 0);
    assert(item.kind == 0x21 && item.attr == 9);
    for (i = 0; i < 5; i++) {
        assert(item.pairs[i].unknown_words[0] == 110 + i && item.pairs[i].unknown_words[1] == 210 + i);
    }
    expect_defaults(&item);
    /* 682: last id of the five-pair table */
    run(&item, 682);
    assert(item.id == 682 && item.pairs[4].unknown_words[1] == 214);
    /* 684 (even, source 0): single pair from the other table, the rest stay -1 */
    run(&item, 684);
    assert(source_id == 684 && item.id == 684);
    assert(item.pairs[0].unknown_words[0] == 902 && item.pairs[0].unknown_words[1] == 952);
    expect_pairs(&item, 1, -1);
    expect_defaults(&item);
    /* source halfword 0xffff: no pairs copied, for ids 1, 683 and 853 (the last id) */
    run(&item, 1);
    assert(item.source == &sources[1] && item.kind == 0x43 && item.attr == 3);
    expect_pairs(&item, 0, -1);
    run(&item, 683);
    assert(item.id == 683 && item.source == &sources[1]);
    expect_pairs(&item, 0, -1);
    run(&item, 853);
    assert(source_calls == 1 && item.id == 853);
    expect_pairs(&item, 0, -1);
    expect_defaults(&item);
}

int main(void)
{
    check_fill();
    return 0;
}
