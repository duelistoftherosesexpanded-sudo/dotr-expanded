/* Synthetic host tests for the open_unit_reply group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/open_unit_reply.h"

enum { X = 3, Y = 5 };

static int card_v, who_v, status_v, kind_v, search_v, react_v;
static unsigned effect_word;
static unsigned *effect_p;
static int search_args[6], search_calls, react_calls;
static int ctl2_side, ctl2_calls, ctl_calls, lst_calls, lst_mode[4];
static int max_v[2], max_calls;
static int loc_v[2], loc_calls;
static int ap_v[3], ap_calls, ap_args[3][4];
static unsigned short *words_seen;

int SzDuel_GetUnitCardNo(int a, int b)
{
    assert(a == X && b == Y);
    return card_v;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    int n = loc_calls++;

    if (n == 0) {
        assert(a == X && b == Y); /* result unused */
        return 0x7777;
    }
    assert(n < 3);
    assert(a == (max_v[n - 1] >> 8) && b == (max_v[n - 1] & 0xff));
    return loc_v[n - 1];
}

int Ta_IsUnitControl(int a, int b)
{
    assert(a == X && b == Y);
    return who_v;
}

unsigned *SzDuel_GetUnitEffect(int a, int b, int n)
{
    assert(a == X && b == Y && n == 0);
    return effect_p;
}

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == X && b == Y && bit == 4);
    return status_v;
}

int SzDuel_GetUnitKind(int a, int b)
{
    assert(a == X && b == Y);
    return kind_v;
}

int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    search_args[0] = a;
    search_args[1] = b;
    search_args[2] = c;
    search_args[3] = d;
    search_args[4] = e;
    search_args[5] = f;
    search_calls++;
    return search_v;
}

int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    return (int)value; /* the stubbed search returns the count itself */
}

int TaLogic_GetActionReaction(int a, int b, int c, int d, int e, int f)
{
    assert(a == X && b == Y && c == 0 && d == 0 && e == 6 && f == 0);
    react_calls++;
    return react_v;
}

void TaLogic_GetUnitCtlLst2(int side, unsigned short *words, int kind, int cond, int view)
{
    assert(words == words_seen && kind == 29 && cond == 0 && view == 0);
    ctl2_side = side;
    ctl2_calls++;
}

void TaLogic_GetUnitCtlLst(int side, unsigned short *words, int kind, int view)
{
    assert(side == who_v && words == words_seen && kind == 29 && view == 1);
    ctl_calls++;
}

void TaUtil_GetUnitLst(int kind, int mode, unsigned short *out)
{
    assert(kind == who_v && lst_calls < 4);
    if (lst_calls == 0) {
        words_seen = out;
    } else {
        assert(out == words_seen);
    }
    lst_mode[lst_calls++] = mode;
}

int TaLogic_GetMaxApUnit(unsigned short *words)
{
    assert(words == words_seen && max_calls < 2);
    return max_v[max_calls++];
}

int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    assert(ap_calls < 3);
    ap_args[ap_calls][0] = a;
    ap_args[ap_calls][1] = b;
    ap_args[ap_calls][2] = c;
    ap_args[ap_calls][3] = d;
    return ap_v[ap_calls++];
}

static int run(void)
{
    search_calls = react_calls = ctl2_calls = ctl_calls = lst_calls = 0;
    max_calls = loc_calls = ap_calls = 0;
    words_seen = 0;
    return TaLogic_IsOpenUnit__00153ae0(X, Y);
}

/* Branch coverage: the status exit, each kind class, both card cases on each
 * side of the count bound, the reaction and missing-effect exits, each effect
 * kind, and the attack comparison on both sides of 2000 and of the second
 * unit's value. */
int main(void)
{
    who_v = 1;
    effect_p = &effect_word;

    /* status bit 4 set: 0 before any list is built */
    status_v = 1;
    assert(run() == 0 && lst_calls == 0);
    status_v = 0;

    /* kind class 0: two card numbers ask for a count of 2 */
    kind_v = 0x1f;
    card_v = 36;
    search_v = 2;
    assert(run() == 1 && lst_calls == 1 && lst_mode[0] == 2 && search_calls == 1);
    assert(search_args[0] == 0 && search_args[1] == 0 && search_args[2] == 1);
    assert(search_args[3] == 1 && search_args[4] == 0x95F0037 && search_args[5] == 14);
    search_v = 1;
    assert(run() == 2 && search_calls == 1);
    card_v = 213;
    search_v = 2;
    assert(run() == 1 && search_calls == 1);
    assert(search_args[2] == 1 && search_args[3] == 1 && search_args[4] == 29 && search_args[5] == 22);
    search_v = 1;
    assert(run() == 2);
    card_v = 35;
    search_v = 2;
    assert(run() == 2 && search_calls == 0);

    /* kind class 2 and above: 2 */
    kind_v = 0x40;
    assert(run() == 2 && react_calls == 0 && search_calls == 0);

    /* kind class 1 */
    kind_v = 0x20;
    react_v = 5;
    effect_word = 13 << 8;
    assert(run() == 2 && react_calls == 1);
    react_v = 4;
    effect_p = 0;
    assert(run() == 2);
    effect_p = &effect_word;
    assert(run() == 1 && ctl2_calls == 0);
    effect_word = (12 << 8) | 0xff;
    assert(run() == 2 && ctl2_calls == 0);

    /* effect kind 33: the opposite side's strongest unit against this side's */
    effect_word = (33 << 8) | 7;
    max_v[0] = 0x0204;
    max_v[1] = 0x0601;
    loc_v[0] = 0x0a0b;
    loc_v[1] = 0x0c0d;
    ap_v[0] = 1999;
    assert(run() == 2 && ap_calls == 1 && ctl2_calls == 1 && ctl2_side == 0 && ctl_calls == 1);
    assert(lst_calls == 2 && lst_mode[1] == 1 && max_calls == 2 && loc_calls == 3);
    assert(ap_args[0][0] == 2 && ap_args[0][1] == 4 && ap_args[0][2] == 0x0b && ap_args[0][3] == 0x0a);
    ap_v[0] = 2000;
    ap_v[1] = 2000;
    ap_v[2] = 1999;
    assert(run() == 1 && ap_calls == 3);
    assert(ap_args[1][0] == 2 && ap_args[1][1] == 4 && ap_args[1][2] == 0x0b && ap_args[1][3] == 0x0a);
    assert(ap_args[2][0] == 6 && ap_args[2][1] == 1 && ap_args[2][2] == 0x0d && ap_args[2][3] == 0x0c);
    ap_v[2] = 2000;
    assert(run() == 2 && ap_calls == 3);
    who_v = 0;
    assert(run() == 2 && ctl2_side == 1);
    return 0;
}
