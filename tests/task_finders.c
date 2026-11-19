/* Synthetic host tests for the task_finders group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics.
 * Every copy is run against one table-driven model of the family (the table below says which cases
 * a copy has, their constants and how it passes the mode nibble) on every first purpose x second
 * purpose x unit status x mission result x step result, and on the three early exits. Compared: the
 * result, every outward call with its arguments in order, the two result-record members the step
 * cases store and the mode nibble. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "dotr/ee/task_finders.h"

enum { R_OWNER, R_ACTIVE, R_NEXT, R_ACTION, R_SET, R_CMD, R_STATUS, R_RUN, R_ATK, R_STEP, R_MOVE, R_LVL, R_ENC, R_MISSION, R_NONE };
enum { K_RUN, K_ATK, K_STEP, K_MOVE, K_ENC, K_MISSION, K_NONE };
enum { F_SETFORM = 1, F_ACTIVE = 2, F_NEXT_M = 4, F_CMD_M = 8, F_CMD_LIST = 16, F_TOP_ONE = 32, F_MIS_M = 64, F_MOVE_G = 128 };

struct call { int role; long addr; int n; long a[8]; };
static struct call got[32], want[32];
static int ngot, nwant;
static int v_next, v_idx, v_status, v_atk, v_step, v_active;

static long rec(int role, long addr, int n, ...)
{
    struct call *c = &got[ngot++];
    __builtin_va_list ap;
    int i;

    assert(ngot <= 32);
    memset(c, 0, sizeof *c);
    c->role = role; c->addr = role == R_MISSION ? addr : 0; c->n = n;
    __builtin_va_start(ap, n);
    for (i = 0; i < n; i++) {
        c->a[i] = __builtin_va_arg(ap, long);
    }
    __builtin_va_end(ap);
    switch (role) {
    case R_OWNER: return 1;
    case R_ACTIVE: return v_active;
    case R_NEXT: return v_next;
    case R_ACTION: return v_idx;
    case R_STATUS: return v_status;
    case R_ATK: return v_atk;
    case R_STEP: return v_step;
    case R_LVL: return 77;
    default: return 0;
    }
}

static void expect(int role, long addr, int n, long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7)
{
    struct call *c = &want[nwant++];

    memset(c, 0, sizeof *c);
    c->role = role; c->addr = addr; c->n = n;
    c->a[0] = a0; c->a[1] = a1; c->a[2] = a2; c->a[3] = a3; c->a[4] = a4; c->a[5] = a5; c->a[6] = a6; c->a[7] = a7;
}

int func_001de540_fnd(void) { return (int)rec(R_OWNER, 0x1de540, 0); }
int func_00276c80_fnd(struct DotrFinderUnit *list, int id) { return (int)rec(R_ACTION, 0x276c80, 2, (long)(intptr_t)list, (long)id); }
void func_0027b1b0_fnd(struct DotrFinderUnit *unit, int mode) { rec(R_SET, 0x27b1b0, 2, (long)(intptr_t)unit, (long)mode); }
int func_001e2100_fnd(int a, int b, int bit) { return (int)rec(R_STATUS, 0x1e2100, 3, (long)a, (long)b, (long)bit); }
int func_0027d020_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int view, int lvl, int g, int h) { return (int)rec(R_ENC, 0x27d020, 8, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)view, (long)lvl, (long)g, (long)h); }
void func_00276ec0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task) { rec(R_NONE, 0x276ec0, 3, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task); }
int func_00276c80_fnd2(struct DotrFinderSet *list, int id) { return (int)rec(R_ACTION, 0x276c80, 2, (long)(intptr_t)list, (long)id); }
int func_0010d080_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x10d080, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_0010ce20_fnd(struct DotrFinderUnit *list, struct DotrFinderUnit *unit) { rec(R_CMD, 0x10ce20, 2, (long)(intptr_t)list, (long)(intptr_t)unit); }
int func_0010fa60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x10fa60, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0010fff0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x10fff0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001106f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x1106f0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_0010dd50_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x10dd50, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0010eda0_fnd(int side) { return (int)rec(R_LVL, 0x10eda0, 1, (long)side); }
int func_001170a0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x1170a0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00116b60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x116b60, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00113370_fnd(struct DotrFinderSet *list, int side, int mode, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x113370, 4, (long)(intptr_t)list, (long)side, (long)mode, (long)(intptr_t)task); }
int func_00117340_fnd(void) { return (int)rec(R_ACTIVE, 0x117340, 0); }
void func_00113140_fnd(struct DotrFinderUnit *unit, int mode) { rec(R_CMD, 0x113140, 2, (long)(intptr_t)unit, (long)mode); }
int func_00115850_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x115850, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00115e20_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x115e20, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001162f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x1162f0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00114080_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x114080, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_00117c30_fnd(int side) { return (int)rec(R_LVL, 0x117c30, 1, (long)side); }
int func_0011b0e0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x11b0e0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
void func_0011c3e0_fnd(struct DotrFinderUnit *unit, int mode) { rec(R_SET, 0x11c3e0, 2, (long)(intptr_t)unit, (long)mode); }
int func_00118510_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x118510, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_00118330_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x118330, 1, (long)(intptr_t)unit); }
int func_0011a4d0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x11a4d0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0011aa70_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x11aa70, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0011b680_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x11b680, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00119300_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x119300, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0011aa10_fnd(int side) { return (int)rec(R_LVL, 0x11aa10, 1, (long)side); }
int func_00120150_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x120150, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
void func_001215e0_fnd(struct DotrFinderUnit *unit, int mode) { rec(R_SET, 0x1215e0, 2, (long)(intptr_t)unit, (long)mode); }
int func_0011cd30_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x11cd30, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_0011cb20_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x11cb20, 1, (long)(intptr_t)unit); }
int func_0011f540_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x11f540, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0011fae0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x11fae0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001206f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x1206f0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_0011dfb0_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x11dfb0, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0011fa80_fnd(int side) { return (int)rec(R_LVL, 0x11fa80, 1, (long)side); }
int func_00121e90_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x121e90, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_00126940_fnd(struct DotrFinderUnit *unit, int mode) { rec(R_SET, 0x126940, 2, (long)(intptr_t)unit, (long)mode); }
void func_00121ce0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x121ce0, 1, (long)(intptr_t)unit); }
int func_00124f60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x124f60, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00125500_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x125500, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00125b70_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x125b70, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00122d30_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x122d30, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_001254a0_fnd(int side) { return (int)rec(R_LVL, 0x1254a0, 1, (long)side); }
int func_00129700_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x129700, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
void func_0012b830_fnd(struct DotrFinderUnit *unit, int mode) { rec(R_SET, 0x12b830, 2, (long)(intptr_t)unit, (long)mode); }
int func_0012aac0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x12aac0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001273e0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x1273e0, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_001271f0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x1271f0, 1, (long)(intptr_t)unit); }
int func_001291c0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x1291c0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00129d00_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x129d00, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0012a2e0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x12a2e0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00127e30_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x127e30, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0012b1b0_fnd(int side) { return (int)rec(R_LVL, 0x12b1b0, 1, (long)side); }
int func_0012ed70_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int m) { return (int)rec(R_MISSION, 0x12ed70, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)m); }
int func_00130120_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int m) { return (int)rec(R_MISSION, 0x130120, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)m); }
int func_0012c3f0_fnd(struct DotrFinderSet *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x12c3f0, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
int func_00130cb0_fnd(void) { return (int)rec(R_ACTIVE, 0x130cb0, 0); }
void func_0012c1a0_fnd(struct DotrFinderUnit *unit, int m) { rec(R_CMD, 0x12c1a0, 2, (long)(intptr_t)unit, (long)m); }
int func_0012e820_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int m) { return (int)rec(R_RUN, 0x12e820, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)m); }
int func_0012f350_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int m) { return (int)rec(R_ATK, 0x12f350, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)m); }
int func_0012f930_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e, unsigned char m) { return (int)rec(R_MOVE, 0x12f930, 6, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e, (long)m); }
int func_0012d270_fnd(int pos, int side, struct DotrFinderTask *task, int m) { return (int)rec(R_STEP, 0x12d270, 4, (long)pos, (long)side, (long)(intptr_t)task, (long)m); }
int func_00130780_fnd(int side) { return (int)rec(R_LVL, 0x130780, 1, (long)side); }
int func_00131750_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x131750, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_00131540_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x131540, 1, (long)(intptr_t)unit); }
int func_001336a0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x1336a0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00133be0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x133be0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001341c0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x1341c0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00132460_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x132460, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_00134ae0_fnd(int side) { return (int)rec(R_LVL, 0x134ae0, 1, (long)side); }
int func_00138780_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x138780, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00138950_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x138950, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00135970_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x135970, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_00135790_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x135790, 1, (long)(intptr_t)unit); }
int func_001374d0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x1374d0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001379f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x1379f0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00137fb0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x137fb0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00136210_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x136210, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0014f9b0_fnd(int side) { return (int)rec(R_LVL, 0x14f9b0, 1, (long)side); }
int func_0013a010_fnd(struct DotrFinderSet *list, int side, int mode, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x13a010, 4, (long)(intptr_t)list, (long)side, (long)mode, (long)(intptr_t)task); }
int func_0013d280_fnd(void) { return (int)rec(R_ACTIVE, 0x13d280, 0); }
void func_00139d90_fnd(struct DotrFinderUnit *unit, int mode) { rec(R_CMD, 0x139d90, 2, (long)(intptr_t)unit, (long)mode); }
int func_0013be60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x13be60, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0013c430_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x13c430, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0013ca10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x13ca10, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_0013ab70_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x13ab70, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0013da70_fnd(int side) { return (int)rec(R_LVL, 0x13da70, 1, (long)side); }
int func_001415f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x1415f0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0013e3d0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x13e3d0, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_0013e1d0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x13e1d0, 1, (long)(intptr_t)unit); }
int func_001402f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x1402f0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00140830_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x140830, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00140e10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x140e10, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_0013eee0_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x13eee0, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_00141b30_fnd(int side) { return (int)rec(R_LVL, 0x141b30, 1, (long)side); }
int func_00142b70_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x142b70, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_001429b0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x1429b0, 1, (long)(intptr_t)unit); }
int func_00144ec0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x144ec0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00145400_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x145400, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001459e0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x1459e0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00143ba0_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x143ba0, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_001461c0_fnd(int side) { return (int)rec(R_LVL, 0x1461c0, 1, (long)side); }
int func_0014aab0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x14aab0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00147100_fnd(struct DotrFinderSet *list, int side, int mode, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x147100, 4, (long)(intptr_t)list, (long)side, (long)mode, (long)(intptr_t)task); }
int func_0014aca0_fnd(void) { return (int)rec(R_ACTIVE, 0x14aca0, 0); }
void func_00146ec0_fnd(struct DotrFinderUnit *unit, int mode) { rec(R_CMD, 0x146ec0, 2, (long)(intptr_t)unit, (long)mode); }
int func_00149690_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x149690, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00149c60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x149c60, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0014a240_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x14a240, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00148020_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x148020, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0014b330_fnd(int side) { return (int)rec(R_LVL, 0x14b330, 1, (long)side); }
int func_0014bfd0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x14bfd0, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_0014bde0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x14bde0, 1, (long)(intptr_t)unit); }
int func_0014df80_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x14df80, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0014e550_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x14e550, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0014eb30_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x14eb30, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_0014cba0_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x14cba0, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_00153550_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x153550, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00150470_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x150470, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_00150270_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x150270, 1, (long)(intptr_t)unit); }
int func_00152100_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x152100, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001526d0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x1526d0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00152cd0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x152cd0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_00150c40_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x150c40, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_00153ed0_fnd(int side) { return (int)rec(R_LVL, 0x153ed0, 1, (long)side); }
int func_00154ad0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x154ad0, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_001547a0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x1547a0, 1, (long)(intptr_t)unit); }
int func_00157480_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x157480, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00156fc0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x156fc0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00157a50_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e, int g) { return (int)rec(R_MOVE, 0x157a50, 6, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e, (long)g); }
int func_00155770_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x155770, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_00158780_fnd(int side) { return (int)rec(R_LVL, 0x158780, 1, (long)side); }
int func_0015c670_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x15c670, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00159380_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x159380, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_001591b0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x1591b0, 1, (long)(intptr_t)unit); }
int func_0015b250_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x15b250, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0015b820_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x15b820, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0015be00_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x15be00, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_0015a360_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x15a360, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0015d120_fnd(int side) { return (int)rec(R_LVL, 0x15d120, 1, (long)side); }
int func_00161ea0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x161ea0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00161bf0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x161bf0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0015daf0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x15daf0, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_0015d8e0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x15d8e0, 1, (long)(intptr_t)unit); }
int func_001607d0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x1607d0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00160da0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x160da0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00161380_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x161380, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_0015f300_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x15f300, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_00162580_fnd(int side) { return (int)rec(R_LVL, 0x162580, 1, (long)side); }
int func_00166d10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x166d10, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00163870_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x163870, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_00163660_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x163660, 1, (long)(intptr_t)unit); }
int func_001658f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x1658f0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00165ec0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x165ec0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001664a0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x1664a0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_001643f0_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x1643f0, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_001676d0_fnd(int side) { return (int)rec(R_LVL, 0x1676d0, 1, (long)side); }
int func_00167e80_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x167e80, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_00167d30_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x167d30, 1, (long)(intptr_t)unit); }
int func_00169a10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x169a10, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00169fe0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x169fe0, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0016a5c0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x16a5c0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_001687a0_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x1687a0, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_0016b0e0_fnd(int side) { return (int)rec(R_LVL, 0x16b0e0, 1, (long)side); }
int func_00172000_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x172000, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00171d60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_MISSION, 0x171d60, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_0016d8c0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task) { return (int)rec(R_NEXT, 0x16d8c0, 3, (long)(intptr_t)list, (long)side, (long)(intptr_t)task); }
void func_0016d6a0_fnd(struct DotrFinderUnit *unit) { rec(R_CMD, 0x16d6a0, 1, (long)(intptr_t)unit); }
int func_00170940_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_RUN, 0x170940, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_00170f10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first) { return (int)rec(R_ATK, 0x170f10, 4, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first); }
int func_001714f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e) { return (int)rec(R_MOVE, 0x1714f0, 5, (long)(intptr_t)unit, (long)side, (long)(intptr_t)task, (long)first, (long)e); }
int func_0016f4d0_fnd(int pos, int side, struct DotrFinderTask *task) { return (int)rec(R_STEP, 0x16f4d0, 3, (long)pos, (long)side, (long)(intptr_t)task); }
int func_00172310_fnd(int side) { return (int)rec(R_LVL, 0x172310, 1, (long)side); }

struct kase { int val, kind, e, g, st; long addr; };
struct desc {
    int (*unit_fn)(struct DotrFinderUnit *, struct DotrFinderTask *, int);
    int (*set_fn)(struct DotrFinderSet *, struct DotrFinderTask *, int);
    int flags;
    int n[2];
    struct kase c[2][12];
};

static const struct desc table[] = {
    { TaLogic_GetMinimumTask__0010c940, NULL, F_CMD_LIST, { 7, 7 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 } } } },
    { NULL, TaLogic_GetMinimumTask__00112b90, F_SETFORM | F_ACTIVE | F_NEXT_M | F_CMD_M, { 9, 9 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x116b60 }, { 12, K_MISSION, 0, 0, 0, 0x1170a0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x116b60 }, { 12, K_MISSION, 0, 0, 0, 0x1170a0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__00117e60, NULL, 0, { 9, 9 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x11b0e0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 9, K_ENC, 0, 3, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x11b0e0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 9, K_ENC, 0, 3, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } } } },
    { TaLogic_GetMinimumTask__0011c600, NULL, 0, { 10, 9 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x120150 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 4, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 }, { 9, K_ENC, 0, 4, 1, 0x0 } } } },
    { TaLogic_GetMinimumTask__00121800, NULL, 0, { 7, 7 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__00126c80, NULL, 0, { 9, 9 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x12aac0 }, { 12, K_MISSION, 0, 0, 0, 0x129700 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x12aac0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x129700 }, { 14, K_NONE, 0, 0, 1, 0x0 } } } },
    { NULL, TaLogic_GetMinimumTask__0012bb70, F_SETFORM | F_ACTIVE | F_CMD_M | F_MIS_M, { 9, 9 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x130120 }, { 12, K_MISSION, 0, 0, 0, 0x12ed70 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x130120 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x12ed70 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__00131060, NULL, 0, { 7, 7 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 4, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 4, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__00135230, NULL, 0, { 9, 9 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 1, 2, 0, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x138780 }, { 13, K_MISSION, 0, 0, 0, 0x138950 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 1, 2, 0, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x138780 }, { 13, K_MISSION, 0, 0, 0, 0x138950 }, { 14, K_NONE, 0, 0, 1, 0x0 } } } },
    { NULL, TaLogic_GetMinimumTask__00139890, F_SETFORM | F_ACTIVE | F_NEXT_M | F_CMD_M, { 7, 7 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 0, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__0013dca0, NULL, 0, { 8, 8 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x1415f0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x1415f0 }, { 9, K_ENC, 0, 2, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__001424b0, NULL, 0, { 9, 9 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 2, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { NULL, TaLogic_GetMinimumTask__00146960, F_SETFORM | F_ACTIVE | F_NEXT_M | F_CMD_M, { 8, 8 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x14aab0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x14aab0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__0014b900, NULL, 0, { 7, 7 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__0014fd40, NULL, 0, { 8, 8 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x153550 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x153550 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__00154220, NULL, F_MOVE_G, { 8, 8 }, {
        { { 2, K_ATK, 0, 0, 1, 0x0 }, { 5, K_RUN, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 7, K_MOVE, 0, 1, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 2, K_ATK, 0, 0, 1, 0x0 }, { 5, K_RUN, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 7, K_MOVE, 0, 1, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__00158c80, NULL, 0, { 8, 8 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 3, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x15c670 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 3, 1, 0x0 }, { 11, K_MISSION, 0, 0, 0, 0x15c670 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__0015d350, NULL, 0, { 11, 11 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 13, K_MISSION, 0, 0, 0, 0x161ea0 }, { 12, K_MISSION, 0, 0, 0, 0x161bf0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 13, K_MISSION, 0, 0, 0, 0x161ea0 }, { 12, K_MISSION, 0, 0, 0, 0x161bf0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__00163130, NULL, 0, { 8, 8 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 1, 5, 1, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x166d10 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 1, 5, 1, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x166d10 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__001678b0, NULL, 0, { 8, 8 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 9, K_ENC, 1, 4, 1, 0x0 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 9, K_ENC, 1, 4, 1, 0x0 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
    { TaLogic_GetMinimumTask__0016d140, NULL, F_TOP_ONE, { 11, 11 }, {
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x171d60 }, { 13, K_MISSION, 0, 0, 0, 0x172000 }, { 14, K_NONE, 0, 0, 1, 0x0 } },
        { { 5, K_RUN, 0, 0, 1, 0x0 }, { 2, K_ATK, 0, 0, 1, 0x0 }, { 10, K_STEP, 0, 0, 0, 0x0 }, { 6, K_MOVE, 0, 0, 1, 0x0 }, { 15, K_MOVE, 0, 0, 1, 0x0 }, { 16, K_MOVE, 0, 0, 1, 0x0 }, { 3, K_MOVE, 1, 0, 1, 0x0 }, { 9, K_ENC, 0, 5, 1, 0x0 }, { 12, K_MISSION, 0, 0, 0, 0x171d60 }, { 13, K_MISSION, 0, 0, 0, 0x172000 }, { 14, K_NONE, 0, 0, 0, 0x0 } } } },
};

static struct DotrFinderSet set;
static struct DotrFinderTask task, task_want;

#define P(x) ((long)(intptr_t)(x))

/* One switch of the model. Returns 2 when the function returns `ret` at once, else 0 with *done updated. */
static int pass(const struct desc *d, int first, int purpose, struct DotrFinderUnit *u, long m, int *done, int *ret)
{
    const struct kase *k = NULL;
    int i, mm = (d->flags & F_MIS_M) != 0;
    long st0 = u->wUnit >> 8, st1 = u->wUnit & 0xff;

    for (i = 0; i < d->n[!first]; i++) {
        if (d->c[!first][i].val == purpose) {
            k = &d->c[!first][i];
        }
    }
    if (k == NULL) {
        if (first) {
            *done = 0;
            return 0;
        }
        *ret = 0;
        return 2;
    }
    switch (k->kind) {
    case K_RUN:
        expect(R_STATUS, 0, 3, st0, st1, 0, 0, 0, 0, 0, 0);
        if (v_status == 0) {
            expect(R_RUN, 0, 4 + mm, P(u), 1, P(&task), first, m, 0, 0, 0);
            *done = 1;
        }
        break;
    case K_ATK:
        expect(R_STATUS, 0, 3, st0, st1, 0, 0, 0, 0, 0, 0);
        if (v_status == 0) {
            expect(R_ATK, 0, 4 + mm, P(u), 1, P(&task), first, m, 0, 0, 0);
            if (v_atk < 0) {
                if (first) {
                    expect(R_SET, 0, 2, P(u), 1, 0, 0, 0, 0, 0, 0);
                    *ret = 1;
                    return 2;
                }
                expect(R_RUN, 0, 4 + mm, P(u), 1, P(&task), 0, m, 0, 0, 0);
            }
            *done = 1;
        }
        break;
    case K_STEP:
        expect(R_STEP, 0, 3 + mm, u->wUnit, 1, P(&task), m, 0, 0, 0, 0);
        expect(R_SET, 0, 2, P(u), first, 0, 0, 0, 0, 0, 0);
        if (v_step < 0) {
            task_want.bPurpose = 255;
            task_want.bOpen = 0;
            if (!first) {
                *done = 1;
            }
        } else {
            *done = 1;
        }
        break;
    case K_MOVE:
        if (k->st) {
            expect(R_STATUS, 0, 3, st0, st1, 0, 0, 0, 0, 0, 0);
            if (v_status != 0) {
                break;
            }
        }
        if (d->flags & F_MOVE_G) {
            expect(R_MOVE, 0, 6, P(u), 1, P(&task), first, k->e, k->g, 0, 0);
        } else {
            expect(R_MOVE, 0, 5 + mm, P(u), 1, P(&task), first, k->e, m, 0, 0);
        }
        *done = 1;
        break;
    case K_ENC:
        if (k->st) {
            expect(R_LVL, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);
        }
        expect(R_ENC, 0, 8, P(u), 1, P(&task), first, k->e, k->st ? 77 : 0, k->g, 1);
        *done = 1;
        break;
    case K_MISSION:
        expect(R_MISSION, k->addr, 4 + mm, P(u), 1, P(&task), first, m, 0, 0, 0);
        *done = 1;
        break;
    default:
        expect(R_NONE, 0, 3, P(u), 1, P(&task), 0, 0, 0, 0, 0);
        if (first || k->st) {
            *done = 1;
        }
        break;
    }
    return 0;
}

static int model(const struct desc *d, int idx)
{
    struct DotrFinderUnit *u = &set.unit[idx < 0 ? 0 : idx];
    long list = P(&set), m = 0;
    int done = 0, ret = 0;

    expect(R_OWNER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (d->flags & F_ACTIVE) {
        expect(R_ACTIVE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        m = v_active & 0xf;
    }
    if (d->flags & F_NEXT_M) {
        expect(R_NEXT, 0, 4, list, 1, m, P(&task), 0, 0, 0, 0);
    } else {
        expect(R_NEXT, 0, 3, list, 1, P(&task), 0, 0, 0, 0, 0);
    }
    if (v_next < 0) {
        return 0;
    }
    expect(R_ACTION, 0, 2, list, v_next & 0xffff, 0, 0, 0, 0, 0, 0);
    if (idx < 0) {
        return 0;
    }
    if ((v_next >> 16) == 1) {
        expect(R_SET, 0, 2, P(u), (d->flags & F_TOP_ONE) || (u->wPurpose >> 8) == 10, 0, 0, 0, 0, 0, 0);
        return 1;
    }
    if (d->flags & F_CMD_LIST) {
        expect(R_CMD, 0, 2, list, P(u), 0, 0, 0, 0, 0, 0);
    } else if (d->flags & F_CMD_M) {
        expect(R_CMD, 0, 2, P(u), m, 0, 0, 0, 0, 0, 0);
    } else {
        expect(R_CMD, 0, 1, P(u), 0, 0, 0, 0, 0, 0, 0);
    }
    if (!(d->flags & F_MIS_M)) {
        m = 0;
    }
    if (pass(d, 1, u->wPurpose >> 8, u, m, &done, &ret) == 2) {
        return ret;
    }
    if (done == 1) {
        return 1;
    }
    if (pass(d, 0, u->wPurpose & 0xff, u, m, &done, &ret) == 2) {
        return ret;
    }
    return done == 1;
}

static void run(const struct desc *d, int idx, int purpose)
{
    int r, w, i;

    memset(&set, 0, sizeof set);
    memset(&task, 0, sizeof task);
    task.bPurpose = 9;
    task.bOpen = 5;
    set.m = 9;
    if (idx >= 0) {
        set.unit[idx].wUnit = 0x0203;
        set.unit[idx].wPurpose = (unsigned short)purpose;
    }
    task_want = task;
    ngot = nwant = 0;
    w = model(d, idx);
    r = d->set_fn != NULL ? d->set_fn(&set, &task, 55) : d->unit_fn(set.unit, &task, 55);
    assert(r == w);
    assert(ngot == nwant);
    for (i = 0; i < ngot; i++) {
        assert(got[i].role == want[i].role && got[i].addr == want[i].addr && got[i].n == want[i].n);
        assert(memcmp(got[i].a, want[i].a, (size_t)got[i].n * sizeof(long)) == 0);
    }
    assert(task.bPurpose == task_want.bPurpose && task.bOpen == task_want.bOpen);
    assert(set.m == ((d->flags & F_ACTIVE) ? (v_active & 0xf) : 9));
}

int main(void)
{
    static const int purposes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 254 };
    size_t t, a, b;
    int s;

    for (t = 0; t < sizeof table / sizeof table[0]; t++) {
        const struct desc *d = &table[t];

        v_active = 0x37;
        v_status = 0; v_atk = 0; v_step = 0;
        v_next = -1; v_idx = 3;
        run(d, 3, 0x0505);
        v_next = 0x00020007; v_idx = -1;
        run(d, -1, 0);
        v_next = 0x00010007; v_idx = 3;
        run(d, 3, 0x0a05);
        run(d, 3, 0x050a);
        v_next = 0x00020007;
        for (a = 0; a < sizeof purposes / sizeof purposes[0]; a++) {
            for (b = 0; b < sizeof purposes / sizeof purposes[0]; b++) {
                for (s = 0; s < 8; s++) {
                    v_status = s & 1;
                    v_atk = (s & 2) ? -1 : 4;
                    v_step = (s & 4) ? -1 : 6;
                    v_idx = 3 + (int)(a % 5);
                    run(d, v_idx, (purposes[a] << 8) | purposes[b]);
                }
            }
        }
    }
    return 0;
}
