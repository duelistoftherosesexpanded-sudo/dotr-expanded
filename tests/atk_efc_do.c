/* Synthetic host tests for the atk_efc_do group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/atk_efc_do.h"

static struct jp_570b5 dat;
static int match_ret, match_n, match_args[6];
static int loc_ret, loc_n, loc_args[2];
static int field[7][7], set_n;
static int lp_n, lp_args[3];
static int rnd[2], rnd_n;

int Match_AtkEfcObj(int mode, int a, int b, int c, int d, int e)
{
    match_args[0] = mode; match_args[1] = a; match_args[2] = b;
    match_args[3] = c; match_args[4] = d; match_args[5] = e;
    match_n++;
    return match_ret;
}
int SzDuel_GetUnitLoc(int a, int b)
{
    assert(loc_n == 0 || (loc_args[0] == a && loc_args[1] == b));
    loc_args[0] = a; loc_args[1] = b;
    loc_n++;
    return loc_ret;
}
int SzDuel_GetField(int x, int y) { assert(x >= 0 && x < 7 && y >= 0 && y < 7); return field[x][y]; }
int SzDuel_SetField(int x, int y, int value)
{
    assert(field[x][y] != 8);
    field[x][y] = value;
    set_n++;
    return 0;
}
int Ta_GetLPRevByNaturalEffect(int a, int b) { return a * 1000 + b; }
int SzDuel_CalcLP(int index, int operation, int value)
{
    lp_args[0] = index; lp_args[1] = operation; lp_args[2] = value;
    lp_n++;
    return 0;
}
int func_0019a5c8(void) { assert(rnd_n < 2); return rnd[rnd_n++]; }

static void reset(void)
{
    memset(&dat, 0, sizeof(dat));
    memset(field, 0, sizeof(field));
    match_n = loc_n = set_n = lp_n = rnd_n = 0;
}
static int dat_clear(void)
{
    static const struct jp_570b5 zero;
    return memcmp(&dat, &zero, sizeof(dat)) == 0;
}
static unsigned run(unsigned id, unsigned param, int id_do, int id_done, unsigned side)
{
    return Do_AtkEfc(id, param, &dat, 0, id_do, 1, id_done, side);
}
static int match_was(int mode, int a, int id_do, int id_done)
{
    return match_n == 1 && match_args[0] == mode && match_args[1] == a && match_args[2] == 0 &&
           match_args[3] == id_do && match_args[4] == 1 && match_args[5] == id_done;
}

/* Do_AtkEfc: branch coverage. */
static void check_do(void)
{
    int i, j, n;

    /* unknown effect kind */
    reset();
    assert(run(0x900, 0x12345, 3, 4, 0) == 0 && dat_clear() && match_n + loc_n + lp_n + rnd_n == 0);

    /* 4: destroy flags after an object match */
    reset(); match_ret = 0;
    assert(run(0x495, 0, 3, 4, 1) == 0 && dat_clear() && match_was(2, 0x15, 3, 4));
    reset(); match_ret = 1;
    assert(run(0x495, 0, 3, 0, 1) == 0 && dat_clear() && match_was(2, 0x15, 3, 0));
    reset();
    assert(run(0x495, 0, 0, 4, 1) == 0 && dat_clear());
    reset(); dat.Do_APDP = 1;
    assert(run(0x495, 0, 3, 4, 1) == 0x10001 && !dat.Do_APDP && dat.fDestroy_1 && !dat.fDestroy_2);
    reset(); dat.Do_APDP = 1;
    assert(run(0x495, 0, 3, 4, 0) == 0x10001 && !dat.Do_APDP && !dat.fDestroy_1 && dat.fDestroy_2);

    /* 2: field cells within the range of the acting (side 0) or the other unit, except cells reading 8 */
    reset(); loc_ret = 0x003; field[2][0] = 8;
    assert(run(0x200, 0x10005, 3, 4, 0) == 0x10000 && loc_n == 2 && loc_args[0] == 0 && loc_args[1] == 3);
    for (n = 0, i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            int near = (i == 3 && j <= 1) || (i == 4 && j == 0);
            assert(field[i][j] == (i == 2 && j == 0 ? 8 : near ? 5 : 0));
            n += near;
        }
    }
    assert(n == 3 && set_n == 3 && dat_clear());
    reset(); loc_ret = 0x605;
    assert(run(0x200, 0x00009, 3, 4, 1) == 0x10000 && loc_n == 2 && loc_args[0] == 1 && loc_args[1] == 4);
    assert(set_n == 1 && field[5][6] == 9);

    /* 27: parameter offset of the acting side */
    reset(); match_ret = 0;
    assert(run(0x1b00, 0x7012c, 3, 4, 0) == 0 && dat_clear() && match_was(2, 7, 3, 4));
    reset(); match_ret = 1;
    assert(run(0x1b00, 0x7012c, 3, 0, 0) == 0 && dat_clear());
    assert(run(0x1b00, 0x7012c, 0, 4, 0) == 0 && dat_clear());
    dat.POfs1 = 1; dat.POfs2 = 2;
    assert(run(0x1b00, 0x7012c, 3, 4, 0) == 0x10000 && dat.POfs1 == 301 && dat.POfs2 == 2);
    assert(run(0x1b00, 0x7012c, 3, 4, 1) == 0x10000 && dat.POfs1 == 301 && dat.POfs2 == 302);

    /* 5: guard flags */
    reset();
    assert(run(0x500, 0, 3, 0, 0) == 0 && run(0x500, 0, 0, 4, 0) == 0 && dat_clear());
    assert(run(0x500, 0, 3, 4, 0) == 0x10000 && dat.fGuard1 && !dat.fGuard2);
    reset();
    assert(run(0x500, 0, 3, 4, 1) == 0x10000 && !dat.fGuard1 && dat.fGuard2 && match_n == 0);

    /* 13: life-point change, revised for operations 0 and 1 */
    reset();
    assert(run(0xd00, 0x00064, 3, 4, 0) == 0x10000 && lp_n == 1);
    assert(lp_args[0] == 0 && lp_args[1] == 0 && lp_args[2] == 43100);
    assert(run(0xd00, 0x10064, 3, 4, 0) == 0x10000 && lp_n == 2);
    assert(lp_args[0] == 0 && lp_args[1] == 1 && lp_args[2] == 44100);
    assert(run(0xd00, 0x20064, 3, 4, 0) == 0x10000 && lp_n == 3);
    assert(lp_args[0] == 0 && lp_args[1] == 2 && lp_args[2] == 100 && dat_clear());

    /* 58: remove flag */
    reset();
    assert(run(0x3a00, 0, 3, 0, 0) == 0 && run(0x3a00, 0, 0, 4, 0) == 0 && dat_clear());
    assert(run(0x3a00, 0, 3, 4, 0) == 0x10000 && dat.fRemove && !dat.fGuard1 && !dat.fGuard2 && !dat.fNoEq1);

    /* 1: no-equip flags */
    reset();
    assert(run(0x100, 0, 3, 0, 0) == 0 && run(0x100, 0, 0, 4, 0) == 0 && dat_clear());
    assert(run(0x100, 0, 3, 4, 0) == 0x10000 && dat.fNoEq2 && !dat.fNoEq1);
    reset();
    assert(run(0x100, 0, 3, 4, 1) == 0x10000 && !dat.fNoEq2 && dat.fNoEq1 && !dat.fRemove);

    /* 63: random offset in hundreds, difference of two draws modulo (low half / 100 + 1) */
    reset(); rnd[0] = 7; rnd[1] = 5;
    assert(run(0x3f00, 250, 0, 0, 0) == 0x10000 && rnd_n == 2 && dat.POfs1 == -100 && dat.POfs2 == 0);
    reset(); rnd[0] = 5; rnd[1] = 7;
    assert(run(0x3f00, 250, 0, 0, 1) == 0x10000 && rnd_n == 2 && dat.POfs1 == 0 && dat.POfs2 == 100);
}

int main(void)
{
    check_do();
    return 0;
}
