/* Synthetic host tests for the perm_efc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/perm_efc.h"

enum { CTL2 = 1, UDP, UAP, UNIT, USTAT, EFFECT, CTL, AGREE, FIELD, LAND, LDOFS };
static int log_id[512], log_a[512], log_b[512], log_c[512], log_n;
static int ctl2_answer, present[2][11], status_off[2][11], has_effect[2][11], agree_answer, land_answer, ld_answer;
static struct jp_65648 effect[2][11];
static struct jp_64b6d unit, subject;
static struct jp_6537d constant;
static void *agree_v, *land_v, *ld_v;
static int agree_args[3];

static void note(int id, int a, int b, int c)
{
    assert(log_n < 512);
    log_id[log_n] = id, log_a[log_n] = a, log_b[log_n] = b, log_c[log_n] = c;
    log_n++;
}
static int is(int at, int id, int a, int b, int c)
{
    return log_id[at] == id && log_a[at] == a && log_b[at] == b && log_c[at] == c;
}
static int count(int id)
{
    int k, n = 0;

    for (k = 0; k < log_n; k++) {
        n += log_id[k] == id;
    }
    return n;
}
int Ta_IsUnitControl2(struct jp_64b6d *vInfo) { assert(vInfo == &subject); note(CTL2, 0, 0, 0); return ctl2_answer; }
int SzDuel_GetUnitDP(int a, int b, int flag) { note(UDP, a, b, flag); return 3000 + a * 100; }
int SzDuel_GetUnitAP(int a, int b, int flag) { note(UAP, a, b, flag); return 2000 + a * 100; }
struct jp_64b6d *SzDuel_GetUnit(int a, int b) { note(UNIT, a, b, 0); return present[a][b] ? &unit : NULL; }
int SzDuel_GetUnitStatus(int a, int b, int bit) { note(USTAT, a, b, bit); return status_off[a][b] != bit; }
struct jp_65648 *Ta_GetUnitEffect(int a, int b, int n) { note(EFFECT, a, b, n); return has_effect[a][b] ? &effect[a][b] : NULL; }
int Ta_IsUnitControl(int a, int b) { note(CTL, a, b, 0); return 50 + a * 10 + b; }
int Ta_GetUnitAgreeWithVarInfo(struct jp_64b6d *vInfo, int side, int a, int b)
{
    agree_v = vInfo, agree_args[0] = side, agree_args[1] = a, agree_args[2] = b;
    note(AGREE, side, a, b);
    return agree_answer;
}
int SzDuel_GetField(int x, int y) { note(FIELD, x, y, 0); return x * 16 + y; }
int CheckLandAgreement(int land, struct jp_64b6d *vInfo) { land_v = vInfo; note(LAND, land, 0, 0); return land_answer; }
int Ta_GetLDParamOfs2(struct jp_64b6d *vInfo, int x, int y) { ld_v = vInfo; note(LDOFS, x, y, 0); return ld_answer; }

static int run(int fAP)
{
    log_n = 0;
    return Ta_GetParamOfs(&subject, 3, 4, fAP);
}
static void clear(void)
{
    memset(present, 0, sizeof(present));
    memset(status_off, 0, sizeof(status_off));
    memset(has_effect, 0, sizeof(has_effect));
    memset(effect, 0, sizeof(effect));
    memset(&subject, 0, sizeof(subject));
    memset(&constant, 0, sizeof(constant));
    subject.pConstInfo = &constant;
    ctl2_answer = 1, agree_answer = 1, land_answer = 0, ld_answer = 7;
}
/* One unit at (side, pos) with an effect of the given kind and amount 100 in the low half. */
static int one(int side, int pos, int kind, int fAP)
{
    clear();
    present[side][pos] = 1, has_effect[side][pos] = 1;
    effect[side][pos].dwID = (kind << 8) | (2 << 6) | 5;
    effect[side][pos].dwParam = 0x10064;
    return run(fAP);
}
/* Ta_GetParamOfs (branch coverage). */
static void check_param_ofs(void)
{
    int k, side;

    /* no record */
    log_n = 0;
    assert(Ta_GetParamOfs(NULL, 3, 4, 0) == 0 && log_n == 0);
    /* no units: five positions on the controlling side, then five on the other, then land and range */
    clear();
    assert(run(0) == 7 && log_n == 14 && is(0, CTL2, 0, 0, 0));
    for (k = 1; k <= 5; k++) {
        assert(is(k, UNIT, 1, k, 0) && is(5 + k, UNIT, 0, k, 0));
    }
    assert(is(11, FIELD, 3, 4, 0) && is(12, LAND, 52, 0, 0) && land_v == &subject && is(13, LDOFS, 3, 4, 0) && ld_v == &subject);
    ctl2_answer = 0;
    assert(run(1) == 7 && log_n == 14 && is(1, UNIT, 0, 1, 0) && is(5, UNIT, 0, 5, 0) && is(6, UNIT, 1, 1, 0) && is(10, UNIT, 1, 5, 0));
    /* land answers 1, 2 and another */
    land_answer = 1;
    assert(run(0) == 507);
    land_answer = 2;
    assert(run(0) == -493);
    land_answer = 3;
    assert(run(0) == 7);
    /* both sides: kinds 19, 18, 34 (defence only) and another; first and last position */
    for (side = 0; side < 2; side++) {
        assert(one(side, 1, 19, 0) == -93 && log_n == 20 && agree_v == &subject);
        assert(agree_args[0] == 50 + side * 10 + 1 && agree_args[1] == 2 && agree_args[2] == 5);
        assert(count(USTAT) == 3 && count(EFFECT) == 1 && count(CTL) == 1 && count(AGREE) == 1);
        k = side == 1 ? 2 : 7;
        assert(is(k, USTAT, side, 1, 4) && is(k + 1, USTAT, side, 1, 3) && is(k + 2, USTAT, side, 1, 5) && is(k + 3, EFFECT, side, 1, 2));
        assert(is(k + 4, CTL, side, 1, 0));
        assert(one(side, 5, 19, 1) == -93);
        assert(one(side, 5, 18, 0) == 107 && one(side, 2, 18, 1) == 107);
        assert(one(side, 3, 34, 0) == 107 && one(side, 3, 34, 1) == 7);
        assert(one(side, 4, 20, 0) == 7);
        /* each status missing; no effect; no agreement */
        one(side, 2, 18, 0), status_off[side][2] = 4;
        assert(run(0) == 7 && count(USTAT) == 1 && count(EFFECT) == 0);
        status_off[side][2] = 3;
        assert(run(0) == 7 && count(USTAT) == 2 && count(EFFECT) == 0);
        status_off[side][2] = 5;
        assert(run(0) == 7 && count(USTAT) == 3 && count(EFFECT) == 0);
        status_off[side][2] = 0, has_effect[side][2] = 0;
        assert(run(0) == 7 && count(EFFECT) == 1 && count(CTL) == 0);
        has_effect[side][2] = 1, agree_answer = 0;
        assert(run(0) == 7 && count(AGREE) == 1);
    }
    /* units on both sides add up */
    one(1, 1, 18, 0);
    present[0][5] = 1, has_effect[0][5] = 1, effect[0][5].dwID = 19 << 8, effect[0][5].dwParam = 30;
    assert(run(0) == 77);
    /* card 675, defence: own total is taken off (capped at 9999), the leader's value added */
    clear();
    subject.wCardNo = 675, subject.sVarDP = 100, subject.sVarAP = 40, constant.wDP = 200, constant.wAP = 60;
    assert(run(0) == 7 - 300 + 3000 && is(1, UDP, 0, 0, 1) && count(UAP) == 0);
    assert(run(1) == 7 - 100 + 2000 && is(1, UAP, 0, 0, 1) && count(UDP) == 0);
    ctl2_answer = 0;
    assert(run(0) == 7 - 300 + 3100 && is(1, UDP, 1, 0, 1));
    assert(run(1) == 7 - 100 + 2100 && is(1, UAP, 1, 0, 1));
    subject.fDefense = -1;
    assert(run(0) == 7 - 300 + 3000 && is(1, UDP, 0, 0, 1));
    assert(run(1) == 7 - 100 + 2000 && is(1, UAP, 0, 0, 1));
    ctl2_answer = 1;
    assert(run(0) == 7 - 300 + 3100 && is(1, UDP, 1, 0, 1));
    assert(run(1) == 7 - 100 + 2100 && is(1, UAP, 1, 0, 1));
    subject.sVarDP = 9799, subject.sVarAP = 9939;
    assert(run(0) == 7 - 9999 + 3100 && run(1) == 7 - 9999 + 2100);
    subject.sVarDP = 9800, subject.sVarAP = 9940;
    assert(run(0) == 7 - 9999 + 3100 && run(1) == 7 - 9999 + 2100);
    subject.sVarDP = 9798, subject.sVarAP = 9938;
    assert(run(0) == 7 - 9998 + 3100 && run(1) == 7 - 9998 + 2100);
    /* another card number leaves the leader out */
    subject.wCardNo = 674;
    assert(run(0) == 7 && count(UDP) == 0 && count(UAP) == 0);
}

static int ldabl_answer, search_answer, range_answer;
int Ta_IsLDAbl(int a, int b) { assert(a == 4 && b == subject.bOwner); return ldabl_answer; }
int Ta_SearchUncTypeNaturalEffectUser(int kind, int n) { assert(kind == 67 && n == 0); return search_answer; }
int Ta_IsLDRangeAvilityFromLoc(int packed, int b, int c, int d)
{
    assert(packed == 0x403 && b == subject.bOwner && c == (subject.bVarKind & 0x1f) && d == 12);
    return range_answer;
}

/* An open, placed subject on land answer 3 (no change), no unit effects, no natural effect user. */
static void rev_setup(void)
{
    clear();
    subject.bID = 1, subject.bOwner = 1, subject.fOpen = -1, subject.bVarKind = 15;
    land_answer = 3, ldabl_answer = 0, search_answer = -1, range_answer = 0;
    log_n = 0;
}
static unsigned rev_slot(int open, int kind, unsigned param)
{
    rev_setup();
    subject.fOpen = open ? -1 : 0;
    subject.aVarEffIdx[1].dwID = (unsigned)kind << 8;
    subject.aVarEffIdx[1].dwParam = param;
    return GetMoveRev(&subject, 3, 4);
}
static unsigned rev_unit(int side, int pos, unsigned id, int kind_byte)
{
    rev_setup();
    land_answer = 1;
    subject.bVarKind = (unsigned char)kind_byte;
    has_effect[side][pos] = 1;
    effect[side][pos].dwID = id, effect[side][pos].dwParam = 0x70003;
    return GetMoveRev(&subject, 3, 4);
}
/* GetMoveRev (branch coverage). */
static void check_move_rev(void)
{
    int k;

    assert(GetMoveRev(NULL, 3, 4) == 0);
    /* not placed: the leader ability decides */
    rev_setup();
    subject.bID = 0, ldabl_answer = 1;
    assert(GetMoveRev(&subject, 3, 4) == 2 && log_n == 0);
    ldabl_answer = 0;
    assert(GetMoveRev(&subject, 3, 4) == 1 && log_n == 0);
    /* face down: 1, with 0x10 for slot-1 effect kinds 8..10 and 0 for kind 79 */
    assert(rev_slot(0, 7, 0) == 1 && log_n == 0);
    assert(rev_slot(0, 8, 0) == 0x11 && rev_slot(0, 9, 0) == 0x11 && rev_slot(0, 10, 0) == 0x11);
    assert(rev_slot(0, 11, 0) == 1 && rev_slot(0, 79, 0) == 0);
    /* face up: the land agreement of the cell (3, 4) gives the base */
    for (k = 0; k < 4; k++) {
        static const unsigned want[4] = { 1, 2, 1, 0 };

        rev_setup();
        land_answer = k;
        assert(GetMoveRev(&subject, 3, 4) == want[k]);
        assert(is(0, FIELD, 3, 4, 0) && is(1, LAND, 3 * 16 + 4, 0, 0) && land_v == &subject);
        assert(count(USTAT) == 44 && count(EFFECT) == 22);
    }
    /* slot-1 effect of the subject: kind 6 replaces, 8..10 add 0x10, 79 returns 0 */
    assert(rev_slot(1, 6, 0x90025) == 0x25 && rev_slot(1, 5, 0x25) == 0 && rev_slot(1, 7, 0x25) == 0);
    assert(rev_slot(1, 8, 0) == 0x10 && rev_slot(1, 9, 0) == 0x10 && rev_slot(1, 10, 0) == 0x10);
    assert(rev_slot(1, 11, 0) == 0);
    assert(rev_slot(1, 79, 0) == 0 && log_n == 2);
    /* standing kind-6 effects of both sides replace the low four bits */
    assert(rev_unit(0, 0, (6 << 8) | 14, 15) == 3 && rev_unit(1, 10, (6 << 8) | 14, 15) == 3);
    assert(rev_unit(0, 10, (6 << 8) | 14, 0) == 3 && rev_unit(1, 0, (5 << 8) | 14, 15) == 2);
    /* low id 15 applies only to a subject of kind 15 with no high bits */
    assert(rev_unit(0, 3, (6 << 8) | 15, 15) == 3 && rev_unit(1, 3, (6 << 8) | 15, 15) == 3);
    assert(rev_unit(0, 3, (6 << 8) | 15, 14) == 2 && rev_unit(1, 3, (6 << 8) | 15, 14) == 2);
    assert(rev_unit(0, 3, (6 << 8) | 15, 15 | 0x20) == 2 && rev_unit(1, 3, (6 << 8) | 15, 15 | 0x20) == 2);
    /* a unit without status 4 or 5 is not asked for its effect */
    for (k = 4; k < 6; k++) {
        rev_setup();
        land_answer = 1;
        has_effect[0][2] = has_effect[1][2] = 1;
        effect[0][2].dwID = effect[1][2].dwID = 6 << 8, effect[0][2].dwParam = effect[1][2].dwParam = 3;
        status_off[0][2] = status_off[1][2] = k;
        assert(GetMoveRev(&subject, 3, 4) == 2);
        assert(count(EFFECT) == 20 && count(USTAT) == (k == 4 ? 42 : 44));
    }
    /* the high part survives the unit, natural-effect and leader-range replacements */
    rev_setup();
    subject.aVarEffIdx[1].dwID = 8 << 8;
    search_answer = 0;
    assert(GetMoveRev(&subject, 3, 4) == 0x11);
    range_answer = 1;
    assert(GetMoveRev(&subject, 3, 4) == 0x12);
    range_answer = 2, search_answer = -1;
    assert(GetMoveRev(&subject, 3, 4) == 0x10);
    has_effect[1][5] = 1, effect[1][5].dwID = 6 << 8, effect[1][5].dwParam = 4;
    assert(GetMoveRev(&subject, 3, 4) == 0x14);
}

/* Stubs for Ta_ManageNaturalEffectInStandbyPhase; the log ids continue after LDOFS. */
enum { CURSIDE = 20, SEARCH, GDELAY, SDELAY, CONV, GCOUNT, INQ, CKIND, DELG, PACK, CONVOFF, LOC };
static int search_unit, delay_answer, grave_n[2], grave_kind[2][4], convoff_answer;
int Ta_GetCurrentSide(int a, int b) { note(CURSIDE, a, b, 0); return 6; }
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0 && b == 0 && f == 14);
    note(SEARCH, c, d, e);
    return search_unit;
}
int SzDuel_GetUnitDelayCount(int a, int b) { note(GDELAY, a, b, 0); return delay_answer; }
int SzDuel_SetUnitDelayCount(int a, int b, int value) { note(SDELAY, a, b, value); return 0; }
unsigned ConvUnitIDtoFlagList(unsigned word, int high, int position) { note(CONV, (int)word, high, position); return 0x800; }
int SzDuel_GetGraveCount(int index) { note(GCOUNT, index, 0, 0); return grave_n[index]; }
int SzDuel_InqGrave(int index, int position) { note(INQ, index, position, 0); return index * 100 + position; }
int TaHand_GetCardKind(int id) { note(CKIND, id, 0, 0); return grave_kind[id / 100][id % 100]; }
int SzDuel_DelGrave(int index, int position) { note(DELG, index, position, 0); return 0; }
void SzDuel_PackGrave(int index) { note(PACK, index, 0, 0); }
unsigned ConvUnitIDtoFlagList_OFF(unsigned x, int flag, int n) { note(CONVOFF, (int)x, flag, n); return convoff_answer; }
int SzDuel_GetUnitLoc(int a, int b) { note(LOC, a, b, 0); return 0x304; }

static struct jp_65da1 info;
static int standby(unsigned int id, unsigned int param)
{
    has_effect[1][5] = 1, effect[1][5].dwID = id, effect[1][5].dwParam = param;
    memset(&info, 0x55, sizeof(info));
    log_n = 0;
    return Ta_ManageNaturalEffectInStandbyPhase(1, 5, &info);
}
static int info_is(int eNO, unsigned int from, unsigned int to, unsigned int par)
{
    return info.eNO == eNO && info.dwFrom == from && info.dwTo == to && info.dwPar == par;
}

/* Ta_ManageNaturalEffectInStandbyPhase: branch coverage, one assertion per case of the effect
 * kind and per outcome of each case's test, with both grave loops at zero and non-zero counts. */
static void check_standby(void)
{
    clear();
    log_n = 0;
    assert(Ta_ManageNaturalEffectInStandbyPhase(1, 5, &info) == 0 && log_n == 1 && is(0, EFFECT, 1, 5, 2));
    assert(standby(99 << 8, 7) == 0 && log_n == 1 && info.eNO == 0x55555555);

    assert(standby(13 << 8, 0x12345678) == 1 && info_is(8, 0, 6, 0x12345678) && log_n == 2 && is(1, CURSIDE, 1, 5, 0));

    search_unit = 0;
    assert(standby((77 << 8) | (2 << 6) | 0x2A, 0x10009) == 0 && log_n == 3 && is(2, SEARCH, 6, 2, 0x2A) && info.eNO == 0x55555555);
    search_unit = 0x40;
    assert(standby((77 << 8) | (2 << 6) | 0x2A, 0x10009) == 1 && info_is(57, 0, 0x40, 9));
    search_unit = 0;
    assert(standby((78 << 8) | (3 << 6) | 0x15, 0x10009) == 0 && is(2, SEARCH, 6, 3, 0x15) && info.eNO == 0x55555555);
    search_unit = 0x40;
    assert(standby((78 << 8) | (3 << 6) | 0x15, 0x10009) == 1 && info_is(29, 1, 0x40, 9));

    delay_answer = 2;
    assert(standby(12 << 8, 0x10009) == 0 && log_n == 2 && is(1, GDELAY, 1, 5, 0));
    delay_answer = 1;
    assert(standby(12 << 8, 0x10009) == 1 && log_n == 4 && is(2, SDELAY, 1, 5, 0) && is(3, CONV, 0, 1, 5) && info_is(60, 0, 0x800, 9));
    delay_answer = 2;
    assert(standby(70 << 8, 0x10009) == 0 && log_n == 2 && is(1, GDELAY, 1, 5, 0));
    delay_answer = 1;
    assert(standby(70 << 8, 0x10009) == 1 && log_n == 3 && is(2, SDELAY, 1, 5, 0) && info_is(46, 0x105, 0, 1));

    /* 83: cards whose kind is below 32 leave both graves; dwPar is their number times the low half */
    grave_n[0] = grave_n[1] = 0;
    assert(standby(83 << 8, 0x10009) == 1 && info_is(29, 1, 0x800, 0) && log_n == 6 && is(1, CONV, 0, 1, 5));
    assert(is(2, GCOUNT, 0, 0, 0) && is(3, PACK, 0, 0, 0) && is(4, GCOUNT, 1, 0, 0) && is(5, PACK, 1, 0, 0));
    grave_n[0] = 3, grave_n[1] = 2;
    grave_kind[0][0] = 31, grave_kind[0][1] = 32, grave_kind[0][2] = 0, grave_kind[1][0] = 64, grave_kind[1][1] = 5;
    assert(standby(83 << 8, 0x10009) == 1 && info_is(29, 1, 0x800, 27) && count(DELG) == 3 && count(INQ) == 5 && count(CKIND) == 5);
    assert(is(5, DELG, 0, 0, 0) && is(10, DELG, 0, 2, 0) && is(11, PACK, 0, 0, 0) && is(17, DELG, 1, 1, 0) && is(18, PACK, 1, 0, 0) && log_n == 19);

    search_unit = 0x40, convoff_answer = 0;
    assert(standby((84 << 8) | (1 << 6) | 7, 0x10009) == 0 && log_n == 4 && is(2, SEARCH, 6, 1, 7) && is(3, CONVOFF, 0x40, 1, 5));
    convoff_answer = 0x30;
    assert(standby((84 << 8) | (1 << 6) | 7, 0x10009) == 1 && info_is(72, 0x304, 0x30, 6) && log_n == 6 && is(4, LOC, 1, 5, 0));
    assert(is(5, CURSIDE, 1, 5, 0));
    clear();
}

enum { NUM22 = 100, OWNER, REST };
static int num22_answer, owner_ids[4], rest_of[11];
int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    note(NUM22, (int)value, 0, 0);
    return value == (unsigned)search_unit ? num22_answer : value != 0;
}
int Ta_GetUnitOwnerIDFromLstCount(int word, int n) { note(OWNER, word, n, 0); return owner_ids[n]; }
int SzDuel_GetUnitSpellRest(int a, int b) { note(REST, a, b, 0); return rest_of[b]; }

static int standby2(unsigned int id, unsigned int param)
{
    has_effect[1][5] = 1, effect[1][5].dwID = id, effect[1][5].dwParam = param;
    memset(&info, 0x55, sizeof(info));
    log_n = 0;
    return Ta_ManageNaturalEffectInStandbyPhase2(1, 5, &info);
}

static void check_standby2(void)
{
    unsigned int id = (23 << 8) | (1 << 6) | 0x2B;

    clear();
    /* another effect kind: only the effect is read */
    assert(standby2(22 << 8, 7) == 0 && log_n == 1 && is(0, EFFECT, 1, 5, 2) && info.eNO == 0x55555555);
    /* no unit in range */
    search_unit = 0;
    assert(standby2(id, 0x10009) == 0 && log_n == 3 && is(1, CURSIDE, 1, 5, 0) && is(2, SEARCH, 6, 1, 0x2B));
    /* an empty list: the loop is not entered and the result list stays zero */
    search_unit = 0x40, num22_answer = 0;
    assert(standby2(id, 0x10009) == 0 && log_n == 5 && is(3, NUM22, 0x40, 0, 0) && is(4, NUM22, 0, 0, 0));
    /* two units, both with spell rest: nothing collected */
    num22_answer = 2, owner_ids[0] = 0x103, owner_ids[1] = 0x007, rest_of[3] = 1, rest_of[7] = 1;
    assert(standby2(id, 0x10009) == 0 && count(OWNER) == 2 && count(CONV) == 0 && info.eNO == 0x55555555);
    assert(is(4, OWNER, 0x40, 0, 0) && is(5, REST, 1, 3, 0) && is(6, OWNER, 0x40, 1, 0) && is(7, REST, 0, 7, 0));
    /* the second has none: it alone is collected, low half of the parameter kept */
    rest_of[7] = 0;
    assert(standby2(id, 0x10009) == 1 && count(CONV) == 1 && is(8, CONV, 0, 0, 7) && is(9, NUM22, 0x800, 0, 0));
    assert(info_is(0, 1, 0x800, 9));
    /* both: the second conversion continues from the first result */
    rest_of[3] = 0;
    assert(standby2(id, 0xFFFF1234) == 1 && count(CONV) == 2 && is(6, CONV, 0, 1, 3) && is(9, CONV, 0x800, 0, 7));
    assert(info_is(0, 1, 0x800, 0x1234));
}

int main(void)
{
    check_standby();
    check_standby2();
    check_param_ofs();
    check_move_rev();
    return 0;
}
