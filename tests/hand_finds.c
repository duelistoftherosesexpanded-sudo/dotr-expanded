/* Synthetic host tests for the hand_finds group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/hand_finds.h"

/* Every callee is a stub; hand_count / hand_ids stand for the two graveyard lists. */
static int side_v, gate_v, gate_calls, own_by[4], own_calls, set_v, set_calls, set_mask;
static int hand_count[2], hand_ids[2][8], hand_reads;

int SzDuel_GetUnitMonsterCount(int side)
{
    assert(side == side_v);
    gate_calls++;
    return gate_v;
}
int TaLogic_GetGraveCardCount(int index, int flags, int id)
{
    assert(index == side_v && flags == 3 && id >= 0 && id < 4);
    own_calls++;
    return own_by[id];
}
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0 && b == 0 && c == side_v && d == 1 && e == set_mask && f == 14);
    set_calls++;
    return set_v;
}
int SzDuel_GetGraveCount(int index)
{
    assert(index == 0 || index == 1);
    return hand_count[index];
}
int SzDuel_InqGrave(int index, int position)
{
    assert((index == 0 || index == 1) && position >= 0 && position < hand_count[index]);
    hand_reads++;
    return hand_ids[index][position];
}

static int find(int (*f)(int), int side)
{
    side_v = side;
    gate_calls = own_calls = set_calls = hand_reads = 0;
    return f(side);
}

static void fill_hands(int value)
{
    int i;

    for (i = 0; i < 8; i++) {
        hand_ids[0][i] = hand_ids[1][i] = value;
    }
    hand_count[0] = 3, hand_count[1] = 4;
}

/* TaLogic001_IsUseRevive (branch and loop coverage). */
static void check_two_then_zero(int (*f)(int), int mask)
{
    int side, other;

    set_mask = mask;
    for (side = 0; side < 2; side++) {
        other = side == 0 ? 1 : 0;
        fill_hands(7);
        hand_ids[side][1] = 2;
        own_by[0] = 1, own_by[2] = 0, set_v = 0;
        /* gate boundary: 5 answers -1 at once, 4 goes on */
        gate_v = 5;
        assert(find(f, side) == -1 && gate_calls == 1 && own_calls == 0);
        gate_v = 4;
        assert(find(f, side) == (side << 16 | 1) && own_calls == 1 && set_calls == 1 && hand_reads == 2);
        /* neither 0x27dd20 answer positive: -1; the second alone is enough */
        own_by[0] = 0;
        assert(find(f, side) == -1 && own_calls == 2 && set_calls == 0);
        own_by[0] = -1, own_by[2] = 1;
        assert(find(f, side) == (side << 16 | 1) && own_calls == 2);
        /* a non-zero set: -1 without reading a slot */
        set_v = 1;
        assert(find(f, side) == -1 && set_calls == 1 && hand_reads == 0);
        set_v = 0;
        /* id 2: own graveyard before the other side's; first slot wins */
        hand_ids[other][2] = 2, hand_ids[other][0] = 0, hand_ids[side][2] = 2, hand_ids[side][0] = 0;
        assert(find(f, side) == (side << 16 | 1));
        hand_ids[side][1] = 7, hand_ids[side][2] = 7;
        assert(find(f, side) == (other << 16 | 2) && hand_reads == hand_count[side] + 3);
        /* no id 2: id 0, own graveyard first */
        hand_ids[other][2] = 7;
        assert(find(f, side) == (side << 16 | 0) && hand_reads == 8);
        hand_ids[side][0] = 7;
        assert(find(f, side) == (other << 16 | 0) && hand_reads == 7 + hand_count[side] + 1);
        /* neither id, and the count bounds the walks: -1 */
        hand_ids[other][0] = 7;
        assert(find(f, side) == -1 && hand_reads == 14);
        hand_ids[side][hand_count[side]] = 2, hand_ids[other][hand_count[other]] = 0;
        assert(find(f, side) == -1 && hand_reads == 14);
        hand_count[0] = hand_count[1] = 0;
        assert(find(f, side) == -1 && hand_reads == 0 && set_calls == 1);
    }
}

int main(void)
{
    check_two_then_zero(TaLogic001_IsUseRevive, 0x0179001c);
    return 0;
}
