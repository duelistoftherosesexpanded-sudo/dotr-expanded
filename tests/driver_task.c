/* Synthetic host tests for the driver_task group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/driver_task.h"

static struct jp_9add8 task;
static struct jp_9b50e unit_lst[2];
static unsigned short size_word, node_word, ignore_lst[2];
static int l3_ans[3], l3_calls, l2_calls, ap2_answer, ap2_calls, loc_answer, loc_calls;
static int win4_answer, win4_calls, win5_answer, win5_calls, set_calls, curse_answer, curse_calls;
static int spell_answer, spell_calls, cur_unit;

/* Fixtures for TaLogic_CommandEncUnit; enc_mode switches the shared stubs to them. */
static int enc_mode, enc_id, enc_has_unit, enc_ctl, enc_gunner, enc_kind, enc_len, enc_get_calls, enc_gunner_calls;
static int enc_kind_calls, enc_lst_calls, enc_ctl2_calls, enc_maxap_calls, enc_ignore_calls;
static int enc_curse_of[16], enc_utilap_of[16], enc_ap0_of[16], enc_ap1_of[16], enc_match_of[16];
static unsigned short enc_lst1[4], enc_lst2[4], enc_lst3[4], enc_near[2][22], enc_want[2][22];
static int enc_n1, enc_n2, enc_n3, enc_maxap[2];
static unsigned short *enc_ignore;

static void enc_fill(unsigned short *out, const unsigned short *from, int n)
{
    int i;

    for (i = 0; i < 22; i++) {
        assert(out[i] == 0xffff);
    }
    memcpy(out, from, n * sizeof *from);
    enc_len = n;
}
struct jp_9a533 *SzDuel_GetUnit(int a, int b)
{
    assert(a == 1 && b == 2);
    enc_get_calls++;
    return enc_has_unit ? (struct jp_9a533 *)0x1234 : NULL;
}
int SzDuel_GetUnitCardNo(int a, int b)
{
    assert(a == 1 && b == 2);
    return 0x10077;
}
int Ta_IsUnitControl(int a, int b)
{
    assert(a == 1 && b == 2);
    return enc_ctl;
}
int Ta_GetCurrentSide(int a, int b)
{
    assert(a == 1 && b == 2);
    return 1;
}
void TaLogic_InitIgnoreLst(unsigned short *wIgnoreLst)
{
    enc_ignore = wIgnoreLst;
    wIgnoreLst[0] = 0x4242;
    enc_ignore_calls++;
}
int TaEnc_GetEncID(int index)
{
    assert(index == 0x77 && enc_ignore_calls == 1);
    return enc_id;
}
int TaLogic_CommandGunnerUnit(struct jp_9add8 *pTask, unsigned char fView)
{
    assert(pTask == &task && fView == 9);
    enc_gunner_calls++;
    return enc_gunner;
}
int SzDuel_GetUnitKind(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    enc_kind_calls++;
    return enc_kind;
}
void TaUtil_GetUnitLst(int kind, int mode, unsigned short *out)
{
    assert(kind == enc_ctl && mode == 1);
    enc_fill(out, enc_lst1, enc_n1);
    enc_lst_calls++;
}
int TaLogic_GetUnitCtlLstLen(unsigned short *words)
{
    (void)words;
    return enc_len;
}
int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    assert((a == 1 || b == 0) && c == 6 && d == 5);
    return enc_utilap_of[b];
}
void TaLogic_GetUnitCtlLst2(int nSide, unsigned short *wUnitLst, int sKind, int sCondView, unsigned char fView)
{
    if (enc_ctl2_calls++ == 0) {
        assert(nSide == !enc_ctl && sKind == 29 && sCondView == 9 && fView == 9);
        enc_fill(wUnitLst, enc_lst2, enc_n2);
    } else {
        assert(nSide == enc_ctl && sKind == 52 && sCondView == 1 && fView == 1);
        enc_fill(wUnitLst, enc_lst3, enc_n3);
    }
}
int SzDuel_GetUnitAP(int a, int b, int flag)
{
    assert(a == 2 && (flag == 0 || flag == 1));
    return flag ? enc_ap1_of[b] : enc_ap0_of[b];
}
int Do_ChkEncMatch(unsigned a, int b, int c)
{
    assert(a == 4 && c == 0x10077);
    return enc_match_of[b];
}
int TaLogic_GetMaxApUnit2(unsigned short *words, int key)
{
    assert(key == 1 && enc_maxap_calls < 2 && l2_calls == enc_maxap_calls + 1);
    assert(memcmp(words, enc_want[enc_maxap_calls], sizeof enc_want[0]) == 0);
    return enc_maxap[enc_maxap_calls++];
}

struct PieceTreetag *TaLogic_GetNearUnitL2(struct PieceTreetag *pTree, struct jp_9a533 *pVarInfo, int nX, int nZ,
    int nSide, unsigned char bChase, int fView, int bWarnLvl, struct jp_9b50e *nUnitLst, unsigned short *wUnitNum,
    unsigned short *wSize, unsigned short *wNodeCount, unsigned short *wIgnoreLst)
{
    if (enc_mode) {
        int i;

        assert(pTree == (l2_calls ? (struct PieceTreetag *)(size_t)(0x3000 + l2_calls) : NULL));
        assert(pVarInfo == (struct jp_9a533 *)0x1234 && nX == 6 && nZ == 5 && nSide == 1 && bChase == l2_calls);
        assert(fView == 9 && bWarnLvl == 6 && wIgnoreLst == enc_ignore && wIgnoreLst[0] == 0x4242);
        assert(wUnitNum != wSize && wSize != wNodeCount && wUnitNum != wNodeCount);
        if (l2_calls == 0) {
            assert(*wUnitNum == 0 && *wSize == 0 && *wNodeCount == 0);
        }
        for (i = 0; i < 22; i++) {
            assert(l2_calls != 0 || (nUnitLst[i].nUnitID == 0xffff && nUnitLst[i].bDist == 0xff));
            nUnitLst[i].nUnitID = enc_near[l2_calls][i];
        }
        l2_calls++;
        return (struct PieceTreetag *)(size_t)(0x3000 + l2_calls);
    }
    assert(pTree == (struct PieceTreetag *)0x2222 && pVarInfo == (struct jp_9a533 *)0x1234 && nX == 3 && nZ == 4);
    assert(nSide == 1 && bChase == 7 && fView == 9 && bWarnLvl == 6 && nUnitLst == unit_lst);
    assert(*wUnitNum == 0x700 && wSize == &size_word && wNodeCount == &node_word && wIgnoreLst == ignore_lst);
    *wUnitNum = 0x705;
    l2_calls++;
    return pTree;
}
int TaLogic_GetNearUnitL3(struct jp_9a533 *pVarInfo, int nX, int nZ, struct jp_9b50e *nUnitLst, unsigned char bChase,
    int sKey, unsigned char fOpen, unsigned char fDef, unsigned char fView, int bWarnLvl, unsigned short wUnitNum)
{
    static const int keys[3] = { 30, 29, 52 };
    assert(l2_calls == 1 && l3_calls < 3);
    assert(pVarInfo == (struct jp_9a533 *)0x1234 && nX == 3 && nZ == 4 && nUnitLst == unit_lst && bChase == 8);
    assert(sKey == keys[l3_calls] && fOpen == (l3_calls < 2) && fDef == 2 && fView == 9 && bWarnLvl == 6);
    assert(wUnitNum == 0x705);
    return cur_unit = l3_ans[l3_calls++];
}
int TaUtil_GetUnitAP2(struct jp_9a533 *pVarInfo, int nX, int nZ)
{
    assert(pVarInfo == (struct jp_9a533 *)0x1234 && nX == 3 && nZ == 4);
    ap2_calls++;
    return ap2_answer;
}
int SzDuel_GetUnitLoc(int a, int b)
{
    if (enc_mode) {
        return 0x0506;
    }
    assert(a == cur_unit >> 8 && b == (cur_unit & 0xff));
    loc_calls++;
    return loc_answer;
}
int SzDuel_GetUnitCurseRest(int a, int b)
{
    if (enc_mode) {
        return enc_curse_of[b];
    }
    assert(a == cur_unit >> 8 && b == (cur_unit & 0xff));
    curse_calls++;
    return curse_answer;
}
int SzDuel_GetUnitSpellRest(int a, int b)
{
    assert(a == cur_unit >> 8 && b == (cur_unit & 0xff));
    spell_calls++;
    return spell_answer;
}
int TaLogic_IsUnitWin4(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z, unsigned char fView)
{
    assert(pVarInfo == (struct jp_9a533 *)0x1234 && wUnit == cur_unit && bWarnLvl == 6 && fView == 9);
    assert(x == (loc_answer & 0xff) && z == loc_answer >> 8);
    assert(task.dwGoal == (((unsigned)cur_unit << 16) | 0xbeef));
    win4_calls++;
    return win4_answer;
}
int TaLogic_IsUnitWin5(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z, unsigned char fView)
{
    assert(pVarInfo == (struct jp_9a533 *)0x1234 && wUnit == cur_unit && bWarnLvl == 6 && fView == 9);
    assert(x == (loc_answer & 0xff) && z == loc_answer >> 8);
    win5_calls++;
    return win5_answer;
}
void TaLogic_SetTaskAtkOrMove(struct jp_9add8 *pTask, int nEnemyID, unsigned char fView)
{
    assert(pTask == &task && nEnemyID == cur_unit && fView == 9);
    set_calls++;
}

static int run_near_unit(int u30, int u29, int u52, int ap2, int win4, int curse, int spell)
{
    memset(&task, 0, sizeof task);
    task.wPurpose = 0x111, task.dwGoal = 0x1234beef;
    l3_ans[0] = u30, l3_ans[1] = u29, l3_ans[2] = u52;
    l2_calls = l3_calls = ap2_calls = loc_calls = win4_calls = win5_calls = set_calls = 0;
    curse_calls = spell_calls = 0;
    ap2_answer = ap2, win4_answer = win4, curse_answer = curse, spell_answer = spell;
    loc_answer = 0x0506, win5_answer = 0x55;
    return TaLogic_GetNearUnit(&task, (struct PieceTreetag *)0x2222, (struct jp_9a533 *)0x1234, 3, 4, 1, 7, 6,
                               unit_lst, 0x700, &size_word, &node_word, 8, 9, 100, ignore_lst);
}

static void check_near_unit(void)
{
    int arm;

    /* Key 30 found and its attack value at the limit: purpose 0x2ff and the goal's high half. */
    assert(run_near_unit(0x0102, -1, -1, 100, 0, 0, 0) == 1);
    assert(l2_calls == 1 && l3_calls == 1 && ap2_calls == 1 && loc_calls == 0 && set_calls == 0);
    assert(task.wPurpose == 0x2ff && task.dwGoal == 0x0102beef);
    /* One above the limit: falls through to key 29 and 52; nothing found. */
    assert(run_near_unit(0x0102, -1, -1, 101, 0, 0, 0) == -1);
    assert(l3_calls == 3 && ap2_calls == 1 && loc_calls == 0 && task.wPurpose == 0x111 && task.dwGoal == 0x1234beef);
    /* Key 30 not found: the attack value is not asked. */
    assert(run_near_unit(-1, -1, -1, 0, 0, 0, 0) == -1);
    assert(l3_calls == 3 && ap2_calls == 0 && win4_calls == 0);
    /* arm 0: key 29 found (key 52 is not asked); arm 1: key 52 found. Both arms behave alike. */
    for (arm = 0; arm < 2; arm++) {
        int u29 = arm ? -1 : 0x0003, u52 = arm ? 0x0104 : -1;
        unsigned goal = arm ? 0x0104beefu : 0x0003beefu;

        assert(run_near_unit(-1, u29, u52, 0, 1, 0, 0) == 1);
        assert(l3_calls == 2 + arm && loc_calls == 1 && win4_calls == 1 && set_calls == 1 && curse_calls == 0);
        assert(task.dwGoal == goal && task.wPurpose == 0x111);
        assert(run_near_unit(-1, u29, u52, 0, 0, 2, 0) == -1);
        assert(win4_calls == 1 && set_calls == 0 && curse_calls == 1 && spell_calls == 0 && win5_calls == 0);
        assert(task.dwGoal == goal);
        assert(run_near_unit(-1, u29, u52, 0, 0, 1, 1) == -1);
        assert(curse_calls == 1 && spell_calls == 1 && win5_calls == 0 && l3_calls == 2 + arm);
        assert(run_near_unit(-1, u29, u52, 0, 0, 1, 0) == 0x55);
        assert(curse_calls == 1 && spell_calls == 1 && win5_calls == 1 && set_calls == 0);
    }
}

static int run_enc(int id, int range)
{
    task.wUnit = 0x0102, task.wPurpose = 0x111, task.dwGoal = 0x1234beef;
    enc_id = id, enc_len = 0;
    enc_get_calls = enc_gunner_calls = enc_kind_calls = enc_lst_calls = enc_ctl2_calls = 0;
    enc_maxap_calls = enc_ignore_calls = l2_calls = 0;
    return TaLogic_CommandEncUnit(&task, range, 9, 6);
}

static void check_enc(void)
{
    static const unsigned short none[22] = {
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
    };
    int i;

    memset(&task, 0, sizeof task);
    enc_mode = 1;
    /* No unit record: 0 and nothing else is asked. */
    enc_has_unit = 0;
    assert(run_enc(3, 2) == 0 && enc_get_calls == 1 && enc_ignore_calls == 0 && task.wPurpose == 0x111);
    enc_has_unit = 1;
    /* Kind 3: the gunner command refused gives purpose 0x5ff. */
    enc_gunner = 0;
    assert(run_enc(3, 2) == 1 && enc_gunner_calls == 1 && enc_kind_calls == 0 && task.wPurpose == 0x5ff);
    /* Accepted: the goal unit's kind decides; high bits or low five bits 14 give 0x5ff. */
    enc_gunner = 1, enc_kind = 0x05;
    assert(run_enc(3, 2) == 1 && enc_kind_calls == 1 && task.wPurpose == 0x111 && task.dwGoal == 0x1234beef);
    enc_kind = 0x0e;
    assert(run_enc(3, 2) == 1 && task.wPurpose == 0x5ff);
    enc_kind = 0x25;
    assert(run_enc(3, 2) == 1 && task.wPurpose == 0x5ff);
    assert(enc_lst_calls == 0 && l2_calls == 0);

    /* Kind 2, first list: curse rest 0xff and TaUtil_GetUnitAP >= 0; the last such entry wins. */
    enc_ctl = 1;
    enc_lst1[0] = 0x0101, enc_lst1[1] = 0x0102, enc_lst1[2] = 0x0103, enc_lst1[3] = 0x0104, enc_n1 = 4;
    enc_curse_of[1] = 0xff, enc_curse_of[2] = 0, enc_curse_of[3] = 0xff, enc_curse_of[4] = 0xff;
    enc_utilap_of[1] = 5, enc_utilap_of[2] = 5, enc_utilap_of[3] = 0, enc_utilap_of[4] = -1;
    assert(run_enc(2, 2) == 1 && enc_lst_calls == 1 && enc_ctl2_calls == 0);
    assert(task.wPurpose == 0x9ff && task.dwGoal == 0x0103beef);
    /* Nothing in the first list (a unit word of 0 does not count): the second list, kind 29, the
     * other side; attack value difference >= 0. */
    enc_lst1[0] = 0x0000, enc_n1 = 1, enc_curse_of[0] = 0xff, enc_utilap_of[0] = 1;
    enc_lst2[0] = 0x0205, enc_lst2[1] = 0x0206, enc_n2 = 2;
    enc_ap0_of[5] = 7, enc_ap1_of[5] = 7, enc_ap0_of[6] = 7, enc_ap1_of[6] = 8;
    enc_curse_of[5] = enc_curse_of[6] = -1;
    assert(run_enc(2, 2) == 1 && enc_ctl2_calls == 1 && task.wPurpose == 0x9ff && task.dwGoal == 0x0205beef);
    enc_ctl = 0;
    assert(run_enc(2, 2) == 1 && enc_ctl2_calls == 1 && task.dwGoal == 0x0205beef);
    /* Second list empty-handed: the third list, kind 52, own side; curse rest >= 0. */
    enc_ap1_of[5] = 8;
    enc_lst3[0] = 0x0307, enc_lst3[1] = 0x0308, enc_n3 = 2, enc_curse_of[7] = 0, enc_curse_of[8] = -1;
    assert(run_enc(2, 2) == 1 && enc_ctl2_calls == 2 && task.wPurpose == 0x9ff && task.dwGoal == 0x0307beef);
    /* Nothing anywhere, also with empty lists: purpose 0x5ff. */
    enc_curse_of[7] = -1;
    assert(run_enc(2, 2) == 1 && enc_ctl2_calls == 2 && task.wPurpose == 0x5ff && task.dwGoal == 0x1234beef);
    enc_n1 = enc_n2 = enc_n3 = 0;
    assert(run_enc(2, 2) == 1 && enc_ctl2_calls == 2 && task.wPurpose == 0x5ff && l2_calls == 0);

    /* Other kinds: per chase step the near list is filtered by Do_ChkEncMatch into a fresh list
     * and TaLogic_GetMaxApUnit2 picks; a pick >= 0 ends the search. */
    for (i = 0; i < 22; i++) {
        enc_near[0][i] = 0x0400 | (i & 3);
        enc_near[1][i] = 0x0404 | (i & 1);
    }
    enc_match_of[0] = 1, enc_match_of[1] = 0, enc_match_of[2] = 2, enc_match_of[3] = 0;
    enc_match_of[4] = 0, enc_match_of[5] = 1;
    memcpy(enc_want, none, sizeof none), memcpy(enc_want[1], none, sizeof none);
    for (i = 0; i < 6; i++) {
        enc_want[0][i] = 0x0400;
    }
    for (i = 0; i < 11; i++) {
        enc_want[1][i] = 0x0405;
    }
    enc_maxap[0] = -1, enc_maxap[1] = 0;
    assert(run_enc(0, 2) == 1 && l2_calls == 2 && enc_maxap_calls == 2);
    assert(task.wPurpose == 0x9ff && task.dwGoal == 0x0000beef && enc_gunner_calls == 0 && enc_lst_calls == 0);
    enc_maxap[0] = 0x0409;
    assert(run_enc(1, 2) == 1 && l2_calls == 1 && task.wPurpose == 0x9ff && task.dwGoal == 0x0409beef);
    /* The chase range bounds the steps; none left gives 0x5ff. */
    enc_maxap[0] = -1, enc_maxap[1] = -1;
    assert(run_enc(0, 2) == 1 && l2_calls == 2 && task.wPurpose == 0x5ff && task.dwGoal == 0x1234beef);
    assert(run_enc(0, 1) == 1 && l2_calls == 1 && task.wPurpose == 0x5ff);
    assert(run_enc(0, 0) == 1 && l2_calls == 0 && enc_get_calls == 2 && task.wPurpose == 0x5ff);
    enc_mode = 0;
}

int main(void)
{
    check_near_unit();
    check_enc();
    return 0;
}
