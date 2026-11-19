/* Synthetic host tests for the move_efc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/move_efc.h"

static unsigned abl_mask;
static int map_id[7][7];
static int map_calls;
static int kind_answer, side_same, dest_answer, curse_answer, spell_answer, status_answer;
static int range_seen, field_answer, side_answer;
static struct jp_8cf7 *effect_answer;
static int effect_args[3];
static int set_n, set_log[64][3];
static int lp_n, lp_log[3];
static int conv_n;

int Ta_IsLDAbl(int a, int b) { (void)b; return (abl_mask >> a) & 1; }
int SzDuel_GetUnitKind(int a, int b) { (void)a, (void)b; return kind_answer; }
int Ta_GetLDChgLand(int Race) { return Race + 100; }
int SzDuel_SetField(int x, int y, int value)
{
    assert(set_n < 64);
    set_log[set_n][0] = x, set_log[set_n][1] = y, set_log[set_n][2] = value;
    set_n++;
    return 0;
}
int SzDuel_GetMapUnitID(int x, int y) { map_calls++; return map_id[x][y]; }
int Ta_GetCurrentSide(int a, int b) { (void)b; return side_same ? 0 : side_answer + (a & 0); }
int Ta_GetLDRange(int a) { return a + 2; }
int Ta_IsSquareRange(int nX, int nZ, int mX, int mZ, int R)
{
    range_seen = R;
    return mX - nX <= R && nX - mX <= R && mZ - nZ <= R && nZ - mZ <= R;
}
int Ta_IsLD_DestObj(int a, int b) { (void)a; return dest_answer && b == (kind_answer & 0x1F); }
unsigned ConvUnitIDtoFlagList(unsigned word, int high, int position) { conv_n++; return word + ((unsigned)high << 8 | (unsigned)position); }
int Ta_IsLD_CurseObj(int a, int b) { (void)a; return curse_answer && b == (kind_answer & 0x1F); }
int SzDuel_GetUnitSpellRest(int a, int b) { (void)a, (void)b; return spell_answer; }
int SzDuel_GetUnitStatus(int a, int b, int bit) { (void)a, (void)b; assert(bit == 4); return status_answer; }
struct jp_8cf7 *Ta_GetUnitEffect(int a, int b, int n)
{
    effect_args[0] = a, effect_args[1] = b, effect_args[2] = n;
    return effect_answer;
}
int Ta_GetLPRevByNaturalEffect(int a, int b) { return a * 0x10000 + b; }
int SzDuel_CalcLP(int index, int operation, int value)
{
    lp_log[0] = index, lp_log[1] = operation, lp_log[2] = value;
    lp_n++;
    return 0;
}
int SzDuel_GetField(int x, int y) { return x == 3 && y == 4 ? field_answer : -1; }
int WallFusion(int a, int b) { return a * 100 + b; }

static struct jp_9026 info;

static int run(int nSide, int nID)
{
    memset(&info, 0xEE, sizeof info);
    set_n = lp_n = conv_n = map_calls = 0;
    return Ta_Do_MoveEfc(nSide, nID, 3, 4, &info);
}

static int untouched(void)
{
    return info.eNO == (int)0xEEEEEEEE && info.dwFrom == 0xEEEEEEEE && info.dwTo == 0xEEEEEEEE && info.dwPar == 0xEEEEEEEE;
}

/* Ta_Do_MoveEfc, nID == 0: branch coverage. */
static void check_leader(void)
{
    memset(map_id, 0xFF, sizeof map_id);
    kind_answer = 0x45, side_answer = 1;
    /* nothing on the map, no ability: all 49 squares asked, nothing written */
    assert(run(0, 0) == 0 && map_calls == 49 && set_n == 0 && untouched());
    /* ability 7 changes the square's land to the one of the kind's low five bits */
    abl_mask = 1u << 7;
    assert(run(0, 0) == 0 && set_n == 1 && set_log[0][0] == 3 && set_log[0][1] == 4 && set_log[0][2] == 105 && untouched());
    /* ability 16: a low kind the destroy test accepts is collected in dwFrom with the top bit */
    abl_mask = 1u << 16, kind_answer = 3, dest_answer = 1;
    map_id[2][5] = 0x105;
    assert(run(0, 0) == 1 && set_n == 0 && conv_n == 1 && range_seen == 2);
    assert(info.eNO == 0x71 && info.dwFrom == 0x80000105 && info.dwTo == 0 && info.dwPar == 0);
    /* rejected: destroy test fails, a kind above 31, the unit's side is nSide, position 0, a
     * negative id, out of range */
    dest_answer = 0;
    assert(run(0, 0) == 0 && conv_n == 0 && untouched());
    dest_answer = 1, kind_answer = 0x23;
    assert(run(0, 0) == 0 && conv_n == 0);
    kind_answer = 3, side_same = 1;
    assert(run(0, 0) == 0 && conv_n == 0);
    side_same = 0, map_id[2][5] = 0x100;
    assert(run(0, 0) == 0 && conv_n == 0);
    map_id[2][5] = -1, map_id[6][4] = 0x105;
    assert(run(0, 0) == 0 && conv_n == 0);
    map_id[6][4] = -1, map_id[5][6] = 0x105;
    assert(run(0, 0) == 1 && conv_n == 1);
    /* ability 15: curse test and no spell rest give dwTo */
    abl_mask = 1u << 15, curse_answer = 1, spell_answer = 0;
    assert(run(0, 0) == 1 && conv_n == 1 && info.eNO == 0x71 && info.dwFrom == 0x80000000 && info.dwTo == 0x105 && info.dwPar == 0);
    spell_answer = 1;
    assert(run(0, 0) == 0 && conv_n == 0);
    spell_answer = 0, curse_answer = 0;
    assert(run(0, 0) == 0 && conv_n == 0);
    curse_answer = 1, kind_answer = 0x23;
    assert(run(0, 0) == 0 && conv_n == 0);
    /* ability 13: any kind whose status bit 4 is clear gives dwPar */
    abl_mask = 1u << 13, status_answer = 0;
    assert(run(0, 0) == 1 && conv_n == 1 && info.dwFrom == 0x80000000 && info.dwTo == 0 && info.dwPar == 0x105);
    status_answer = 1;
    assert(run(0, 0) == 0 && conv_n == 0);
    /* all three at once, two units */
    abl_mask = 1u << 13 | 1u << 15 | 1u << 16, status_answer = 0, kind_answer = 3, map_id[1][2] = 0x102;
    assert(run(0, 0) == 1 && conv_n == 6 && info.dwFrom == 0x80000207 && info.dwTo == 0x207 && info.dwPar == 0x207);
    abl_mask = 0;
}

/* Ta_Do_MoveEfc, nID != 0: branch coverage. */
static void check_unit(void)
{
    struct jp_8cf7 dat;
    int n;

    /* no effect entry */
    effect_answer = NULL;
    assert(run(1, 6) == 0 && effect_args[0] == 1 && effect_args[1] == 6 && effect_args[2] == 1 && map_calls == 0 && untouched());
    effect_answer = &dat;
    /* an effect number outside the four */
    dat.dwID = 5 << 8, dat.dwParam = 0;
    assert(run(1, 6) == 0 && untouched());
    /* 0x42: mode 1 pays the unit's current side, mode 2 the other one, other modes nobody */
    dat.dwID = 0x42 << 8 | 1 << 6, dat.dwParam = 0xABCD1234, side_answer = 1;
    assert(run(1, 6) == 1 && lp_n == 1 && lp_log[0] == 1 && lp_log[1] == 1 && lp_log[2] == 0x2C1234);
    assert(info.eNO == 0xFF && info.dwFrom == 0 && info.dwTo == 0x106 && info.dwPar == 0);
    dat.dwID = 0x42 << 8 | 2 << 6;
    assert(run(1, 6) == 1 && lp_n == 1 && lp_log[0] == 0 && lp_log[1] == 1 && lp_log[2] == 0x2C1234 && info.dwTo == 0x106);
    side_answer = 0;
    assert(run(1, 6) == 1 && lp_n == 1 && lp_log[0] == 1);
    dat.dwID = 0x42 << 8 | 3 << 6;
    assert(run(1, 6) == 1 && lp_n == 0 && info.eNO == 0xFF && info.dwTo == 0x106);
    dat.dwID = 0x42 << 8;
    assert(run(1, 6) == 1 && lp_n == 0 && info.eNO == 0xFF);
    /* 2: every square within the Manhattan distance in the high half gets the land in the low half */
    dat.dwID = 2 << 8, dat.dwParam = 0 << 16 | 9;
    assert(run(1, 6) == 1 && set_n == 1 && set_log[0][0] == 3 && set_log[0][1] == 4 && set_log[0][2] == 9);
    assert(info.eNO == 0xFF && info.dwFrom == 0 && info.dwTo == 0x106 && info.dwPar == 0);
    dat.dwParam = 1 << 16 | 9;
    assert(run(1, 6) == 1 && set_n == 5 && set_log[0][0] == 2 && set_log[0][1] == 4 && set_log[1][0] == 3 && set_log[1][1] == 3);
    assert(set_log[3][0] == 3 && set_log[3][1] == 5 && set_log[4][0] == 4 && set_log[4][1] == 4);
    dat.dwParam = 3 << 16 | 9;
    assert(run(1, 6) == 1);
    for (n = 0; n < set_n; n++) {
        int dx = set_log[n][0] - 3, dz = set_log[n][1] - 4;
        assert((dx < 0 ? -dx : dx) + (dz < 0 ? -dz : dz) <= 3 && set_log[n][2] == 9);
    /* the full diamond of 25 squares less (3, 7), which is off the map */
    }
    assert(set_n == 24);
    /* 10: land 8 under the unit becomes 7 */
    dat.dwID = 10 << 8, field_answer = 8;
    assert(run(1, 6) == 1 && set_n == 1 && set_log[0][0] == 3 && set_log[0][1] == 4 && set_log[0][2] == 7);
    assert(info.eNO == 0xFF && info.dwFrom == 0 && info.dwTo == 0x106 && info.dwPar == 0);
    field_answer = 7;
    assert(run(1, 6) == 0 && set_n == 0 && untouched());
    /* 9: on land 8 effect 0x3C with the unit's flag list and WallFusion's answer */
    dat.dwID = 9 << 8, field_answer = 8;
    assert(run(1, 6) == 0x10001 && set_n == 0 && conv_n == 1);
    assert(info.eNO == 0x3C && info.dwFrom == 0 && info.dwTo == 0x106 && info.dwPar == 106);
    field_answer = 0;
    assert(run(1, 6) == 0 && conv_n == 0 && untouched());
}

int main(void)
{
    check_leader();
    check_unit();
    return 0;
}
