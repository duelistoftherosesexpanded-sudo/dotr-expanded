/* Synthetic host tests for the driver_near group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/driver_near.h"

static char unit_tag[2][16];
static unsigned unit_gone;
static int loc[16], status[2][16], cat[16], win4[16], win2[16], trap_act[16];
static int danger_limit[16], danger_n[16];

void *SzDuel_GetUnit(int a, int b) { return a == 0 && (unit_gone >> (b & 15) & 1) ? NULL : &unit_tag[a & 1][b & 15]; }
int SzDuel_GetUnitLoc(int a, int b) { assert(a == 0); return loc[b & 15]; }
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 0 && (bit == 2 || bit == 5));
    return status[bit == 5][b & 15];
}
int TaLogic_IsStatusMatch(int value, int mode) { return value == mode; }
int TaLogic_IsUnitCategory(int nSide, int nID, int sKind) { assert(nSide == 0); return cat[nID & 15] == sKind; }
int TaLogic_IsUnitWin4(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z, unsigned char fView)
{
    assert(pVarInfo == (struct jp_9a533 *)0x1234 && bWarnLvl == 6 && fView == 9 && wUnit < 16);
    assert(x == (loc[wUnit] & 0xff) && z == loc[wUnit] >> 8);
    return win4[wUnit];
}
/* Answers 1 for the first danger_limit[b] calls of a run, then 0. */
int TaLogic_IsJudgeUnitDanger(int a, int b, int skip)
{
    assert(a == 0 && skip == 9);
    return danger_n[b & 15]++ < danger_limit[b & 15];
}
int TaTrapA_IsTrapAct(int x, int y, int a, int b, int c, int d)
{
    assert(x == 1 && y == 9 && a == 0 && c == (loc[b & 15] & 0xff) && d == loc[b & 15] >> 8);
    return trap_act[b & 15];
}
int TaLogic_IsUnitWin2(void *item, int position, int c, int d)
{
    assert(item == &unit_tag[1][9] && position < 16);
    assert(c == (loc[position] & 0xff) && d == loc[position] >> 8);
    return win2[position];
}

static struct jp_9b50e lst[2];

static void reset(void)
{
    int i;

    unit_gone = 0;
    for (i = 0; i < 16; i++) {
        status[0][i] = 1, status[1][i] = 2, cat[i] = 7, win4[i] = 0, win2[i] = 0, trap_act[i] = 1;
        danger_limit[i] = 100;
    }
    loc[1] = 0x205, loc[2] = 0x104;       /* both one cell from (4, 2) */
    lst[0] = (struct jp_9b50e){ 0x0001, 3 }, lst[1] = (struct jp_9b50e){ 0x0002, 1 };
}

static int l3(int chase, int n)
{
    return TaLogic_GetNearUnitL3((struct jp_9a533 *)0x1234, 4, 2, lst, chase, 7, 1, 2, 9, 6, n);
}

static int trap(int chase, int n, int fTrap)
{
    memset(danger_n, 0, sizeof(danger_n));
    return TaLogic_GetTrapNearCard(1, 9, 4, 2, lst, chase, 7, 1, 2, 9, 6, n, fTrap);
}

/* Filters shared by both functions; branch coverage: one assertion per outcome and bound. */
static void check_filters(int (*run)(int chase, int n))
{
    reset();
    assert(run(5, 0) == -1);                      /* empty list */
    assert(run(5, 1) == 1);                       /* list distance 3 < 7 */
    unit_gone = 1u << 1;
    assert(run(5, 1) == -1);                      /* unit does not exist */
    unit_gone = 0, loc[1] = 0x207;                /* three cells away */
    assert(run(2, 1) == -1);                      /* cell distance above bChase */
    lst[0].bDist = 2;
    assert(run(2, 1) == -1);                      /* still refused by the cell distance */
    lst[0].bDist = 3;
    assert(run(3, 1) == 1);                       /* cell and list distance equal to bChase */
    loc[1] = 0x205, lst[0].bDist = 6;
    assert(run(5, 1) == -1);                      /* list distance above bChase */
    lst[0].bDist = 0;
    assert(run(5, 1) == -1);                      /* list distance 0 is refused */
    lst[0].bDist = 3, status[0][1] = 0;
    assert(run(5, 1) == -1);                      /* status 2 does not match fOpen */
    status[0][1] = 1, status[1][1] = 0;
    assert(run(5, 1) == -1);                      /* status 5 does not match fDef */
    status[1][1] = 2, cat[1] = 6;
    assert(run(5, 1) == -1);                      /* category mismatch */
    cat[1] = 7;
    assert(run(5, 1) == 1);                       /* wUnitNum bounds the scan */
    /* unit 1 at x 6, z 4 is four cells away: minimum 4, chase bound 3; unit 2 at x 3, z 0 is three */
    loc[1] = 0x406, loc[2] = 0x003, lst[1].bDist = 2;
    assert(run(5, 2) == 2);                       /* 2 < 4 */
    lst[1].bDist = 4;
    assert(run(5, 2) == 1);                       /* 4 > new chase bound 3 */
    reset();
    lst[1].bDist = 2;
    assert(run(5, 2) == 1);                       /* between the minimum 1 and the chase bound 3 */
}

static int l3_run(int chase, int n) { return l3(chase, n); }
static int trap_run(int chase, int n) { return trap(chase, n, 0); }

/* Tie of list distance and minimum: unit 2 is the candidate (v1), unit 1 the holder (v2). */
static void check_near_unit_l3(void)
{
    check_filters(l3_run);
    reset();
    assert(l3(5, 2) == 1);                        /* v1 0, v2 0 */
    win4[2] = 5, win4[1] = 3;
    assert(l3(5, 2) == 2);                        /* both positive, v1 > v2 */
    win4[1] = 5;
    assert(l3(5, 2) == 1);                        /* both positive, equal */
    win4[2] = 3;
    assert(l3(5, 2) == 1);                        /* both positive, v1 < v2 */
    win4[1] = 0;
    assert(l3(5, 2) == 2);                        /* v1 positive, v2 0 */
    win4[1] = -1, win4[2] = 1;
    assert(l3(5, 2) == 2);                        /* v1 positive, v2 negative */
    win4[2] = 0, win4[1] = 5;
    assert(l3(5, 2) == 1);                        /* v1 0, v2 positive */
}

static void check_trap_near_card(void)
{
    check_filters(trap_run);
    reset();
    danger_limit[1] = 0;
    assert(trap(5, 1, 0) == -1);                  /* not in danger */
    assert(trap(5, 1, 1) == -1);                  /* fTrap alone is not enough */
    danger_limit[1] = 100, trap_act[1] = 0;
    assert(trap(5, 1, 0) == -1);                  /* in danger, trap does not act, fTrap 0 */
    assert(trap(5, 1, 1) == 1);                   /* in danger and fTrap 1 */
    assert(trap(5, 1, 2) == -1);                  /* fTrap must be exactly 1 */
    reset();
    win2[2] = 3, win2[1] = 5;
    assert(trap(5, 2, 0) == 2);                   /* tie, v1 < v2 */
    win2[2] = 5;
    assert(trap(5, 2, 0) == 1);                   /* tie, equal */
    win2[2] = 6;
    assert(trap(5, 2, 0) == 1);                   /* tie, v1 > v2 */
    /* the danger answer turns 0 in the tie: that side's value is bWarnLvl (6) */
    danger_limit[2] = 1, win2[1] = 7;
    assert(trap(5, 2, 0) == 2);                   /* v1 6 < v2 7 */
    win2[1] = 6;
    assert(trap(5, 2, 0) == 1);                   /* v1 6, v2 6 */
    danger_limit[2] = 100, danger_limit[1] = 1, win2[2] = 5;
    assert(trap(5, 2, 0) == 2);                   /* v1 5 < v2 6 */
    win2[2] = 6;
    assert(trap(5, 2, 0) == 1);                   /* v1 6, v2 6 */
}

int main(void)
{
    check_near_unit_l3();
    check_trap_near_card();
    return 0;
}
