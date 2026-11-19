/* Synthetic host tests for the end_phase group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/end_phase.h"

static int grave[2];
static int status_mask;
static int sb_status[2][12]; /* status bits for Ta_Do_StandbyPhase */
static int n86;
static int exp_calls;
static int exp_args[8][4];

int SzDuel_GetGraveCount(int index)
{
    assert(index == 0 || index == 1);
    return grave[index];
}

int SzDuel_GetUnitCardNo(int a, int b)
{
    assert((a == 0 || a == 1) && b >= 0 && b < 6);
    return 100 * a + b + 7;
}

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert((a == 0 || a == 1) && b >= 0 && b < 12 && (bit == 4 || bit == 5 || bit == 1));
    if (bit == 4 && ((status_mask >> b) & 1)) {
        return 1;
    }
    return (sb_status[a][b] >> bit) & 1;
}

int SzData_CalcExp(int side, int id, int operation, int value)
{
    assert(exp_calls < 8);
    exp_args[exp_calls][0] = side;
    exp_args[exp_calls][1] = id;
    exp_args[exp_calls][2] = operation;
    exp_args[exp_calls][3] = value;
    exp_calls++;
    return 0;
}

/* World for Ta_Do_EndPhase. A map unit id is side << 8 | number and a
 * location is x | z << 8, as the function's own shifts and masks assume. */
static int turn_owner, turn_first, turn_count;
static int ld_abl[2], ld_range[2], dest_kinds, curse_kinds;
static int virus_ret;
static int *virus_arg;
static int unit_loc[2][12], unit_kind[2][12], unit_here[2][12], cur_side[2][12];
static int spell_rest[2][12], curse_rest[2][12], curse_flag[2][12];
static int control[2][12], perm_curse[2][12];
static int map_unit[7][7], field[7][7], map_calls;
static int n85, n71, pref[8], pref_calls, flag_sets;

int SzDuel_GetTurnOwner(void) { return turn_owner; }
int SzDuel_GetTurnFirst(void) { return turn_first; }
int SzDuel_GetTurnCount(void) { return turn_count; }

#define UNIT_OK(a, b) assert(((a) == 0 || (a) == 1) && (b) >= 0 && (b) < 12)

/* Ta_Do_StandbyPhase asks for negative numbers; assumed to answer null. */
void *SzDuel_GetUnit(int a, int b) { if (b < 0) return NULL; UNIT_OK(a, b); return unit_here[a][b] ? &unit_here[a][b] : NULL; }
int SzDuel_GetUnitLoc(int a, int b) { UNIT_OK(a, b); return unit_loc[a][b]; }
int SzDuel_GetUnitKind(int a, int b) { UNIT_OK(a, b); return unit_kind[a][b]; }
int SzDuel_GetUnitSpellRest(int a, int b) { UNIT_OK(a, b); return spell_rest[a][b]; }
int SzDuel_SetUnitSpellRest(int a, int b, int value) { UNIT_OK(a, b); spell_rest[a][b] = value; return 0; }
int SzDuel_GetUnitCurseRest(int a, int b) { UNIT_OK(a, b); return curse_rest[a][b]; }
int SzDuel_SetUnitCurseRest(int a, int b, int value) { UNIT_OK(a, b); curse_rest[a][b] = value; return 0; }
int SzDuel_GetUnitCurseFlag(int a, int b) { UNIT_OK(a, b); return curse_flag[a][b]; }
int SzDuel_SetUnitCurseFlag(int a, int b, int flag) { UNIT_OK(a, b); curse_flag[a][b] = flag; flag_sets++; return 0; }
int Ta_GetCurrentSide(int a, int b) { UNIT_OK(a, b); return cur_side[a][b]; }
int Ta_IsUnitControl(int a, int b) { UNIT_OK(a, b); return control[a][b]; }
int TaPerm_IsPermCurseActive(int a, int b) { UNIT_OK(a, b); return perm_curse[a][b]; }

int SzDuel_GetMapUnitID(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    map_calls++;
    return map_unit[x][y];
}

int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    return field[x][y];
}

int Ta_IsLDAbl(int a, int b) { assert(b == 0 || b == 1); return (ld_abl[b] >> a) & 1; }
int Ta_IsVirusAll(int *out) { virus_arg = out; return virus_ret; }
int Ta_GetLDRange(int a) { assert(a == 0 || a == 1); return ld_range[a]; }
int Ta_IsLD_DestObj(int a, int b) { assert((a == 0 || a == 1) && b >= 0 && b < 32); return (dest_kinds >> b) & 1; }
int Ta_IsLD_CurseObj(int a, int b) { assert((a == 0 || a == 1) && b >= 0 && b < 32); return (curse_kinds >> b) & 1; }
unsigned ConvUnitIDtoFlagList(unsigned word, int high, int position) { return word | 1u << (high * 12 + position); }
int GetCap(int limit, int value) { return value < limit ? limit : value; }

int Ta_IsSquareRange(int nX, int nZ, int mX, int mZ, int R)
{
    int dx = nX < mX ? mX - nX : nX - mX;
    int dz = nZ < mZ ? mZ - nZ : nZ - mZ;

    return dx <= R && dz <= R;
}

int Ta_SearchUncTypeNaturalEffectUserNum(int kind)
{
    assert(kind == 85 || kind == 71 || kind == 86);
    return kind == 85 ? n85 : kind == 86 ? n86 : n71;
}

int Ta_GetSommonPiecePreference(int a, int i)
{
    assert(a == turn_owner && i >= 0 && i < 8);
    pref_calls++;
    return pref[i];
}

static void world_reset(int owner)
{
    int i, j;

    turn_owner = owner;
    turn_first = owner;
    turn_count = 0;
    ld_abl[0] = ld_abl[1] = 0;
    ld_range[0] = ld_range[1] = 1;
    dest_kinds = curse_kinds = 1 << 2;
    virus_ret = 0;
    virus_arg = NULL;
    memset(unit_loc, 0, sizeof unit_loc);
    memset(unit_kind, 0, sizeof unit_kind);
    memset(unit_here, 0, sizeof unit_here);
    memset(cur_side, 0, sizeof cur_side);
    memset(spell_rest, 0, sizeof spell_rest);
    memset(curse_rest, 0, sizeof curse_rest);
    memset(curse_flag, 0, sizeof curse_flag);
    memset(control, 0, sizeof control);
    memset(perm_curse, 0, sizeof perm_curse);
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            map_unit[i][j] = -1;
            field[i][j] = 8;
        }
    }
    for (i = 0; i < 8; i++) {
        pref[i] = -1;
    }
    map_calls = pref_calls = flag_sets = exp_calls = 0;
    n85 = n71 = 0;
    status_mask = 0;
}

static const struct jp_908da untouched = { -7, 0x11111111u, 0x22222222u, 0x33333333u };

/* Steps 0, 1, 5 and the default: each branch outcome, and the turn-count
 * boundary 99/100. */
static void check_end_phase_simple(void)
{
    struct jp_908da info;
    int owner;

    for (owner = 0; owner < 2; owner++) {
        world_reset(owner);
        info = untouched;
        assert(Ta_Do_EndPhase(0, &info) == 0x10000);
        assert(memcmp(&info, &untouched, sizeof info) == 0);
        ld_abl[!owner] = 1 << 5;
        assert(Ta_Do_EndPhase(0, &info) == 0x10000);
        ld_abl[owner] = 1 << 5;
        assert(Ta_Do_EndPhase(0, &info) == 0x10001);
        assert(info.eNO == 8 && info.dwFrom == 0 && info.dwTo == (unsigned)owner && info.dwPar == 50);

        info = untouched;
        assert(Ta_Do_EndPhase(1, &info) == 0x20000);
        assert(virus_arg == (int *)&info);
        virus_ret = 1;
        assert(Ta_Do_EndPhase(1, &info) == 0x20001);
        assert(memcmp(&info, &untouched, sizeof info) == 0);

        turn_count = 500;
        assert(Ta_Do_EndPhase(5, &info) == 0xff0000);
        turn_first = !owner;
        turn_count = 99;
        assert(Ta_Do_EndPhase(5, &info) == 0xff0000);
        turn_count = 100;
        assert(Ta_Do_EndPhase(5, &info) == 0xff0008);
        assert(Ta_Do_EndPhase(6, &info) == 0xff0000);
        assert(Ta_Do_EndPhase(-1, &info) == 0xff0000);
        assert(memcmp(&info, &untouched, sizeof info) == 0);
    }
}

/* One unit on the map for step 2; want is a mask of the lists it must enter
 * (1 ability 16, 2 ability 15, 4 ability 13). */
static void step2_case(int owner, int x, int y, int side, int number, int abl, int kind,
                       int rest, int status, int want)
{
    struct jp_908da info = untouched;
    unsigned bit = 1u << (side * 12 + number);
    int got;

    world_reset(owner);
    unit_loc[owner][0] = 5 | 4 << 8;
    ld_abl[owner] = abl;
    map_unit[x][y] = side << 8 | number;
    cur_side[side][number] = side;
    unit_kind[side][number] = kind;
    spell_rest[side][number] = rest;
    status_mask = status << number;
    got = Ta_Do_EndPhase(2, &info);
    assert(map_calls == 49);
    if (want == 0) {
        assert(got == 0x30000);
        assert(memcmp(&info, &untouched, sizeof info) == 0);
    } else {
        assert(got == 0x30001);
        assert(info.eNO == 0x71);
        assert(info.dwFrom == (((want & 1) ? bit : 0) | 0x80000000u));
        assert(info.dwTo == ((want & 2) ? bit : 0));
        assert(info.dwPar == ((want & 4) ? bit : 0));
    }
}

/* Step 2, branch coverage: empty map, own unit, range boundary (distance 1
 * and 2 on each axis, map index 6), each ability alone and all together,
 * kind accepted / refused / masked to five bits / above 31, unit number 0,
 * spell timer and status bit. */
static void check_end_phase_scan(void)
{
    enum { A16 = 1 << 16, A15 = 1 << 15, A13 = 1 << 13, ALL = A16 | A15 | A13 };
    struct jp_908da info = untouched;
    int owner;

    for (owner = 0; owner < 2; owner++) {
        int foe = !owner;

        world_reset(owner);
        assert(Ta_Do_EndPhase(2, &info) == 0x30000);
        assert(map_calls == 49);
        assert(memcmp(&info, &untouched, sizeof info) == 0);

        step2_case(owner, 6, 5, foe, 3, ALL, 2, 0, 0, 7);
        step2_case(owner, 6, 5, owner, 3, ALL, 2, 0, 0, 0);
        step2_case(owner, 4, 3, foe, 3, ALL, 2, 0, 0, 7);
        step2_case(owner, 3, 4, foe, 3, ALL, 2, 0, 0, 0);
        step2_case(owner, 5, 2, foe, 3, ALL, 2, 0, 0, 0);
        step2_case(owner, 5, 6, foe, 3, ALL, 2, 0, 0, 0);
        step2_case(owner, 6, 5, foe, 3, 0, 2, 0, 0, 0);
        step2_case(owner, 6, 5, foe, 3, A16, 2, 0, 0, 1);
        step2_case(owner, 6, 5, foe, 3, A15, 2, 0, 0, 2);
        step2_case(owner, 6, 5, foe, 3, A13, 2, 0, 0, 4);
        step2_case(owner, 6, 5, foe, 3, ALL, 3, 0, 0, 4);
        step2_case(owner, 6, 5, foe, 3, ALL, 0x22, 0, 0, 4);
        step2_case(owner, 6, 5, foe, 3, ALL, 2, 1, 0, 5);
        step2_case(owner, 6, 5, foe, 3, ALL, 2, 0, 1, 3);
        step2_case(owner, 6, 5, foe, 0, ALL, 2, 0, 0, 0);
        step2_case(owner, 6, 5, foe, 11, ALL, 2, 0, 0, 7);
    }
}

/* Step 3, branch coverage: both experience amounts; curse timers for no
 * timer, 1, 3, the 254/255 boundary, another controller and a set curse
 * flag, with and without the halving effect (85) and the blocking effect
 * (71); spell timers for an absent unit, a unit inside and outside the
 * cursing leader's range, a refused kind, a permanent curse and no leader
 * ability; all 24 curse flags cleared. */
static void step3_case(int owner, int twice, int block, int abl)
{
    static const int rest_in[12] = { 3, 0, 3, 3, 3, 255, 254, 1, 0, 0, 0, 3 };
    struct jp_908da info = untouched;
    int side, j;

    world_reset(owner);
    n85 = twice;
    n71 = block;
    status_mask = 1 << 2 | 1 << 5;
    ld_abl[0] = ld_abl[1] = abl;
    unit_loc[0][0] = 1 | 1 << 8;
    unit_loc[1][0] = 5 | 5 << 8;
    for (side = 0; side < 2; side++) {
        int near = side == 0 ? (5 | 4 << 8) : (1 | 2 << 8);
        int far = side == 0 ? (5 | 3 << 8) : (3 | 1 << 8);

        for (j = 0; j < 12; j++) {
            curse_rest[side][j] = rest_in[j];
            control[side][j] = j == 3 ? !owner : owner;
            curse_flag[side][j] = j == 4;
            spell_rest[side][j] = 9;
            unit_here[side][j] = j != 1;
            unit_kind[side][j] = j == 4 ? 3 : 0x22;
            unit_loc[side][j] = j == 3 || j == 5 ? far : near;
            perm_curse[side][j] = j == 5;
        }
        unit_loc[side][0] = side == 0 ? (1 | 1 << 8) : (5 | 5 << 8);
    }
    assert(Ta_Do_EndPhase(3, &info) == 0x40000);
    assert(memcmp(&info, &untouched, sizeof info) == 0);
    assert(exp_calls == 5);
    for (j = 1; j < 6; j++) {
        assert(exp_args[j - 1][0] == owner);
        assert(exp_args[j - 1][1] == 100 * owner + j + 7);
        assert(exp_args[j - 1][2] == 0);
        assert(exp_args[j - 1][3] == (j == 2 || j == 5 ? 4 : 2));
    }
    assert(flag_sets == 24);
    for (side = 0; side < 2; side++) {
        for (j = 0; j < 12; j++) {
            int rest = rest_in[j];
            int spell = 9;

            if (j >= 1 && block <= 0 && rest > 0 && rest < 255 && j != 3 && j != 4) {
                rest = twice > 0 ? (rest - 2 < 0 ? 0 : rest - 2) : rest - 1;
            }
            assert(curse_rest[side][j] == rest);
            assert(curse_flag[side][j] == 0);
            if (j >= 2 && j != 5 && (abl == 0 || j == 3 || j == 4)) {
                spell = 0;
            }
            assert(spell_rest[side][j] == spell);
        }
    }
}

static void check_end_phase_timers(void)
{
    int owner;

    for (owner = 0; owner < 2; owner++) {
        step3_case(owner, 0, 0, 1 << 15);
        step3_case(owner, 1, 0, 1 << 15);
        step3_case(owner, 0, 1, 1 << 15);
        step3_case(owner, 1, -1, 0);
    }
}

/* Step 4, branch coverage: no preference, an empty square and an owned or
 * foreign unit on field 8 or another field, at the first and last index. */
static void step4_case(int owner, int index, int unit, int ctl, int fld, int want)
{
    struct jp_908da info = untouched;

    world_reset(owner);
    pref[index] = 2 | 6 << 8;
    map_unit[2][6] = unit;
    if (unit >= 0) {
        control[unit >> 8][unit & 0xff] = ctl;
    }
    field[2][6] = fld;
    assert(Ta_Do_EndPhase(4, &info) == want);
    assert(pref_calls == (want == 0x50000 ? index + 1 : 8));
    assert(memcmp(&info, &untouched, sizeof info) == 0);
}

static void check_end_phase_summon(void)
{
    struct jp_908da info = untouched;
    int owner, index;

    for (owner = 0; owner < 2; owner++) {
        world_reset(owner);
        assert(Ta_Do_EndPhase(4, &info) == 0x50005);
        assert(pref_calls == 8);
        for (index = 0; index < 8; index += 7) {
            step4_case(owner, index, -1, 0, 8, 0x50005);
            step4_case(owner, index, -1, 0, 7, 0x50000);
            step4_case(owner, index, 1 << 8 | 4, owner, 8, 0x50005);
            step4_case(owner, index, 1 << 8 | 4, owner, 0, 0x50000);
            step4_case(owner, index, 1 << 8 | 4, !owner, 0, 0x50005);
            step4_case(owner, index, 0, owner, 9, 0x50000);
        }
    }
}

/* Branch coverage: both status outcomes for every unit, every bCond case
 * and the default, and each graveyard boundary (4/5, 9/10, 14/15) for both
 * sides. */
static void check_duel_end(void)
{
    static const int counts[] = { 0, 4, 5, 9, 10, 14, 15, 40 };
    static const int gain[4] = { 40, 32, 44, 48 };
    static const int loss[4] = { -32, -28, -36, -40 };
    int side, cond, a, b, i;

    for (side = 0; side < 2; side++) {
        for (cond = 0; cond < 5; cond++) {
            for (a = 0; a < 8; a++) {
                for (b = 0; b < 8; b++) {
                    int step_a = counts[a] < 5 ? 0 : counts[a] < 10 ? 1 : counts[a] < 15 ? 2 : 3;
                    int step_b = counts[b] < 5 ? 0 : counts[b] < 10 ? 1 : counts[b] < 15 ? 2 : 3;

                    grave[side] = counts[a];
                    grave[!side] = counts[b];
                    status_mask = (a * 8 + b) & 0x3e;
                    exp_calls = 0;
                    assert(Ta_Do_DuelEnd(side, (unsigned char)cond) == (cond < 4 ? 1 : -1));
                    assert(exp_calls == (cond < 4 ? 7 : 5));
                    for (i = 1; i < 6; i++) {
                        assert(exp_args[i - 1][0] == side);
                        assert(exp_args[i - 1][1] == 100 * side + i + 7);
                        assert(exp_args[i - 1][2] == 0);
                        assert(exp_args[i - 1][3] == (((status_mask >> i) & 1) ? 6 : 4));
                    }
                    if (cond < 4) {
                        assert(exp_args[5][0] == side);
                        assert(exp_args[5][1] == 100 * side + 7);
                        assert(exp_args[5][2] == 0);
                        assert(exp_args[5][3] == gain[cond] - 8 * step_a);
                        assert(exp_args[6][0] == !side);
                        assert(exp_args[6][1] == 100 * !side + 7);
                        assert(exp_args[6][2] == 0);
                        assert(exp_args[6][3] == loss[cond] + 8 * step_b);
                    }
                }
            }
        }
    }
}

/* World for Ta_Do_StandbyPhase, on top of the one above. */
static int sb_charge[2], sb_delay[2][12], sb_has_effect[2][12];
static struct jp_6835c sb_effect[2][12];
static int sb_manage_which, sb_manage_side, sb_manage_id, sb_manage_ret;
static void *sb_manage_info;
static int sb_exz, sb_trap17, sb_trap46, sb_parts[4], sb_tuto, sb_revision;
static int sb_destiny[2], sb_inq_calls, sb_draw, sb_draw_calls, sb_hand[2];
static int sb_power_calls, sb_power_args[3];

int SzDuel_GetSummonCharge(int index) { assert(index == 0 || index == 1); return sb_charge[index]; }
void SzDuel_SetTurnCount(int value) { turn_count = value; }
unsigned *Ta_GetUnitEffect(int a, int b, int n)
{
    UNIT_OK(a, b);
    assert(n == 2);
    return sb_has_effect[a][b] ? (unsigned *)&sb_effect[a][b] : NULL;
}
int SzDuel_SetUnitStatus(int a, int b, int bit, int value)
{
    UNIT_OK(a, b);
    assert((bit == 0 || bit == 1) && (value == 0 || value == 1));
    sb_status[a][b] = (sb_status[a][b] & ~(1 << bit)) | value << bit;
    return 0;
}
int SzDuel_GetUnitDelayCount(int a, int b) { UNIT_OK(a, b); return sb_delay[a][b]; }
int SzDuel_SetUnitDelayCount(int a, int b, int value) { UNIT_OK(a, b); sb_delay[a][b] = value; return 0; }
int Ta_ManageNaturalEffectInStandbyPhase(int nSide, int nID, struct jp_65da1 *eInfo)
{
    sb_manage_which = 1; sb_manage_side = nSide; sb_manage_id = nID; sb_manage_info = eInfo;
    return sb_manage_ret;
}
int Ta_ManageNaturalEffectInStandbyPhase2(int nSide, int nID, struct jp_6826f *eInfo)
{
    sb_manage_which = 2; sb_manage_side = nSide; sb_manage_id = nID; sb_manage_info = eInfo;
    return sb_manage_ret;
}
int SzDuel_Kind2Type(int value) { return value; }
int Ta_IsExzodia(int side) { assert(side == turn_owner); return sb_exz; }
int Ta_IsActiveTrapB(int a, int b)
{
    assert(a == !turn_owner && (b == 17 || b == 46));
    return b == 17 ? sb_trap17 : sb_trap46;
}
int Ta_GetExzodiaPartsUnit(int side, int who) { assert(side == turn_owner && who >= 0 && who < 4); return sb_parts[who]; }
int TaTuto_IsTuto(int value) { assert(value == turn_owner); return sb_tuto; }
int Ta_GetSummonCalcRevision(int who) { assert(who == turn_owner); return sb_revision; }
int SzDuel_CalcSummonPower(int index, int operation, int value)
{
    sb_power_calls++;
    sb_power_args[0] = index; sb_power_args[1] = operation; sb_power_args[2] = value;
    return 0;
}
int SzDuel_InqDestinyFlag(int index) { assert(index == turn_owner); sb_inq_calls++; return sb_destiny[index]; }
int Ta_IsDestinyDraw(int side) { assert(side == turn_owner); sb_draw_calls++; return sb_draw; }
void SzDuel_SetDestinyFlag(int index, int value) { assert(index == turn_owner); sb_destiny[index] = value; }
void SzDuel_SetHandPhase(int index, int value) { assert(index == 0 || index == 1); sb_hand[index] = value; }

static void sb_reset(int owner)
{
    world_reset(owner);
    status_mask = 0;
    n86 = 0;
    memset(sb_status, 0, sizeof sb_status);
    memset(sb_charge, 0, sizeof sb_charge);
    memset(sb_delay, 0, sizeof sb_delay);
    memset(sb_has_effect, 0, sizeof sb_has_effect);
    memset(sb_effect, 0, sizeof sb_effect);
    sb_manage_which = sb_manage_side = sb_manage_id = sb_manage_ret = 0;
    sb_manage_info = NULL;
    sb_exz = 0;
    sb_trap17 = sb_trap46 = -1;
    memset(sb_parts, 0, sizeof sb_parts);
    sb_tuto = sb_revision = 0;
    memset(sb_destiny, 0, sizeof sb_destiny);
    sb_inq_calls = sb_draw = sb_draw_calls = 0;
    sb_hand[0] = sb_hand[1] = -1;
    sb_power_calls = 0;
}

/* Step 0: turn count, status bit 1 and the delay countdown. */
static void check_standby_first(void)
{
    struct jp_6826f info;
    int i;

    /* Neither first mover nor a kind-85 effect: the count stays. */
    sb_reset(0);
    turn_first = 1;
    turn_count = 4;
    for (i = 0; i < 12; i++) {
        sb_status[0][i] = sb_status[1][i] = 1 << 1;
    }
    sb_has_effect[0][3] = 1; sb_effect[0][3].dwID = 81 << 8 | 5;
    sb_has_effect[0][4] = 1; sb_effect[0][4].dwID = 80 << 8;
    sb_has_effect[1][10] = 1; sb_effect[1][10].dwID = 81 << 8;
    sb_status[0][1] |= 1 << 4 | 1 << 5; sb_delay[0][1] = 3;
    sb_status[0][2] |= 1 << 4; sb_delay[0][2] = 3;
    sb_status[0][3] |= 1 << 5; sb_delay[0][3] = 3;
    sb_status[0][4] |= 1 << 4 | 1 << 5; sb_delay[0][4] = 0;
    sb_status[0][5] |= 1 << 4 | 1 << 5; sb_delay[0][5] = 1;
    sb_status[0][6] |= 1 << 4 | 1 << 5; sb_delay[0][6] = 3;
    sb_status[1][1] |= 1 << 4 | 1 << 5; sb_delay[1][1] = 3;
    assert(Ta_Do_StandbyPhase(0, &info) == 0x200000);
    assert(turn_count == 4);
    for (i = 0; i < 12; i++) {
        int kept0 = i == 0 || i == 11 || i == 3;
        int kept1 = i == 0 || i == 11 || i == 10;

        assert(((sb_status[0][i] >> 1) & 1) == kept0);
        assert(((sb_status[1][i] >> 1) & 1) == kept1);
    }
    assert(sb_delay[0][1] == 2 && sb_delay[0][2] == 3 && sb_delay[0][3] == 3);
    assert(sb_delay[0][4] == 0 && sb_delay[0][5] == 1 && sb_delay[0][6] == 3 && sb_delay[1][1] == 3);

    /* The first mover's turn advances the count. */
    sb_reset(1);
    turn_count = 4;
    sb_status[1][5] = 1 << 4 | 1 << 5; sb_delay[1][5] = 9;
    assert(Ta_Do_StandbyPhase(0, &info) == 0x200000);
    assert(turn_count == 5 && sb_delay[1][5] == 8);

    /* A kind-85 effect advances it as well and counts delays down by two,
     * not below one. */
    sb_reset(0);
    turn_first = 1;
    turn_count = 4;
    n85 = 1;
    sb_status[0][1] = 1 << 4 | 1 << 5; sb_delay[0][1] = 5;
    sb_status[0][2] = 1 << 4 | 1 << 5; sb_delay[0][2] = 2;
    assert(Ta_Do_StandbyPhase(0, &info) == 0x200000);
    assert(turn_count == 5 && sb_delay[0][1] == 3 && sb_delay[0][2] == 1);
}

static int sb_unit_step(int owner, int PrNo, int side, int id, int status, int effect, struct jp_6826f *info)
{
    sb_reset(owner);
    sb_manage_ret = 3;
    unit_here[side][id] = 1;
    sb_status[side][id] = status;
    sb_has_effect[side][id] = effect;
    return Ta_Do_StandbyPhase(PrNo, info);
}

/* Steps 32 to 63: one unit per step. */
static void check_standby_units(void)
{
    struct jp_6826f info;
    int full = 1 << 4 | 1 << 5;

    /* Owner's unit, status bit 1 clear. */
    assert(sb_unit_step(0, 33, 0, 1, full, 1, &info) == (34 << 16 | 3));
    assert(sb_manage_which == 1 && sb_manage_side == 0 && sb_manage_id == 1 && sb_manage_info == &info);
    assert(sb_unit_step(1, 37, 1, 5, full, 1, &info) == (38 << 16 | 3));
    assert(sb_manage_which == 1 && sb_manage_side == 1 && sb_manage_id == 5);
    assert(sb_unit_step(0, 33, 0, 1, 1 << 5, 1, &info) == 34 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 33, 0, 1, 1 << 4, 1, &info) == 34 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 33, 0, 1, full | 1 << 1, 1, &info) == 34 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 33, 0, 1, full, 0, &info) == 34 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 33, 0, 2, full, 1, &info) == 34 << 16 && sb_manage_which == 0);
    /* Unit 0 is never offered. */
    assert(sb_unit_step(0, 32, 0, 0, full, 1, &info) == 33 << 16 && sb_manage_which == 0);
    /* Below 32 nothing is found and the step advances. */
    assert(sb_unit_step(0, 1, 0, 1, full, 1, &info) == 2 << 16 && sb_manage_which == 0);

    /* The other side's unit, through the second manager. */
    assert(sb_unit_step(0, 38, 1, 1, full, 1, &info) == (39 << 16 | 3));
    assert(sb_manage_which == 2 && sb_manage_side == 1 && sb_manage_id == 1 && sb_manage_info == &info);
    assert(sb_unit_step(1, 42, 0, 5, full, 1, &info) == (43 << 16 | 3));
    assert(sb_manage_which == 2 && sb_manage_side == 0 && sb_manage_id == 5);
    assert(sb_unit_step(0, 38, 1, 1, 1 << 5, 1, &info) == 39 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 38, 1, 1, 1 << 4, 1, &info) == 39 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 38, 1, 1, full | 1 << 1, 1, &info) == 39 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 38, 1, 1, full, 0, &info) == 39 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 37, 1, 0, full, 1, &info) == 38 << 16 && sb_manage_which == 0);

    /* Owner's unit again, status bit 1 set this time. */
    assert(sb_unit_step(0, 43, 0, 1, full | 1 << 1, 1, &info) == (44 << 16 | 3));
    assert(sb_manage_which == 1 && sb_manage_side == 0 && sb_manage_id == 1 && sb_manage_info == &info);
    assert(sb_unit_step(0, 43, 0, 1, full, 1, &info) == 44 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 43, 0, 1, 1 << 5 | 1 << 1, 1, &info) == 44 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 43, 0, 1, 1 << 4 | 1 << 1, 1, &info) == 44 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 43, 0, 1, full | 1 << 1, 0, &info) == 44 << 16 && sb_manage_which == 0);
    assert(sb_unit_step(0, 42, 0, 0, full | 1 << 1, 1, &info) == 43 << 16 && sb_manage_which == 0);

    /* Sixteen steps, then 64. */
    assert(sb_unit_step(0, 47, 1, 11, 0, 0, &info) == 48 << 16);
    assert(sb_unit_step(0, 48, 1, 11, 0, 0, &info) == 64 << 16);
}

/* Steps 64 to 95 and the closing part. */
static void check_standby_status(void)
{
    struct jp_6826f info;
    int i;

    sb_reset(0);
    memset(&info, 0x55, sizeof info);
    for (i = 0; i < 12; i++) {
        sb_status[0][i] = sb_status[1][i] = 1;
    }
    cur_side[0][2] = 1;
    cur_side[1][0] = cur_side[1][3] = cur_side[1][11] = 1;
    curse_rest[0][4] = 1;
    spell_rest[0][5] = 1;
    unit_kind[0][6] = 2 << 5; sb_status[0][6] |= 1 << 4;
    unit_kind[0][7] = 2 << 5;
    unit_kind[0][8] = 3 << 5; sb_status[0][8] |= 1 << 4;
    curse_rest[0][0] = 1;
    curse_rest[0][11] = 1;
    curse_rest[1][4] = 1;
    spell_rest[1][5] = 1;
    unit_kind[1][6] = 2 << 5; sb_status[1][6] |= 1 << 4;
    unit_kind[1][7] = 2 << 5;
    spell_rest[1][11] = 1;
    sb_charge[0] = 6;
    turn_count = 1;
    sb_draw = 0x105;
    assert(Ta_Do_StandbyPhase(64, &info) == 0xff0000);
    for (i = 0; i < 12; i++) {
        int set0 = i == 2 || i == 4 || i == 5 || i == 6 || i == 11;
        int set1 = i == 0 || (i >= 3 && i <= 6) || i == 11;

        assert((sb_status[0][i] & 1) == set0);
        assert((sb_status[1][i] & 1) == set1);
    }
    assert(info.eNO == 0x55555555);
    assert(sb_power_calls == 0);
    assert(sb_inq_calls == 2 && sb_draw_calls == 1 && sb_destiny[0] == 5);
    assert(sb_hand[0] == 1 && sb_hand[1] == -1);

    /* Other side to move: bit 0 follows the opposite control and the other
     * side's timers are read through the mirrored loop. */
    sb_reset(1);
    cur_side[0][2] = cur_side[0][3] = cur_side[0][4] = cur_side[0][5] = cur_side[0][6] = 1;
    cur_side[1][3] = 1;
    curse_rest[0][3] = 1;
    spell_rest[0][4] = 1;
    unit_kind[0][5] = 2 << 5; sb_status[0][5] = 1 << 4;
    unit_kind[0][6] = 2 << 5;
    sb_status[1][3] = 1;
    sb_charge[1] = 6;
    sb_revision = 2;
    turn_count = 2;
    sb_destiny[1] = 0x80;
    sb_draw = 5;
    assert(Ta_Do_StandbyPhase(95, &info) == 0xff0000);
    for (i = 0; i < 11; i++) {
        assert((sb_status[0][i] & 1) == (i != 2 && i != 6));
        assert((sb_status[1][i] & 1) == (i != 3));
    }
    assert(sb_power_calls == 1 && sb_power_args[0] == 1 && sb_power_args[1] == 0 && sb_power_args[2] == 6);
    assert(sb_inq_calls == 2 && sb_draw_calls == 0 && sb_destiny[1] == 0x80);
    assert(sb_hand[1] == 1 && sb_hand[0] == -1);

    /* From 96 the status loops are skipped; a kind-86 effect adds the
     * revision; a draw of zero leaves the flag. */
    sb_reset(0);
    cur_side[0][2] = 1;
    sb_charge[0] = 6;
    sb_revision = 2;
    n86 = 1;
    turn_count = 2;
    assert(Ta_Do_StandbyPhase(96, &info) == 0xff0000);
    assert(sb_status[0][2] == 0);
    assert(sb_power_calls == 1 && sb_power_args[0] == 0 && sb_power_args[1] == 0 && sb_power_args[2] == 8);
    assert(sb_draw_calls == 1 && sb_destiny[0] == 0);

    /* 160 and above do not ask about Exodia's effect. */
    sb_reset(0);
    sb_exz = 4;
    memset(&info, 0x55, sizeof info);
    assert(Ta_Do_StandbyPhase(160, &info) == 0xff0000);
    assert(info.eNO == 0x55555555);
}

/* A complete Exodia. */
static void check_standby_exzodia(void)
{
    struct jp_6826f info;

    sb_reset(0);
    sb_exz = 4;
    sb_parts[0] = 0x101; sb_parts[1] = 2; sb_parts[2] = 3; sb_parts[3] = 4;
    assert(Ta_Do_StandbyPhase(64, &info) == 0xff0005);
    assert(info.eNO == 109 && info.dwFrom == 0 && info.dwPar == 0);
    assert(info.dwTo == (1 << 9 | 2 << 6 | 3 << 3 | 4));
    assert(sb_hand[0] == -1);

    sb_reset(1);
    sb_exz = 4;
    sb_tuto = 1;
    sb_parts[3] = 7;
    assert(Ta_Do_StandbyPhase(128, &info) == 0xff0001);
    assert(info.eNO == 109 && info.dwFrom == 1 && info.dwTo == 7 && info.dwPar == 0);

    /* Not complete: the closing part runs. */
    sb_reset(0);
    sb_exz = 3;
    assert(Ta_Do_StandbyPhase(128, &info) == 0xff0000 && sb_hand[0] == 1);

    /* A kind-17 trap answers; with a kind-46 trap as well that one does
     * and the step moves on to 129. */
    sb_reset(0);
    sb_exz = 4;
    sb_trap17 = 7;
    assert(Ta_Do_StandbyPhase(128, &info) == 0xa00001);
    assert(info.eNO == 102 && info.dwFrom == 1 && info.dwTo == 7 && info.dwPar == 0);
    sb_trap17 = 0;
    sb_trap46 = 9;
    assert(Ta_Do_StandbyPhase(128, &info) == (129 << 16 | 1));
    assert(info.eNO == 102 && info.dwFrom == 1 && info.dwTo == 9 && info.dwPar == 0);
    sb_trap46 = 0;
    assert(Ta_Do_StandbyPhase(128, &info) == (129 << 16 | 1) && info.dwTo == 0);

    /* Step 129 does not ask about the traps again. */
    sb_parts[3] = 2;
    assert(Ta_Do_StandbyPhase(129, &info) == 0xff0005);
    assert(info.eNO == 109 && info.dwTo == 2);
    assert(Ta_Do_StandbyPhase(159, &info) == 0xff0005);
}

int main(void)
{
    check_standby_first();
    check_standby_units();
    check_standby_status();
    check_standby_exzodia();
    check_duel_end();
    check_end_phase_simple();
    check_end_phase_scan();
    check_end_phase_timers();
    check_end_phase_summon();
    puts("end_phase: ok");
    return 0;
}
