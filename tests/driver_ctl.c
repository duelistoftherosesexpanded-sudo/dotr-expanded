/* Synthetic host tests for the driver_ctl group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/driver_ctl.h"

static char unit_tag[2][16];
static unsigned unit_gone[2];
static int ctl[2][16], danger[2][16], cat[2][16], kind_now;

struct jp_9a533 *SzDuel_GetUnit(int a, int b)
{
    assert((a == 0 || a == 1) && b >= 0 && b <= 10);
    return unit_gone[a] >> b & 1 ? NULL : (struct jp_9a533 *)&unit_tag[a][b];
}
int Ta_IsUnitControl2(struct jp_9a533 *vInfo)
{
    int n = (char *)vInfo - &unit_tag[0][0];

    return ctl[n / 16][n % 16];
}
int TaLogic_IsJudgeUnitDanger(int a, int b, int skip) { assert(skip == 9); return danger[a][b]; }
int TaLogic_IsUnitCategory(int nSide, int nID, int sKind) { assert(sKind == kind_now); return cat[nSide][nID]; }

static unsigned short out[24];

/* Every unit exists, is controlled by `side`, is not in danger and has category value `c`. */
static void reset(int side, int c)
{
    int a, i;

    unit_gone[0] = unit_gone[1] = 0;
    for (a = 0; a < 2; a++) {
        for (i = 0; i < 16; i++) {
            ctl[a][i] = side, danger[a][i] = 0, cat[a][i] = c;
        }
    }
}

/* Runs one of the two functions and returns how many entries it wrote. */
static int run(int two, int side, int kind, int cond)
{
    int n;

    memset(out, 0xff, sizeof(out));
    kind_now = kind;
    if (two) {
        TaLogic_GetUnitCtlLst2(side, out, kind, cond, 9);
    } else {
        TaLogic_GetUnitCtlLst(side, out, kind, 9);
    }
    for (n = 0; n < 24 && out[n] != 0xffff; n++) {
    }
    return n;
}

/* Slot range per kind, both functions (branch coverage: each switch arm and each range bound).
 * The `22 entries` early return cannot be reached with these ranges (at most 11 per side and the
 * test comes before each unit), so the full case below ends through the loop bound instead. */
static void check_ranges(int two)
{
    int i;

    reset(0, 1);
    if (two) {
        for (i = 0; i < 16; i++) {
            danger[0][i] = danger[1][i] = 1;
        }
    }
    assert(run(two, 0, 0x22, 0) == 2 && out[0] == 0x000 && out[1] == 0x100);
    assert(run(two, 0, 0x1f, 0) == 22 && out[0] == 0x000 && out[10] == 0x00a && out[11] == 0x100 && out[21] == 0x10a);
    assert(out[22] == 0xffff);
    assert(run(two, 0, 0x34, 0) == 20 && out[0] == 0x001 && out[9] == 0x00a && out[10] == 0x101 && out[19] == 0x10a);
    assert(run(two, 0, 7, 0) == 20 && out[0] == 0x001 && out[19] == 0x10a);
    if (two) {
        assert(run(two, 0, 0x1d, 0) == 10 && out[0] == 0x001 && out[4] == 0x005 && out[5] == 0x101 && out[9] == 0x105);
    } else {
        assert(run(two, 0, 0x1d, 0) == 20);
    }
    reset(1, 1);                                  /* side 1: own entries 0x1NN, the other side's 0x0NN */
    if (two) {
        for (i = 0; i < 16; i++) {
            danger[0][i] = danger[1][i] = 1;
        }
    }
    assert(run(two, 1, 0x22, 0) == 2 && out[0] == 0x100 && out[1] == 0x000);
}

/* Unit filters shared by both functions, on either side's pass. */
static void check_units(int two)
{
    int a;

    for (a = 0; a < 2; a++) {
        reset(0, 1);
        danger[0][0] = danger[1][0] = two;        /* Lst2 lists category 1 only when in danger */
        assert(run(two, 0, 0x22, 0) == 2 && out[a] == a << 8);
        unit_gone[a] = 1;                         /* no unit in the slot */
        assert(run(two, 0, 0x22, 0) == 1 && out[0] == !a << 8);
        unit_gone[a] = 0, ctl[a][0] = 1;          /* controlled by the other side */
        assert(run(two, 0, 0x22, 0) == 1 && out[0] == !a << 8);
    }
}

/* TaLogic_GetUnitCtlLst: listed when in danger, or when the category call answers exactly 1. */
static void check_ctl_lst(void)
{
    int a;

    for (a = 0; a < 2; a++) {
        reset(0, 2);
        assert(run(0, 0, 0x22, 0) == 0);          /* no danger, category 2 */
        danger[a][0] = 1;
        assert(run(0, 0, 0x22, 0) == 1 && out[0] == a << 8);
        danger[a][0] = 0, cat[a][0] = 1;
        assert(run(0, 0, 0x22, 0) == 1 && out[0] == a << 8);
        cat[a][0] = 0;
        assert(run(0, 0, 0x22, 0) == 0);
    }
}

/* TaLogic_GetUnitCtlLst2: in danger needs category 1; not in danger needs sCondView == 1. */
static void check_ctl_lst2(void)
{
    int a;

    for (a = 0; a < 2; a++) {
        reset(0, 2);
        assert(run(1, 0, 0x22, 0) == 0);          /* no danger, sCondView 0 */
        assert(run(1, 0, 0x22, 2) == 0);
        assert(run(1, 0, 0x22, 1) == 2);
        unit_gone[!a] = 1;
        assert(run(1, 0, 0x22, 1) == 1 && out[0] == a << 8);
        danger[a][0] = 1;                         /* danger, category 2: not listed whatever sCondView is */
        assert(run(1, 0, 0x22, 1) == 0);
        cat[a][0] = 1;
        assert(run(1, 0, 0x22, 0) == 1 && out[0] == a << 8);
    }
}

int main(void)
{
    check_ranges(0);
    check_ranges(1);
    check_units(0);
    check_units(1);
    check_ctl_lst();
    check_ctl_lst2();
    return 0;
}
