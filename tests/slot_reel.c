/* Synthetic host tests for the slot_reel group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/slot_reel.h"

static int grave[16], grave_count, grave_full, add_calls, del_calls, pack_calls;
static int leader[2], destiny, kind_ret, rank_ret, rank_calls;
static struct jp_8d5b infos[64];

int SzData_GetCurrentDeck(int side) { assert(side == 0 || side == 1); return 40 + side; }
int SzData_GetLeader(int side, int list) { assert(list == 40 + side); return leader[side]; }
int SzDuel_AddGrave(int index, int value)
{
    assert(index == 0 || index == 1);
    add_calls++;
    if (!grave_full) {
        grave[grave_count++] = value;
    }
    return 0;
}
int SzDuel_GetGraveCount(int index) { (void)index; return grave_count; }
int SzDuel_InqGrave(int index, int position)
{
    (void)index;
    assert(position >= 0 && position < grave_count);
    return grave[position];
}
struct jp_8d5b *SzDuel_GetCardConstInfo(int id) { assert(id >= 0 && id < 64); return &infos[id]; }
int SzDuel_DelGrave(int index, int position)
{
    (void)index;
    assert(position >= 0 && position < grave_count);
    grave[position] = -1;
    del_calls++;
    return 0;
}
void SzDuel_PackGrave(int index)
{
    int i, n = 0;

    (void)index;
    for (i = 0; i < grave_count; i++) {
        if (grave[i] >= 0) {
            grave[n++] = grave[i];
        }
    }
    grave_count = n;
    pack_calls++;
}
static int destiny_side;
int SzDuel_InqDestinyFlag(int index) { destiny_side = index; return destiny; }
int TaHand_GetCardKind(int id) { assert(id == leader[destiny_side]); return kind_ret | 0x40; }
int SzData_GetRank(int side, int id)
{
    assert(side == destiny_side && id == leader[side]);
    rank_calls++;
    return rank_ret;
}

static void slot_reset(void)
{
    int i;

    memset(infos, 0, sizeof(infos));
    for (i = 0; i < 64; i++) {
        infos[i].fGetSlot = (i & 1);
    }
    grave_count = grave_full = 0;
    add_calls = del_calls = pack_calls = rank_calls = 0;
    leader[0] = 11;
    leader[1] = 21;
    destiny = 0x7F;
    destiny_side = -1;
    kind_ret = 1;
    rank_ret = 11;
}

static int run(int side, struct jp_9a20 *order)
{
    memset(order, 0xAA, sizeof(*order));
    grave_count = 0;
    grave[grave_count++] = 5;
    grave[grave_count++] = 6;
    grave[grave_count++] = 7;
    return TaSlot_SetSlotInitialDat(side, order);
}

static void expect_lines(const struct jp_9a20 *order, int plain)
{
    int i;

    for (i = 0; i < 5; i++) {
        assert(order->wLine[i] == (i < plain ? 0xFFFE : 0xFFFF));
    }
}

/* Branch coverage: the null record, both empty-list exits, the per-card slot flag, the destiny
 * bit, each leader kind of the fixed set and its neighbours, and each rank bound on both sides. */
static void check_slot_initial(void)
{
    static const int fixed[10] = { 0, 2, 5, 6, 7, 9, 12, 16, 19, 20 };
    static const int open_kinds[8] = { 1, 3, 4, 8, 10, 15, 18, 21 };
    static const int rank_plain[12][2] = {
        { -1, 0 }, { 0, 0 }, { 1, 1 }, { 3, 1 }, { 4, 2 }, { 6, 2 },
        { 7, 3 }, { 9, 3 }, { 10, 4 }, { 11, 5 }, { 12, 5 }, { 99, 5 }
    };
    struct jp_9a20 order;
    int i, side;

    slot_reset();
    assert(TaSlot_SetSlotInitialDat(0, 0) == -1 && add_calls == 0);

    /* the leader could not be added and the list is empty */
    slot_reset();
    grave_full = 1;
    memset(&order, 0xAA, sizeof(order));
    assert(TaSlot_SetSlotInitialDat(0, &order) == 0);
    assert(add_calls == 1 && pack_calls == 0 && order.bCardMax == 0xAA);

    /* every card lacks the slot flag: all are deleted and nothing is filled */
    slot_reset();
    memset(infos, 0, sizeof(infos));
    assert(run(0, &order) == 0);
    assert(del_calls == 4 && pack_calls == 1 && order.bCardMax == 0xAA && order.bMode == 0xAA);
    assert(order.wReelBuf[0][0] == 0xAAAA);

    for (side = 0; side < 2; side++) {
        /* own leader appended; even card 6 has no slot flag and is dropped */
        slot_reset();
        assert(run(side, &order) == 1);
        assert(add_calls == 1 && del_calls == 1 && pack_calls == 1);
        /* three cards reach TaSlot_SetReelData (real here): padded to eight with 820 */
        assert(order.bCardMax == 8 && order.bMode == 2 && destiny_side == 1 - side);
        for (i = 0; i < 6; i++) {
            assert(order._pad[i] == 0xAA);
        }
        for (i = 0; i < 3; i++) {
            int j;

            assert(order.wReelBuf[i][0] == 5 && order.wReelBuf[i][1] == 7 && order.wReelBuf[i][2] == leader[side]);
            for (j = 3; j < 21; j++) {
                assert(order.wReelBuf[i][j] == (j < 8 ? 820 : 999));
            }
        }
        expect_lines(&order, 5);

        /* destiny bit 7 of the other side clears the mode */
        slot_reset();
        destiny = 0x80;
        assert(run(side, &order) == 1 && order.bMode == 0);

        /* fixed leader kinds: one plain line whatever the rank, bits above the low five ignored */
        for (i = 0; i < 10; i++) {
            slot_reset();
            kind_ret = fixed[i];
            rank_ret = 11;
            assert(run(side, &order) == 1);
            expect_lines(&order, 1);
        }
        /* other kinds: the rank opens lines at 1, 4, 7, 10 and 11 */
        for (i = 0; i < 8; i++) {
            slot_reset();
            kind_ret = open_kinds[i];
            rank_ret = 4;
            assert(run(side, &order) == 1 && rank_calls == 1);
            expect_lines(&order, 2);
        }
        for (i = 0; i < 12; i++) {
            slot_reset();
            rank_ret = rank_plain[i][0];
            assert(run(side, &order) == 1);
            expect_lines(&order, rank_plain[i][1]);
        }
    }
}

/* Branch coverage: counts on both sides of 8, 9 and 16, the padding loop, the 999 scan and
 * both copy shapes. */
static void check_reel_one(int count)
{
    unsigned short list[120];
    struct jp_9a20 order;
    int i, j, want, ret;

    for (i = 0; i < 120; i++) {
        list[i] = (unsigned short)(i < count ? 100 + i : 999);
    }
    memset(&order, 0xAA, sizeof(order));
    ret = TaSlot_SetReelData(count, list, &order);
    assert(order.bCardMax == 0xAA && order.bMode == 0xAA);
    for (i = 0; i < 5; i++) {
        assert(order.wLine[i] == 0xAAAA);
    }
    if (count < 16) {
        /* short lists are padded to eight with 820; the result counts entries up to the first 999 */
        assert(ret == (count < 8 ? 8 : count));
        for (i = 0; i < 120; i++) {
            want = i < count ? 100 + i : (i < 8 ? 820 : 999);
            assert(list[i] == want);
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 21; j++) {
                assert(order.wReelBuf[i][j] == list[j]);
            }
        }
    } else {
        /* sixteen or more: the list is left alone and each reel takes its first sixteen */
        assert(ret == 16);
        for (i = 0; i < 120; i++) {
            assert(list[i] == (i < count ? 100 + i : 999));
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 21; j++) {
                assert(order.wReelBuf[i][j] == (j < 16 ? 100 + j : 0xAAAA));
            }
        }
    }
}

static void check_reel(void)
{
    static const int counts[10] = { 0, 1, 7, 8, 9, 15, 16, 17, 21, 40 };
    int i;

    for (i = 0; i < 10; i++) {
        check_reel_one(counts[i]);
    }
}

int main(void)
{
    check_slot_initial();
    check_reel();
    puts("slot_reel: ok");
    return 0;
}
