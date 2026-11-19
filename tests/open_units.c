/* Synthetic host tests for the open_units group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/open_units.h"

static int card[2][8], ctl[2][12], loc[2][8], kind[2][8], unit[2][8];
static unsigned eff_word, *eff_p;
static int status_n, status_late_on, status_late, react_f;
static int status_v, react_v, cerem_v, field_v, wall_v;
static int react_calls, cerem_calls, field_calls, wall_calls;

int SzDuel_GetUnitCardNo(int a, int b) { assert((a == 0 || a == 1) && b >= 1 && b < 8); return card[a][b]; }
int func_00250360_pair(int a, int b) { assert((a == 0 || a == 1) && b >= 1 && b < 12); return ctl[a][b]; }
int SzDuel_GetUnitLoc(int a, int b) { assert((a == 0 || a == 1) && b >= 1 && b < 8); return loc[a][b]; }
int SzDuel_GetUnitKind(int a, int b) { assert((a == 0 || a == 1) && b >= 1 && b < 8); return kind[a][b]; }
int func_001df540_word(int a, int b) { assert((a == 0 || a == 1) && b >= 1 && b < 8); return unit[a][b]; }
unsigned *SzDuel_GetUnitEffect(int a, int b, int n) { assert(b == 7 && n == 0); (void)a; return eff_p; }
/* status_late_on: calls after the first one since status_n was cleared answer status_late. */
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert((a == 0 || a == 1) && b == 7 && bit == 4);
    return status_n++ == 0 || !status_late_on ? status_v : status_late;
}
int TaLogic_GetActionReaction(int a, int b, int c, int d, int e, int f)
{
    assert(b == 7 && c == 0 && d == 0 && e == 6 && f == react_f);
    (void)a;
    react_calls++;
    return react_v;
}
int TaLogic008_IsDoCerem(int value, int index)
{
    assert(value == 0x55 && index == 77);
    cerem_calls++;
    return cerem_v;
}
int SzDuel_GetField(int x, int y) { assert(x == 0x12 && y == 0x03); field_calls++; return field_v; }
int TaLogic008_GetWallEmpty(int side) { assert(side == 0 || side == 1); wall_calls++; return wall_v; }

static void reset(int x, int k)
{
    memset(card, 0, sizeof card); memset(ctl, 0, sizeof ctl); memset(loc, 0, sizeof loc);
    memset(kind, 0, sizeof kind); memset(unit, 0, sizeof unit);
    status_n = 0; status_late_on = 0; status_late = 0;
    status_v = 0; react_v = 0; cerem_v = 0; field_v = 0; wall_v = -1;
    react_calls = cerem_calls = field_calls = wall_calls = 0;
    eff_word = 0; eff_p = &eff_word;
    kind[x][7] = k << 5;
    card[x][7] = 77;
    loc[x][7] = 0x0312;
    ctl[x][7] = 0x55;
}

static void test_134b40(int x)
{
    int o = x == 0 ? 1 : 0;
    int i;
    /* status bit set: 0 before anything else */
    reset(x, 0); status_v = 1;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 0);
    /* kind classes outside 0, 1, 5 */
    reset(x, 2);
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2);
    /* class 0, a card other than 190 */
    reset(x, 0);
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2);
    /* class 0, card 190: counts slots 1..5 of both sides with low kind 9 controlled by x */
    reset(x, 0); card[x][7] = 190;
    kind[x][1] = 9; ctl[x][1] = x; kind[o][5] = 0x29; ctl[o][5] = x;
    kind[x][2] = 9; ctl[x][2] = o;        /* not controlled by x */
    kind[o][3] = 8; ctl[o][3] = x;        /* wrong kind */
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2);   /* two: below the bound */
    kind[o][3] = 9;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 1);   /* three */
    /* class 5 */
    reset(x, 5); react_v = 5;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2 && react_calls == 1 && cerem_calls == 0);
    reset(x, 5); cerem_v = 0;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2 && cerem_calls == 1 && field_calls == 0);
    reset(x, 5); cerem_v = 1; field_v = 9;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2 && field_calls == 1);
    reset(x, 5); cerem_v = 1; field_v = 8;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 1 && field_calls == 1);
    /* class 1 */
    reset(x, 1); react_v = 5;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2 && react_calls == 1);
    reset(x, 1); eff_p = NULL;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2);
    reset(x, 1); eff_word = 99 << 8;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2);
    reset(x, 1); eff_word = (13 << 8) | 0xff;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 1);
    for (i = 0; i < 2; i++) {
        unsigned e = (i ? 72u : 2u) << 8;
        reset(x, 1); eff_word = e; wall_v = -1;
        assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2 && wall_calls == 1);
        reset(x, 1); eff_word = e; wall_v = 0x0313;
        assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2);
        reset(x, 1); eff_word = e; wall_v = 0x0312;
        assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 1);
    }
    /* effect 37: any present unit of either side that is not card 145 and is controlled by x */
    reset(x, 1); eff_word = 37 << 8;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2);
    unit[x][1] = 1; card[x][1] = 145; ctl[x][1] = x;     /* excluded card */
    unit[o][2] = 1; card[o][2] = 3; ctl[o][2] = o;       /* not controlled by x */
    card[x][3] = 3; ctl[x][3] = x;                       /* no unit */
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 2);
    unit[o][5] = 1; card[o][5] = 3; ctl[o][5] = x;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 1);
    reset(x, 1); eff_word = 37 << 8;
    unit[x][1] = 1; card[x][1] = 3; ctl[x][1] = x;
    assert(TaLogic_IsOpenUnit__00134b40(x, 7) == 1);
}

#define LIST 0x1234
#define LIST2 0x4321
#define CERLIST 0x2468
/* m9: the 0x139180 fixtures (two searches, a sacrifice list). */
static int m9, cer_n, cer_own[3], own9, sac_v, land_v, revolt_v, curse[2][12], ap9[2][8];
static struct DotrOpenPair pair9 = { 0, 0x99 };
static int sac_calls, land_calls, revolt_calls, ap_calls, land_c = 3;
struct DotrOpenPair *TaHand_GetCardEffect(int id, int b) { assert(id == 77 && b == 0); return &pair9; }
void Ta_GetCeremSacrifice(int *Buf, int Obj) { assert(Obj == 0x99); Buf[0] = 11; Buf[1] = 22; Buf[2] = 33; }
int Ta_GetSacrificeUnit(int *triple, int value)
{
    assert(triple[0] == 11 && triple[1] == 22 && triple[2] == 33 && value == 0x55);
    sac_calls++;
    return sac_v;
}
int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    assert((a == 0 || a == 1) && b >= 1 && b < 8 && c == (loc[a][b] & 0xff) && d == loc[a][b] >> 8);
    ap_calls++;
    return ap9[a][b];
}
int SzDuel_GetUnitCurseRest(int a, int b) { assert((a == 0 || a == 1) && b >= 1 && b < 12); return curse[a][b]; }
int TaLogic_GetLandChange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0x7777 && b == 0x12 && c == land_c && d == 6 && e == 5 && f == 2);
    land_calls++;
    return land_v;
}
int TaLogic009_GetRevoltObj(int side) { assert(side == 0 || side == 1); revolt_calls++; return revolt_v; }
static int side_v, hand_ids[5], hand_n, parts_v, exzodia_v, lp_v, search_e, list_n, safe_v, combo_v, unitnum_v;
static int owner_by[8], danger_by[8], cardkind_by[16], type_by[8];
static int search_c = -1, search_d = 2, search_a, search_b, search_f = 14;
static int search_calls, combo_calls, unitnum_calls, lp_calls, exzodia_calls;
void TaHand_GetNextHand(int index, int *out) { assert(index == 0 || index == 1); memcpy(out, hand_ids, sizeof hand_ids); }
int TaLogic017_IsExzParts(int id) { assert(id == card[0][7] || id == card[1][7]); return parts_v; }
int TaLogic017_IsExzodia(int side) { assert(side == side_v); exzodia_calls++; return exzodia_v; }
int SzDuel_GetLP(int side) { assert(side == 0 || side == 1); lp_calls++; return lp_v; }
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    if (m9) {
        assert(a == 0 && b == 0 && (c == 0 || c == 1) && f == 14 && ((d == 2 && e == 0) || (d == 3 && e == 33)));
        search_calls++;
        return d == 2 ? LIST : LIST2;
    }
    assert(a == search_a && b == search_b && c == (search_c < 0 ? side_v : search_c) && d == search_d && e == search_e && f == search_f);
    search_calls++;
    return LIST;
}
int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    if (m9 && value == CERLIST) { return cer_n; }
    assert(value == LIST);
    return list_n;
}
int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    if (m9 && word == LIST2) { assert(n == 0); return own9; }
    if (m9 && word == CERLIST) { assert(n >= 0 && n < cer_n); return cer_own[n]; }
    assert(word == LIST && n >= 0 && n < list_n);
    return owner_by[n];
}
int TaLogic_IsJudgeUnitDanger(int a, int b, int skip) { assert((a == 0 || a == 1) && b >= 1 && b < 8 && skip == 0); return danger_by[b]; }
int TaHand_GetNextHandNum(int index) { assert(index == side_v); return hand_n; }
int TaHand_GetCardKind(int id) { assert(id >= 0 && id < 16); return cardkind_by[id]; }
int SzDuel_Kind2Type(int value) { assert(value >= 0 && value < 8); return type_by[value]; }
int TaLogic017_IsExzSafe(int side) { assert(side == side_v); return safe_v; }
int TaLogic017_PlayExzCombo(int side, unsigned char *item, struct DotrOpenCardInfo *info)
{
    assert(side == side_v && item != NULL && info != NULL && (void *)item != (void *)info);
    combo_calls++;
    return combo_v;
}
int Ta_GetUnitNum(int a, unsigned char k) { assert(a == (side_v == 0 ? 1 : 0) && k == 3); unitnum_calls++; return unitnum_v; }

static void reset17(int x, int k, int s)
{
    reset(x, k);
    side_v = s; ctl[x][7] = s;
    memset(hand_ids, 0, sizeof hand_ids); memset(owner_by, 0, sizeof owner_by);
    memset(danger_by, 0, sizeof danger_by); memset(cardkind_by, 0, sizeof cardkind_by);
    memset(type_by, 0, sizeof type_by);
    hand_n = 0; parts_v = 0; exzodia_v = 0; lp_v = 0; search_e = -1; list_n = 0; safe_v = 0; combo_v = -1; unitnum_v = 0;
    search_calls = combo_calls = unitnum_calls = lp_calls = exzodia_calls = 0;
}

#define OPEN17(x) TaLogic_IsOpenUnit__00158360((x), 7, NULL)
static void test_158360(int x, int s)
{
    /* status bit set: 0 before anything else */
    reset17(x, 0, s); status_v = 1; parts_v = 1; exzodia_v = 4;
    assert(OPEN17(x) == 0 && exzodia_calls == 0);
    /* an Exodia part with answer 4: 1 whatever the kind class */
    reset17(x, 2, s); parts_v = 1; exzodia_v = 4;
    assert(OPEN17(x) == 1 && exzodia_calls == 1);
    reset17(x, 2, s); parts_v = 1; exzodia_v = 3;
    assert(OPEN17(x) == 2 && exzodia_calls == 1);
    /* not a part: the second question is not asked; class outside 0 and 1 */
    reset17(x, 2, s); exzodia_v = 4;
    assert(OPEN17(x) == 2 && exzodia_calls == 0);
    /* class 0, a card other than 64 and 95 */
    reset17(x, 0, s);
    assert(OPEN17(x) == 2 && lp_calls == 0 && search_calls == 0);
    /* class 0, card 95: life points below 1200 */
    reset17(x, 0, s); card[x][7] = 95; lp_v = 1199;
    assert(OPEN17(x) == 1 && lp_calls == 1);
    reset17(x, 0, s); card[x][7] = 95; lp_v = 1200;
    assert(OPEN17(x) == 2 && lp_calls == 1);
    /* class 0, card 64: two listed units judged in danger */
    reset17(x, 0, s); card[x][7] = 64; search_e = 3; list_n = 0;
    assert(OPEN17(x) == 2 && search_calls == 1);
    reset17(x, 0, s); card[x][7] = 64; search_e = 3; list_n = 3;
    owner_by[0] = 0x0101; owner_by[1] = 0x0002; owner_by[2] = 0x0103; danger_by[1] = 1; danger_by[2] = 2;
    assert(OPEN17(x) == 2);                             /* one */
    danger_by[3] = 1;
    assert(OPEN17(x) == 1);                             /* two */
    /* class 1 */
    reset17(x, 1, s); react_v = 5;
    assert(OPEN17(x) == 2 && react_calls == 1);
    reset17(x, 1, s); eff_p = NULL;
    assert(OPEN17(x) == 2);
    reset17(x, 1, s); eff_word = 99 << 8;
    assert(OPEN17(x) == 2 && search_calls == 0);
    /* effect 34: at least four units in the range-29 list */
    reset17(x, 1, s); eff_word = 34 << 8; search_e = 29; list_n = 3;
    assert(OPEN17(x) == 2 && search_calls == 1);
    list_n = 4;
    assert(OPEN17(x) == 1);
    /* effect 23: a unit of the range-52 list whose location row is 2 or more */
    reset17(x, 1, s); eff_word = 23 << 8; search_e = 52; list_n = 0;
    assert(OPEN17(x) == 2 && search_calls == 1);
    reset17(x, 1, s); eff_word = 23 << 8; search_e = 52; list_n = 2;
    owner_by[0] = 0x0001; owner_by[1] = 0x0102; loc[0][1] = 0x01ff; loc[1][2] = 0x0100;
    assert(OPEN17(x) == 2);
    loc[1][2] = 0x0200;
    assert(OPEN17(x) == 1);
    /* effect 38: a hand card of type 2, else the Exodia combination */
    reset17(x, 1, s); eff_word = 38 << 8;
    assert(OPEN17(x) == 2 && combo_calls == 0);
    reset17(x, 1, s); eff_word = 38 << 8; hand_n = 2; hand_ids[0] = 3; hand_ids[1] = 4;
    cardkind_by[3] = 1 << 5; cardkind_by[4] = (2 << 5) | 0x1f; type_by[1] = 1; type_by[2] = 3;
    assert(OPEN17(x) == 2 && combo_calls == 0);
    type_by[2] = 2;
    assert(OPEN17(x) == 1 && combo_calls == 0);
    hand_n = 1;                                         /* the count bounds the scan */
    assert(OPEN17(x) == 2);
    reset17(x, 1, s); eff_word = 38 << 8; safe_v = 1; combo_v = -1;
    assert(OPEN17(x) == 2 && combo_calls == 1);
    reset17(x, 1, s); eff_word = 38 << 8; safe_v = 1; combo_v = 0;
    assert(OPEN17(x) == 1 && combo_calls == 1);
    /* effect 13: 1 unless its low six bits are 50; then three kind-3 units of the other side */
    reset17(x, 1, s); eff_word = (13 << 8) | 49;
    assert(OPEN17(x) == 1 && unitnum_calls == 0);
    reset17(x, 1, s); eff_word = (13 << 8) | 50 | 0x40; unitnum_v = 2;
    assert(OPEN17(x) == 2 && unitnum_calls == 1);
    reset17(x, 1, s); eff_word = (13 << 8) | 50; unitnum_v = 3;
    assert(OPEN17(x) == 1 && unitnum_calls == 1);
}

static int cerem10_v, cerem10_calls, thunder_v, thunder_calls;
int TaLogic010_IsDoCerem(int value, int index) { assert(value == 0x55 && index == 77); cerem10_calls++; return cerem10_v; }
int TaLogic010_GetUseThunderBolt(int side) { assert(side == 0x55); thunder_calls++; return thunder_v; }

#define OPEN10(x) TaLogic_IsOpenUnit__0013d3d0((x), 7, NULL)
static void test_13d3d0(int x)
{
    static const int pair[4][2] = { { 197, 153 }, { 153, 197 }, { 170, 171 }, { 171, 170 } };
    int o = x == 0 ? 1 : 0;
    int i;
    /* status bit set: 0 before anything else */
    reset(x, 0); status_v = 1;
    assert(OPEN10(x) == 0);
    /* kind classes outside 0, 1, 5 */
    reset(x, 2);
    assert(OPEN10(x) == 2);
    /* class 0, a card outside the four */
    reset(x, 0);
    assert(OPEN10(x) == 2);
    /* class 0: each of the four cards counts its partner card in slots 1..5 of both sides, controlled by x */
    for (i = 0; i < 4; i++) {
        reset(x, 0); card[x][7] = pair[i][0];
        card[x][1] = pair[i][1]; ctl[x][1] = x;
        card[x][2] = pair[i][1]; ctl[x][2] = o;        /* not controlled by x */
        card[o][3] = pair[i][0]; ctl[o][3] = x;        /* the card itself, not the partner */
        card[x][6] = pair[i][1]; ctl[x][6] = x;        /* slot beyond the scan */
        assert(OPEN10(x) == 2);                        /* one: below the bound */
        card[o][5] = pair[i][1]; ctl[o][5] = x;
        assert(OPEN10(x) == 1);                        /* two */
    }
    /* class 5 */
    reset(x, 5); react_v = 5; cerem10_calls = 0;
    assert(OPEN10(x) == 2 && react_calls == 1 && cerem10_calls == 0);
    reset(x, 5); cerem10_v = 0; cerem10_calls = 0;
    assert(OPEN10(x) == 2 && cerem10_calls == 1 && field_calls == 0);
    reset(x, 5); cerem10_v = 1; field_v = 9;
    assert(OPEN10(x) == 2 && field_calls == 1);
    reset(x, 5); cerem10_v = 1; field_v = 8;
    assert(OPEN10(x) == 1 && field_calls == 1);
    /* class 1: the chosen unit shares a column or a row with this one */
    reset(x, 1); react_v = 5; thunder_calls = 0;
    assert(OPEN10(x) == 2 && react_calls == 1 && thunder_calls == 0);
    reset(x, 1); thunder_v = 0x0102; loc[1][2] = 0x0413; thunder_calls = 0;
    assert(OPEN10(x) == 2 && thunder_calls == 1);
    reset(x, 1); thunder_v = 0x0102; loc[1][2] = 0x0412;      /* same low byte */
    assert(OPEN10(x) == 1);
    reset(x, 1); thunder_v = 0x0003; loc[0][3] = 0x0313;      /* same high byte */
    assert(OPEN10(x) == 1);
}

static int cerem05_v, cerem05_calls;
int TaLogic005_IsDoCerem(int value, int index) { assert(value == 0x55 && index == 77); cerem05_calls++; return cerem05_v; }

#define OPEN05(x) (status_n = 0, TaLogic_IsOpenUnit__00126380((x), 7, NULL))
static void test_126380(int x)
{
    static const int pair[2][2] = { { 560, 600 }, { 600, 560 } };
    int o = x == 0 ? 1 : 0;
    int i;
    /* status bit set: 0 before anything else */
    reset(x, 0); status_v = 1;
    assert(OPEN05(x) == 0);
    /* kind classes 2..4 and beyond the table */
    for (i = 2; i < 8; i++) {
        if (i == 5) {
            continue;
        }
        reset(x, i);
        assert(OPEN05(x) == 2);
    }
    /* class 0, a card outside the four */
    reset(x, 0);
    assert(OPEN05(x) == 2);
    /* class 0, cards 560 and 600: the partner card in slots 1..5 of both sides, controlled by x */
    for (i = 0; i < 2; i++) {
        reset(x, 0); card[x][7] = pair[i][0];
        card[x][1] = pair[i][1]; ctl[x][1] = x;
        card[x][2] = pair[i][1]; ctl[x][2] = o;        /* not controlled by x */
        card[o][3] = pair[i][0]; ctl[o][3] = x;        /* the card itself, not the partner */
        card[x][6] = pair[i][1]; ctl[x][6] = x;        /* slot beyond the scan */
        assert(OPEN05(x) == 2);                        /* one: below the bound */
        card[o][5] = pair[i][1]; ctl[o][5] = x;
        assert(OPEN05(x) == 1);                        /* two */
    }
    /* class 0, card 593: three units of low kind 16 in slots 1..5 of both sides, controlled by x */
    reset(x, 0); card[x][7] = 593;
    kind[x][1] = 16; ctl[x][1] = x; kind[o][5] = 0x30; ctl[o][5] = x;
    kind[x][2] = 16; ctl[x][2] = o;        /* not controlled by x */
    kind[o][3] = 15; ctl[o][3] = x;        /* wrong kind */
    assert(OPEN05(x) == 2);                /* two: below the bound */
    kind[o][3] = 16;
    assert(OPEN05(x) == 1);                /* three */
    /* class 0, card 565: while the unit's own status bit answers set inside the scan, any of
     * slots 6..10 of either side controlled by x */
    reset(x, 0); card[x][7] = 565; status_late_on = 1; status_late = 1;
    for (i = 6; i <= 10; i++) { ctl[x][i] = 0x55; ctl[o][i] = 0x55; }
    ctl[x][5] = x; ctl[o][5] = x;          /* slot before the scan */
    assert(OPEN05(x) == 2);
    ctl[o][10] = x;
    assert(OPEN05(x) == 1);
    ctl[o][10] = 0x55; ctl[x][6] = x;
    assert(OPEN05(x) == 1);
    status_late = 0;                       /* status clear inside the scan: nothing counts */
    assert(OPEN05(x) == 2);
    /* class 5 */
    reset(x, 5); react_v = 5; cerem05_calls = 0;
    assert(OPEN05(x) == 2 && react_calls == 1 && cerem05_calls == 0);
    reset(x, 5); cerem05_v = 0; cerem05_calls = 0;
    assert(OPEN05(x) == 2 && cerem05_calls == 1 && field_calls == 0);
    reset(x, 5); cerem05_v = 1; field_v = 9;
    assert(OPEN05(x) == 2 && field_calls == 1);
    reset(x, 5); cerem05_v = 1; field_v = 8;
    assert(OPEN05(x) == 1 && field_calls == 1);
    /* class 1: no effect record, the reaction answer 5, then more than two units in the range list */
    search_c = 0x55; search_d = 1; search_calls = 0;
    reset(x, 1); eff_p = NULL;
    assert(OPEN05(x) == 2 && react_calls == 0);
    reset(x, 1); react_v = 5;
    assert(OPEN05(x) == 2 && react_calls == 1 && search_calls == 0);
    reset(x, 1); eff_word = 0xffffff40u | 21; search_e = 21; list_n = 2;
    assert(OPEN05(x) == 2 && search_calls == 1);
    list_n = 3;
    assert(OPEN05(x) == 1 && search_calls == 2);
    search_c = -1; search_d = 2; search_e = -1; list_n = 0;
}

static int cerem06_v, cerem06_calls, attacked_v, attacked_calls, hole_v, hole_calls, grave_v, mons_v, mons_calls;
int TaLogic006_IsDoCerem(int value, int index) { assert(value == 0x55 && index == 77); cerem06_calls++; return cerem06_v; }
int TaLogic_IsAttackedNow(int a, int b, int c) { assert((a == 0 || a == 1) && b == 0 && c == 0); attacked_calls++; return attacked_v; }
int TaLogic006_IsBlackHoleCondition(int side) { assert(side == 0 || side == 1); hole_calls++; return hole_v; }
int TaLogic_GetMaxAPGraveCard(int side, int mask) { assert((side == 0 || side == 1) && mask == 3); return grave_v; }
int SzDuel_GetUnitMonsterCount(int side) { assert(side == 0 || side == 1); mons_calls++; return mons_v; }

static void reset06(int x, int k)
{
    reset(x, k);
    cerem06_v = 0; attacked_v = 0; hole_v = 0; grave_v = -1; mons_v = 0;
    cerem06_calls = attacked_calls = hole_calls = mons_calls = 0;
}

#define OPEN06(x) TaLogic_IsOpenUnit__0012b210((x), 7, NULL)
static void test_12b210(int x)
{
    static const int same[3] = { 101, 102, 119 };
    int o = x == 0 ? 1 : 0;
    int i;
    /* status bit set: 0 before anything else */
    reset06(x, 0); status_v = 1;
    assert(OPEN06(x) == 0);
    /* kind classes 2..4 and beyond the table */
    for (i = 2; i < 8; i++) {
        if (i == 5) {
            continue;
        }
        reset06(x, i);
        assert(OPEN06(x) == 2);
    }
    /* class 0, a card outside the four */
    reset06(x, 0);
    assert(OPEN06(x) == 2);
    /* class 0, card 108: always 1 */
    reset06(x, 0); card[x][7] = 108;
    assert(OPEN06(x) == 1);
    /* class 0, cards 101, 102, 119: the same card in slots 1..5 of both sides, controlled by x */
    for (i = 0; i < 3; i++) {
        reset06(x, 0); card[x][7] = same[i];
        card[x][1] = same[i]; ctl[x][1] = x;
        card[x][2] = same[i]; ctl[x][2] = o;        /* not controlled by x */
        card[o][3] = same[i] + 200; ctl[o][3] = x;  /* another card */
        card[x][6] = same[i]; ctl[x][6] = x;        /* slot beyond the scan */
        assert(OPEN06(x) == 2);                     /* one: below the bound */
        card[o][5] = same[i]; ctl[o][5] = x;
        assert(OPEN06(x) == 1);                     /* two */
    }
    /* class 5: no reaction query in this copy */
    reset06(x, 5); react_v = 5;
    assert(OPEN06(x) == 2 && react_calls == 0 && cerem06_calls == 1 && field_calls == 0);
    reset06(x, 5); cerem06_v = 1; field_v = 9;
    assert(OPEN06(x) == 2 && field_calls == 1);
    reset06(x, 5); cerem06_v = 1; field_v = 8;
    assert(OPEN06(x) == 1 && field_calls == 1);
    /* class 1 */
    reset06(x, 1); react_v = 5; card[x][7] = 699; attacked_v = 1;
    assert(OPEN06(x) == 2 && react_calls == 1 && attacked_calls == 0);
    reset06(x, 1);                                   /* a card outside the three */
    assert(OPEN06(x) == 2 && react_calls == 1);
    /* card 699: attacked now, else the black hole condition */
    reset06(x, 1); card[x][7] = 699; attacked_v = 1;
    assert(OPEN06(x) == 1 && attacked_calls == 1 && hole_calls == 0);
    reset06(x, 1); card[x][7] = 699; attacked_v = 2; hole_v = 1;
    assert(OPEN06(x) == 1 && hole_calls == 1);
    reset06(x, 1); card[x][7] = 699; hole_v = 2;
    assert(OPEN06(x) == 2 && hole_calls == 1);
    /* card 685: a grave card of 2400 or more in the low 16 bits, fewer than five monsters, field not 9 */
    reset06(x, 1); card[x][7] = 685; grave_v = -1;
    assert(OPEN06(x) == 2 && mons_calls == 0);
    reset06(x, 1); card[x][7] = 685; grave_v = 0x10000 | 2399;
    assert(OPEN06(x) == 2 && mons_calls == 0);
    reset06(x, 1); card[x][7] = 685; grave_v = 0x10000 | 2400; mons_v = 5;
    assert(OPEN06(x) == 2 && mons_calls == 1 && field_calls == 0);
    reset06(x, 1); card[x][7] = 685; grave_v = 2400; mons_v = 4; field_v = 9;
    assert(OPEN06(x) == 2 && field_calls == 1);
    field_v = 8;
    assert(OPEN06(x) == 1);
    /* card 745: three present units of low kind other than 2 in slots 1..5, controlled by x */
    reset06(x, 1); card[x][7] = 745;
    for (i = 1; i <= 5; i++) { ctl[x][i] = 0x55; ctl[o][i] = 0x55; }
    unit[x][1] = 1; kind[x][1] = 3; ctl[x][1] = x;
    unit[o][5] = 1; kind[o][5] = 0x23; ctl[o][5] = x;
    unit[x][2] = 1; kind[x][2] = 0x22; ctl[x][2] = x;   /* low kind 2 */
    unit[o][3] = 0; kind[o][3] = 3; ctl[o][3] = x;      /* no unit */
    unit[x][4] = 1; kind[x][4] = 3; ctl[x][4] = o;      /* not controlled by x */
    assert(OPEN06(x) == 2);                             /* two: below the bound */
    unit[o][3] = 1;
    assert(OPEN06(x) == 1);                             /* three */
}

static unsigned eff20[2];
static int cerem20_v, conv_n, conv_log[4][2], remove_calls, lp20_v, count20_v, count20_side, hand20_v;
int TaLogic020_IsDoCerem(int value, int index) { assert(value == 0x55 && index == 77); return cerem20_v; }
unsigned ConvUnitIDtoFlagList_OFF(unsigned v, int flag, int n)
{
    assert(v == LIST);
    if (conv_n < 4) {
        conv_log[conv_n][0] = flag; conv_log[conv_n][1] = n;
    }
    conv_n++;
    return v;
}
int Ta_RemoveObjFromFlagLst(int word, int side) { assert(word == LIST && side == 0x55); remove_calls++; return word; }
int func_001df470_side(int side) { assert(side == 0x55); return lp20_v; }
int SzDuel_GetUnitCount(int side) { count20_side = side; return count20_v; }
int SzDuel_InqHandPhase(int index) { assert(index == 0x55); return hand20_v; }

/* Class-1 fixture: effect word 0 is (effect << 8) | (2 << 6) | 21, word 1 is hi << 16. */
static void reset20(int x, int k, unsigned effect, unsigned hi)
{
    reset(x, k);
    eff20[0] = (effect << 8) | (2 << 6) | 21; eff20[1] = hi << 16; eff_p = eff20;
    search_c = 0x55; search_d = 2; search_e = 21; search_a = 0; search_b = 0; search_f = 14; search_calls = 0;
    list_n = 0; memset(owner_by, 0, sizeof owner_by);
    cerem20_v = 0; conv_n = 0; remove_calls = 0; lp20_v = 0; count20_v = 0; count20_side = -1; hand20_v = 0;
}

#define OPEN20(x) TaLogic_IsOpenUnit__001670a0((x), 7, NULL)
static void test_1670a0(int x)
{
    int o = x == 0 ? 1 : 0;
    int i;
    react_f = 1;
    /* status bit set: 0 before anything else */
    reset20(x, 1, 38, 0); status_v = 1;
    assert(OPEN20(x) == 0);
    /* kind classes other than 1 and 5 */
    for (i = 0; i < 8; i++) {
        if (i == 1 || i == 5) {
            continue;
        }
        reset20(x, i, 38, 0);
        assert(OPEN20(x) == 2 && react_calls == 0);
    }
    /* class 5: no reaction query */
    reset20(x, 5, 38, 0); react_v = 5;
    assert(OPEN20(x) == 2 && react_calls == 0 && field_calls == 0);
    reset20(x, 5, 38, 0); cerem20_v = 1; field_v = 9;
    assert(OPEN20(x) == 2 && field_calls == 1);
    reset20(x, 5, 38, 0); cerem20_v = 1; field_v = 8;
    assert(OPEN20(x) == 1 && field_calls == 1);
    /* class 1: the reaction answer 5, no effect record, an effect outside the five */
    reset20(x, 1, 38, 0); react_v = 5;
    assert(OPEN20(x) == 2 && react_calls == 1);
    reset20(x, 1, 38, 0); eff_p = NULL;
    assert(OPEN20(x) == 2 && react_calls == 1);
    reset20(x, 1, 99, 0);
    assert(OPEN20(x) == 2 && search_calls == 0);
    /* effect 38: the hand phase answer 0 */
    reset20(x, 1, 38, 0);
    assert(OPEN20(x) == 1);
    reset20(x, 1, 38, 0); hand20_v = 1;
    assert(OPEN20(x) == 2);
    /* effect 24: five units left in the range list */
    reset20(x, 1, 24, 0); list_n = 4;
    assert(OPEN20(x) == 2 && search_calls == 1 && remove_calls == 1);
    assert(conv_n == 1 && conv_log[0][0] == x && conv_log[0][1] == 7);
    reset20(x, 1, 24, 0); list_n = 5;
    assert(OPEN20(x) == 1);
    /* effect 23: the first unit of the list exists and is not controlled like this one */
    reset20(x, 1, 23, 0); list_n = 1; owner_by[0] = -1;
    assert(OPEN20(x) == 2 && search_calls == 1 && remove_calls == 1 && conv_n == 1);
    reset20(x, 1, 23, 0); list_n = 1; owner_by[0] = 0x0102; ctl[1][2] = 0x55;
    assert(OPEN20(x) == 2);
    ctl[1][2] = 0x54;
    assert(OPEN20(x) == 1);
    /* effect 13 by card */
    reset20(x, 1, 13, 0);                              /* a card outside the three */
    assert(OPEN20(x) == 2);
    reset20(x, 1, 13, 0); card[x][7] = 713;
    assert(OPEN20(x) == 1);
    reset20(x, 1, 13, 0); card[x][7] = 707; lp20_v = 1999;
    assert(OPEN20(x) == 1);
    lp20_v = 2000;
    assert(OPEN20(x) == 2);
    reset20(x, 1, 13, 0); card[x][7] = 714; count20_v = 2;
    assert(OPEN20(x) == 2 && count20_side == 0);       /* controller 0x55: side 0 is asked */
    count20_v = 3;
    assert(OPEN20(x) == 1);
    /* effect 4, second word 18: range search from the unit's location; a unit controlled like
     * this one gives 2, otherwise 1 */
    reset20(x, 1, 4, 18); search_a = 0x12; search_b = 3; search_f = 18;
    assert(OPEN20(x) == 1 && search_calls == 1 && remove_calls == 0 && conv_n == 3);
    assert(conv_log[0][0] == x && conv_log[0][1] == 7 && conv_log[1][0] == x && conv_log[1][1] == 0);
    assert(conv_log[2][0] == o && conv_log[2][1] == 0);
    reset20(x, 1, 4, 18); search_a = 0x12; search_b = 3; search_f = 18;
    list_n = 2; owner_by[0] = -1; owner_by[1] = 0x0102; ctl[1][2] = 0x54;
    assert(OPEN20(x) == 1);
    ctl[1][2] = 0x55;
    assert(OPEN20(x) == 2);
    list_n = 1;                                        /* the count bounds the scan */
    assert(OPEN20(x) == 1);
    /* effect 4, another second word: the whole-board list; the count of units scanned is returned */
    reset20(x, 1, 4, 17);
    assert(OPEN20(x) == 0 && search_calls == 1 && remove_calls == 1 && conv_n == 1);
    reset20(x, 1, 4, 17); list_n = 4;
    owner_by[0] = 0x0102; owner_by[1] = -1; owner_by[2] = 0x0003; owner_by[3] = 0x0104;
    ctl[1][2] = 0x54; ctl[0][3] = 0x54; ctl[1][4] = 0x54;
    assert(OPEN20(x) == 3);
    list_n = 1;
    assert(OPEN20(x) == 1);
    list_n = 4; ctl[1][4] = 0x55;
    assert(OPEN20(x) == 2);
    search_c = -1; search_d = 2; search_e = -1; list_n = 0; react_f = 0;
}

static void reset09(int x, int k)
{
    reset(x, k);
    m9 = 1; react_f = 1;
    memset(curse, 0, sizeof curse); memset(ap9, 0, sizeof ap9); memset(cer_own, 0, sizeof cer_own);
    cer_n = 0; own9 = 0; sac_v = -1; land_v = -1; revolt_v = -1; list_n = 0;
    sac_calls = land_calls = revolt_calls = ap_calls = search_calls = 0;
    unit[x][7] = 0x7777;
}

#define OPEN09(x) TaLogic_IsOpenUnit__00139180((x), 7)
static void test_139180(int x)
{
    int o = x == 0 ? 1 : 0;
    int e;
    /* status bit set: 0 before anything else */
    reset09(x, 0); status_v = 1;
    assert(OPEN09(x) == 0);
    /* kind classes outside 0, 1, 5; class 0 with a card other than 613 */
    reset09(x, 2);
    assert(OPEN09(x) == 2);
    reset09(x, 0);
    assert(OPEN09(x) == 2);
    /* class 0, card 613: slots 1..11 of both sides with a curse rest above 0 controlled by x */
    reset09(x, 0); card[x][7] = 613;
    curse[x][1] = 1; ctl[x][1] = x;
    curse[x][2] = 1; ctl[x][2] = o;       /* not controlled by x */
    curse[o][3] = -1; ctl[o][3] = x;      /* rest not above zero */
    curse[x][11] = 0; ctl[x][11] = x;     /* no rest */
    assert(OPEN09(x) == 2);               /* one: below the bound */
    curse[o][11] = 5; ctl[o][11] = x;     /* last slot, other side */
    assert(OPEN09(x) == 1);               /* two */
    /* class 5 */
    reset09(x, 5); react_v = 5;
    assert(OPEN09(x) == 2 && react_calls == 1 && sac_calls == 0);
    reset09(x, 5); sac_v = -1;            /* no sacrifice list */
    assert(OPEN09(x) == 2 && sac_calls == 1 && field_calls == 0);
    reset09(x, 5); sac_v = CERLIST; cer_n = 0; field_v = 8;   /* empty list passes */
    assert(OPEN09(x) == 1 && field_calls == 1 && ap_calls == 0);
    reset09(x, 5); sac_v = CERLIST; cer_n = 0; field_v = 9;
    assert(OPEN09(x) == 2 && field_calls == 1);
    reset09(x, 5); sac_v = CERLIST; cer_n = 2; field_v = 8;
    cer_own[0] = 0x0002; cer_own[1] = 0x0103; loc[0][2] = 0x0405; loc[1][3] = 0x0201;
    ap9[0][2] = 2599; ap9[1][3] = 2600;   /* second unit reaches the bound */
    assert(OPEN09(x) == 2 && ap_calls == 2 && field_calls == 0);
    ap9[1][3] = 2599; ap_calls = 0;
    assert(OPEN09(x) == 1 && ap_calls == 2 && field_calls == 1);
    /* class 1 */
    reset09(x, 1); react_v = 5;
    assert(OPEN09(x) == 2 && react_calls == 1);
    reset09(x, 1); eff_p = NULL;
    assert(OPEN09(x) == 2);
    reset09(x, 1); eff_word = 99 << 8;
    assert(OPEN09(x) == 2);
    reset09(x, 1); eff_word = (13 << 8) | 0xff;
    assert(OPEN09(x) == 1);
    /* effect 2: the land-change answer must be the unit's own location, row below 4 */
    reset09(x, 1); eff_word = 2 << 8; land_v = -1;
    assert(OPEN09(x) == 2 && land_calls == 1);
    reset09(x, 1); eff_word = 2 << 8; land_v = 0x0313;
    assert(OPEN09(x) == 2);
    reset09(x, 1); eff_word = 2 << 8; land_v = 0x0312;
    assert(OPEN09(x) == 1);
    reset09(x, 1); eff_word = 2 << 8; land_v = 0x0412; loc[x][7] = 0x0412; land_c = 4;   /* row 4 */
    assert(OPEN09(x) == 2 && land_calls == 1);
    land_c = 3;
    /* effect 23: two or more in the first search */
    reset09(x, 1); eff_word = 23 << 8; list_n = 2;
    assert(OPEN09(x) == 1 && search_calls == 1);
    /* otherwise the first unit of the second search: not controlled by x, kind 0 */
    reset09(x, 1); eff_word = 23 << 8; list_n = 1; own9 = (o << 8) | 3; ctl[o][3] = x;
    assert(OPEN09(x) == 2 && search_calls == 2);
    ctl[o][3] = o;
    assert(OPEN09(x) == 1);
    kind[o][3] = 1 << 5;                  /* class not 0 */
    assert(OPEN09(x) == 2);
    kind[o][3] = 4;                       /* low kind not 0 */
    assert(OPEN09(x) == 2);
    /* effect 31: card 733 only, a revolt object that is neither negative nor 0xffff low */
    reset09(x, 1); eff_word = 31 << 8; revolt_v = 0x0102;
    assert(OPEN09(x) == 2 && revolt_calls == 0);
    reset09(x, 1); eff_word = 31 << 8; card[x][7] = 733;
    assert(OPEN09(x) == 2 && revolt_calls == 1);          /* -1 */
    /* the card id is also the sacrifice key in class 5 only; 733 is not looked up here */
    revolt_v = 0x1ffff;
    assert(OPEN09(x) == 2);
    revolt_v = 0x0102;
    assert(OPEN09(x) == 1);
    /* effect 32: as card 613 but any non-zero curse rest counts */
    for (e = 0; e < 2; e++) {
        reset09(x, 1); eff_word = 32 << 8;
        curse[x][1] = e ? -1 : 1; ctl[x][1] = x;
        curse[o][2] = 1; ctl[o][2] = o;   /* not controlled by x */
        assert(OPEN09(x) == 2);
        curse[o][11] = -3; ctl[o][11] = x;
        assert(OPEN09(x) == 1);
    }
    m9 = 0; react_f = 0;
}

int main(void)
{
    test_139180(0);
    test_139180(1);
    test_134b40(0);
    test_134b40(1);
    test_13d3d0(0);
    test_13d3d0(1);
    test_126380(0);
    test_126380(1);
    test_12b210(0);
    test_12b210(1);
    test_1670a0(0);
    test_1670a0(1);
    test_158360(0, 0);
    test_158360(0, 1);
    test_158360(1, 0);
    test_158360(1, 1);
    return 0;
}
