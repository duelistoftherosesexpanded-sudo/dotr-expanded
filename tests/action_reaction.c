/* Synthetic host tests for the action_reaction group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/action_reaction.h"

static int b_answer, b_defense, b_view, b_calls;
static int loc_answer, loc_calls;
static int a_answer, a_eno, a_calls;
static int nat_answer, control_answer, active_answer, active_side, status_answer, status_calls;

int Ta_IsTrapBInDecideOperation2(int nSide, int nID, int stDefense, int IsView)
{
    assert(nSide == 3 && nID == 7);
    b_defense = stDefense;
    b_view = IsView;
    b_calls++;
    return b_answer;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == 1 && b == 9);
    loc_calls++;
    return loc_answer;
}

int Chk_TrapA(int nSide, int nID, int nX, int nZ, struct jp_7b52 *eInfo, int PrNo)
{
    assert(nSide == 3 && nID == 7);
    assert(nX == 0x34 && nZ == 0x12);
    assert(PrNo == 3 << 16);
    assert(eInfo->eNO == -1 && eInfo->dwFrom == 0xffffffffu);
    assert(eInfo->dwTo == 0xffffffffu && eInfo->dwPar == 0xffffffffu);
    eInfo->eNO = a_eno;
    a_calls++;
    return a_answer;
}

int Ta_SearchUncTypeNaturalEffectUserNum(int kind)
{
    assert(kind == 14);
    return nat_answer;
}

int Ta_IsUnitControl(int a, int b)
{
    assert(a == 3 && b == 7);
    return control_answer;
}

int Ta_IsActiveTrapB(int side, int id)
{
    assert(id == 14);
    active_side = side;
    return active_answer;
}

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 0x12 && b == 0x34 && bit == 4);
    status_calls++;
    return status_answer;
}

static int run(int n, int last)
{
    return TaLogic_GetActionReaction(3, 7, 1, 9, (unsigned char)n, last);
}

static void check_trap_b(void)
{
    /* case 4: low half must be 1; high half 1 gives 2, otherwise 5 */
    b_answer = 0x10001;
    assert(run(4, 0x102) == 2);
    assert(b_defense == 1 && b_view == 0x102);
    b_answer = 0x20001;
    assert(run(4, 0) == 5);
    b_answer = 0x10002;
    assert(run(4, 0) == 0);
    /* case 5 */
    b_answer = 0x10001;
    assert(run(5, 0) == 5);
    assert(b_defense == 0);
    b_answer = 0;
    assert(run(5, 0) == 0);
    /* other kinds call nothing */
    b_calls = 0;
    assert(run(1, 0) == 0 && run(7, 0) == 0);
    assert(b_calls == 0);
}

static void check_trap_a(void)
{
    static const int eno[] = { 113, 44, 45, 46, 43, 47, -1 };
    static const int want[] = { 1, 2, 1, 1, 0, 0, 0 };
    size_t i;

    b_answer = 1;
    loc_calls = 0;
    assert(run(0, 0x102) == 5);
    assert(loc_calls == 0);
    b_answer = 0;
    loc_answer = -1;
    a_calls = 0;
    assert(run(0, 0x102) == 0);
    assert(loc_calls == 1 && a_calls == 0);
    loc_answer = 0x1234;
    a_eno = 113;
    a_answer = 0x100;
    assert(run(0, 0x102) == 0);
    assert(a_calls == 1);
    a_answer = 0x201;
    for (i = 0; i < sizeof eno / sizeof eno[0]; i++) {
        a_eno = eno[i];
        assert(run(0, 0x102) == want[i]);
    }
}

static void check_natural(void)
{
    nat_answer = 1;
    assert(run(6, 0) == 5);
    nat_answer = 0;
    control_answer = 0;
    active_answer = -1;
    assert(run(6, 1) == 0);
    assert(active_side == 1);
    control_answer = 2;
    assert(run(6, 1) == 0);
    assert(active_side == 0);
    active_answer = 0x1234;
    status_calls = 0;
    assert(run(6, 0x101) == 5);
    assert(status_calls == 0);
    status_answer = 0;
    assert(run(6, 0x100) == 0);
    assert(status_calls == 1);
    status_answer = 1;
    assert(run(6, 0x100) == 5);
}

int main(void)
{
    check_trap_b();
    check_trap_a();
    check_natural();
    puts("action_reaction: ok");
    return 0;
}
