/* Synthetic host tests for the cell_grades group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cell_grades.h"

static int cell_v, kind_v, bit_v, word_v, a_v, b_v;
static int want_bit = 5;
static int calls_1090, calls_d830, calls_2100, calls_0700, calls_0290, calls_0370;

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == 7 && b == 9);
    calls_1090++;
    return cell_v;
}
int TaLogic_GetActionReaction(int a, int b, int x, int y, int n, int last)
{
    assert(a == 7 && b == 9 && x == 0x05 && y == 0x03 && last == 0x66);
    assert(n == (calls_d830 == 0 ? 4 : 5));
    calls_d830++;
    return n == 4 ? kind_v : 99;
}
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 7 && b == 9 && bit == want_bit);
    calls_2100++;
    return bit_v;
}
static int near_mode;

int SzDuel_GetUnitKind(int a, int b)
{
    if (near_mode) {
        assert(a == 6 && b == 7);
    } else {
        assert(a == 7 && b == 9);
    }
    calls_0700++;
    return word_v;
}
int func_001e0290_try(int a, int b, int flag)
{
    assert(a == 7 && b == 9 && flag == 0 && calls_0370 == 0);
    calls_0290++;
    return a_v;
}
int SzDuel_GetUnitDP(int a, int b, int flag)
{
    assert(a == 7 && b == 9 && flag == 0 && calls_0290 == 1);
    calls_0370++;
    return b_v;
}

/* One run on cell 0x0305; `pairs` is how many 0x1e0290 / 0x1e0370 pairs must run. */
static int grade(int kind, int bit, int word, int a, int b, int flag, int pairs)
{
    int r;

    cell_v = 0x0305;
    kind_v = kind; bit_v = bit; word_v = word; a_v = a; b_v = b;
    calls_1090 = calls_d830 = calls_2100 = calls_0700 = calls_0290 = calls_0370 = 0;
    r = TaLogic_IsDefStyleUnit(7, 9, 0x0305, flag, 0x66);
    assert(calls_1090 == 1 && calls_d830 == 2 && calls_2100 == 1 && calls_0700 == 1);
    assert(calls_0290 == pairs && calls_0370 == pairs);
    return r;
}

/* Branch coverage: every early exit, both sides of each test, and the a == b boundaries. */
static void check_grade(void)
{
    int kind;

    /* column 0: nothing is asked */
    calls_1090 = 0;
    assert(TaLogic_IsDefStyleUnit(7, 0, 0x0305, 0, 0x66) == 2 && calls_1090 == 0);
    /* another cell: only 0x1e1090 runs */
    cell_v = 0x0306; calls_1090 = calls_d830 = 0;
    assert(TaLogic_IsDefStyleUnit(7, 9, 0x0305, 0, 0x66) == 2 && calls_1090 == 1 && calls_d830 == 0);

    /* bit 5 set: 0 for kinds 2 and 5, else 2, whichever path is taken */
    for (kind = 2; kind <= 5; kind++) {
        int want = kind == 2 || kind == 5 ? 0 : 2;

        assert(grade(kind, 1, 0x20, 0, 0, 0, 0) == want);  /* upper word bits set */
        assert(grade(kind, 1, 0x1f, 5, 4, 0, 1) == want);  /* b < a, flag 0 */
        assert(grade(kind, 1, 0x1f, 5, 4, 1, 1) == want);  /* b < a, flag not 0 */
        assert(grade(kind, 1, 0x1f, 4, 4, 0, 1) == want);  /* b == a */
    }
    /* bit 5 clear */
    for (kind = 2; kind <= 5; kind++) {
        int plain = kind == 2 || kind == 5;

        if (plain) {
            assert(grade(kind, 0, 0x20, 3, 4, 0, 1) == 2);  /* upper bits set: goes on */
        } else {
            assert(grade(kind, 0, 0x20, 3, 4, 0, 0) == 1);  /* upper bits set: 1 at once */
        }
        assert(grade(kind, 0, 0x1f, 3, 4, 0, 1) == (plain ? 2 : 1));  /* a < b */
        assert(grade(kind, 0, 0x1f, 4, 4, 0, 1) == 2);                /* a == b, flag 0 */
        assert(grade(kind, 0, 0x1f, 4, 4, 2, 1) == 2);                /* a == b, flag 2 */
        assert(grade(kind, 0, 0x1f, 4, 4, 1, 1) == (plain ? 2 : 1));  /* a == b, flag 1 */
        assert(grade(kind, 0, 0x1f, 5, 4, 1, 1) == (plain ? 2 : 1));  /* a > b, flag 1 */
    }
}

static int side_v, pick_v, fills, picks, sent, sent_word, sent_mode;
static struct DotrGradeOrder *sent_order;

int Ta_GetCurrentSide(int a, int b)
{
    assert(a == 7 && b == 9);
    return side_v;
}
void TaLogic_GetUnitCtlLst2(int who, unsigned short *words, int n, int d, int e)
{
    int i;

    assert(who == (side_v == 0 ? 1 : 0) && n == 52 && d == 0 && e == 0);
    for (i = 0; i < 22; i++) {
        assert(words[i] == 0xffff);
    }
    words[0] = 0x0b0c;
    fills++;
}
int TaLogic_GetMaxApUnit3(unsigned short *words, int key)
{
    assert(fills == 1 && words[0] == 0x0b0c && key == (side_v == 0 ? 1 : 0));
    picks++;
    return pick_v;
}
void TaLogic_SetTaskAtkOrMove(struct DotrGradeOrder *order, int word, int mode)
{
    sent++;
    sent_order = order;
    sent_word = word;
    sent_mode = mode;
}

static int act(struct DotrGradeOrder *order, int side, int word, int bit, int pick)
{
    side_v = side; word_v = word; bit_v = bit; pick_v = pick;
    calls_0700 = calls_2100 = fills = picks = sent = 0;
    want_bit = 4;
    return TaLogic_CommandGunnerUnit(order, 0x31);
}

/* Branch coverage: the switch both ways, the bit both ways, the pick below, at and above 0, both sides. */
static void check_act(void)
{
    struct DotrGradeOrder order = { 0x0709 };
    int side;

    for (side = 0; side < 2; side++) {
        /* upper word bits set: bit 4 is not asked */
        assert(act(&order, side, 0x20, 1, 0x0204) == 1 && calls_0700 == 1 && calls_2100 == 0);
        assert(fills == 1 && picks == 1 && sent == 1 && sent_order == &order && sent_word == 0x0204 && sent_mode == 0x31);
        /* upper bits clear and bit 4 set: nothing else runs */
        assert(act(&order, side, 0x1f, 1, 0x0204) == 0 && calls_2100 == 1 && fills == 0 && picks == 0 && sent == 0);
        /* upper bits clear and bit 4 clear */
        assert(act(&order, side, 0x1f, 0, 0) == 1 && calls_2100 == 1 && fills == 1 && sent == 1 && sent_word == 0);
        /* negative pick */
        assert(act(&order, side, 0x1f, 0, -1) == 0 && fills == 1 && picks == 1 && sent == 0);
    }
    assert(order.position == 0x0709);
    want_bit = 5;
}

/* 0x27e5b0 fixtures: the word 0x0102 names cell (1, 2); a found word 0x0607
 * names cell (6, 7). The clamp stub keeps values in 0..6 unless overridden. */
static int own_side, found_side, slot_v, found_id, clamp_calls, look_calls, id_calls;
static int look_x[16], look_y[16], look_hit_x, look_hit_y;
static int clamp_over[16];

int func_00250360_pair(int a, int b)
{
    if (a == 1 && b == 2) {
        return own_side;
    }
    assert(a == 6 && b == 7);
    return found_side;
}
int Ta_IsActiveTrapB(int a, int b)
{
    assert(a == own_side && b == 4);
    return slot_v;
}
int GetCap(int limit, int value)
{
    assert(limit == 0 && value >= -1 && value < 15);
    clamp_calls++;
    if (clamp_over[value + 1] >= 0) {
        return clamp_over[value + 1];
    }
    return value < 0 ? 0 : value > 6 ? 6 : value;
}
int SzDuel_GetMapUnitID(int x, int y)
{
    assert(look_calls < 16);
    look_x[look_calls] = x;
    look_y[look_calls] = y;
    look_calls++;
    return x == look_hit_x && y == look_hit_y ? 0x0607 : -1;
}
int SzDuel_GetUnitCardNo(int a, int b)
{
    assert(a == 6 && b == 7);
    id_calls++;
    return found_id;
}

static int near(int slot, int cx, int cy, int hit_x, int hit_y, int kind, int id, int side)
{
    int r;

    own_side = 5;
    slot_v = slot;
    look_hit_x = hit_x;
    look_hit_y = hit_y;
    word_v = kind;
    found_id = id;
    found_side = side;
    clamp_calls = look_calls = id_calls = calls_0700 = 0;
    near_mode = 1;
    r = TaLogic_IsSafeTrap(0x0102, cx, cy);
    near_mode = 0;
    return r;
}

static void check_near(void)
{
    static const int around[4][2] = {{2, 2}, {2, 4}, {1, 3}, {3, 3}};
    int n;

    memset(clamp_over, 0xff, sizeof(clamp_over));
    /* a non-negative 0x26ab80 answer (0 and above): 1 with nothing walked */
    assert(near(0, 2, 3, -1, -1, 0x60, 100, 5) == 1 && clamp_calls == 0 && look_calls == 0);
    assert(near(7, 2, 3, -1, -1, 0x60, 100, 5) == 1 && clamp_calls == 0);
    /* nothing found: the four cells at distance 1 are asked, in walk order */
    assert(near(-1, 2, 3, -1, -1, 0x60, 100, 5) == 0 && look_calls == 4 && calls_0700 == 0);
    assert(look_x[0] == 1 && look_y[0] == 3 && look_x[1] == 2 && look_y[1] == 2);
    assert(look_x[2] == 2 && look_y[2] == 4 && look_x[3] == 3 && look_y[3] == 3);
    /* each of the four cells answers 1 when its word passes every test */
    for (n = 0; n < 4; n++) {
        assert(near(-1, 2, 3, around[n][0], around[n][1], 0x60, 100, 5) == 1);
        assert(calls_0700 == 1 && id_calls == 1);
    }
    /* kind >> 5 of 2 and 4; id 813; another side */
    assert(near(-1, 2, 3, 2, 2, 0x5f, 100, 5) == 0 && calls_0700 == 1 && id_calls == 0 && look_calls == 4);
    assert(near(-1, 2, 3, 2, 2, 0x80, 100, 5) == 0 && id_calls == 0);
    assert(near(-1, 2, 3, 2, 2, 0x7f, 813, 5) == 0 && id_calls == 1);
    assert(near(-1, 2, 3, 2, 2, 0x7f, 812, 5) == 1);
    assert(near(-1, 2, 3, 2, 2, 0x7f, 814, 4) == 0 && id_calls == 1 && look_calls == 4);
    /* a diagonal or the centre cell is never asked */
    assert(near(-1, 2, 3, 1, 2, 0x60, 100, 5) == 0 && near(-1, 2, 3, 2, 3, 0x60, 100, 5) == 0);
    /* clamped ranges: a corner has two cells at distance 1 */
    assert(near(-1, 0, 0, -1, -1, 0x60, 100, 5) == 0 && look_calls == 2);
    assert(look_x[0] == 0 && look_y[0] == 1 && look_x[1] == 1 && look_y[1] == 0);
    assert(near(-1, 6, 6, -1, -1, 0x60, 100, 5) == 0 && look_calls == 2);
    /* an empty outer range: two bound reads only */
    clamp_over[9 + 1] = 5;
    clamp_over[11 + 1] = 4;
    assert(near(-1, 10, 3, -1, -1, 0x60, 100, 5) == 0 && clamp_calls == 2 && look_calls == 0);
    /* an empty inner range for each of the three columns */
    assert(near(-1, 2, 10, -1, -1, 0x60, 100, 5) == 0 && look_calls == 0 && clamp_calls == 2 + 3 * 3);
    memset(clamp_over, 0xff, sizeof(clamp_over));
}

int main(void)
{
    check_near();
    check_act();
    check_grade();
    return 0;
}
