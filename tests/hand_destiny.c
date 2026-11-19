/* Synthetic host tests for the hand_destiny group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/hand_destiny.h"

/* A unit id is (owner << 8) | position; the stubs answer from these tables.
 * A search is identified by its fourth and fifth arguments (kind * 100 + e). */
static int t_player, t_owner, t_rnd, search_calls;
static int u333, num207, num229, num329, num230;
static int l229[4], l329[4], l230[4];
static int kind_of[2][8], loc_of[2][8], land_of[2][8], ap_of[2][8];
static struct jp_88243 eff_of[2][8];
static int eff_set[2][8];
static int field[7][7], hand[5], apdp[8], lp[2];

int SzDuel_GetTurnOwner(void) { return t_owner; }
int SzDuel_GetPlayer(int index) { assert(index == t_owner); return t_player; }
int SearchUnitByRange(int a, int b, int side, int kind, int e, int f)
{
    assert(a == 0 && b == 0 && f == 14);
    (void)side;
    search_calls++;
    return kind * 100 + e;
}
int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert(n >= 0 && n < 4);
    switch (word) {
    case 333: assert(n == 0); return u333;
    case 229: return l229[n];
    case 329: return l329[n];
    case 230: return l230[n];
    }
    assert(0);
    return 0;
}
int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    switch (value) {
    case 207: return num207;
    case 229: return num229;
    case 329: return num329;
    case 230: return num230;
    }
    assert(0);
    return 0;
}
/* The owner byte of the id is the controlling side. */
int Ta_IsUnitControl(int a, int b) { (void)b; return a; }
int SzDuel_GetUnitKind(int a, int b) { return kind_of[a][b]; }
int SzDuel_GetUnitLoc(int a, int b) { return loc_of[a][b]; }
int CheckLandAgreement2(int a, int b, int c) { assert(a == 0); return land_of[b][c]; }
int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    assert(c == (loc_of[a][b] & 0xff) && d == (loc_of[a][b] >> 8));
    return ap_of[a][b];
}
int SzDuel_GetField(int x, int y) { return field[x][y]; }
unsigned *Ta_GetUnitEffect(int a, int b, int n)
{
    assert(n == 1);
    return eff_set[a][b] ? (unsigned *)&eff_of[a][b] : NULL;
}
int SzDuel_InqHand(int index, int position) { (void)index; return hand[position]; }
int TaHand_GetCardAPDP(int id) { return apdp[id]; }
int SzDuel_GetLP(int side) { return lp[side]; }
int SzDuel_Kind2Type(int value) { return value; }
int func_0019a5c8(void) { return t_rnd; }

/* Baseline: player 100, the nearest unit 0x0101 is the other side's, nothing
 * else qualifies and the side has 1000 life points: the result is -1. */
static void reset(void)
{
    int i;

    t_player = 100; t_owner = 1; t_rnd = 0; search_calls = 0;
    u333 = 0x0101; num207 = num229 = num329 = num230 = 0;
    memset(l229, 0, sizeof(l229)); memset(l329, 0, sizeof(l329)); memset(l230, 0, sizeof(l230));
    memset(kind_of, 0, sizeof(kind_of)); memset(loc_of, 0, sizeof(loc_of));
    memset(land_of, 0, sizeof(land_of)); memset(ap_of, 0, sizeof(ap_of));
    memset(eff_of, 0, sizeof(eff_of)); memset(eff_set, 0, sizeof(eff_set));
    memset(field, 0, sizeof(field)); memset(apdp, 0, sizeof(apdp));
    for (i = 0; i < 5; i++) hand[i] = 999;
    ap_of[1][1] = -5;
    lp[0] = 1000; lp[1] = 8000;
}

/* Branch coverage: one assertion per branch outcome and boundary. */
static void check_tail(void)
{
    reset();
    assert(TaHand_IsDestinyCondition(0) == -1);
    assert(search_calls == 5);
    lp[0] = 250;
    assert(TaHand_IsDestinyCondition(0) == -1);
    lp[0] = 249; t_rnd = 2;
    assert(TaHand_IsDestinyCondition(0) == 6);
    t_rnd = 1;
    assert(TaHand_IsDestinyCondition(0) == 7);

    reset();
    t_player = 99; t_rnd = 0;
    assert(TaHand_IsDestinyCondition(0) == 6);
    t_rnd = 5;
    assert(TaHand_IsDestinyCondition(0) == 7);
    t_rnd = -1;
    assert(TaHand_IsDestinyCondition(0) == 7);
    assert(search_calls == 0);
}

static void check_near(void)
{
    reset();
    kind_of[1][1] = 7; loc_of[1][1] = 0x0203; loc_of[0][0] = 0x0304; num207 = 3;
    assert(TaHand_IsDestinyCondition(0) == 1);
    loc_of[1][1] = 0x0405;                    /* the other operand is the larger one */
    assert(TaHand_IsDestinyCondition(0) == 1);
    loc_of[1][1] = 0x0203; loc_of[0][0] = 0x0305; /* distance 3 */
    assert(TaHand_IsDestinyCondition(0) == -1);
    loc_of[0][0] = 0x0304; num207 = 2;
    assert(TaHand_IsDestinyCondition(0) == -1);
    num207 = 3; kind_of[1][1] = 6;
    assert(TaHand_IsDestinyCondition(0) == -1);
    kind_of[1][1] = 7; u333 = 0x0001; kind_of[0][1] = 7; loc_of[0][1] = 0x0203; /* own unit */
    assert(TaHand_IsDestinyCondition(0) == -1);
}

static void check_strong(void)
{
    reset();
    num229 = 3; kind_of[0][0] = 9;
    l229[0] = 0x0102; l229[1] = 0x0103; l229[2] = 0x0104;
    loc_of[1][2] = 0x0102; loc_of[1][3] = 0x0506; loc_of[1][4] = 0x0300;
    ap_of[1][2] = 3000; ap_of[1][3] = 3999; ap_of[1][4] = 3500;
    assert(TaHand_IsDestinyCondition(0) == 2);
    ap_of[1][2] = 2999;
    assert(TaHand_IsDestinyCondition(0) == -1);
    ap_of[1][2] = 3000; ap_of[1][3] = 4000;
    assert(TaHand_IsDestinyCondition(0) == -1);
    ap_of[1][3] = 3999; land_of[1][4] = 1;
    assert(TaHand_IsDestinyCondition(0) == -1);
    land_of[1][4] = 2;
    assert(TaHand_IsDestinyCondition(0) == 2);
    kind_of[0][0] = 8;
    assert(TaHand_IsDestinyCondition(0) == -1);
    kind_of[0][0] = 9; num229 = 2;
    assert(TaHand_IsDestinyCondition(0) == -1);
}

static void set_field(int count)
{
    int i;

    memset(field, 0, sizeof(field));
    for (i = 0; i < count; i++) field[i / 7][i % 7] = 8;
}

static void check_field(void)
{
    reset();
    num329 = 3; set_field(31);
    l329[0] = 0x0102; l329[1] = 0x0103; l329[2] = 0x0104; l329[3] = 0x0005;
    eff_set[1][2] = eff_set[1][3] = eff_set[1][4] = eff_set[0][5] = 1;
    eff_of[1][2].dwID = 0x800; eff_of[1][3].dwID = 0x9ff; eff_of[1][4].dwID = 0xa00;
    eff_of[0][5].dwID = 0x800;
    assert(TaHand_IsDestinyCondition(0) == 3);
    set_field(30);
    assert(TaHand_IsDestinyCondition(0) == -1);
    set_field(49);
    assert(TaHand_IsDestinyCondition(0) == 3);
    eff_set[1][3] = 0;
    assert(TaHand_IsDestinyCondition(0) == -1);
    eff_set[1][3] = 1; eff_of[1][4].dwID = 0xb00;
    assert(TaHand_IsDestinyCondition(0) == -1);
    eff_of[1][4].dwID = 0x7ff;
    assert(TaHand_IsDestinyCondition(0) == -1);
    eff_of[1][4].dwID = 0xa00; num329 = 4;  /* one own unit with such an effect */
    assert(TaHand_IsDestinyCondition(0) == -1);
    eff_of[0][5].dwID = 0x700;
    assert(TaHand_IsDestinyCondition(0) == 3);
    num329 = 2;
    assert(TaHand_IsDestinyCondition(0) == -1);
}

static void check_hand(void)
{
    reset();
    loc_of[1][1] = 0x0605; ap_of[1][1] = 2000;
    hand[1] = 5; hand[3] = 6; apdp[5] = (1500 << 16) | 77; apdp[6] = 1800 << 16;
    lp[1] = 402;
    assert(TaHand_IsDestinyCondition(0) == 4);
    lp[1] = 400;
    assert(TaHand_IsDestinyCondition(0) == -1);
    lp[1] = 8000; ap_of[1][1] = 1800;
    assert(TaHand_IsDestinyCondition(0) == -1);
    hand[1] = hand[3] = 999; ap_of[1][1] = 10; /* empty hand: best is -1 */
    assert(TaHand_IsDestinyCondition(0) == 4);
    hand[4] = 7; apdp[7] = (int)0x80000000u;   /* the high half is taken unsigned */
    ap_of[1][1] = 32769;
    assert(TaHand_IsDestinyCondition(0) == 4);

    reset();                                   /* side 1 asks for side 0's life points */
    u333 = 0x0001; ap_of[0][1] = 10; lp[1] = 1000; lp[0] = 8000;
    assert(TaHand_IsDestinyCondition(1) == 4);
    lp[0] = 22;
    assert(TaHand_IsDestinyCondition(1) == -1);
    lp[0] = 24;
    assert(TaHand_IsDestinyCondition(1) == 4);
}

static void check_types(void)
{
    reset();
    u333 = 0x0001; num230 = 4;
    l230[0] = 0x0102; l230[1] = 0x0103; l230[2] = 0x0104; l230[3] = 0x0105;
    kind_of[1][2] = 64; kind_of[1][3] = 95; kind_of[1][4] = 70; kind_of[1][5] = 80;
    assert(TaHand_IsDestinyCondition(0) == 5);
    kind_of[1][5] = 96;
    assert(TaHand_IsDestinyCondition(0) == -1);
    kind_of[1][5] = 63;
    assert(TaHand_IsDestinyCondition(0) == -1);
    kind_of[1][5] = 80; num230 = 3;
    assert(TaHand_IsDestinyCondition(0) == -1);
    num230 = 4; u333 = 0x0101;                 /* the nearest unit is the other side's */
    assert(TaHand_IsDestinyCondition(0) == -1);
}

int main(void)
{
    check_tail();
    check_near();
    check_strong();
    check_field();
    check_hand();
    check_types();
    return 0;
}
