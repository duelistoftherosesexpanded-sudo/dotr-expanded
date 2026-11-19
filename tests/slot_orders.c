/* Synthetic host tests for the slot_orders group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/slot_orders.h"

/* Every callee is a stub; the order's position is always (2, 3) and its cell 0x0705. */
static unsigned *slot_answer;
static int place_answer, place_calls, set_calls, count_v, count_calls, entry_calls, cell_calls;
static int gate_calls, gate_at;
int func_00250360_pair(int a, int b)
{
    assert(a == 2 && b == 3);
    return 1;
}
unsigned *func_00250740_word(int a, int b, int n)
{
    assert(a == 2 && b == 3 && n == 0);
    return slot_answer;
}
int SzDuel_GetUnitLoc(int a, int b)
{
    cell_calls++;
    if (a == 4) {
        /* entry n is (4, 8 + n); its cell is (6 + 4n, 9 + 2n) */
        assert(b == 8 || b == 9);
        return b == 8 ? 0x0906 : 0x0b0a;
    }
    assert(a == 2 && b == 3);
    return 0x0705;
}
int func_001df540_word(int a, int b)
{
    assert(a == 2 && b == 3);
    return 9;
}
int TaLogic_GetLandChange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 9 && b == 5 && c == 7 && d == 2 && e == 6 && f == 2);
    place_calls++;
    return place_answer;
}
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0 && b == 0 && c == 1 && d == 2 && e == 30 && f == 14);
    set_calls++;
    return 0x44;
}
int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    assert(value == 0x44);
    count_calls++;
    return count_v;
}
int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert(word == 0x44 && n == entry_calls && n < 2);
    entry_calls++;
    return 0x0408 + n;
}
/* The gate_at-th question (from 1) answers 1; questions alternate (own low, its high), (its
 * low, own high). */
int TaLogic013_IsUseHarpySweaper(int a, int b, int c)
{
    static const int want[4][2] = { { 5, 9 }, { 6, 7 }, { 5, 0xb }, { 0xa, 7 } };

    assert(gate_calls < 4 && a == 1 && b == want[gate_calls][0] && c == want[gate_calls][1]);
    return ++gate_calls == gate_at;
}

static void run_slot_order(struct DotrSlotOrder *order, int flag)
{
    order->position = 0x0203;
    order->unknown_04 = 0x7777;
    order->flags.word = 0x12345678;
    place_calls = set_calls = count_calls = entry_calls = cell_calls = gate_calls = 0;
    TaLogic013_CommandSpellUnit(order, flag);
}

/* TaLogic013_CommandSpellUnit (branch and loop coverage). */
static void check_slot_order(void)
{
    struct DotrSlotOrder order;
    unsigned word;

    /* no slot word: 0x5ff */
    slot_answer = NULL;
    run_slot_order(&order, 1);
    assert(cell_calls == 1 && order.unknown_04 == 0x5ff && order.flags.word == 0x12345678);
    /* 20: 0xcff; 21 and a set top bit (unsigned shift): 0x5ff */
    slot_answer = &word;
    word = (20 << 8) | 0xff;
    run_slot_order(&order, 1);
    assert(order.unknown_04 == 0xcff && order.flags.word == 0x12345678);
    word = 21 << 8;
    run_slot_order(&order, 1);
    assert(order.unknown_04 == 0x5ff && place_calls == 0 && set_calls == 0);
    word = 0x80000000u | (20 << 8);
    run_slot_order(&order, 1);
    assert(order.unknown_04 == 0x5ff);
    /* 2: only the low byte of the flag counts */
    word = 2 << 8;
    place_answer = 0x33;
    run_slot_order(&order, 0x100);
    assert(place_calls == 0 && order.unknown_04 == 0x5ff && order.flags.word == 0x12345678);
    place_answer = -1;
    run_slot_order(&order, 0x101);
    assert(place_calls == 1 && order.unknown_04 == 0x5ff && order.flags.word == 0x12345678);
    /* a placed answer lands in the high half; the 0x5ff store still follows */
    place_answer = 0;
    run_slot_order(&order, 1);
    assert(place_calls == 1 && order.unknown_04 == 0x5ff && order.flags.word == 0x5678);
    place_answer = 0x33;
    run_slot_order(&order, 1);
    assert(order.unknown_04 == 0x5ff && order.flags.word == 0x335678);
    /* 4 without the flag, and with an empty set */
    word = 4 << 8;
    count_v = 2;
    run_slot_order(&order, 0);
    assert(set_calls == 0 && order.unknown_04 == 0x5ff);
    count_v = 0;
    run_slot_order(&order, 1);
    assert(set_calls == 1 && count_calls == 1 && entry_calls == 0);
    assert(order.unknown_04 == 0x5ff && order.flags.word == 0x12345678);
    /* two entries, no question answering 1 */
    count_v = 2, gate_at = 0;
    run_slot_order(&order, 1);
    assert(count_calls == 3 && entry_calls == 2 && cell_calls == 3 && gate_calls == 4);
    assert(order.unknown_04 == 0x5ff && order.flags.word == 0x12345678);
    /* the first question answers 1: (its high, own low) */
    gate_at = 1;
    run_slot_order(&order, 1);
    assert(gate_calls == 1 && count_calls == 1);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x09055678);
    /* the second: (own high, its low) */
    gate_at = 2;
    run_slot_order(&order, 1);
    assert(gate_calls == 2 && count_calls == 1);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x07065678);
    /* the last, on the second entry */
    gate_at = 4;
    run_slot_order(&order, 1);
    assert(gate_calls == 4 && count_calls == 2 && entry_calls == 2);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x070a5678);
    /* an entry past the count is not read */
    count_v = 1, gate_at = 3;
    run_slot_order(&order, 1);
    assert(gate_calls == 2 && entry_calls == 1 && order.unknown_04 == 0x5ff);
}

static struct jp_4e65f effect_seen[4];
static int effect_limit = 2;
static struct jp_4e704 *effect_temp;
static int effect_calls;
int Ta_Do_Effect(struct jp_4e65f *eInfo, struct jp_4e704 *Temp)
{
    assert(Temp == effect_temp && effect_calls < effect_limit);
    effect_seen[effect_calls] = *eInfo;
    return 0x1f0 + effect_calls++;
}

/* Ta_SetEffectDependOnFlagLst (branch coverage): empty list, two entries with eNO 603 and with another
 * number. The `>= 0` test on the stored unsigned byte has no false outcome to exercise. */
static void check_effect_flag_list(void)
{
    struct jp_4e65f info = { 1, 2, 0x44, 3 };
    struct jp_4e65f call;
    struct jp_4e704 temp;
    int pass;

    assert(offsetof(struct jp_4e704, eID) == 6 && offsetof(struct jp_4e704, ePos) == 32);
    assert(offsetof(struct jp_4e704, oNum) == 212 && sizeof(struct jp_4e65f) == 16);
    effect_temp = &temp;
    memset(&temp, 0, sizeof temp);
    memset(&call, 0x55, sizeof call);
    count_v = 0;
    count_calls = entry_calls = effect_calls = 0;
    Ta_SetEffectDependOnFlagLst(&info, &temp, &call, 603);
    assert(count_calls == 1 && entry_calls == 0 && effect_calls == 0 && temp.oNum == 0 && call.eNO == 0x55555555);

    for (pass = 0; pass < 2; pass++) {
        int number = pass == 0 ? 603 : 602;
        memset(&temp, 0, sizeof temp);
        temp.oNum = 3;
        count_v = 2;
        count_calls = entry_calls = effect_calls = 0;
        Ta_SetEffectDependOnFlagLst(&info, &temp, &call, number);
        assert(count_calls == 1 && entry_calls == 2 && effect_calls == 2 && temp.oNum == 5);
        assert(effect_seen[0].eNO == number && effect_seen[0].dwFrom == 0 && effect_seen[0].dwTo == 0x408);
        assert(effect_seen[1].eNO == number && effect_seen[1].dwFrom == 0 && effect_seen[1].dwTo == 0x409);
        assert(effect_seen[0].dwPar == (unsigned)(pass == 0) && effect_seen[1].dwPar == (unsigned)(pass == 0));
        assert(temp.eID[3] == 0xf0 && temp.eID[4] == 0xf1 && temp.eID[5] == 0 && temp.eID[2] == 0);
        assert(temp.ePos[3] == 0x408 && temp.ePos[4] == 0x409 && temp.ePos[5] == 0);
        assert(info.eNO == 1 && info.dwFrom == 2 && info.dwTo == 0x44 && info.dwPar == 3);
    }
}

/* Ta_SetEffectDependOnFlagLst2 / 3 (branch coverage): empty mask, the first and last bit of each half
 * (positions 0, 10, 11, 21), eNO 603 and another number; a bit outside the 22 is ignored. */
static void check_effect_flag_bits(void (*fn)(struct jp_4e65f *, struct jp_4e704 *, struct jp_4e65f *, int), int third)
{
    static const unsigned want[4] = { 0, 10, 0x100, 0x10a };
    struct jp_4e65f info = { 1, 0, 0x44, 0 };
    struct jp_4e65f call;
    struct jp_4e704 temp;
    unsigned mask = 1u << 21 | 1u << 11 | 1u << 10 | 1u;
    int pass, k;

    effect_temp = &temp;
    effect_limit = 4;
    memset(&temp, 0, sizeof temp);
    memset(&call, 0x55, sizeof call);
    effect_calls = 0;
    /* the other word and bit 22 of the mask word select nothing */
    info.dwFrom = third ? 1u << 22 : 0x3fffff;
    info.dwPar = third ? 0x3fffff : 1u << 22;
    fn(&info, &temp, &call, 603);
    assert(effect_calls == 0 && temp.oNum == 0 && call.eNO == 0x55555555);

    for (pass = 0; pass < 2; pass++) {
        int number = pass == 0 ? 603 : 604;
        memset(&temp, 0, sizeof temp);
        temp.oNum = 2;
        effect_calls = 0;
        info.dwFrom = third ? mask : 0;
        info.dwPar = third ? 0 : mask;
        fn(&info, &temp, &call, number);
        assert(effect_calls == 4 && temp.oNum == 6);
        for (k = 0; k < 4; k++) {
            assert(effect_seen[k].eNO == number && effect_seen[k].dwFrom == 0 && effect_seen[k].dwTo == want[k]);
            assert(effect_seen[k].dwPar == (unsigned)(pass == 0));
            assert(temp.eID[2 + k] == 0xf0 + k && temp.ePos[2 + k] == (int)want[k]);
        }
        assert(temp.eID[6] == 0 && temp.ePos[6] == 0 && temp.eID[1] == 0);
    }
    effect_limit = 2;
}

int main(void)
{
    check_slot_order();
    check_effect_flag_list();
    check_effect_flag_bits(Ta_SetEffectDependOnFlagLst2, 0);
    check_effect_flag_bits(Ta_SetEffectDependOnFlagLst3, 1);
    return 0;
}
