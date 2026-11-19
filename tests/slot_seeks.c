/* Synthetic host tests for the slot_seeks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/slot_seeks.h"

static int busy_answer, busy_calls, slot_calls, rank_calls;
static unsigned slot_word[2][16];
static int slot_live[2][16], slot_kind[2][16], slot_rank[2][16];

int Ta_SearchUncTypeNaturalEffectUserNum(int kind)
{
    assert(kind == 15);
    busy_calls++;
    return busy_answer;
}

unsigned *func_00250740_word(int a, int b, int n)
{
    assert((a == 0 || a == 1) && b >= 6 && b <= 10 && n == 0);
    slot_calls++;
    return slot_live[a][b] ? &slot_word[a][b] : NULL;
}

int SzDuel_GetUnitKind(int a, int b)
{
    assert(slot_live[a][b]);
    return slot_kind[a][b];
}

int SzDuel_GetUnitSummonOrder(int a, int b)
{
    assert(slot_live[a][b] && slot_kind[a][b] >> 5 == 4);
    rank_calls++;
    return slot_rank[a][b];
}

static void set_slot(int side, int slot, int kind, unsigned id, int rank)
{
    slot_live[side][slot] = 1;
    slot_kind[side][slot] = kind;
    slot_word[side][slot] = (id << 8) | 0x5a;
    slot_rank[side][slot] = rank;
}

static void clear_slots(void)
{
    memset(slot_live, 0, sizeof(slot_live));
    busy_calls = slot_calls = rank_calls = 0;
}

/* 0x26ac90: branch and loop coverage. */
static void check_seek(void)
{
    clear_slots();
    set_slot(1, 9, 0x9f, 300, 50);
    /* a positive gate answers -1 before any slot is read */
    busy_answer = 1;
    assert(Ta_GetActiveTrapBUnit(1, 3, 300) == -1 && busy_calls == 1 && slot_calls == 0);
    /* zero and negative answers go on; mask 0 reads nothing */
    busy_answer = -1;
    assert(Ta_GetActiveTrapBUnit(1, 0, 300) == -1 && slot_calls == 0);
    busy_answer = 0;
    /* mask 1: own side, slots 6..10 */
    assert(Ta_GetActiveTrapBUnit(1, 1, 300) == 0x109 && slot_calls == 5 && rank_calls == 2);
    /* null entry, kind >> 5 of 3 and 5, another id */
    set_slot(1, 7, 0x7f, 300, 1);
    set_slot(1, 8, 0x80, 301, 1);
    set_slot(1, 10, 0xa0, 300, 1);
    assert(Ta_GetActiveTrapBUnit(1, 1, 300) == 0x109);
    /* an equal answer does not replace; a lower one does */
    set_slot(1, 10, 0x80, 300, 50);
    assert(Ta_GetActiveTrapBUnit(1, 1, 300) == 0x109);
    set_slot(1, 10, 0x80, 300, 49);
    assert(Ta_GetActiveTrapBUnit(1, 1, 300) == 0x10a);
    /* 255 is not below the start; 254 is */
    clear_slots();
    set_slot(1, 6, 0x80, 300, 255);
    assert(Ta_GetActiveTrapBUnit(1, 1, 300) == -1 && rank_calls == 1);
    set_slot(1, 6, 0x80, 300, 254);
    assert(Ta_GetActiveTrapBUnit(1, 1, 300) == 0x106);
    /* mask 2: the other side only (side 1 -> 0, side 0 -> 1, side 2 -> 0) */
    slot_calls = 0;
    assert(Ta_GetActiveTrapBUnit(0, 2, 300) == 0x106 && slot_calls == 5);
    assert(Ta_GetActiveTrapBUnit(1, 2, 300) == -1);
    set_slot(0, 8, 0x80, 300, 30);
    assert(Ta_GetActiveTrapBUnit(1, 2, 300) == 8);
    assert(Ta_GetActiveTrapBUnit(1, 4, 300) == 8); /* any bit above bit 0 */
    /* mask 3: both; the other side wins only with a lower answer */
    slot_calls = 0;
    assert(Ta_GetActiveTrapBUnit(1, 3, 300) == 8 && slot_calls == 10);
    set_slot(0, 8, 0x80, 300, 254);
    assert(Ta_GetActiveTrapBUnit(1, 3, 300) == 0x106);
    assert(Ta_GetActiveTrapBUnit(0, 3, 300) == 8); /* an equal answer on the other side does not replace */
}

/* 0x26ab80: one call per listed id and the default, through the real 0x26ac90. */
static void check_select(void)
{
    static const int both[6] = {43, 46, 17, 45, 76, 80};
    static const int other[10] = {49, 48, 4, 14, 75, 74, 35, 44, 42, 41};
    static const int none[6] = {0, 3, 40, 47, 81, -1};
    int n;

    busy_answer = 0;
    for (n = 0; n < 6; n++) {
        clear_slots();
        set_slot(1, 6, 0x80, (unsigned)both[n], 10);
        set_slot(0, 7, 0x80, (unsigned)both[n], 20);
        assert(Ta_IsActiveTrapB(1, both[n]) == 0x106 && slot_calls == 10);
    }
    for (n = 0; n < 10; n++) {
        clear_slots();
        set_slot(1, 6, 0x80, (unsigned)other[n], 10);
        set_slot(0, 7, 0x80, (unsigned)other[n], 20);
        assert(Ta_IsActiveTrapB(1, other[n]) == 7 && slot_calls == 5);
    }
    for (n = 0; n < 6; n++) {
        clear_slots();
        set_slot(1, 6, 0x80, (unsigned)none[n], 10);
        assert(Ta_IsActiveTrapB(1, none[n]) == -1 && busy_calls == 0 && slot_calls == 0);
    }
}

static int d_loc, d_cur, d_range, d_control, d_hide, d_resist, d_hide_side, d_status_calls;
static int d_status[2][16];
static int d_self_loc, d_visual_calls;
static struct DotrStateItem *d_unit = (struct DotrStateItem *)&d_loc;

int SzDuel_GetUnitLoc(int a, int b)
{
    if (b == 0 && d_self_loc && a == d_cur) {
        return d_self_loc;
    }
    assert(a == 1 && (b == 5 || b == 0));
    return d_loc;
}
int Ta_GetCurrentSide(int a, int b) { assert(a == 1 && (b == 5 || b == 0)); return d_cur; }
int Ta_IsLDRangeAvilityWithLocation(int a, int b, int c, int d, int e)
{
    assert(a == 1 && b == 5 && c == (d_loc & 0xff) && d == d_loc >> 8 && e == 11);
    return d_range;
}
int Ta_IsUnitControl(int a, int b) { assert(a == 1 && b == 5); return d_control; }
struct DotrStateItem *SzDuel_GetUnit(int a, int b) { assert(a == 1 && b == 5); return d_unit; }
int Ta_IsUnitHide(struct DotrStateItem *p, int side) { assert(p == d_unit); d_hide_side = side; return d_hide; }
int Ta_IsResistAll(int a, int b) { assert(a == 1 && b == 5); return d_resist; }
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(bit == 4);
    d_status_calls++;
    return d_status[a][b];
}

/* 0x26b1f0: branch coverage, with the real 0x26ab80 / 0x26ac90 behind it. */
static void check_decide(void)
{
    clear_slots();
    busy_answer = 0;
    memset(d_status, 0, sizeof(d_status));
    d_loc = 0x304; d_cur = 0; d_range = 1; d_control = 0; d_hide = 0; d_resist = 0;
    /* nID 0 */
    assert(Ta_IsTrapBInDecideOperation2(1, 0, 0, 1) == 0);
    /* in range and not hidden: 1; the side asked is 1 without control, 0 with */
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 1) == 1 && d_hide_side == 1);
    d_control = 2;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 1) == 1 && d_hide_side == 0);
    /* hidden goes on to the resist test; so does out of range */
    d_hide = 1; d_resist = 1;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 1) == 0);
    d_range = 0;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 1) == 0);
    d_resist = 0;
    /* stDefense 0: no trap 44 */
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 1) == 0);
    /* trap 44 across from current side 0; IsView 0 asks its status */
    set_slot(1, 6, 0x80, 44, 5);
    d_status_calls = 0;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 0) == 0 && d_status_calls == 1);
    d_status[1][6] = 1;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 0) == 0x10001 && d_status_calls == 2);
    d_status[1][6] = 0;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 1) == 0x10001 && d_status_calls == 2);
    /* 74 and 35 are not looked at for stDefense 0, 44 not otherwise */
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 1) == 0);
    /* an active trap 46 cancels */
    set_slot(0, 7, 0x80, 46, 5);
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 1) == 0);
    /* stDefense set: trap 35 alone */
    clear_slots();
    set_slot(1, 8, 0x80, 35, 5);
    d_status_calls = 0;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 0) == 0 && d_status_calls == 1);
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 1) == 1);
    d_status[1][8] = 1;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 0) == 1);
    set_slot(0, 7, 0x80, 46, 5);
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 1) == 0);
    /* trap 74 comes first; without view or status 35 is not tried */
    clear_slots();
    set_slot(1, 8, 0x80, 35, 5);
    set_slot(1, 9, 0x80, 74, 5);
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 0) == 0);
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 1) == 0x10001);
    d_status[1][9] = 1;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 0) == 0x10001);
    set_slot(0, 7, 0x80, 46, 5);
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 1, 1) == 0);
    /* the current side picks where 44 is looked for */
    clear_slots();
    set_slot(1, 6, 0x80, 44, 5);
    d_cur = 1;
    assert(Ta_IsTrapBInDecideOperation2(1, 5, 0, 1) == 0);
}

int Ta_IsLDAbl(int a, int b) { assert(a == 9 && b == d_cur); return 77; }
void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int kind)
{
    assert(x == (d_self_loc & 0xff) && y == d_self_loc >> 8 && c == 0 && reach == 77);
    assert(e == 1 && mode == 1 && kind == 11);
    d_visual_calls++;
}

static int rec_is(const struct jp_749a8 *e, int no, unsigned from, unsigned to, unsigned par)
{
    return e->eNO == no && e->dwFrom == from && e->dwTo == to && e->dwPar == par;
}

/* 0x26ae70: branch coverage, with the real 0x26ab80 / 0x26ac90 behind it. */
static void check_decide_record(void)
{
    static const int trap[3] = { 44, 74, 35 };
    static const int no[3] = { 70, 71, 71 };
    static const unsigned par[3] = { 0, 1, 0 };
    static const int ret[3] = { 0x10001, 0x10001, 1 };
    struct jp_749a8 e;
    int n, unit, unit2;

    clear_slots();
    busy_answer = 0;
    d_loc = 0x304; d_cur = 0; d_range = 0; d_resist = 0; d_self_loc = 0x607; d_visual_calls = 0;
    memset(&e, 0x55, sizeof(e));
    /* nID 0, resisted, and no trap for either stDefense: 0, record untouched */
    assert(Ta_IsTrapBInDecideOperation(1, 0, 0, &e) == 0);
    d_resist = 1;
    assert(Ta_IsTrapBInDecideOperation(1, 5, 0, &e) == 0);
    d_resist = 0;
    assert(Ta_IsTrapBInDecideOperation(1, 5, 0, &e) == 0);
    assert(Ta_IsTrapBInDecideOperation(1, 5, 1, &e) == 0);
    assert(rec_is(&e, 0x55555555, 0x55555555u, 0x55555555u, 0x55555555u) && d_visual_calls == 0);
    for (n = 0; n < 3; n++) {
        clear_slots();
        set_slot(1, 6, 0x80, (unsigned)trap[n], 5);
        unit = Ta_IsActiveTrapB(0, trap[n]);
        assert(unit >= 0);
        /* the trap of the other stDefense is not looked at */
        memset(&e, 0x55, sizeof(e));
        assert(Ta_IsTrapBInDecideOperation(1, 5, n == 0, &e) == 0 && e.eNO == 0x55555555);
        /* out of range, no trap 46 */
        d_range = 0;
        assert(Ta_IsTrapBInDecideOperation(1, 5, n != 0, &e) == ret[n]);
        assert(rec_is(&e, no[n], (unsigned)unit, 0x105, par[n]) && d_visual_calls == 0);
        /* in range: the range is shown, whatever trap 46 says */
        d_range = 2;
        assert(Ta_IsTrapBInDecideOperation(1, 5, n != 0, &e) == 1);
        assert(rec_is(&e, 254, 0, 0, 16) && d_visual_calls == 1);
        d_cur = 1;
        clear_slots();
        set_slot(0, 6, 0x80, (unsigned)trap[n], 5);
        set_slot(0, 7, 0x80, 46, 5);
        assert(Ta_IsTrapBInDecideOperation(1, 5, n != 0, &e) == 1);
        assert(rec_is(&e, 254, 0, 0x100, 16) && d_visual_calls == 2);
        d_visual_calls = 0;
        /* out of range with an active trap 46 across from nSide */
        d_range = 0;
        unit2 = Ta_IsActiveTrapB(0, 46);
        assert(unit2 >= 0);
        assert(Ta_IsTrapBInDecideOperation(1, 5, n != 0, &e) == 1);
        assert(rec_is(&e, 102, 1, (unsigned)unit2, 0) && d_visual_calls == 0);
        d_cur = 0;
    }
    /* trap 74 comes before 35 */
    clear_slots();
    set_slot(1, 8, 0x80, 35, 5);
    set_slot(1, 9, 0x80, 74, 5);
    unit = Ta_IsActiveTrapB(0, 74);
    assert(Ta_IsTrapBInDecideOperation(1, 5, 1, &e) == 0x10001 && rec_is(&e, 71, (unsigned)unit, 0x105, 1));
    d_self_loc = 0;
}

int main(void)
{
    check_seek();
    check_select();
    check_decide();
    check_decide_record();
    puts("slot_seeks: ok");
    return 0;
}
