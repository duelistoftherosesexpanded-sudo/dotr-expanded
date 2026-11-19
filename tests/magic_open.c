/* Synthetic host tests for the magic_open group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/magic_open.h"

/* Plain recording stubs: each returns a value the test sets and records what it was given. */
static struct {
    int kind, st4, st2, loc, act_ret, magic_ret, side, rate;
    int search14, search76, search17, trap14, trap76, trap17, trap46;
    struct jp_10845 eff0, eff3;
    int has_eff3;
} cfg;
static struct {
    int open_n, open_a, open_b, open_mode;
    int del_n, del_a, del_b;
    int act_n, act[7];
    struct jp_10845 *act_eff;
    struct jp_108cd *act_dat;
    struct jp_107e2 *act_info;
    int act_last;
    int magic_n, magic_a, magic_b, magic_pr;
    struct jp_1073d *magic_dat;
    struct jp_107e2 *magic_info;
    struct jp_10845 *magic_eff;
    int rate_a, rate_b;
} rec;

int SzDuel_GetUnitKind(int a, int b) { assert(a == 1 && b == 9); return cfg.kind; }
struct jp_10845 *Ta_GetUnitEffect(int a, int b, int n)
{
    assert(a == 1 && b == 9 && (n == 0 || n == 3));
    if (n == 0) {
        return &cfg.eff0;
    }
    return cfg.has_eff3 ? &cfg.eff3 : 0;
}
int SzDuel_GetUnitStatus(int a, int b, int n)
{
    assert(a == 1 && b == 9 && (n == 4 || n == 2));
    return n == 4 ? cfg.st4 : cfg.st2;
}
int SzDuel_GetUnitLoc(int a, int b) { assert(a == 1 && b == 9); return cfg.loc; }
void TaUtil_DoOpen(int a, int b, int mode)
{
    rec.open_n++;
    rec.open_a = a;
    rec.open_b = b;
    rec.open_mode = mode;
}
int Ta_Manage_ActEfc(struct jp_10845 *e, int a, int b, int c, int d, int x, int z, struct jp_108cd *dat,
                     struct jp_107e2 *info, int last)
{
    rec.act_n++;
    rec.act_eff = e;
    rec.act[0] = a;
    rec.act[1] = b;
    rec.act[2] = c;
    rec.act[3] = d;
    rec.act[4] = x;
    rec.act[5] = z;
    rec.act_dat = dat;
    rec.act_info = info;
    rec.act_last = last;
    return cfg.act_ret;
}
int Ta_SearchUncTypeNaturalEffectUser(int a, int b)
{
    assert(b == 0 && (a == 14 || a == 76 || a == 17));
    return a == 14 ? cfg.search14 : a == 76 ? cfg.search76 : cfg.search17;
}
int Ta_DelUnit(int a, int b)
{
    rec.del_n++;
    rec.del_a = a;
    rec.del_b = b;
    return 77;
}
int Ta_GetCurrentSide(int a, int b) { assert(a == 1 && b == 9); return cfg.side; }
int Ta_IsActiveTrapB(int a, int b)
{
    if (b == 46) {
        assert(a == 0);
        return cfg.trap46;
    }
    assert(a == cfg.side && (b == 14 || b == 76 || b == 17));
    return b == 14 ? cfg.trap14 : b == 76 ? cfg.trap76 : cfg.trap17;
}
int SzDuel_GetUnitOpenRate(int a, int b)
{
    rec.rate_a = a;
    rec.rate_b = b;
    return cfg.rate;
}
int Ta_Do_Magic(int a, int b, struct jp_1073d *dat, struct jp_107e2 *info, int pr, struct jp_10845 *e)
{
    rec.magic_n++;
    rec.magic_a = a;
    rec.magic_b = b;
    rec.magic_dat = dat;
    rec.magic_info = info;
    rec.magic_pr = pr;
    rec.magic_eff = e;
    return cfg.magic_ret;
}

static struct jp_108cd dat;

static void reset(void)
{
    memset(&cfg, 0, sizeof(cfg));
    cfg.search14 = cfg.search76 = cfg.search17 = -1;
    cfg.trap14 = cfg.trap76 = cfg.trap17 = cfg.trap46 = -1;
    cfg.side = 1;
    cfg.act_ret = 0x30002;
    cfg.magic_ret = 0x1234;
}

/* Runs one step on a pDat filled with 0xFF and returns the result. */
static int run(int PrNo)
{
    memset(&rec, 0, sizeof(rec));
    memset(&dat, 0xFF, sizeof(dat));
    return Ta_Open(1, 9, &dat, PrNo);
}

static void expect_info(int eNO, unsigned from, unsigned to, unsigned par)
{
    assert(dat.eInfo.eNO == eNO && dat.eInfo.dwFrom == from && dat.eInfo.dwTo == to && dat.eInfo.dwPar == par);
    assert(dat.pInfo.bType == 0 && dat.pInfo.bKind == 0);
}

/* Steps 1..3 of the three natural-effect blocks: sequence 101, sequence 102 with the finder's
 * result, Ta_DelUnit, and no case for step 4. */
static void check_natural(int found)
{
    assert(run(1) == 0x20001);
    expect_info(101, 0, 0x109, 0);
    assert(run(2) == 0x30001);
    expect_info(102, 0, (unsigned)found, 0);
    assert(run(3) == 0 && rec.del_n == 1 && rec.del_a == 1 && rec.del_b == 9);
    expect_info(0, 0, 0, 0);
    assert(run(4) == 0 && rec.del_n == 0);
    expect_info(0, 0, 0, 0);
}

/* Steps 1 and 2 shared by every trap block. */
static void check_trap_head(void)
{
    assert(run(1) == 0x20000);
    expect_info(0, 0, 0, 0);
    assert(run(2) == 0x30001);
    expect_info(101, 0, 0x109, 0);
}

/* The trap-46 arm of a trap block: step 3 opens the unit packed in open3, step 4 tests trap
 * 46's unit, step 5 is sequence 101 with parameter 2, step 6 has no case. */
static void check_trap46(int open3)
{
    cfg.trap46 = 0x0102;
    check_trap_head();
    assert(run(3) == 0x40000 && rec.open_n == 1 && rec.open_a == open3 >> 8 && rec.open_b == (open3 & 0xFF) &&
           rec.open_mode == 1);
    cfg.rate = 255;
    assert(run(4) == 0x50001 && rec.rate_a == 1 && rec.rate_b == 2);
    expect_info(102, 1, 0x0102, 0);
    cfg.rate = 254;
    assert(run(4) == 0x40000);
    expect_info(0, 0, 0, 0);
    assert(run(5) == 0x70001);
    expect_info(101, 0, 0x109, 2);
    assert(run(6) == 0 && rec.open_n == 0);
    expect_info(0, 0, 0, 0);
    cfg.trap46 = -1;
}

/* Branch coverage: every arm of the step switch, of the kind tests of step 0, of each finder
 * and trap test with its effect-type condition, and of each inner step switch including the
 * steps it has no case for. */
int main(void)
{
    assert(offsetof(struct jp_108cd, pInfo) == 0x10 && offsetof(struct jp_107e2, bKind) == 1);

    /* step 0, kind 0 */
    reset();
    assert(run(0) == 0x100 && rec.open_n == 0 && rec.act_n == 0);
    expect_info(0, 0, 0, 0);
    cfg.has_eff3 = 1;
    cfg.st4 = 1;
    cfg.st2 = 1;
    assert(run(0) == 0x100 && rec.act_n == 0);
    cfg.st4 = 0;
    cfg.st2 = 0;
    assert(run(0) == 0x100 && rec.act_n == 0);
    cfg.st2 = 1;
    cfg.loc = 0x0603;
    assert(run(0) == 0x30102);
    assert(rec.open_n == 1 && rec.open_a == 1 && rec.open_b == 9 && rec.open_mode == 1);
    assert(rec.act_n == 1 && rec.act_eff == &cfg.eff3 && rec.act[0] == 1 && rec.act[1] == 9 && rec.act[2] == -1 &&
           rec.act[3] == -1 && rec.act[4] == 3 && rec.act[5] == 6 && rec.act_dat == &dat &&
           rec.act_info == &dat.pInfo && rec.act_last == 0);
    cfg.kind = 31; /* still kind 0 after the shift */
    assert(run(0) == 0x30102);
    /* step 0, kinds 1 and 5 need status 2; other kinds refuse */
    cfg.kind = 1 << 5;
    assert(run(0) == 0x10000 && rec.act_n == 0);
    cfg.st2 = 0;
    assert(run(0) == 0x100);
    cfg.kind = 5 << 5;
    assert(run(0) == 0x100);
    cfg.st2 = 1;
    assert(run(0) == 0x10000);
    cfg.kind = 2 << 5;
    assert(run(0) == 0x100);
    cfg.kind = 6 << 5;
    assert(run(0) == 0x100);

    /* steps outside 0..6 go to Ta_Do_Magic with the type-0 effect */
    reset();
    assert(run(7) == 0x1234 && rec.magic_n == 1 && rec.magic_a == 1 && rec.magic_b == 9 && rec.magic_dat == &dat.eInfo &&
           rec.magic_info == &dat.pInfo && rec.magic_pr == 7 && rec.magic_eff == &cfg.eff0);
    expect_info(0, 0, 0, 0);
    assert(run(-1) == 0x1234 && rec.magic_n == 1 && rec.magic_pr == -1);

    /* nothing active: sequence 101 with parameter 1, for the first and last table step */
    assert(run(1) == 0x70001 && rec.magic_n == 0);
    expect_info(101, 0, 0x109, 1);
    assert(run(6) == 0x70001);
    expect_info(101, 0, 0x109, 1);

    /* natural effect 14, found at 0 and above */
    cfg.search14 = 0;
    check_natural(0);
    cfg.search14 = 7;
    check_natural(7);
    cfg.search14 = -1;
    /* natural effect 76 counts only with effect type 20 */
    cfg.search76 = 8;
    assert(run(1) == 0x70001);
    cfg.eff0.dwID = (20 << 8) | 0x55;
    check_natural(8);
    cfg.search76 = -1;
    /* natural effect 17 counts only with effect type 50 */
    cfg.search17 = 0;
    assert(run(1) == 0x70001);
    cfg.eff0.dwID = (50 << 8) | 0x55;
    check_natural(0);
    cfg.search17 = -1;

    /* trap 14, without trap 46 */
    cfg.eff0.dwID = 0;
    cfg.trap14 = 0x0305;
    check_trap_head();
    assert(run(3) == 0x40000 && rec.open_n == 1 && rec.open_a == 3 && rec.open_b == 5 && rec.open_mode == 1);
    cfg.rate = 255;
    assert(run(4) == 1 && rec.rate_a == 3 && rec.rate_b == 5);
    expect_info(51, 0x0305, 0x109, 1);
    cfg.rate = 0;
    assert(run(4) == 0x40000);
    expect_info(0, 0, 0, 0);
    assert(run(5) == 0 && rec.open_n == 0);
    /* trap 14 with trap 46: step 3 opens trap 46's unit */
    check_trap46(0x0102);
    cfg.trap14 = 0; /* unit 0 still counts as found */
    assert(run(1) == 0x20000);
    cfg.trap14 = -1;

    /* trap 76 counts only with effect type 20; steps 4..6 all take the open-rate test */
    cfg.trap76 = 0x0406;
    assert(run(1) == 0x70001);
    cfg.eff0.dwID = 20 << 8;
    check_trap_head();
    assert(run(3) == 0x40000 && rec.open_n == 1 && rec.open_a == 4 && rec.open_b == 6 && rec.open_mode == 1);
    cfg.rate = 255;
    assert(run(4) == 1 && rec.rate_a == 4 && rec.rate_b == 6);
    expect_info(51, 0x0406, 0x109, 0);
    assert(run(6) == 1);
    cfg.rate = 1;
    assert(run(4) == 0x40000);
    assert(run(5) == 0x40000);
    /* with trap 46, step 3 still opens trap 76's unit */
    check_trap46(0x0406);
    cfg.trap76 = -1;

    /* trap 17 counts only with effect type 50 */
    cfg.trap17 = 0x0507;
    assert(run(1) == 0x70001);
    cfg.eff0.dwID = 50 << 8;
    check_trap_head();
    assert(run(3) == 0x40000 && rec.open_n == 1 && rec.open_a == 5 && rec.open_b == 7 && rec.open_mode == 1);
    cfg.rate = 255;
    assert(run(4) == 1 && rec.rate_a == 5 && rec.rate_b == 7);
    expect_info(51, 0x0507, 0x109, 0);
    cfg.rate = 1;
    assert(run(5) == 0x40000);
    /* with trap 46, step 3 opens trap 46's unit */
    check_trap46(0x0102);
    return 0;
}
