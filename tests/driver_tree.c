/* Synthetic host tests for the driver_tree group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/driver_tree.h"

static struct PieceTreetag pool[64];
static int pool_used, pool_limit;
static int set_log[64][2], set_count;
static struct PieceTreetag *set_prev[64];
static unsigned int move_rev;
static int rev_calls, pass_result, past_result, pass_calls, past_calls;
static int pass_last[9], pass_skip;
static unsigned short *size_seen, *count_seen;

unsigned int GetMoveRev(struct jp_9a533 *vInfo, int nX, int nZ)
{
    (void)vInfo; (void)nX; (void)nZ;
    rev_calls++;
    return move_rev;
}

struct PieceTreetag *TaLogic_RootTreeAloc(unsigned short *wSize, unsigned short *wNodeCount)
{
    size_seen = wSize;
    count_seen = wNodeCount;
    if (pool_used >= pool_limit) {
        return NULL;
    }
    return &pool[pool_used++];
}

void TaLogic_SetRootTreeDat(struct PieceTreetag *pTree, struct PieceTreetag *pPrev, int nX, int nZ)
{
    (void)pTree;
    set_prev[set_count] = pPrev;
    set_log[set_count][0] = nX;
    set_log[set_count][1] = nZ;
    set_count++;
}

/* Assumed: the larger of the two values. */
int GetCap(int limit, int value)
{
    return value < limit ? limit : value;
}

int TaLogic_IsPassRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned short *gUnitID)
{
    (void)pVarInfo; (void)gUnitID;
    pass_last[0] = nX; pass_last[1] = nZ; pass_last[2] = gX; pass_last[3] = gZ;
    if (pass_calls++ < pass_skip) {
        return 0;
    }
    return pass_result;
}

int TaLogic_IsPassInvadeRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, int *gUnitID, int fView, unsigned char Node0, unsigned short bWarnLvl)
{
    (void)pVarInfo; (void)gUnitID;
    pass_last[0] = nX; pass_last[1] = nZ; pass_last[2] = gX; pass_last[3] = gZ;
    pass_last[4] = fView; pass_last[5 + (Node0 ? 0 : 1)] = Node0 + 10; pass_last[7] = bWarnLvl;
    if (pass_calls++ < pass_skip) {
        return 0;
    }
    return pass_result;
}

int TaLogic_IsRootPast2(struct PieceTreetag *pTree, int nX, int nZ, int pX, int pZ, unsigned char bDepth)
{
    (void)pTree; (void)nX; (void)nZ; (void)pX; (void)pZ; (void)bDepth;
    past_calls++;
    return past_result;
}

/* Views that TaLogic_GetRootTree2 calls (driver_tree.h): they forward to the stubs
 * above, turning the 32-bit words back into the pointers they carry. */
int func_002464f0_tree(DotrTreeWord arg0, int arg1, int arg2)
{
    return (int)GetMoveRev((struct jp_9a533 *)arg0, arg1, arg2);
}
unsigned char *func_0027b150_tree(unsigned short *arg0, unsigned short *arg1)
{
    return (unsigned char *)TaLogic_RootTreeAloc(arg0, arg1);
}
void func_0027af80_tree(void *arg0, DotrTreeWord arg1, int arg2, int arg3)
{
    TaLogic_SetRootTreeDat(arg0, (struct PieceTreetag *)arg1, arg2, arg3);
}
int func_002798c0_tree(DotrTreeWord arg0, int arg1, int arg2, int arg3, int arg4, DotrTreeWord arg5)
{
    return TaLogic_IsPassRoot2((struct jp_9a533 *)arg0, arg1, arg2, arg3, arg4, (unsigned short *)arg5);
}
int func_0027b010_tree(void *arg0, int arg1, int arg2, int arg3, int arg4, unsigned char arg5)
{
    return TaLogic_IsRootPast2(arg0, arg1, arg2, arg3, arg4, arg5);
}

static void reset(void)
{
    memset(pool, 0, sizeof(pool));
    memset(pass_last, 0, sizeof(pass_last));
    pool_used = set_count = rev_calls = pass_calls = past_calls = 0;
    pool_limit = 64;
    pass_skip = 0;
    move_rev = 1;
    pass_result = 1;
    past_result = 0;
}

static struct PieceTreetag *run(int invade, struct PieceTreetag *tree, struct jp_9a533 *info, int x, int z, int gx, int gz,
                                unsigned char dist, unsigned char *end, unsigned char depth)
{
    static unsigned short size, count, ignore, unit;
    static int unit_i;

    if (invade) {
        return TaLogic_GetInvadeRootTree2(tree, NULL, info, x, z, gx, gz, dist, end, &unit_i, 77, 9, depth, &size, &count, &ignore);
    }
    return (struct PieceTreetag *)TaLogic_GetRootTree2((unsigned char *)tree, 0, (DotrTreeWord)info, x, z, gx, gz, dist,
                                                       (signed char *)end, (DotrTreeWord)&unit, depth, &size, &count,
                                                       (DotrTreeWord)&ignore);
}

static void check_tree(int invade)
{
    struct jp_9a533 *info = (struct jp_9a533 *)pool;
    unsigned char end = 0;
    struct PieceTreetag *root;
    int i;

    /* No variable information: the move range is still read, nothing else happens. */
    reset();
    assert(run(invade, NULL, NULL, 3, 3, 6, 6, 1, &end, 0) == NULL);
    assert(rev_calls == 1 && pool_used == 0);

    /* New node deeper than the distance limit. */
    reset();
    assert(run(invade, NULL, info, 3, 3, 6, 6, 1, &end, 2) == NULL);
    assert(pool_used == 0);
    /* Depth equal to the limit still allocates. */
    reset();
    pass_result = 0;
    assert(run(invade, NULL, info, 3, 3, 6, 6, 1, &end, 1) == &pool[0]);

    /* Allocation fails. */
    reset();
    pool_limit = 0;
    assert(run(invade, NULL, info, 3, 3, 6, 6, 1, &end, 0) == NULL);
    assert(set_count == 0 && size_seen != NULL && count_seen != NULL);

    /* The new node is the goal: flag set, no scan. */
    reset();
    assert(run(invade, NULL, info, 3, 3, 3, 3, 1, &end, 0) == &pool[0]);
    assert(end == 1 && set_count == 1 && set_log[0][0] == 3 && set_log[0][1] == 3 && pass_calls == 0);
    /* Only one coordinate equal is not the goal. */
    reset();
    end = 0;
    pass_result = 0;
    assert(run(invade, NULL, info, 3, 3, 3, 4, 1, &end, 0) == &pool[0]);
    assert(end == 0 && pass_calls == 4);

    /* Range 1 in the middle: four neighbours in x-then-z order, each one level deep. */
    reset();
    root = run(invade, NULL, info, 3, 3, 6, 6, 1, &end, 0);
    assert(root == &pool[0] && pool_used == 5 && set_count == 5 && end == 0);
    assert(set_log[1][0] == 2 && set_log[1][1] == 3 && set_prev[1] == root);
    assert(set_log[2][0] == 3 && set_log[2][1] == 2);
    assert(set_log[3][0] == 3 && set_log[3][1] == 4);
    assert(set_log[4][0] == 4 && set_log[4][1] == 3);
    for (i = 0; i < 4; i++) {
        assert(root->pNextP[i] == &pool[i + 1]);
    }
    assert(root->pNextP[4] == NULL);
    /* The children's own neighbours are past the limit and stay empty. */
    assert(pool[1].pNextP[0] == NULL && pool[1].pNextP[3] == NULL);
    assert(pass_calls == 4 + 4 * 4 && past_calls == pass_calls);
    if (invade) {
        /* Node0 is 1 only for the depth 0 caller; fView and bWarnLvl are passed on. */
        assert(pass_last[4] == 77 && pass_last[7] == 9 && pass_last[5] == 11 && pass_last[6] == 10);
    }

    /* A neighbour that is the goal sets the flag; the scan goes on. */
    reset();
    root = run(invade, NULL, info, 3, 3, 3, 2, 1, &end, 0);
    assert(end == 1 && pool_used == 5);

    /* Corner: the high bound is capped at 6, the low bound at 0. */
    reset();
    end = 0;
    root = run(invade, NULL, info, 6, 0, 3, 3, 0, &end, 0);
    assert(pool_used == 1 && pass_calls == 2);
    assert(pass_last[0] == 6 && pass_last[1] == 0 && pass_last[2] == 6 && pass_last[3] == 1);

    /* Only the low four bits of the move word are the range. */
    reset();
    move_rev = 0x12;
    run(invade, NULL, info, 3, 3, 6, 6, 0, &end, 0);
    assert(pass_calls == 12);

    /* The path check refuses: no visited check, no child. */
    reset();
    pass_result = 0;
    root = run(invade, NULL, info, 3, 3, 6, 6, 1, &end, 0);
    assert(pool_used == 1 && past_calls == 0 && root->pNextP[0] == NULL);

    /* Already visited: no child. */
    reset();
    past_result = 1;
    root = run(invade, NULL, info, 3, 3, 6, 6, 1, &end, 0);
    assert(pool_used == 1 && past_calls == 4 && root->pNextP[0] == NULL);

    /* An existing node is scanned without allocation or the depth test. */
    reset();
    root = run(invade, &pool[40], info, 3, 3, 3, 3, 0, &end, 5);
    assert(root == &pool[40] && pool_used == 0 && pass_calls == 4 && end == 0);
}

/* The two builders differ in where the child index advances: the plain one only after a child
 * call, the invade one for every cell in range. The first cell is refused here. */
static void check_child_index(void)
{
    struct jp_9a533 *info = (struct jp_9a533 *)pool;
    unsigned char end = 0;
    struct PieceTreetag *root;

    reset();
    pass_skip = 1;
    root = run(0, &pool[40], info, 3, 3, 6, 6, 1, &end, 0);
    assert(pool_used == 3 && root->pNextP[0] == &pool[0] && root->pNextP[2] == &pool[2] && root->pNextP[3] == NULL);

    reset();
    pass_skip = 1;
    root = run(1, &pool[40], info, 3, 3, 6, 6, 1, &end, 0);
    assert(pool_used == 3 && root->pNextP[0] == NULL && root->pNextP[1] == &pool[0] && root->pNextP[3] == &pool[2]);
}

static int field[7][7], root_dist[7][7], root_calls, root_bad;

int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    return field[x][y];
}

int TaLogic_GetRoot2(struct jp_9a533 *pVarInfo, struct jp_9b2bd *pMovePiece, int nX, int nZ, int gX, int gZ,
                     unsigned short gUnitID, unsigned char bAbort)
{
    int n;

    for (n = 0; n < 12; n++) {
        if (pMovePiece[n].nX != 15 || pMovePiece[n].nZ != 15) {
            root_bad++;
        }
    }
    if (pVarInfo != (struct jp_9a533 *)pool || nX != 4 || nZ != 2 || gUnitID != 0 || bAbort != 30) {
        root_bad++;
    }
    root_calls++;
    return root_dist[gX][gZ];
}

/* View that TaLogic_GetLandChange calls (driver_tree.h), forwarding to the stub. */
int func_002792c0_tree(DotrTreeWord arg0, DotrTreeNibbles *arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);
int func_002792c0_tree(DotrTreeWord arg0, DotrTreeNibbles *arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    return TaLogic_GetRoot2((struct jp_9a533 *)arg0, (struct jp_9b2bd *)arg1, arg2, arg3, arg4, arg5, (unsigned short)arg6, (unsigned char)arg7);
}

static int land(unsigned char kind, unsigned char change, unsigned char range)
{
    root_calls = 0;
    return TaLogic_GetLandChange((DotrTreeWord)pool, 4, 2, kind, change, range);
}

static void fill(int f, int d)
{
    int x, z;

    for (x = 0; x < 7; x++) {
        for (z = 0; z < 7; z++) {
            field[x][z] = f;
            root_dist[x][z] = d;
        }
    }
}

static void check_land_change(void)
{
    /* Every cell already has the wanted kind: nothing counts, no route is asked for. */
    fill(1, 5);
    assert(land(1, 0, 0) == -1 && root_calls == 0);
    /* Kind 8 never counts either. */
    fill(8, 5);
    assert(land(1, 0, 2) == -1 && root_calls == 0);

    /* One other cell, range 0: the result packs z in the high byte and x in the low byte. */
    fill(1, 5);
    field[2][5] = 3;
    assert(land(1, 0, 0) == ((5 << 8) | 2) && root_calls == 1 && root_bad == 0);
    /* The count has to exceed bCngNum. */
    assert(land(1, 1, 0) == -1 && root_calls == 0);
    /* A route of 14 or more is not taken; 13 is. */
    root_dist[2][5] = 14;
    assert(land(1, 0, 0) == -1 && root_calls == 1);
    root_dist[2][5] = 13;
    assert(land(1, 0, 0) == ((5 << 8) | 2));

    /* Equal counts: a later cell wins only with a strictly shorter route. */
    fill(1, 5);
    field[1][1] = 3;
    field[5][5] = 3;
    root_dist[1][1] = 10;
    root_dist[5][5] = 4;
    assert(land(1, 0, 0) == ((1 << 8) | 1) + 0x404 && root_calls == 2);
    root_dist[5][5] = 10;
    assert(land(1, 0, 0) == ((1 << 8) | 1) && root_calls == 2);

    /* Range 1 around one cell: five cells see it, the first in x-then-z order is kept. */
    fill(1, 5);
    field[1][1] = 3;
    assert(land(1, 0, 1) == ((1 << 8) | 0) && root_calls == 5);

    /* Two neighbours in the corner, range 1: only cells that reach the best count ask for a route,
     * and the window is cut at the board edge. */
    fill(1, 3);
    field[0][0] = 3;
    field[0][1] = 3;
    assert(land(1, 0, 1) == 0 && root_calls == 2);
    /* Far corner, range 2: the high bounds are capped at 6. */
    fill(1, 3);
    field[6][6] = 3;
    assert(land(1, 0, 2) == ((6 << 8) | 4) && root_calls == 6);
    assert(root_bad == 0);
}

static unsigned int hsync;
static void *map_unit[7][7];
static int map_id[7][7], control_side, adjust_result, adjust_calls, adjust_last[2], win_result, win_calls, win_last[5];
static struct jp_9b50e unit_list[24];
static unsigned short unit_num;

unsigned int SzGame_GetHSyncCount(void)
{
    return hsync;
}

void *SzDuel_GetMapUnit(int x, int y)
{
    return map_unit[x][y];
}

int SzDuel_GetMapUnitID(int x, int y)
{
    return map_id[x][y];
}

int Ta_IsUnitControl2(struct jp_9a533 *vInfo)
{
    assert(vInfo != NULL);
    return control_side;
}

int TaLogic_IsAdjustUnitLst(struct jp_9b50e *nUnitLst, int Unit, int bDist)
{
    assert(nUnitLst == unit_list);
    adjust_last[0] = Unit;
    adjust_last[1] = bDist;
    adjust_calls++;
    return adjust_result;
}

int TaLogic_IsUnitWin4(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z, unsigned char fView)
{
    (void)pVarInfo;
    win_last[0] = wUnit; win_last[1] = bWarnLvl; win_last[2] = x; win_last[3] = z; win_last[4] = fView;
    win_calls++;
    return win_result;
}

static struct PieceTreetag *near_run(struct PieceTreetag *tree, struct jp_9a533 *info, int x, int z, unsigned char dist,
                                     unsigned char depth)
{
    static unsigned short size, count, ignore;

    return TaLogic_GetNearUnitPickUpTree(tree, NULL, info, x, z, 2, dist, 77, 9, depth, &size, &count, unit_list,
                                         &unit_num, &ignore);
}

static void near_reset(void)
{
    reset();
    memset(map_unit, 0, sizeof(map_unit));
    memset(unit_list, 0, sizeof(unit_list));
    hsync = 12000;
    unit_num = 0;
    control_side = 2;
    adjust_result = adjust_calls = win_result = win_calls = 0;
}

/* TaLogic_GetNearUnitPickUpTree. One case per branch outcome: no variable information; the
 * sync count above and at 12000; a new node deeper than, and at, the distance limit; a failed
 * allocation; no unit on the node, a unit of the other side, a unit of the wanted side (listed,
 * already listed, list full at 22 and not full at 21); the win check below zero and at zero;
 * the scan (range, caps at 0 and 6, range from the low four bits, refused path, visited cell,
 * child index advancing only after a child call); an existing node. */
static void check_near_tree(void)
{
    struct jp_9a533 *info = (struct jp_9a533 *)pool;
    struct PieceTreetag *root;
    int i;

    near_reset();
    assert(near_run(NULL, NULL, 3, 3, 1, 0) == NULL && rev_calls == 1 && pool_used == 0);

    near_reset();
    hsync = 12001;
    assert(near_run(NULL, info, 3, 3, 1, 0) == NULL && pool_used == 0);

    near_reset();
    assert(near_run(NULL, info, 3, 3, 1, 2) == NULL && pool_used == 0);
    near_reset();
    pass_result = 0;
    assert(near_run(NULL, info, 3, 3, 1, 1) == &pool[0]);

    near_reset();
    pool_limit = 0;
    assert(near_run(NULL, info, 3, 3, 1, 0) == NULL && set_count == 0 && size_seen != NULL && count_seen != NULL);

    /* Range 1 in the middle, sync count at the limit, no unit: four children one level deep. */
    near_reset();
    root = near_run(NULL, info, 3, 3, 1, 0);
    assert(root == &pool[0] && pool_used == 5 && set_count == 5 && adjust_calls == 0 && win_calls == 0);
    assert(set_log[0][0] == 3 && set_log[0][1] == 3);
    assert(set_log[1][0] == 2 && set_log[1][1] == 3 && set_prev[1] == root);
    assert(set_log[2][0] == 3 && set_log[2][1] == 2);
    assert(set_log[3][0] == 3 && set_log[3][1] == 4);
    assert(set_log[4][0] == 4 && set_log[4][1] == 3);
    for (i = 0; i < 4; i++) {
        assert(root->pNextP[i] == &pool[i + 1]);
    }
    assert(root->pNextP[4] == NULL && pool[1].pNextP[0] == NULL);
    assert(pass_calls == 4 + 4 * 4 && past_calls == pass_calls);
    assert(pass_last[4] == 77 && pass_last[7] == 9 && pass_last[5] == 11 && pass_last[6] == 10);

    /* A unit of the other side on the node: not listed, not checked. */
    near_reset();
    map_unit[3][3] = pool;
    map_id[3][3] = 55;
    control_side = 1;
    assert(near_run(NULL, info, 3, 3, 0, 0) == &pool[0]);
    assert(adjust_calls == 0 && win_calls == 0 && unit_num == 0 && pass_calls == 4);

    /* A unit of the wanted side: listed with the depth, win check at zero lets the scan run. */
    near_reset();
    map_unit[3][3] = pool;
    assert(near_run(NULL, info, 3, 3, 0, 0) == &pool[0]);
    assert(adjust_calls == 1 && adjust_last[0] == 55 && adjust_last[1] == 0);
    assert(unit_num == 1 && unit_list[0].nUnitID == 55 && unit_list[0].bDist == 0);
    assert(win_calls == 1 && win_last[0] == 55 && win_last[1] == 9 && win_last[2] == 3 && win_last[3] == 3 && win_last[4] == 77);
    assert(pass_calls == 4);

    /* Already in the list: not added again, still checked. */
    near_reset();
    map_unit[3][3] = pool;
    adjust_result = 1;
    assert(near_run(NULL, info, 3, 3, 0, 0) == &pool[0] && unit_num == 0 && win_calls == 1);

    /* The list holds 22 entries at most. */
    near_reset();
    map_unit[3][3] = pool;
    unit_num = 22;
    assert(near_run(NULL, info, 3, 3, 0, 0) == &pool[0] && unit_num == 22 && unit_list[22].nUnitID == 0);
    near_reset();
    map_unit[3][3] = pool;
    unit_num = 21;
    assert(near_run(NULL, info, 3, 3, 3, 3) == &pool[0] && unit_num == 22);
    assert(unit_list[21].nUnitID == 55 && unit_list[21].bDist == 3);

    /* Win check below zero: the node is returned without a scan. */
    near_reset();
    map_unit[3][3] = pool;
    win_result = -1;
    assert(near_run(NULL, info, 3, 3, 0, 0) == &pool[0] && pass_calls == 0);

    /* Corner: the high bound is capped at 6, the low bound at 0. */
    near_reset();
    near_run(NULL, info, 6, 0, 0, 0);
    assert(pool_used == 1 && pass_calls == 2);
    assert(pass_last[0] == 6 && pass_last[1] == 0 && pass_last[2] == 6 && pass_last[3] == 1);
    near_reset();
    near_run(NULL, info, 0, 6, 0, 0);
    assert(pass_calls == 2 && pass_last[2] == 1 && pass_last[3] == 6);

    /* Only the low four bits of the move word are the range. */
    near_reset();
    move_rev = 0x12;
    near_run(NULL, info, 3, 3, 0, 0);
    assert(pass_calls == 12);

    /* The path check refuses: no visited check, no child. Already visited: no child. */
    near_reset();
    pass_result = 0;
    root = near_run(NULL, info, 3, 3, 1, 0);
    assert(pool_used == 1 && past_calls == 0 && root->pNextP[0] == NULL);
    near_reset();
    past_result = 1;
    root = near_run(NULL, info, 3, 3, 1, 0);
    assert(pool_used == 1 && past_calls == 4 && root->pNextP[0] == NULL);

    /* An existing node is scanned without allocation, depth test or unit check; the child
     * index advances only after a child call (the first cell is refused here). */
    near_reset();
    map_unit[3][3] = pool;
    root = near_run(&pool[40], info, 3, 3, 0, 5);
    assert(root == &pool[40] && adjust_calls == 0 && win_calls == 0 && pool_used == 0 && pass_calls == 4);
    near_reset();
    pass_skip = 1;
    root = near_run(&pool[40], info, 3, 3, 1, 0);
    assert(pool_used == 3 && root->pNextP[0] == &pool[0] && root->pNextP[2] == &pool[2] && root->pNextP[3] == NULL);
}

int main(void)
{
    check_near_tree();
    check_tree(0);
    check_tree(1);
    check_child_index();
    check_land_change();
    return 0;
}
