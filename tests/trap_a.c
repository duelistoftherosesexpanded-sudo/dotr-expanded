/* Synthetic host tests for the trap_a group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/trap_a.h"

/* The moving unit of every test, and the trap units' side. */
static int m_side, m_id, m_x, m_z;
#define ENEMY (m_side == 0 ? 1 : 0)
#define UNIT(n) ((ENEMY << 8) | (n))

/* Fixtures the stubs answer from, by (side, id). */
static int kind[2][16], status[2][16], kind_ofs[2][16];
static unsigned words[2][16][2];
static int map[7][7];
/* Leader side: range, abilities by number (the value is returned), object answers. */
static int ld_range, abl[32], dest_obj, curse_obj;
static unsigned mover_words[2];
static int mover_has_words;
static int nat15, trap_b, cap_result, leader_loc, range_abl, resist, ap;
/* The call of Ta_GetCurrentSide (counted from 1) that answers `side_force` instead. */
static int side_calls, side_force_at, side_force;

/* What was called. */
static int ld_range_calls, ld_range_args[3];
static int abl_asked[8], abl_n;
static int dest_calls, dest_args[2], curse_calls, curse_args[2];
static int conv_calls;
static int nat_calls, map_calls, trap_b_calls, cap_calls, cap_value;
static int sort_calls, sort_high, sorted[5];
static int loc_calls, ofs_calls, range_calls, resist_calls, ap_calls;
static int visual_calls, visual[7];
static int effect_calls[2];

int Ta_GetCurrentSide(int a, int b)
{
    assert((a == 0 || a == 1) && b >= 0 && b < 16);
    side_calls++;
    if (side_calls == side_force_at) {
        return side_force;
    }
    return a;
}
int Ta_IsLDRange(int a, int b, int c)
{
    ld_range_calls++;
    ld_range_args[0] = a;
    ld_range_args[1] = b;
    ld_range_args[2] = c;
    return ld_range;
}
int SzDuel_GetUnitKind(int a, int b)
{
    assert((a == 0 || a == 1) && b >= 0 && b < 16);
    return kind[a][b];
}
int Ta_IsLDAbl(int a, int b)
{
    assert(a >= 0 && a < 32 && abl_n < 8);
    /* 9 is asked about the moving unit's side, the others about its opponent. */
    assert(b == (a == 9 ? m_side : ENEMY));
    abl_asked[abl_n++] = a;
    return abl[a];
}
int Ta_IsLD_DestObj(int a, int b)
{
    dest_calls++;
    dest_args[0] = a;
    dest_args[1] = b;
    return dest_obj;
}
int Ta_IsLD_CurseObj(int a, int b)
{
    curse_calls++;
    curse_args[0] = a;
    curse_args[1] = b;
    return curse_obj;
}
unsigned ConvUnitIDtoFlagList(unsigned word, int high, int position)
{
    assert(word == 0 && high == m_side && position == m_id);
    conv_calls++;
    return 0x40u + (unsigned)conv_calls;        /* 0x41, 0x42, ... in call order */
}
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert((a == 0 || a == 1) && b >= 0 && b < 16);
    return (status[a][b] & bit) != 0;
}
unsigned *func_00250740_trapa(int a, int b, int n)
{
    assert((a == 0 || a == 1) && b >= 0 && b < 16 && (n == 0 || n == 1));
    effect_calls[n]++;
    if (n == 1) {
        assert(a == m_side && b == m_id);
        return mover_has_words ? mover_words : NULL;
    }
    return words[a][b];
}
int Ta_SearchUncTypeNaturalEffectUserNum(int k)
{
    assert(k == 15);
    nat_calls++;
    return nat15;
}
int SzDuel_GetMapUnitID(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    /* Column by column, each from row 0 to 6. */
    assert(map_calls == x * 7 + y);
    map_calls++;
    return map[x][y];
}
int Ta_IsActiveTrapB(int side, int id)
{
    assert(side == 0 && id == 46);
    trap_b_calls++;
    return trap_b;
}
int GetCap(int limit, int value)
{
    assert(limit == 0);
    cap_calls++;
    cap_value = value;
    return cap_result;
}
void SortByOrder(int *w, int low, int high)
{
    assert(low == 0);
    sort_calls++;
    sort_high = high;
    memcpy(sorted, w, sizeof sorted);
}
int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == m_side && b == 0);
    loc_calls++;
    return leader_loc;
}
int SzDuel_GetUnitKindOfs(int a, int b)
{
    assert((a == 0 || a == 1) && b >= 0 && b < 16);
    ofs_calls++;
    return kind_ofs[a][b];
}
int Ta_IsLDRangeAvilityWithLocation(int a, int b, int c, int d, int e)
{
    assert(a == m_side && b == m_id && c == m_x && d == m_z && e == 11);
    range_calls++;
    return range_abl;
}
void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int k)
{
    visual_calls++;
    visual[0] = x;
    visual[1] = y;
    visual[2] = c;
    visual[3] = reach;
    visual[4] = e;
    visual[5] = mode;
    visual[6] = k;
}
int Ta_IsResistAll(int a, int b)
{
    assert(a == m_side && b == m_id);
    resist_calls++;
    return resist;
}
int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    assert(a == m_side && b == m_id && c == m_x && d == m_z);
    ap_calls++;
    return ap;
}

static struct jp_7b52 info;

/* A quiet board: unit (side, 3) moves to (3, 3); nothing answers. */
static void reset(int side)
{
    int x, y;

    m_side = side;
    m_id = 3;
    m_x = 3;
    m_z = 3;
    memset(kind, 0, sizeof kind);
    memset(status, 0, sizeof status);
    memset(kind_ofs, 0, sizeof kind_ofs);
    memset(words, 0, sizeof words);
    memset(abl, 0, sizeof abl);
    for (x = 0; x < 7; x++) {
        for (y = 0; y < 7; y++) {
            map[x][y] = -1;
        }
    }
    kind[m_side][m_id] = 0x05;          /* class 0, low bits 5 */
    ld_range = dest_obj = curse_obj = 0;
    mover_has_words = 0;
    mover_words[0] = mover_words[1] = 0;
    nat15 = 0;
    trap_b = -1;
    cap_result = 0;
    leader_loc = 0x0206;
    range_abl = resist = 0;
    ap = 0;
    side_force_at = 0;
}

/* Puts trap unit n of the other side (kind class 3) on a cell. */
static void trap(int n, int x, int y, int ofs, unsigned par)
{
    map[x][y] = UNIT(n);
    kind[ENEMY][n] = 0x60 | n;
    kind_ofs[ENEMY][n] = ofs;
    words[ENEMY][n][0] = 0x700;         /* must not matter for a trap unit */
    words[ENEMY][n][1] = par;
}

static int run(int pr)
{
    side_calls = 0;
    ld_range_calls = abl_n = dest_calls = curse_calls = conv_calls = 0;
    nat_calls = map_calls = trap_b_calls = cap_calls = sort_calls = 0;
    loc_calls = ofs_calls = range_calls = resist_calls = ap_calls = visual_calls = 0;
    effect_calls[0] = effect_calls[1] = 0;
    info.eNO = -77;
    info.dwFrom = info.dwTo = info.dwPar = 0x77777777u;
    return Chk_TrapA(m_side, m_id, m_x, m_z, &info, pr);
}

static int untouched(void)
{
    return info.eNO == -77 && info.dwFrom == 0x77777777u && info.dwTo == 0x77777777u && info.dwPar == 0x77777777u;
}

static int is(int no, unsigned from, unsigned to, unsigned par)
{
    return info.eNO == no && info.dwFrom == from && info.dwTo == to && info.dwPar == par;
}

static void check_entry(void)
{
    reset(0);
    m_id = 0;
    kind[0][0] = 0x05;
    ld_range = 1;
    abl[16] = dest_obj = 1;
    assert(run(0) == 0 && untouched());
    /* Only the current side was asked. */
    assert(side_calls == 1 && ld_range_calls == 0 && effect_calls[1] == 0 && map_calls == 0);

    /* A quiet board: the whole 7x7 scan runs and finds nothing. */
    reset(0);
    assert(run(0) == 0 && untouched());
    assert(ld_range_calls == 1 && ld_range_args[0] == 1 && ld_range_args[1] == 3 && ld_range_args[2] == 3);
    assert(effect_calls[1] == 1 && nat_calls == 1 && map_calls == 49);
    assert(trap_b_calls == 0 && sort_calls == 0 && loc_calls == 0);
}

static void check_leader(void)
{
    /* Low 16 bits of PrNo non-zero: the leader part is not asked at all. */
    reset(0);
    ld_range = 1;
    abl[16] = abl[15] = abl[13] = dest_obj = curse_obj = 1;
    assert(run(1) == 0 && untouched() && ld_range_calls == 0 && abl_n == 0);
    assert(run(0x10000 | 0x8000) == 0 && ld_range_calls == 0);
    /* Bit 16 alone leaves it on. */
    assert(run(0x10000) == 257 && ld_range_calls == 1);

    /* Out of the leader's range. */
    ld_range = 0;
    assert(run(0) == 0 && untouched() && ld_range_calls == 1 && abl_n == 0);
    assert(ld_range_args[0] == 1 && ld_range_args[1] == m_x && ld_range_args[2] == m_z);

    /* Ability 16 with its object test: first flag list only. */
    reset(0);
    ld_range = 1;
    abl[16] = dest_obj = 1;
    assert(run(0) == 257 && is(113, 0x41, 0, 0x80000000u));
    assert(abl_n == 2 && abl_asked[0] == 16 && abl_asked[1] == 15);
    assert(dest_calls == 1 && dest_args[0] == 1 && dest_args[1] == 5 && curse_calls == 0);
    assert(effect_calls[1] == 0 && map_calls == 0);
    /* Ability 16 without its object: nothing, and the scan follows. */
    dest_obj = 0;
    assert(run(0) == 0 && untouched() && dest_calls == 1 && conv_calls == 0 && map_calls == 49);
    assert(abl_n == 3 && abl_asked[2] == 13);

    /* Ability 15 with its object test: second flag list only. */
    reset(0);
    ld_range = 1;
    abl[15] = curse_obj = 1;
    kind[0][3] = 0x1f;
    assert(run(0) == 257 && is(113, 0, 0x41, 0x80000000u));
    assert(curse_calls == 1 && curse_args[0] == 1 && curse_args[1] == 0x1f && dest_calls == 0);
    curse_obj = 0;
    assert(run(0) == 0 && untouched() && curse_calls == 1);

    /* Both. */
    abl[16] = dest_obj = curse_obj = 1;
    assert(run(0) == 257 && is(113, 0x41, 0x42, 0x80000000u) && conv_calls == 2);

    /* A unit of another kind class is not asked about 16 and 15, only about 13. */
    kind[0][3] = 0x25;
    assert(run(0) == 0 && untouched() && abl_n == 1 && abl_asked[0] == 13 && dest_calls == 0 && curse_calls == 0);

    /* Ability 13: needs status bits 2 and 4 both clear. */
    abl[13] = 1;
    assert(run(0) == 0x10201 && is(113, 0, 0, 0x80000041u) && conv_calls == 1 && effect_calls[1] == 0);
    status[0][3] = 2;
    assert(run(0) == 0 && untouched() && conv_calls == 0 && map_calls == 49);
    status[0][3] = 4;
    assert(run(0) == 0 && untouched() && conv_calls == 0);
    status[0][3] = 1 | 8;
    assert(run(0) == 0x10201);
    /* Class 0 with 16 and 15 failing still reaches 13. */
    kind[0][3] = 0x05;
    dest_obj = curse_obj = 0;
    status[0][3] = 0;
    assert(run(0) == 0x10201 && is(113, 0, 0, 0x80000041u) && abl_n == 3);

    /* The other side as mover: the leader asked is side 0. */
    reset(1);
    ld_range = 1;
    abl[16] = dest_obj = 1;
    assert(run(0) == 257 && is(113, 0x41, 0, 0x80000000u));
    assert(ld_range_args[0] == 0 && dest_args[0] == 0);
}

static void check_early_outs(void)
{
    reset(0);
    trap(1, 3, 2, 0, 9);

    /* The moving unit's own effect word 0 with 7 above the low byte. */
    mover_has_words = 1;
    mover_words[0] = 0x700;
    assert(run(0) == 0 && untouched() && nat_calls == 0 && map_calls == 0);
    mover_words[0] = 0x7ff;
    assert(run(0) == 0 && untouched() && map_calls == 0);
    mover_words[0] = 0x800;
    assert(run(0) == 257 && map_calls == 49);
    mover_words[0] = 0x6ff;
    assert(run(0) == 257);
    mover_words[0] = 0x10700;
    assert(run(0) == 257);
    mover_has_words = 0;
    mover_words[0] = 0x700;             /* no record: the word is not read */
    assert(run(0) == 257);

    nat15 = 1;
    assert(run(0) == 0 && untouched() && nat_calls == 1 && map_calls == 0);
    nat15 = 0;
    assert(run(0) == 257);
    nat15 = -1;
    assert(run(0) == 257);
}

static void check_scan(void)
{
    int n;

    /* Distance: the cell itself and its four neighbours count; diagonals and two away do not. */
    reset(0);
    trap(1, 2, 2, 0, 9);
    trap(2, 4, 4, 0, 9);
    trap(3, 2, 4, 0, 9);
    trap(4, 4, 2, 0, 9);
    trap(5, 3, 5, 0, 9);
    trap(6, 1, 3, 0, 9);
    trap(7, 3, 1, 0, 9);
    trap(8, 5, 3, 0, 9);
    assert(run(0) == 0 && untouched() && map_calls == 49 && trap_b_calls == 0 && cap_calls == 0);

    /* All five cells in reach: the list is in scan order (column, then row). */
    reset(0);
    trap(1, 3, 3, 13, 0);
    trap(2, 3, 2, 13, 0);
    trap(3, 3, 4, 13, 0);
    trap(4, 2, 3, 13, 0);
    trap(5, 4, 3, 13, 0);
    cap_result = 4;
    assert(run(0) == 0 && untouched());
    assert(trap_b_calls == 1 && cap_calls == 1 && cap_value == 4 && sort_calls == 1 && sort_high == 4 && loc_calls == 1);
    assert(sorted[0] == UNIT(4) && sorted[1] == UNIT(2) && sorted[2] == UNIT(1) && sorted[3] == UNIT(3) && sorted[4] == UNIT(5));
    assert(ofs_calls == 5 && effect_calls[0] == 5);
    /* The sort's upper bound is what GetCap answers, not the count. */
    cap_result = 2;
    assert(run(0) == 0 && sort_high == 2 && cap_value == 4);

    /* One unit: unused list entries are -1. */
    reset(0);
    trap(2, 3, 4, 13, 0);
    assert(run(0) == 0 && cap_value == 0 && sorted[0] == UNIT(2));
    for (n = 1; n < 5; n++) {
        assert(sorted[n] == -1);
    }

    /* A unit of the moving unit's own side is not a trap. */
    reset(0);
    map[3][2] = (0 << 8) | 7;
    kind[0][7] = 0x60;
    assert(run(0) == 0 && untouched() && trap_b_calls == 0);
    /* Nor is one whose kind class is not 3. */
    reset(0);
    trap(1, 3, 2, 0, 9);
    kind[1][1] = 0x40;
    assert(run(0) == 0 && untouched() && trap_b_calls == 0);
    kind[1][1] = 0x80;
    assert(run(0) == 0 && trap_b_calls == 0);
    kind[1][1] = 0x7f;
    assert(run(0) == 257 && trap_b_calls == 1);

    /* The second comparison of sides, asked again for the moving unit: calls 1 (entry),
     * 2 (the unit), then 3 (moving unit) and 4 (the unit). */
    reset(0);
    trap(1, 3, 2, 0, 9);
    assert(run(0) == 257 && side_calls == 4);
    side_force_at = 3;
    side_force = 1;
    assert(run(0) == 0 && untouched() && side_calls == 4 && trap_b_calls == 0);
    side_force_at = 0;

    /* Bit 16 of PrNo: only units with status bit 4 count. */
    reset(0);
    trap(1, 3, 2, 0, 9);
    trap(2, 3, 4, 0, 8);
    assert(run(0x10000) == 0 && untouched() && trap_b_calls == 0);
    status[1][2] = 4;
    assert(run(0x10000) == 257 && is(44, (unsigned)UNIT(2), 3, 8) && cap_value == 0);
    status[1][1] = 1 | 2 | 8;
    assert(run(0x10000) == 257 && info.dwFrom == (unsigned)UNIT(2));
    /* Other bits of the upper half do not turn the gate on. */
    assert(run(0x20000) == 257 && is(44, (unsigned)UNIT(1), 3, 9) && cap_value == 1);
    assert(run(0x30000) == 257 && info.dwFrom == (unsigned)UNIT(2));
}

static void check_trap_b(void)
{
    reset(0);
    trap(1, 3, 2, 0, 9);
    trap_b = 0;
    assert(run(0) == 1 && is(102, 1, 0, 0) && sort_calls == 0 && loc_calls == 0 && ofs_calls == 0);
    trap_b = 0x123;
    assert(run(0) == 1 && is(102, 1, 0x123, 0));
    trap_b = -1;
    assert(run(0) == 257 && sort_calls == 1);
    trap_b = -2;
    assert(run(0) == 257);
}

/* One trap unit of the given kind offset next to the moving unit; returns the result. */
static int one(int ofs, int pr)
{
    kind_ofs[ENEMY][1] = ofs;
    return run(pr);
}

static void check_cases(void)
{
    static const int limit[5] = {1001, 1501, 2001, 3001, 4001};
    int ofs, side;

    for (side = 0; side < 2; side++) {
        unsigned to;

        reset(side);
        trap(1, 3, 2, 0, 0xabcd1234u);
        to = ((unsigned)side << 8) | 3;

        /* 0-5: effect 44 with the trap unit's word 1. */
        for (ofs = 0; ofs < 6; ofs++) {
            assert(one(ofs, 0) == 257 && is(44, (unsigned)UNIT(1), to, 0xabcd1234u));
            assert(range_calls == 1 && resist_calls == 1 && visual_calls == 0 && ap_calls == 0);
            assert(effect_calls[0] == 1);

            /* The range ability answers first. */
            range_abl = 1;
            abl[9] = 5;
            assert(one(ofs, 0) == 1 && is(254, 0, (unsigned)side << 8, 16));
            assert(visual_calls == 1 && resist_calls == 0);
            assert(visual[0] == 0x06 && visual[1] == 0x02 && visual[2] == 0 && visual[3] == 5);
            assert(visual[4] == 1 && visual[5] == 1 && visual[6] == 11);
            assert(abl_asked[abl_n - 1] == 9);
            range_abl = 0;

            /* A unit that resists everything ends the whole check. */
            resist = 1;
            assert(one(ofs, 0) == 0 && untouched() && resist_calls == 1);
            resist = 0;
        }

        /* 6-10: effect 45 for a kind-class-0 unit under the limit. */
        for (ofs = 6; ofs < 11; ofs++) {
            ap = limit[ofs - 6] - 1;
            assert(one(ofs, 0) == 257 && is(45, (unsigned)UNIT(1), to, 0) && ap_calls == 1);
            ap = limit[ofs - 6];
            assert(one(ofs, 0) == 0 && untouched() && ap_calls == 1 && range_calls == 0);
            ap = 0;
            kind[side][3] = 0x25;
            assert(one(ofs, 0) == 0 && untouched() && ap_calls == 0 && range_calls == 0);
            kind[side][3] = 0x05;

            range_abl = 1;
            abl[9] = 2;
            assert(one(ofs, 0) == 1 && is(254, 0, (unsigned)side << 8, 16) && visual_calls == 1 && visual[3] == 2);
            range_abl = 0;
            resist = 1;
            assert(one(ofs, 0) == 0 && untouched());
            resist = 0;
        }

        /* 11: effect 46, whatever the unit. */
        ap = 99999;
        kind[side][3] = 0x45;
        assert(one(11, 0) == 257 && is(46, (unsigned)UNIT(1), to, 0) && ap_calls == 0);
        range_abl = 1;
        assert(one(11, 0) == 1 && is(254, 0, (unsigned)side << 8, 16));
        range_abl = 0;
        resist = 1;
        assert(one(11, 0) == 0 && untouched());
        resist = 0;

        /* 12: effect 45 for a unit whose kind class is not 0. */
        assert(one(12, 0) == 257 && is(45, (unsigned)UNIT(1), to, 0) && ap_calls == 0);
        range_abl = 1;
        assert(one(12, 0) == 1 && is(254, 0, (unsigned)side << 8, 16));
        range_abl = 0;
        resist = 1;
        assert(one(12, 0) == 0 && untouched());
        resist = 0;
        kind[side][3] = 0x05;
        assert(one(12, 0) == 0 && untouched() && range_calls == 0);

        /* Outside the table. */
        assert(one(13, 0) == 0 && untouched() && ofs_calls == 1);
        assert(one(-1, 0) == 0 && untouched());
    }

    /* A unit that does not fire is passed over for the next one in the list. */
    reset(0);
    trap(1, 2, 3, 6, 0x11);
    trap(2, 3, 3, 12, 0x22);
    trap(3, 4, 3, 2, 0x33);
    ap = 5000;
    cap_result = 2;
    assert(run(0) == 257 && is(44, (unsigned)UNIT(3), 3, 0x33) && ofs_calls == 3 && effect_calls[0] == 3);
    assert(ap_calls == 1 && range_calls == 1);
    ap = 1000;
    assert(run(0) == 257 && is(45, (unsigned)UNIT(1), 3, 0) && ofs_calls == 1);
}

int main(void)
{
    assert(offsetof(struct jp_7b52, dwPar) == 12 && sizeof(struct jp_7b52) == 16);
    check_entry();
    check_leader();
    check_early_outs();
    check_scan();
    check_trap_b();
    check_cases();
    puts("trap_a: ok");
    return 0;
}
