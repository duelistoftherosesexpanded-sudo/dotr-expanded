/* Synthetic host tests for the driver_root group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/driver_root.h"

static char unit_tag;
static struct jp_9a533 *const unit = (struct jp_9a533 *)&unit_tag;
static int field[8][8], unit_id[8][8], rev, virus, ctl_cell, ctl_unit, rev_calls;

unsigned int GetMoveRev(struct jp_9a533 *vInfo, int nX, int nZ)
{
    assert(vInfo == unit || vInfo == NULL);
    assert(nX >= 0 && nX < 8 && nZ >= 0 && nZ < 8);
    rev_calls++;
    return rev;
}
int SzDuel_GetField(int x, int y) { return field[x][y]; }
int Ta_IsVirus_Chk3(struct jp_9a533 *vInfo, int x, int y)
{
    assert(vInfo == unit && field[x][y] == 9);
    return virus;
}
void *SzDuel_GetMapUnit(int x, int y) { return unit_id[x][y] ? &unit_id[x][y] : NULL; }
int SzDuel_GetMapUnitID(int x, int y) { return unit_id[x][y]; }
int Ta_IsUnitControl(int a, int b)
{
    assert(a == 0x01 && (b == 0x05 || b == 0x00));
    return ctl_cell;
}
int Ta_IsUnitControl2(struct jp_9a533 *vInfo)
{
    assert(vInfo == unit);
    return ctl_unit;
}

static int danger, win2, win3, win2_calls, win3_calls;
int TaLogic_IsJudgeUnitDanger(int a, int b, int skip)
{
    assert(a == 0x01 && b == 0x05 && skip == 7);
    return danger;
}
int TaLogic_IsUnitWin2(struct jp_9a533 *item, int position, int c, int d)
{
    assert(item == unit && position == unit_id[c][d]);
    win2_calls++;
    return win2;
}
int TaLogic_IsUnitWin3(struct jp_9a533 *item, int base, int x, int y)
{
    assert(item == unit && base == 0xFFFE && unit_id[x][y] != 0);
    win3_calls++;
    return win3;
}

static unsigned short own_id = 0x0003;

static void reset(void)
{
    memset(field, 0, sizeof(field));
    memset(unit_id, 0, sizeof(unit_id));
    rev = 0, virus = 0, ctl_cell = 0, ctl_unit = 0;
}

/* Runs either function; `run` selects TaLogic_IsRunPassRoot2 with fFoot = run - 1. */
static int pass(int run, int nX, int nZ, int gX, int gZ)
{
    if (run) {
        return TaLogic_IsRunPassRoot2(unit, nX, nZ, gX, gZ, &own_id, run - 1);
    }
    return TaLogic_IsPassRoot2(unit, nX, nZ, gX, gZ, &own_id);
}

/* Branch coverage, the part both functions share: one assertion per outcome of the null test,
 * each distance class, each blocking test of an adjacent goal and each arm of the two-cell
 * switch. Its `default` arm cannot be reached: with a distance of 2 the X difference is 0, 1 or 2. */
static void check_shared(int run)
{
    reset();
    rev_calls = 0;
    if (run) {
        assert(TaLogic_IsRunPassRoot2(NULL, 3, 3, 3, 4, &own_id, 1) == 0);
    } else {
        assert(TaLogic_IsPassRoot2(NULL, 3, 3, 3, 4, &own_id) == 0);
    }
    assert(rev_calls == 1); /* GetMoveRev is called before the null test */
    assert(pass(run, 3, 3, 3, 3) == 1); /* same cell */
    assert(pass(run, 3, 3, 3, 4) == 1); /* adjacent, nothing in the way */
    assert(pass(run, 3, 3, 2, 3) == 1);
    assert(pass(run, 3, 3, 6, 3) == 0); /* three cells away */
    assert(pass(run, 3, 3, 4, 5) == 0);

    field[3][4] = 8; /* field 8 blocks unless the move flags have bit 0x10 */
    assert(pass(run, 3, 3, 3, 4) == 0);
    rev = 0x10;
    assert(pass(run, 3, 3, 3, 4) == 1);
    rev = 0xef;
    assert(pass(run, 3, 3, 3, 4) == 0);
    rev = 0;
    field[3][4] = 9; /* field 9 blocks only when Ta_IsVirus_Chk3 says 1 */
    assert(pass(run, 3, 3, 3, 4) == 1);
    virus = 1;
    assert(pass(run, 3, 3, 3, 4) == 0);
    virus = 2;
    assert(pass(run, 3, 3, 3, 4) == 1);

    reset();
    unit_id[3][4] = 0x0003; /* the goal holds the unit named by gUnitID */
    assert(pass(run, 3, 3, 3, 4) == 1);

    /* two cells in a line along Z: both halves must pass */
    reset();
    assert(pass(run, 3, 3, 3, 5) == 1);
    assert(pass(run, 3, 5, 3, 3) == 1);
    field[3][4] = 8;
    assert(pass(run, 3, 3, 3, 5) == 0);
    reset();
    field[3][5] = 8;
    assert(pass(run, 3, 3, 3, 5) == 0);

    /* two cells in a line along X */
    reset();
    assert(pass(run, 3, 3, 5, 3) == 1);
    assert(pass(run, 5, 3, 3, 3) == 1);
    field[4][3] = 8;
    assert(pass(run, 3, 3, 5, 3) == 0);
    assert(pass(run, 5, 3, 3, 3) == 0);
    reset();
    field[5][3] = 8;
    assert(pass(run, 3, 3, 5, 3) == 0);

    /* diagonal: through (gX, nZ) first, then through (nX, gZ) */
    reset();
    assert(pass(run, 3, 3, 4, 4) == 1);
    assert(pass(run, 3, 3, 2, 2) == 1);
    field[4][3] = 8;
    assert(pass(run, 3, 3, 4, 4) == 1);
    field[3][4] = 8;
    assert(pass(run, 3, 3, 4, 4) == 0);
    field[4][3] = 0;
    assert(pass(run, 3, 3, 4, 4) == 1);
    reset();
    field[4][4] = 8; /* both corners pass, the goal does not */
    assert(pass(run, 3, 3, 4, 4) == 0);
}

/* Branch coverage, the map-unit test that differs: another unit on the adjacent goal. */
static void check_other_unit(void)
{
    reset();
    unit_id[3][4] = 0x0105;
    assert(pass(0, 3, 3, 3, 4) == 0); /* TaLogic_IsPassRoot2: any other unit blocks */
    assert(pass(1, 3, 3, 3, 4) == 0); /* fFoot == 0 blocks */
    ctl_cell = 1, ctl_unit = 1;
    assert(pass(1, 3, 3, 3, 4) == 0);
    assert(pass(2, 3, 3, 3, 4) == 1); /* fFoot != 0: passes when both are controlled alike */
    ctl_unit = 0;
    assert(pass(2, 3, 3, 3, 4) == 0);
    ctl_cell = 0;
    assert(pass(2, 3, 3, 3, 4) == 1);

    /* The two-cell arms of TaLogic_IsRunPassRoot2 call TaLogic_IsPassRoot2, without fFoot. */
    reset();
    unit_id[3][4] = 0x0105;
    ctl_cell = 1, ctl_unit = 1;
    assert(pass(2, 3, 3, 3, 5) == 0);
}

static int invade(int nX, int nZ, int gX, int gZ, int *id, int node0)
{
    return TaLogic_IsPassInvadeRoot2(unit, nX, nZ, gX, gZ, id, 7, node0, 0xFFFE);
}

/* Branch coverage for TaLogic_IsPassInvadeRoot2: the null test, each distance class, each blocking
 * test of an adjacent goal, each outcome of the unit-on-goal tests and each arm of the two-cell
 * switch (its `default` arm cannot be reached, as above). */
static void check_invade(void)
{
    int id = 0x0105;

    reset();
    assert(TaLogic_IsPassInvadeRoot2(NULL, 3, 3, 3, 4, &id, 7, 0, 0xFFFE) == 0);
    assert(invade(3, 3, 3, 3, &id, 0) == 1);
    assert(invade(3, 3, 3, 4, &id, 0) == 1);
    assert(invade(3, 3, 6, 3, &id, 0) == 0); /* three cells away */
    field[3][4] = 8;
    assert(invade(3, 3, 3, 4, &id, 0) == 0);
    rev = 0x10;
    assert(invade(3, 3, 3, 4, &id, 0) == 1);
    rev = 0;
    field[3][4] = 9, virus = 1;
    assert(invade(3, 3, 3, 4, &id, 0) == 0);
    virus = 0;
    assert(invade(3, 3, 3, 4, &id, 0) == 1);

    /* a unit on the adjacent goal */
    reset();
    unit_id[3][4] = 0x0105;
    assert(invade(3, 3, 3, 4, &id, 0) == 1); /* it is *gUnitID */
    id = 0x0003;
    ctl_cell = 1, ctl_unit = 0, danger = 1, win2 = 1, win2_calls = win3_calls = 0;
    assert(invade(3, 3, 3, 4, NULL, 0) == 1); /* null gUnitID: no identity test */
    assert(invade(3, 3, 3, 4, &id, 0) == 1);
    win2 = 0;
    assert(invade(3, 3, 3, 4, &id, 0) == 2);
    assert(win2_calls == 3 && win3_calls == 0);
    danger = 0, win3 = 1;
    assert(invade(3, 3, 3, 4, &id, 0) == 1);
    win3 = 0;
    assert(invade(3, 3, 3, 4, &id, 0) == 2);
    assert(win2_calls == 3 && win3_calls == 2);
    ctl_unit = 1; /* controlled alike: decided by the low byte of the unit ID and Node0 */
    assert(invade(3, 3, 3, 4, &id, 0) == 1);
    assert(invade(3, 3, 3, 4, &id, 2) == 1);
    assert(invade(3, 3, 3, 4, &id, 1) == 0);
    unit_id[3][4] = 0x0100;
    assert(invade(3, 3, 3, 4, &id, 0) == 0);

    /* two cells along Z, along X, and the diagonal through either corner; all arguments recur */
    reset();
    assert(invade(3, 3, 3, 5, &id, 0) == 1);
    field[3][4] = 8;
    assert(invade(3, 3, 3, 5, &id, 0) == 0);
    reset();
    field[3][5] = 8;
    assert(invade(3, 3, 3, 5, &id, 0) == 0);
    reset();
    assert(invade(5, 3, 3, 3, &id, 0) == 1);
    field[4][3] = 8;
    assert(invade(5, 3, 3, 3, &id, 0) == 0);
    reset();
    field[3][3] = 8;
    assert(invade(5, 3, 3, 3, &id, 0) == 0);
    reset();
    assert(invade(3, 3, 4, 4, &id, 0) == 1);
    field[4][3] = 8;
    assert(invade(3, 3, 4, 4, &id, 0) == 1);
    field[3][4] = 8;
    assert(invade(3, 3, 4, 4, &id, 0) == 0);
    field[4][3] = 0;
    assert(invade(3, 3, 4, 4, &id, 0) == 1);
    reset();
    field[4][4] = 8;
    assert(invade(3, 3, 4, 4, &id, 0) == 0);
    /* a recursive call that answers 2 (a unit it cannot beat) does not count as passable */
    reset();
    unit_id[3][4] = 0x0105;
    ctl_cell = 1, ctl_unit = 0, danger = 0, win3 = 0;
    assert(invade(3, 3, 3, 5, &id, 0) == 0);
    win3 = 1;
    assert(invade(3, 3, 3, 5, &id, 0) == 1);
}

int main(void)
{
    check_invade();
    check_shared(0);
    check_shared(1);
    check_shared(2);
    check_other_unit();
    return 0;
}
