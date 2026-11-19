/* Synthetic host tests for the driver_scans group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/driver_scans.h"

static int unit_present, control_answer, list_len, rev_answer, pass_answer, win_answer, curse_answer, spell_answer,
    trap_answer;
static int own_loc, enemy_loc[4];
static unsigned short list_words[4];
static int lst_args[3], pass_calls, pass_args[6], win_args[4], trap_args[3], trap_calls, rev_args[2], rev_calls;
static unsigned short *lst_out;
static char unit_tag[2][16];

static unsigned unit_gone[2]; /* bit b set: SzDuel_GetUnit(a, b) is NULL */
void *SzDuel_GetUnit(int a, int b) { return unit_present && !(unit_gone[a & 1] >> (b & 15) & 1) ? &unit_tag[a & 1][b & 15] : NULL; }
/* With tables_on the three per-unit stubs answer from these tables instead of one constant. */
static int tables_on;
static signed char control_of[2][11], curse_of[2][11], spell_of[2][11];
int Ta_IsUnitControl(int a, int b) { if (tables_on) return control_of[a][b]; return control_answer; }
void TaLogic_GetUnitCtlLst(int nSide, unsigned short *wUnitLst, int sKind, unsigned char fView)
{
    int i;

    for (i = 0; i < 22; i++) {
        assert(wUnitLst[i] == 0xFFFF);
    }
    lst_args[0] = nSide, lst_args[1] = sKind, lst_args[2] = fView, lst_out = wUnitLst;
    memcpy(wUnitLst, list_words, sizeof(list_words));
}
int SzDuel_GetUnitLoc(int a, int b) { return a == 1 && b == 3 ? own_loc : enemy_loc[b & 3]; }
int TaLogic_GetUnitCtlLstLen(unsigned short *words) { assert(words == lst_out); return list_len; }
unsigned GetMoveRev(void *item, int x, int y)
{
    assert(item != NULL);
    rev_args[0] = x, rev_args[1] = y, rev_calls++;
    return rev_answer;
}
int TaLogic_IsPassRoot(int a, int b, int c, int d, int e, int f, unsigned short *g)
{
    pass_args[0] = a, pass_args[1] = b, pass_args[2] = c, pass_args[3] = d, pass_args[4] = e, pass_args[5] = f;
    /* the word starts as nSide << 8 | nID and keeps what an earlier call stored */
    assert(*g == (pass_calls ? 0x207 : 0x103));
    *g = 0x207;
    pass_calls++;
    return pass_answer;
}
int TaLogic_IsUnitWin(int packed, int b, int c, int d)
{
    win_args[0] = packed, win_args[1] = b, win_args[2] = c, win_args[3] = d;
    return win_answer;
}
int SzDuel_GetUnitCurseRest(int a, int b) { if (tables_on) return curse_of[a][b]; return curse_answer; }
int SzDuel_GetUnitSpellRest(int a, int b) { if (tables_on) return spell_of[a][b]; return spell_answer; }
int TaLogic_IsSafeTrap(int word, int cx, int cy)
{
    trap_args[0] = word, trap_args[1] = cx, trap_args[2] = cy, trap_calls++;
    return trap_answer;
}

/* Unit (1, 3) stands at column 4, row 2; variant 0 reads that from SzDuel_GetUnitLoc,
 * variant 1 passes the cell. */
static int run(int variant)
{
    pass_calls = trap_calls = rev_calls = 0;
    return variant ? TaLogic_IsAttackedNow2(1, 3, 4, 2, 0x81) : TaLogic_IsAttackedNow(1, 3, 0x81);
}

/* TaLogic_IsAttackedNow and TaLogic_IsAttackedNow2 (branch coverage). */
static void check_attacked(int variant)
{
    own_loc = 0x204;
    /* no such unit */
    unit_present = 0;
    lst_out = NULL;
    assert(run(variant) == 0 && lst_out == NULL);
    /* empty list: the list is asked for the other side when the unit is not controlled */
    unit_present = 1;
    control_answer = 0;
    list_len = 0;
    assert(run(variant) == 0 && lst_args[0] == 1 && lst_args[1] == 31 && lst_args[2] == 0x81 && rev_calls == 0);
    control_answer = 5;
    assert(run(variant) == 0 && lst_args[0] == 0);
    /* one enemy (0, 2) at column 6, row 5: distance 5; reach 4 is short, 5 is enough */
    list_len = 1;
    list_words[0] = 0x002;
    enemy_loc[2] = 0x506;
    rev_answer = 0x34;
    pass_answer = 1, win_answer = 0, curse_answer = spell_answer = trap_answer = 0;
    assert(run(variant) == 0 && rev_calls == 1 && rev_args[0] == 6 && rev_args[1] == 5 && pass_calls == 0);
    rev_answer = 0x35;
    assert(run(variant) == 1 && pass_calls == 1 && trap_calls == 1);
    assert(pass_args[0] == 0 && pass_args[1] == 2 && pass_args[2] == 6 && pass_args[3] == 5 && pass_args[4] == 4 && pass_args[5] == 2);
    assert(win_args[0] == 0x207 && win_args[1] == 0x002 && win_args[2] == 4 && win_args[3] == 2);
    assert(trap_args[0] == 0x103 && trap_args[1] == 4 && trap_args[2] == 2);
    /* the other side of each coordinate difference: enemy at column 1, row 0 (distance 5) */
    enemy_loc[2] = 0x001;
    rev_answer = 4;
    assert(run(variant) == 0 && pass_calls == 0);
    rev_answer = 5;
    assert(run(variant) == 1);
    /* each later test rejects */
    pass_answer = 0;
    assert(run(variant) == 0 && pass_calls == 1 && trap_calls == 0);
    pass_answer = 1, win_answer = 1;
    assert(run(variant) == 0 && trap_calls == 0);
    win_answer = -1, curse_answer = 1;
    assert(run(variant) == 0 && trap_calls == 0);
    curse_answer = 0, spell_answer = 1;
    assert(run(variant) == 0 && trap_calls == 0);
    spell_answer = 0, trap_answer = 1;
    assert(run(variant) == 0 && trap_calls == 1);
    /* a second entry is tried after a rejected first one */
    list_len = 2;
    list_words[1] = 0x001;
    enemy_loc[1] = 0x205;
    trap_answer = 0, pass_answer = 0;
    assert(run(variant) == 0 && pass_calls == 2 && rev_calls == 2 && pass_args[1] == 1 && pass_args[2] == 5);
    pass_answer = 1;
    assert(run(variant) == 1 && pass_calls == 1);
}

static int owner_answer, tuto_answer, tuto_arg, side_of[2][16], order_of[2][16], sort_calls, assign_n;
static unsigned assigned[2];
static unsigned short assign_log[8];
static struct jp_9b352 ctl;
int SzDuel_GetTurnOwner(void) { return owner_answer; }
int TaTuto_IsTuto(int value) { tuto_arg = value; return tuto_answer; }
int Ta_GetCurrentSide(int a, int b) { return side_of[a & 1][b & 15]; }
int SzDuel_GetUnitSummonOrder(int a, int b) { return order_of[a & 1][b & 15]; }
int TaLogic_IsUnitAssign(unsigned short value, struct jp_9b352 *pCtl)
{
    assert(pCtl == &ctl);
    return assigned[value >> 8] >> (value & 0xff) & 1;
}
int TaLogic_DoUnitAssign(unsigned short id, struct jp_9b352 *pCtl)
{
    assert(pCtl == &ctl && assign_n < 8);
    assign_log[assign_n++] = id;
    return 1;
}
void TaLogic_DoSortTaskList(struct jp_9b352 *pCtl) { assert(pCtl == &ctl); sort_calls++; }

/* TaLogic_DoAdjustLogicData: branch coverage. */
static void check_adjust(void)
{
    static const struct jp_9add8 kept = { 0x0005, 5, 3, 44, 2 };
    int i;

    assert(offsetof(struct jp_9b352, wPadQue) == 0x160 && offsetof(struct jp_9b352, wPhase) == 0x1a2);
    assert(sizeof(struct jp_9add8) == 0x10 && offsetof(struct jp_9add8, dwGoal) == 8);
    /* the tutorial side changes nothing */
    owner_answer = 1, tuto_answer = 1, tuto_arg = -1;
    assert(TaLogic_DoAdjustLogicData(&ctl) == 1 && tuto_arg == 1 && sort_calls == 0);
    /* no control block */
    tuto_answer = 0;
    assert(TaLogic_DoAdjustLogicData(NULL) == -1 && sort_calls == 0);
    /* turn owner 0: side 0 units are its own, of side 1 only units 7 and 8 */
    owner_answer = 0, unit_present = 1;
    for (i = 0; i < 16; i++) {
        side_of[0][i] = 0, side_of[1][i] = 1;
    }
    side_of[1][7] = side_of[1][8] = 0;
    ctl.bCount = 7, ctl.bActionUnit = 9, ctl.wPhase = 0x1111;
    for (i = 0; i < 32; i++) {
        ctl.wPadQue[i] = 0x1234;
    }
    for (i = 0; i < 22; i++) {
        ctl.nTask[i].wUnit = 0xffff, ctl.nTask[i].wOrder = 77, ctl.nTask[i].wPurpose = 3, ctl.nTask[i].dwGoal = 44;
        ctl.nTask[i].bCond = 2;
    }
    ctl.nTask[1].wUnit = 0x0002, unit_gone[0] = 1u << 2;      /* its unit no longer exists */
    ctl.nTask[2].wUnit = 0x0103;                               /* its unit belongs to the other side */
    ctl.nTask[3].wUnit = 0x0004, ctl.nTask[3].wOrder = 9;      /* order differs: asks (0, 1) */
    ctl.nTask[21] = kept;                                      /* order equal: asks (0, 0), zero */
    order_of[0][0] = 0, order_of[0][1] = 6, order_of[0][4] = 8;
    assigned[0] = 0x7ff & ~((1u << 1) | (1u << 2)), assigned[1] = 1u << 8;
    assert(TaLogic_DoAdjustLogicData(&ctl) == 1 && tuto_arg == 0 && sort_calls == 1);
    assert(ctl.bCount == 0 && ctl.bActionUnit == 9 && ctl.wPhase == 0x1111);
    for (i = 0; i < 32; i++) {
        assert(ctl.wPadQue[i] == 0xffff);
    }
    assert(ctl.nTask[0].wUnit == 0xffff && ctl.nTask[0].wOrder == 77 && ctl.nTask[0].dwGoal == 44 && ctl.nTask[0].bCond == 2);
    for (i = 1; i < 3; i++) {
        assert(ctl.nTask[i].wUnit == 0xffff && ctl.nTask[i].wOrder == 255 && ctl.nTask[i].dwGoal == 0xffffffffu);
        assert(ctl.nTask[i].wPurpose == 0xffff && ctl.nTask[i].bCond == 0);
    }
    assert(ctl.nTask[3].wUnit == 0x0004 && ctl.nTask[3].wOrder == 8 && ctl.nTask[3].dwGoal == 0xffffffffu);
    assert(ctl.nTask[3].wPurpose == 0xffff && ctl.nTask[3].bCond == 0);
    assert(memcmp(&ctl.nTask[21], &kept, sizeof(kept)) == 0);
    /* unassigned own units that exist are assigned: (0,1) and (1,7); (0,2) is gone, (1,8) is assigned */
    assert(assign_n == 2 && assign_log[0] == 0x0001 && assign_log[1] == 0x0107);
    unit_gone[0] = 0, unit_present = 0;
}

/* TaLogic_GetNearCard2 (0x276f10): the clock, the two search steps and the ignore list are stubs. */
static unsigned nc_clock[8];
static int nc_clock_n, nc_init_n, nc_l2_n, nc_l3_n;
static int nc_l3_ans[4];
static unsigned short *nc_ignore;
unsigned int SzGame_GetHSyncCount(void) { return nc_clock[nc_clock_n++]; }
void TaLogic_InitIgnoreLst(unsigned short *wIgnoreLst)
{
    nc_ignore = wIgnoreLst;
    nc_init_n++;
}
struct PieceTreetag *TaLogic_GetNearUnitL2(struct PieceTreetag *pTree, struct jp_9a533 *pVarInfo, int nX, int nZ,
    int nSide, unsigned char bChase, int fView, int bWarnLvl, struct jp_9b50e *nUnitLst, unsigned short *wUnitNum,
    unsigned short *wSize, unsigned short *wNodeCount, unsigned short *wIgnoreLst)
{
    assert(pTree == (struct PieceTreetag *)(nc_l2_n ? (size_t)(0x5000 + nc_l2_n - 1) : 0));
    assert(pVarInfo == (struct jp_9a533 *)0x1234 && nX == 3 && nZ == 4 && nSide == 1 && bChase == nc_l2_n + 1);
    assert(fView == 0x109 && bWarnLvl == 6 && wIgnoreLst == nc_ignore);
    assert(nUnitLst[0].nUnitID == 0xffff && nUnitLst[0].bDist == 255);
    assert(nUnitLst[21].nUnitID == 0xffff && nUnitLst[21].bDist == 255);
    assert(*wUnitNum == (nc_l2_n ? 0x700 + nc_l2_n - 1 : 0) && *wSize == 0 && *wNodeCount == 0);
    assert(wSize != wNodeCount && wSize != wUnitNum && wNodeCount != wUnitNum);
    *wUnitNum = (unsigned short)(0x700 + nc_l2_n);
    return (struct PieceTreetag *)(size_t)(0x5000 + nc_l2_n++);
}
int TaLogic_GetNearUnitL3(struct jp_9a533 *pVarInfo, int nX, int nZ, struct jp_9b50e *nUnitLst, unsigned char bChase,
    int sKey, unsigned char fOpen, unsigned char fDef, unsigned char fView, int bWarnLvl, unsigned short wUnitNum)
{
    assert(nc_l2_n == nc_l3_n + 1);
    assert(pVarInfo == (struct jp_9a533 *)0x1234 && nX == 3 && nZ == 4 && nUnitLst[0].nUnitID == 0xffff);
    assert(sKey == 0x7777 && fOpen == 0x22 && fDef == 0x33 && fView == 9 && bWarnLvl == 6);
    assert(wUnitNum == 0x700 + nc_l3_n);
    (void)bChase;
    return nc_l3_ans[nc_l3_n++];
}
static int nc_run(int chase)
{
    nc_clock_n = nc_init_n = nc_l2_n = nc_l3_n = 0;
    return TaLogic_GetNearCard2((struct jp_9a533 *)0x1234, 3, 4, 1, (unsigned char)chase, 0x7777, 0x122, 0x133, 0x109, 6);
}
static void check_near_card2(void)
{
    /* the clock is already past 12000: nothing else is called */
    nc_clock[0] = 12001;
    assert(nc_run(3) == -1 && nc_clock_n == 1 && nc_init_n == 0 && nc_l2_n == 0);
    /* 12000 exactly passes; a chase length of 0 runs no step */
    nc_clock[0] = 12000;
    assert(nc_run(0) == -1 && nc_clock_n == 1 && nc_init_n == 1 && nc_l2_n == 0);
    /* the second step finds a unit (0 counts as found) */
    nc_clock[0] = 0; nc_clock[1] = 12000;
    nc_l3_ans[0] = -1; nc_l3_ans[1] = 0;
    assert(nc_run(3) == 0 && nc_clock_n == 2 && nc_l2_n == 2 && nc_l3_n == 2);
    nc_l3_ans[1] = 0x0503;
    assert(nc_run(3) == 0x0503);
    /* the clock runs out after the first step */
    nc_clock[1] = 12001;
    assert(nc_run(3) == -1 && nc_clock_n == 2 && nc_l2_n == 1 && nc_l3_n == 1);
    /* every step up to the chase length fails */
    nc_clock[1] = 1; nc_clock[2] = 2; nc_clock[3] = 3;
    nc_l3_ans[1] = -1; nc_l3_ans[2] = -1;
    assert(nc_run(3) == -1 && nc_clock_n == 4 && nc_l2_n == 3 && nc_l3_n == 3 && nc_init_n == 1);
}

/* TaLogic_GetRoot2 (0x2792c0) and TaLogic_GetInvadeRoot2 (0x27a130): the tree builders, the
 * connection test and the short-root step are stubs; the clock and ignore-list stubs are shared
 * with TaLogic_GetNearCard2. */
static int rt_n, rt_short_n, rt_conn_n, rt_c[4];
static unsigned char rt_end[8];
static int rt_conn[8];
static struct PieceTreetag *rt_tree(struct PieceTreetag *pTree, struct PieceTreetag *pPrev, struct jp_9a533 *pVarInfo,
    int nX, int nZ, int gX, int gZ, unsigned char bDist, unsigned char *EndCond, unsigned char bDepth,
    unsigned short *wSize, unsigned short *wNodeCount, unsigned short *wIgnoreLst)
{
    assert(rt_n < 8 && pTree == (struct PieceTreetag *)(rt_n ? (size_t)(0x6000 + rt_n - 1) : 0) && pPrev == 0);
    assert(pVarInfo == (struct jp_9a533 *)0x1234 && nX == rt_c[0] && nZ == rt_c[1] && gX == rt_c[2] && gZ == rt_c[3]);
    assert(bDist == rt_n + 1 && *EndCond == (rt_n ? rt_end[rt_n - 1] : 0) && bDepth == 0);
    assert(*wSize == 0 && *wNodeCount == 0 && wSize != wNodeCount && wIgnoreLst == nc_ignore);
    *EndCond = rt_end[rt_n];
    return (struct PieceTreetag *)(size_t)(0x6000 + rt_n++);
}
struct PieceTreetag *TaLogic_GetRootTree2(struct PieceTreetag *pTree, struct PieceTreetag *pPrev,
    struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned char bDist, unsigned char *EndCond,
    unsigned short *gUnitID, unsigned char bDepth, unsigned short *wSize, unsigned short *wNodeCount,
    unsigned short *wIgnoreLst)
{
    assert(*gUnitID == 0x4321);
    return rt_tree(pTree, pPrev, pVarInfo, nX, nZ, gX, gZ, bDist, EndCond, bDepth, wSize, wNodeCount, wIgnoreLst);
}
struct PieceTreetag *TaLogic_GetInvadeRootTree2(struct PieceTreetag *pTree, struct PieceTreetag *pPrev,
    struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned char bDist, unsigned char *EndCond,
    int *gUnitID, int fView, unsigned short bWarnLvl, unsigned char bDepth, unsigned short *wSize,
    unsigned short *wNodeCount, unsigned short *wIgnoreLst)
{
    assert(*gUnitID == 0x54321 && fView == 0x10007 && bWarnLvl == 0x8009);
    return rt_tree(pTree, pPrev, pVarInfo, nX, nZ, gX, gZ, bDist, EndCond, bDepth, wSize, wNodeCount, wIgnoreLst);
}
int TaLogic_GetShortRoot(struct jp_9b2bd *pMovePiece, struct PieceTreetag *pTree, int gX, int gZ)
{
    assert(pMovePiece == (struct jp_9b2bd *)0x2468 && pTree == (struct PieceTreetag *)(size_t)(0x6000 + rt_n - 1));
    assert(gX == rt_c[2] && gZ == rt_c[3]);
    rt_short_n++;
    return 0;
}
int TaLogic_IsConnectRoot(struct PieceTreetag *pTree, unsigned char bDepth, unsigned char bDist)
{
    assert(pTree == (struct PieceTreetag *)(size_t)(0x6000 + rt_n - 1) && bDepth == 0 && bDist == rt_n);
    return rt_conn[rt_conn_n++];
}
static int rt_run(int inv, int have, int nX, int nZ, int gX, int gZ, int bAbort)
{
    struct jp_9a533 *pVarInfo = have ? (struct jp_9a533 *)0x1234 : 0;

    nc_clock_n = nc_init_n = rt_n = rt_short_n = rt_conn_n = rev_calls = 0;
    rt_c[0] = nX, rt_c[1] = nZ, rt_c[2] = gX, rt_c[3] = gZ;
    if (inv) {
        return TaLogic_GetInvadeRoot2(pVarInfo, (struct jp_9b2bd *)0x2468, nX, nZ, gX, gZ, 0x54321, 0x10007,
                                      (unsigned char)bAbort, 0x8009);
    }
    return TaLogic_GetRoot2(pVarInfo, (struct jp_9b2bd *)0x2468, nX, nZ, gX, gZ, 0x4321, (unsigned char)bAbort);
}
static void check_roots(int inv)
{
    int i;

    memset(rt_end, 0, sizeof rt_end);
    for (i = 0; i < 8; i++) {
        rt_conn[i] = 1;
        nc_clock[i] = 12000;
    }
    rev_answer = 1;
    /* no unit information: -1; GetRoot2 tests this before the clock, GetInvadeRoot2 after it */
    assert(rt_run(inv, 0, 1, 5, 4, 3, 9) == -1 && nc_clock_n == inv && nc_init_n == 0 && rev_calls == 0);
    /* the clock is past 12000 on entry (12000 itself passes in every other case here) */
    nc_clock[0] = 12001;
    assert(rt_run(inv, 1, 1, 5, 4, 3, 9) == -1 && nc_clock_n == 1 && nc_init_n == !inv && rev_calls == 0);
    nc_clock[0] = 12000;
    /* the unit cannot move from its cell: 0 */
    rev_answer = 0;
    assert(rt_run(inv, 1, 1, 5, 4, 3, 9) == 0 && rev_calls == 1 && rev_args[0] == 1 && rev_args[1] == 5 && rt_n == 0);
    rev_answer = 1;
    /* the goal is 3 + 2 cells away: one more than the limit fails, the limit itself passes;
     * both coordinate orders */
    assert(rt_run(inv, 1, 1, 5, 4, 3, 4) == -1 && rt_n == 0 && nc_init_n == !inv);
    assert(rt_run(inv, 1, 4, 3, 1, 5, 4) == -1 && rt_n == 0);
    rt_end[0] = 1;
    assert(rt_run(inv, 1, 1, 5, 4, 3, 5) == 1 && rt_n == 1 && rt_short_n == 1 && rt_conn_n == 0 && nc_init_n == 1);
    assert(rt_run(inv, 1, 4, 3, 1, 5, 5) == 1 && rt_n == 1 && rt_short_n == 1);
    /* found at distance 3: the distance is returned */
    rt_end[0] = 0, rt_end[2] = 1;
    assert(rt_run(inv, 1, 1, 5, 4, 3, 5) == 3 && rt_n == 3 && rt_short_n == 1 && rt_conn_n == 2 && nc_clock_n == 3);
    /* the tree does not connect at the first distance */
    rt_conn[0] = 0;
    assert(rt_run(inv, 1, 1, 5, 4, 3, 5) == -1 && rt_n == 1 && rt_short_n == 0 && nc_clock_n == 1);
    rt_conn[0] = 1;
    /* the clock runs out after the first distance */
    nc_clock[1] = 12001;
    assert(rt_run(inv, 1, 1, 5, 4, 3, 5) == -1 && rt_n == 1 && nc_clock_n == 2);
    nc_clock[1] = 12000;
    /* never found: stops after the limit of 5 distances */
    rt_end[2] = 0;
    assert(rt_run(inv, 1, 1, 5, 4, 3, 5) == -1 && rt_n == 5 && rt_conn_n == 5 && rt_short_n == 0);
    /* an end condition other than 1: GetRoot2 leaves the loop on any non-zero value,
     * GetInvadeRoot2 only on 1 */
    rt_end[0] = 2, rt_end[1] = 1;
    if (inv) {
        assert(rt_run(inv, 1, 1, 5, 4, 3, 5) == 2 && rt_n == 2 && rt_short_n == 1);
    } else {
        assert(rt_run(inv, 1, 1, 5, 4, 3, 5) == -1 && rt_n == 1 && rt_short_n == 0 && rt_conn_n == 1);
    }
}

/* TaLogic_GetFusionTree (0x278a90): the hand, the summon test, the fusion table, the node pool
 * and the combo-list helpers are stubs that keep a small working model, so the recursion runs. */
static struct TreeTag ft_pool[8];
static int ft_alloc, ft_cap, ft_summon[5], ft_bad_a, ft_bad_b, ft_use_calls;
static unsigned short ft_size, ft_count;
void TaHand_GetNextHand(int nSide, int *arNextHand)
{
    int i;

    assert(nSide == 1);
    for (i = 0; i < 5; i++) {
        arNextHand[i] = 10 + i;
    }
}
int TaHand_IsCardSummon2(int nSide, int nCardNo)
{
    assert(nSide == 1 && nCardNo >= 10 && nCardNo < 15);
    return ft_summon[nCardNo - 10];
}
int TaLogic_GetHandFusion(unsigned short a, unsigned short b)
{
    /* the bits above the low 16 must be dropped by the caller */
    return a == ft_bad_a && b == ft_bad_b ? 0x10000 + 999 : 0x10000 + a + b;
}
struct TreeTag *TaLogic_TreeAloc(unsigned short *wSize, unsigned short *wCount)
{
    assert(wSize == &ft_size && wCount == &ft_count);
    return ft_alloc < ft_cap ? &ft_pool[ft_alloc++] : 0;
}
void TaLogic_InitTree(struct TreeTag *pTree, unsigned short nRoot, unsigned char *pCombo, unsigned char nOrder)
{
    int i;

    memset(pTree, 0, sizeof *pTree);
    memset(pTree->nComboLink, 0xff, 5);
    pTree->nRoot = nRoot;
    for (i = 0; pCombo != 0 && i < 5 && pCombo[i] != 0xff; i++) {
        pTree->nComboLink[i] = pCombo[i];
    }
    assert(i < 5);
    pTree->nComboLink[i] = nOrder;
}
int TaLogic_GetArrayLength(unsigned char *bytes)
{
    int n = 0;

    while (n < 5 && bytes[n] != 0xff) {
        n++;
    }
    return 0x100 + n; /* only the low byte is compared */
}
int TaLogic_IsUseHandInCombo(unsigned char value, unsigned char *p)
{
    int i;

    ft_use_calls++;
    for (i = 0; i < 5; i++) {
        if (p[i] == value) {
            return 1;
        }
    }
    return 0;
}
static struct TreeTag *ft_run(int cap, int nOrder, int pRoot, unsigned char *pCombo)
{
    ft_alloc = ft_use_calls = 0;
    ft_cap = cap;
    memset(ft_pool, 0x55, sizeof ft_pool);
    return TaLogic_GetFusionTree(1, (struct TreeTag *)0x99, (unsigned char)nOrder, (unsigned short)pRoot, pCombo,
                                 &ft_size, &ft_count);
}
static void check_fusion_tree(void)
{
    static unsigned char one[5] = { 4, 0xff, 0xff, 0xff, 0xff };
    static unsigned char four[5] = { 0, 1, 2, 3, 0xff };
    struct TreeTag *p = ft_pool;

    ft_summon[0] = ft_summon[1] = 1;
    ft_summon[2] = ft_summon[3] = ft_summon[4] = 0;
    ft_bad_a = ft_bad_b = -1;
    /* root call (no combo list): no summon test or fusion for the root; hand cards 0 and 1 can
     * be summoned, so the tree is root -> {0 -> 1, 1 -> 0}, allocated depth first */
    assert(ft_run(8, 2, 500, 0) == p && ft_alloc == 5);
    assert(p[0].nRoot == 500 && p[0].nComboLink[0] == 2 && p[0].nComboLink[1] == 0xff);
    assert(p[0].pNextLink[0] == p + 1 && p[0].pNextLink[1] == p + 3 && p[0].pNextLink[2] == 0);
    assert(p[1].nRoot == 510 && p[1].nComboLink[1] == 0 && p[1].pNextLink[0] == p + 2 && p[1].pNextLink[1] == 0);
    assert(p[2].nRoot == 521 && p[2].nComboLink[2] == 1 && p[2].pNextLink[0] == 0);
    assert(p[3].nRoot == 511 && p[3].nComboLink[1] == 1 && p[3].pNextLink[0] == p + 4);
    assert(p[4].nRoot == 521 && p[4].nComboLink[2] == 0 && p[4].pNextLink[0] == 0);
    /* a failed branch does not take a link slot: 500 with card 10 has no fusion */
    ft_bad_a = 500, ft_bad_b = 10;
    assert(ft_run(8, 2, 500, 0) == p && ft_alloc == 3);
    assert(p[0].pNextLink[0] == p + 1 && p[0].pNextLink[1] == 0 && p[1].nRoot == 511 && p[1].pNextLink[0] == p + 2);
    ft_bad_a = ft_bad_b = -1;
    /* the pool runs out below the root: the missing branches are left out */
    assert(ft_run(2, 2, 500, 0) == p && ft_alloc == 2 && p[0].pNextLink[0] == p + 1 && p[0].pNextLink[1] == 0);
    assert(p[1].pNextLink[0] == 0);
    /* no node for the root itself */
    assert(ft_run(0, 2, 500, 0) == 0 && ft_use_calls == 0);
    /* a root number of 999 is never extended */
    assert(ft_run(8, 2, 999, 0) == p && ft_alloc == 1 && ft_use_calls == 5 && p[0].pNextLink[0] == 0);
    /* with a combo list: the card cannot be summoned, has no fusion, or no node is left */
    assert(ft_run(8, 2, 500, one) == 0 && ft_alloc == 0);
    ft_bad_a = 500, ft_bad_b = 10;
    assert(ft_run(8, 0, 500, one) == 0 && ft_alloc == 0);
    ft_bad_a = ft_bad_b = -1;
    assert(ft_run(0, 0, 500, one) == 0 && ft_use_calls == 0);
    /* a list that reaches five entries is returned without looking for more */
    ft_summon[4] = 1;
    assert(ft_run(8, 4, 500, four) == p && ft_alloc == 1 && ft_use_calls == 0);
    assert(p[0].nRoot == 514 && p[0].nComboLink[4] == 4);
}

/* TaLogic_DoGetShortRoot: branch coverage. */
static struct PieceTreetag *gnp_p;
static int gnp_n, gnp_ret;
int TaLogic_GetNextPiece(struct PieceTreetag *p, unsigned char depth)
{
    assert(depth == 0);
    gnp_p = p;
    gnp_n++;
    return gnp_ret;
}

static void check_short_root(void)
{
    struct PieceTreetag root, kid, miss;
    struct jp_9b2bd move[2], first[2];
    int count = 7;
    unsigned char dist = 9;
    int i;

    memset(&root, 0, sizeof(root));
    memset(&kid, 0, sizeof(kid));
    memset(&miss, 0, sizeof(miss));
    memset(move, 0, sizeof(move));
    memset(first, 0, sizeof(first));
    root.bPiece = 0x53;
    gnp_n = 0;
    gnp_ret = 0x1a4;

    /* no node: nothing happens */
    TaLogic_DoGetShortRoot(move, &count, &dist, 0, 3, 5, first, 2);
    assert(gnp_n == 0 && count == 7 && dist == 9);

    /* goal reached nearer than the best distance: count reset, distance and first step stored */
    TaLogic_DoGetShortRoot(move, &count, &dist, &root, 3, 5, first, 2);
    assert(gnp_n == 1 && gnp_p == &root && count == 0 && dist == 2);
    assert(first[0].nX == 4 && first[0].nZ == 10 && first[1].nX == 15 && first[1].nZ == 15);
    assert(move[0].nX == 0 && move[0].nZ == 0 && move[1].nX == 0 && move[1].nZ == 0);

    /* same distance: counted, second move entry stored */
    gnp_ret = 0x27;
    memset(first, 0, sizeof(first));
    TaLogic_DoGetShortRoot(move, &count, &dist, &root, 3, 5, first, 2);
    assert(gnp_n == 2 && count == 1 && dist == 2);
    assert(move[1].nX == 7 && move[1].nZ == 2 && move[0].nX == 0 && move[0].nZ == 0);
    assert(first[0].nX == 0 && first[0].nZ == 0 && first[1].nX == 0 && first[1].nZ == 0);

    /* farther: untouched */
    memset(move, 0, sizeof(move));
    TaLogic_DoGetShortRoot(move, &count, &dist, &root, 3, 5, first, 3);
    assert(gnp_n == 2 && count == 1 && dist == 2 && move[1].nX == 0 && move[1].nZ == 0);

    /* column differs / row differs: all twelve children are searched one level deeper */
    kid.bPiece = 0x21;
    miss.bPiece = 0x11;
    for (i = 0; i < 12; i++) {
        root.pNextP[i] = i == 11 ? &kid : i == 4 ? &miss : 0;
    }
    kid.pNextP[0] = &root;
    dist = 9;
    TaLogic_DoGetShortRoot(move, &count, &dist, &root, 1, 2, first, 0);
    assert(gnp_n == 3 && gnp_p == &kid && count == 0 && dist == 1 && first[0].nX == 7 && first[0].nZ == 2);
    root.bPiece = 0x31;
    miss.bPiece = 0x22;
    dist = 1;
    TaLogic_DoGetShortRoot(move, &count, &dist, &root, 1, 2, first, 0);
    assert(gnp_n == 4 && gnp_p == &kid && count == 1 && dist == 1 && move[1].nX == 7 && move[1].nZ == 2);
}

static int fu_status[2][16], fu_cat[16];
static struct jp_9aeb0 fu_info[32];
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 0 && (bit == 2 || bit == 5));
    return fu_status[bit == 5][b & 15];
}
int TaLogic_IsStatusMatch(int value, int mode) { return value == mode; }
int TaLogic_IsUnitCategory(int nSide, int nID, int sKind) { assert(nSide == 0); return fu_cat[nID & 15] == sKind; }
int SzDuel_GetUnitCardNo(int a, int b) { return 100 + (a & 1) * 16 + (b & 15); }
struct jp_9aeb0 *SzDuel_GetCardConstInfo(int id) { assert(id >= 100 && id < 132); return &fu_info[id - 100]; }

/* Branch coverage: one assertion per filter outcome, per bound of bDist and per tie outcome. */
static void check_follow_near_unit(void)
{
    struct jp_9b50e lst[2] = { { 0x0001, 3 }, { 0x0002, 1 } };
    int i;

    unit_present = 1, unit_gone[0] = 0;
    for (i = 0; i < 16; i++) {
        fu_status[0][i] = 1, fu_status[1][i] = 2, fu_cat[i] = 7;
    }
    memset(fu_info, 0, sizeof(fu_info));
    enemy_loc[1] = 0x205, enemy_loc[2] = 0x001;
#define FOLLOW(n) TaLogic_GetFollowNearUnit(4, 2, lst, 5, 7, 1, 2, (n))
    assert(FOLLOW(0) == -1);                                  /* empty list */
    assert(FOLLOW(1) == 1);                                   /* bDist 3 < 7 */
    unit_gone[0] = 1u << 1;
    assert(FOLLOW(1) == -1);                                  /* unit does not exist */
    unit_gone[0] = 0, fu_status[0][1] = 0;
    assert(FOLLOW(1) == -1);                                  /* status 2 does not match fOpen */
    fu_status[0][1] = 1, fu_status[1][1] = 0;
    assert(FOLLOW(1) == -1);                                  /* status 5 does not match fDef */
    fu_status[1][1] = 2, fu_cat[1] = 6;
    assert(FOLLOW(1) == -1);                                  /* category mismatch */
    fu_cat[1] = 7, lst[0].bDist = 6;
    assert(FOLLOW(1) == -1);                                  /* beyond bChase */
    lst[0].bDist = 5;
    assert(FOLLOW(1) == 1);                                   /* bDist == bChase */
    lst[0].bDist = 0;
    assert(FOLLOW(1) == -1);                                  /* bDist 0 is refused */
    /* Unit 1 at x 5, z 2 is one cell from (4, 2): the minimum becomes 1 and the chase bound 3. */
    lst[0].bDist = 3;
    assert(FOLLOW(2) == 1);                                   /* tie, equal deck cost: first stays */
    fu_info[2].bDeckCost = 9, fu_info[1].bDeckCost = 8;
    assert(FOLLOW(2) == 2);                                   /* tie, dearer card wins */
    fu_info[1].bDeckCost = 10;
    assert(FOLLOW(2) == 1);                                   /* tie, cheaper card loses */
    assert(FOLLOW(1) == 1);                                   /* wUnitNum bounds the scan */
    lst[1].bDist = 2;
    assert(FOLLOW(2) == 1);                                   /* between minimum and chase: ignored */
    /* Unit 1 at x 6, z 4 (both above) is four cells away; unit 2 at x 1, z 0 (both below). */
    enemy_loc[1] = 0x406;
    assert(FOLLOW(2) == 2);                                   /* 2 < 4: nearer by list distance */
    lst[1].bDist = 4;
    assert(FOLLOW(2) == 1);                                   /* 4 > new chase bound 3 */
    lst[0].bDist = 4, lst[1].bDist = 5, fu_info[2].bDeckCost = 11;
    assert(FOLLOW(2) == 1);                                   /* 5 > chase bound 4 */
    lst[0] = (struct jp_9b50e){ 0x0002, 5 }, lst[1] = (struct jp_9b50e){ 0x0001, 5 };
    assert(FOLLOW(2) == 2);                                   /* unit 2 first: minimum 3 + 2 = 5 */
    fu_info[1].bDeckCost = 12;
    assert(FOLLOW(2) == 1);                                   /* tie at 5, dearer card wins */
#undef FOLLOW
}

/* TaLogic007_IsActiveMode (branch coverage): each of the three "hindered" tests on both sides, the
 * bound 3/4 on hindered units and the bound 1/2 on units not under the turn owner's control. The last
 * test (fewer than four controlled units) cannot fail with a callee that answers consistently: the
 * controlled and uncontrolled counts add up to 22, so fewer than two uncontrolled means at least 21
 * controlled. It is not reached here. */
static void check_active_mode(int owner)
{
    int a, b;

    tables_on = 1;
    owner_answer = owner;
    memset(curse_of, 0, sizeof(curse_of));
    memset(spell_of, 0, sizeof(spell_of));
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 11; b++) {
            control_of[a][b] = owner;
        }
    }
    assert(TaLogic007_IsActiveMode() == 1);
    curse_of[owner][0] = 1, spell_of[owner][10] = 1, curse_of[!owner][3] = 1;
    assert(TaLogic007_IsActiveMode() == 1);             /* three hindered units */
    spell_of[!owner][10] = 1;
    assert(TaLogic007_IsActiveMode() == 0);             /* four */
    spell_of[!owner][10] = -1, curse_of[owner][0] = -1;
    assert(TaLogic007_IsActiveMode() == 1);             /* negative counts do not hinder */
    control_of[owner][10] = !owner;
    assert(TaLogic007_IsActiveMode() == 1);             /* one uncontrolled unit, three hindered */
    control_of[!owner][0] = !owner;
    assert(TaLogic007_IsActiveMode() == 0);             /* four hindered */
    memset(curse_of, 0, sizeof(curse_of));
    memset(spell_of, 0, sizeof(spell_of));
    assert(TaLogic007_IsActiveMode() == 0);             /* two hindered, two uncontrolled */
    control_of[owner][10] = owner;
    assert(TaLogic007_IsActiveMode() == 1);
    control_of[!owner][0] = owner, control_of[owner][5] = !owner;
    assert(TaLogic007_IsActiveMode() == 1);
    tables_on = 0;
}

int main(void)
{
    check_attacked(0);
    check_attacked(1);
    check_adjust();
    check_near_card2();
    check_roots(0);
    check_roots(1);
    check_fusion_tree();
    check_short_root();
    check_follow_near_unit();
    check_active_mode(0);
    check_active_mode(1);
    return 0;
}
