/* Synthetic host tests for the battle_resolve group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/battle_resolve.h"

/* Units: attacker (0, 5) and defender (1, 7) unless a case says otherwise. */
#define S1 0
#define I1 5
#define S2 1
#define I2 7
#define CX 3
#define CZ 4

enum { C_CALC_AP = 1, C_CALC_DP, C_EXP, C_SET_STATUS, C_MANAGE, C_PARCAP };

struct call {
    int fn, a, b, c, d;
};

static struct call log_[64];
static int log_n;

static int kind_of[2];      /* SzDuel_GetUnitKind result per side (kind group << 5) */
static int attr_adv;        /* ChkAttrAdv result */
static int curse[2];        /* SzDuel_GetUnitCurseRest per side */
static int status3;         /* SzDuel_GetUnitStatus(.., 3) */
static int ap[2], dp[2];    /* SzDuel_GetUnitAP / DP per side */
static int param_ofs[2];    /* Ta_GetParamOfs by fAP */
static int ldabl;           /* Ta_IsLDAbl(6, nSide_1) */
static int range_ret, resist_ret, enc_ret;
static unsigned int manage_ret;
static struct DotrBattleResolveAtkEfc manage_set; /* what Manage_AtkEfc writes */
static int manage_seen_init;

static void add(int fn, int a, int b, int c, int d)
{
    assert(log_n < 64);
    log_[log_n].fn = fn;
    log_[log_n].a = a;
    log_[log_n].b = b;
    log_[log_n].c = c;
    log_[log_n].d = d;
    log_n++;
}

static int count(int fn)
{
    int i, n = 0;

    for (i = 0; i < log_n; i++) {
        n += log_[i].fn == fn;
    }
    return n;
}

static const struct call *nth(int fn, int k)
{
    int i;

    for (i = 0; i < log_n; i++) {
        if (log_[i].fn == fn && k-- == 0) {
            return &log_[i];
        }
    }
    assert(0);
    return NULL;
}

static void expect(int fn, int k, int a, int b, int c, int d)
{
    const struct call *e = nth(fn, k);

    assert(e->a == a && e->b == b && e->c == c && e->d == d);
}

static int side_index(int side)
{
    assert(side == 0 || side == 1);
    return side;
}

int SzDuel_GetUnitKind(int a, int b)
{
    (void)b;
    return kind_of[side_index(a)];
}
int Manage_AtkEfc(int a, int b, int c, int d, struct DotrBattleResolveAtkEfc *pDat)
{
    /* Battle clears the record (and sets Do_APDP) before the call. */
    manage_seen_init = pDat->head.Do_APDP == 1 && pDat->head.fDestroy_1 == 0 && pDat->head.fDestroy_2 == 0
        && pDat->POfs1 == 0 && pDat->POfs2 == 0 && pDat->AttrAdv == 0 && pDat->flags.fGuard1 == 0
        && pDat->flags.fGuard2 == 0 && pDat->flags.fRemove == 0 && pDat->flags.fNoEq1 == 0
        && pDat->flags.fNoEq2 == 0 && pDat->flags.fLand1 == 0 && pDat->flags.fLand2 == 0;
    add(C_MANAGE, a, b, c, d);
    pDat->POfs1 = manage_set.POfs1;
    pDat->POfs2 = manage_set.POfs2;
    pDat->flags = manage_set.flags;
    pDat->head.fDestroy_2 = manage_set.head.fDestroy_2;
    return (int)manage_ret;
}
int SzDuel_GetUnit(int a, int b)
{
    return 100 * (a + 1) + b;
}
int Ta_GetParamOfs(int unit, int nX, int nZ, int fAP)
{
    assert(unit == 100 * (S1 + 1) + I1 || unit == 100 * (S2 + 1) + I2);
    assert(nX == CX && nZ == CZ && (fAP == 0 || fAP == 1));
    return param_ofs[fAP];
}
int SzDuel_GetUnitAP(int a, int b, int flag)
{
    (void)b;
    assert(flag == 0);
    return ap[side_index(a)];
}
int SzDuel_GetUnitDP(int a, int b, int flag)
{
    (void)b;
    assert(flag == 0);
    return dp[side_index(a)];
}
int GetParCap(int value)
{
    add(C_PARCAP, value, 0, 0, 0);
    return value > 9999 ? 9999 : value;
}
void SzDuel_CalcUnitAP(int a, int b, int operation, int value)
{
    add(C_CALC_AP, a, b, operation, value);
}
int SzDuel_CalcUnitDP(int a, int b, int operation, int value)
{
    add(C_CALC_DP, a, b, operation, value);
    return 0;
}
int Ta_IsLDAbl(int a, int b)
{
    assert(a == 6 && b == S1);
    return ldabl;
}
int SzDuel_GetUnitAttr(int a, int b)
{
    return 10 * a + b;
}
int ChkAttrAdv(int kind, int other)
{
    assert(kind == 10 * S2 + I2 && other == 10 * S1 + I1);
    return attr_adv;
}
int SzDuel_GetUnitCurseRest(int a, int b)
{
    (void)b;
    return curse[side_index(a)];
}
int SzDuel_GetUnitCardNo(int a, int b)
{
    return 700 + 10 * a + b;
}
int SzDuel_GetUnitLevel(int a, int b, int flag)
{
    (void)b;
    assert(flag == 0);
    return 3 + side_index(a);
}
int SzData_CalcExp(int side, int id, int operation, int value)
{
    add(C_EXP, side, id, operation, value);
    return 0;
}
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == S1 && b == I1 && bit == 3);
    return status3;
}
int Ta_IsLDRangeAvility(int a, int b, int c)
{
    (void)a;
    (void)b;
    assert(c == 0xb);
    return range_ret;
}
int Ta_IsResistAll(int a, int b)
{
    (void)a;
    (void)b;
    return resist_ret;
}
int Chk_Enc(int nSide, int cID, int eNO)
{
    assert(eNO == 700 + 10 * (1 - nSide) + (nSide == S1 ? I2 : I1));
    (void)cID;
    return enc_ret;
}
int SzDuel_SetUnitStatus(int a, int b, int bit, int value)
{
    add(C_SET_STATUS, a, b, bit, value);
    return 0;
}
int Ta_GetCurrentSide(int a, int b)
{
    (void)a;
    (void)b;
    return 2;
}

static int out[4];

static void reset(void)
{
    log_n = 0;
    memset(out, 0x55, sizeof out);
    kind_of[0] = kind_of[1] = 0;
    attr_adv = 0;
    curse[0] = curse[1] = 1;
    status3 = 0;
    ap[0] = 1200;
    ap[1] = 1000;
    dp[0] = 900;
    dp[1] = 800;
    param_ofs[0] = 30;
    param_ofs[1] = 50;
    ldabl = 0;
    range_ret = resist_ret = 0;
    enc_ret = 0;
    manage_ret = 0;
    memset(&manage_set, 0, sizeof manage_set);
    manage_seen_init = 0;
}

static void check_direct(void)
{
    /* ID_1 == 0: Manage_AtkEfc, then the defender's AP with POfs2 and the terrain offset;
     * event 0x5d. Ta_IsLDAbl(6, side) adds POfs2 above bit 16 and halves the value. */
    reset();
    manage_ret = 0x01010001;
    manage_set.POfs2 = 200;
    manage_set.flags.fLand2 = 1;
    ldabl = 1;
    assert(Battle(S1, 0, S2, I2, CX, CZ, out) == 1);
    assert(manage_seen_init && count(C_MANAGE) == 1);
    expect(C_MANAGE, 0, S1, 0, S2, I2);
    expect(C_PARCAP, 0, 200 + 1000 + 50, 0, 0, 0);
    expect(C_CALC_AP, 0, S2, I2, 0, 200);
    expect(C_CALC_DP, 0, S2, I2, 0, 200);
    assert(out[0] == 0x5d && out[1] == ((200 << 16) | (S2 << 8) | I2) && out[2] == S1);
    assert(out[3] == ((3 << 22) | (1250 >> 1) | (1 << 20)));
    assert(count(C_EXP) == 0);

    /* Without the ability: no POfs2 in word 1, value not halved; a negative POfs2 is
     * passed to the Calc calls as it is. */
    reset();
    manage_ret = 0;
    manage_set.POfs2 = -300;
    assert(Battle(S1, 0, S2, I2, CX, CZ, out) == 1);
    expect(C_CALC_AP, 0, S2, I2, 0, -300);
    assert(out[0] == 0x5d && out[1] == ((S2 << 8) | I2) && out[2] == S1);
    assert(out[3] == 750);
}

static void check_effect_results(void)
{
    /* Effect result with low byte 1 and both bits 24 and 16: both units get experience. */
    reset();
    manage_ret = 0x01010001;
    attr_adv = 1;
    curse[0] = 0; /* attacker advantaged and not cursed: bit 25 */
    manage_set.flags.fRemove = 1;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[0] == 0x5e && out[1] == ((S2 << 8) | I2) && out[2] == ((S1 << 8) | I1));
    assert(out[3] == ((1 << 27) | (1 << 25) | (1 << 22) | (1 << 20) | 0xcffff | 0x10000000));
    assert(count(C_EXP) == 2);
    expect(C_EXP, 0, S1, 700 + I1, 0, (3 + 1 + 2) * 2);
    expect(C_EXP, 1, S2, 710 + I2, 0, (3 + 0 + 2) * 2);
    assert(count(C_CALC_AP) == 0);

    /* Bit 24 only: attacker's experience only. */
    reset();
    manage_ret = 0x01000001;
    attr_adv = -1;
    curse[1] = 0; /* defender advantaged and not cursed: bit 26 */
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[3] == ((1 << 26) | (1 << 22) | 0x8ffff | 0x10000000));
    assert(count(C_EXP) == 1);
    expect(C_EXP, 0, S1, 700 + I1, 0, 12);

    /* Bit 16 only: defender's experience only, no bit 28. */
    reset();
    manage_ret = 0x00010001;
    manage_set.flags.fLand2 = 1;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[3] == ((1 << 23) | (1 << 20) | 0x4ffff));
    assert(count(C_EXP) == 1);
    expect(C_EXP, 0, S2, 710 + I2, 0, 10);
}

static void check_status3(void)
{
    /* Status bit 3 set: the attacker's DP against the defender's AP. */
    reset();
    status3 = 1;
    manage_set.POfs1 = 100;
    manage_set.POfs2 = 10;
    dp[0] = 1500; /* 100 + 1500 + 30 = 1630 against 10 + 1000 + 50 = 1060 */
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    expect(C_PARCAP, 0, 1630, 0, 0, 0);
    expect(C_PARCAP, 1, 1060, 0, 0, 0);
    expect(C_CALC_AP, 0, S1, I1, 0, 100);
    expect(C_CALC_AP, 1, S2, I2, 0, 10);
    assert(out[0] == 0x5e && out[1] == ((10 << 16) | (S2 << 8) | I2) && out[2] == ((100 << 16) | (S1 << 8) | I1));
    assert(out[3] == (570 | 0x20000 | 0x01000000 | 0x10000000));
    assert(count(C_EXP) == 0);

    /* fGuard2 keeps the damage at 0; fNoEq1/fNoEq2 reset both units' bonuses (operation 4)
     * and leave the offsets out of the sums (and unencoded in words 1 and 2). */
    reset();
    status3 = 1;
    manage_set.POfs1 = 100;
    manage_set.POfs2 = 10;
    manage_set.flags.fGuard2 = 1;
    manage_set.flags.fNoEq1 = 1;
    manage_set.flags.fNoEq2 = 1;
    manage_set.flags.fLand1 = 1;
    manage_set.flags.fLand2 = 1;
    dp[0] = 1500;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    expect(C_CALC_AP, 0, S1, I1, 4, 0);
    expect(C_CALC_DP, 0, S1, I1, 4, 0);
    expect(C_CALC_AP, 1, S2, I2, 4, 0);
    expect(C_PARCAP, 0, 1530, 0, 0, 0);
    expect(C_PARCAP, 1, 1050, 0, 0, 0);
    assert(out[1] == ((10 << 16) | (S2 << 8) | I2) && out[2] == ((100 << 16) | (S1 << 8) | I1));
    assert(out[3] == ((1 << 23) | (1 << 21) | 0x20000 | 0x01000000 | 0x10000000));

    /* Equal: bit 24 without bit 28. */
    reset();
    status3 = 1;
    dp[0] = 1020; /* 1050 == 1050 */
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[3] == 0x01000000 && count(C_EXP) == 0);

    /* Lower: 0x40000 and the defender's experience. */
    reset();
    status3 = 1;
    dp[0] = 100;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[3] == 0x40000 && count(C_EXP) == 1);
    expect(C_EXP, 0, S2, 710 + I2, 0, 10);
}

static void check_attack(void)
{
    /* Status bit 3 clear: AP against AP (offset fAP 1 for both). */
    reset();
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    expect(C_PARCAP, 0, 1250, 0, 0, 0);
    expect(C_PARCAP, 1, 1050, 0, 0, 0);
    assert(out[0] == 0x5e && out[3] == (200 | 0xa0000 | 0x10000000));
    assert(count(C_EXP) == 1);
    expect(C_EXP, 0, S1, 700 + I1, 0, 4 * 4);

    /* fGuard2: no damage, experience (level + 2) * 2. */
    reset();
    manage_set.flags.fGuard2 = 1;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[3] == (0xa0000 | 0x10000000));
    expect(C_EXP, 0, S1, 700 + I1, 0, (4 + 2) * 2);

    /* Equal: both units get experience. */
    reset();
    ap[0] = 1000;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[3] == (0xc0000 | 0x10000000) && count(C_EXP) == 2);
    expect(C_EXP, 0, S1, 700 + I1, 0, 12);
    expect(C_EXP, 1, S2, 710 + I2, 0, 10);

    /* Lower: damage to the attacker unless fGuard1, defender's experience. */
    reset();
    ap[0] = 900;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[3] == (100 | 0x50000));
    expect(C_EXP, 0, S2, 710 + I2, 0, 3 * 4);
    reset();
    ap[0] = 900;
    manage_set.flags.fGuard1 = 1;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    assert(out[3] == 0x50000);
    expect(C_EXP, 0, S2, 710 + I2, 0, (3 + 2) * 2);
}

static void check_kinds(void)
{
    /* Defender of kind group 1 or 3..5: event 0x5f with 1. */
    static const int groups[] = {1, 3, 4, 5};
    int i;

    for (i = 0; i < 4; i++) {
        reset();
        kind_of[1] = groups[i] << 5;
        assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
        assert(out[0] == 0x5f && out[1] == ((S1 << 8) | I1) && out[2] == ((S2 << 8) | I2) && out[3] == 1);
        assert(count(C_MANAGE) == 0);
    }

    /* Defender of group 2 (an enchantment target): range ability, resistance, Chk_Enc. */
    reset();
    kind_of[1] = 2 << 5;
    range_ret = 1;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 0x10001 && out[0] == 0x5f && out[3] == 3);
    reset();
    kind_of[1] = 2 << 5;
    resist_ret = 1;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 0x10001 && out[0] == 0x5f && out[3] == 1);
    reset();
    kind_of[1] = 2 << 5;
    enc_ret = 0x3e7;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 0x10001 && out[0] == 0x5f && out[3] == 1);
    assert(count(C_SET_STATUS) == 0);
    reset();
    kind_of[1] = 2 << 5;
    enc_ret = (1 << 30) | (0x123 << 14) | 5;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 0x10001);
    assert(out[0] == 0x2b && out[1] == ((S2 << 8) | I2) && out[2] == ((S1 << 8) | I1) && out[3] == 0x123);
    expect(C_SET_STATUS, 0, S2, I2, 2, 1);
    reset();
    kind_of[1] = 2 << 5;
    enc_ret = (0x219 << 14) | 0x45;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 0x10001);
    assert(out[0] == 0x25 && out[1] == ((S1 << 8) | I1) && out[3] == (0x45 | (2 << 16)));
    reset();
    kind_of[1] = 2 << 5;
    enc_ret = (0x11 << 14) | 0x45;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 0x10001);
    assert(out[0] == 0x24 && out[2] == ((S2 << 8) | I2) && out[3] == enc_ret);

    /* Attacker of group 2 on a group-0 unit: the unit gets 6 experience first; return 1. */
    reset();
    kind_of[0] = 2 << 5;
    enc_ret = (0x11 << 14) | 0x46;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1);
    expect(C_EXP, 0, S2, 710 + I2, 0, 6);
    expect(C_SET_STATUS, 0, S1, I1, 2, 1);
    assert(out[0] == 0x24 && out[1] == ((S1 << 8) | I1) && out[3] == enc_ret);
    reset();
    kind_of[0] = 2 << 5;
    range_ret = 1;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1 && out[0] == 0x5f && out[3] == 2);
    reset();
    kind_of[0] = 2 << 5;
    resist_ret = 1;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1 && out[3] == 0);
    reset();
    kind_of[0] = 2 << 5;
    enc_ret = (1 << 30) | (0x77 << 14);
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1 && out[0] == 0x2b && out[1] == ((S1 << 8) | I1) && out[3] == 0x77);

    /* Attacker group 1..5 against group 0..5: 0x5f with 0, experience only for group 0. */
    reset();
    kind_of[0] = 3 << 5;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1 && out[0] == 0x5f && out[3] == 0);
    expect(C_EXP, 0, S2, 710 + I2, 0, 6);
    reset();
    kind_of[0] = 1 << 5;
    kind_of[1] = 5 << 5;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 1 && out[3] == 0 && count(C_EXP) == 0);

    /* Nothing applies: 0, eInfo untouched. */
    reset();
    kind_of[0] = 6 << 5;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 0 && out[0] == 0x55555555);
    reset();
    kind_of[0] = 1 << 5;
    kind_of[1] = 6 << 5;
    assert(Battle(S1, I1, S2, I2, CX, CZ, out) == 0);
}

int main(void)
{
    check_direct();
    check_effect_results();
    check_status3();
    check_attack();
    check_kinds();
    return 0;
}
