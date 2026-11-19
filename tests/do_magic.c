/* Synthetic host tests for the do_magic group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/do_magic.h"

/* Plain recording stubs: each returns a value the test sets and records what it was given. */
static struct {
    int nside, side, loc, list, num, card, grave, gcount, rnd, unum, nat, sacunit;
    int sac[3];
    int field[7][7];
} cfg;
static struct {
    int del_n;
    int search_n, search[6];
    int conv_n, conv_x[3], conv_flag[3], conv_pos[3];
    int remove_n, remove_word, remove_side;
    int grave_n, grave_index[10], grave_kind[10];
    int gcount_n, gcount_index[2];
    int field_n;
    int set_n, set_ok;
    void *set_out;
    int unum_a, unum_kind;
    int owner_n;
    int conv2_n, conv2_high;
    int cerem_obj, sac_value;
} rec;
static struct jp_1073d info;
static struct jp_107e2 pinfo;
static struct jp_10845 eff;

int Ta_GetCurrentSide(int a, int b) { assert(a == cfg.nside && b == 9); return cfg.side; }
int Ta_DelUnit(int a, int b) { assert(a == cfg.nside && b == 9); rec.del_n++; return 77; }
int SzDuel_GetUnitLoc(int a, int b) { assert(a == cfg.nside && b == 9); return cfg.loc; }
int SzDuel_GetUnitCardNo(int a, int b) { assert(a == cfg.nside && b == 9); return cfg.card; }
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    rec.search_n++;
    rec.search[0] = a;
    rec.search[1] = b;
    rec.search[2] = c;
    rec.search[3] = d;
    rec.search[4] = e;
    rec.search[5] = f;
    return 0x100;
}
unsigned ConvUnitIDtoFlagList_OFF(unsigned x, int flag, int n)
{
    assert(rec.conv_n < 3);
    rec.conv_x[rec.conv_n] = (int)x;
    rec.conv_flag[rec.conv_n] = flag;
    rec.conv_pos[rec.conv_n] = n;
    rec.conv_n++;
    return x + 1;
}
unsigned ConvUnitIDtoFlagList(unsigned word, int high, int position)
{
    assert(word == 0 && position == 0);
    rec.conv2_n++;
    rec.conv2_high = high;
    return 0x777;
}
int Ta_RemoveObjFromFlagLst2(int word, int side)
{
    rec.remove_n++;
    rec.remove_word = word;
    rec.remove_side = side;
    return cfg.list;
}
int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    assert(value == (unsigned)cfg.list || value == 0x777 || value == 0x103);
    return cfg.num;
}
int TaUtil_GetGraveCardNum(int index, int kind)
{
    assert(rec.grave_n < 10);
    rec.grave_index[rec.grave_n] = index;
    rec.grave_kind[rec.grave_n] = kind;
    rec.grave_n++;
    return cfg.grave;
}
int SzDuel_GetGraveCount(int index)
{
    assert(rec.gcount_n < 2);
    rec.gcount_index[rec.gcount_n++] = index;
    return cfg.gcount;
}
int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    rec.field_n++;
    return cfg.field[x][y];
}
int func_0019a5c8(void) { return cfg.rnd; }
void Ta_SetEffectObjByField(struct DotrMiscBitWords *out, int *source)
{
    int i;

    rec.set_n++;
    rec.set_out = out;
    rec.set_ok = 1;
    for (i = 0; i < 49; i++) {
        if (source[i] != -1) {
            rec.set_ok = 0;
        }
    }
}
int Ta_GetUnitNum(int a, unsigned char kind)
{
    rec.unum_a = a;
    rec.unum_kind = kind;
    return cfg.unum;
}
int Ta_GetEOwnerSide(int value, int mode) { return value * 10 + mode; }
int Ta_SearchUncTypeNaturalEffectUserNum(int kind) { assert(kind == 57); return cfg.nat; }
int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert(word == 0x777 && n == rec.owner_n);
    rec.owner_n++;
    return 40 + n;
}
int SzDuel_GetSummonPower(int index) { return 500 + index; }
void Ta_GetCeremSacrifice(int *Buf, int Obj)
{
    rec.cerem_obj = Obj;
    memcpy(Buf, cfg.sac, sizeof cfg.sac);
}
int Ta_GetSacrificeUnit(int *triple, int value)
{
    assert(triple[0] == cfg.sac[0] && triple[1] == cfg.sac[1] && triple[2] == cfg.sac[2]);
    rec.sac_value = value;
    return cfg.sacunit;
}

/* One call for effect type `type`, side selector `sel`, kind `kind` and parameter `param`.
 * Every path removes the unit exactly once. */
static int run(int type, int sel, int kind, unsigned param)
{
    int result;

    memset(&rec, 0, sizeof rec);
    memset(&info, 0xEE, sizeof info);
    memset(&pinfo, 0xEE, sizeof pinfo);
    eff.dwID = (unsigned)((type << 8) | (sel << 6) | kind);
    eff.dwParam = param;
    result = Ta_Do_Magic(cfg.nside, 9, &info, &pinfo, 0, &eff);
    assert(rec.del_n == 1);
    assert(eff.dwID == (unsigned)((type << 8) | (sel << 6) | kind) && eff.dwParam == param);
    return result;
}

static void expect(int eNO, unsigned from, unsigned to, unsigned par)
{
    assert(info.eNO == eNO && info.dwFrom == from && info.dwTo == to && info.dwPar == par);
}

static void expect_search(int a, int b, int sel, int kind, int range)
{
    assert(rec.search_n == 1 && rec.search[0] == a && rec.search[1] == b && rec.search[2] == cfg.side &&
           rec.search[3] == sel && rec.search[4] == kind && rec.search[5] == range);
}

/* The common chain: search, one conversion with the unit, removal with the owner side. */
static void expect_chain(int sel, int kind)
{
    expect_search(0, 0, sel, kind, 14);
    assert(rec.conv_n == 1 && rec.conv_x[0] == 0x100 && rec.conv_flag[0] == cfg.nside && rec.conv_pos[0] == 9);
    assert(rec.remove_n == 1 && rec.remove_word == 0x101 && rec.remove_side == cfg.side);
}

/* A type whose record is (eNO, from, list, par) and which falls back to sequence 254 when the
 * list is empty. */
static void check_list_type(int type, unsigned param, int eNO, unsigned from, unsigned par)
{
    cfg.num = 2;
    assert(run(type, 1, 7, param) == 1);
    expect(eNO, from, 0x2222, par);
    expect_chain(1, 7);
    cfg.num = 0;
    assert(run(type, 1, 7, param) == 1);
    expect(254, 0, 0xFFFF, 16);
    cfg.num = 2;
}

int main(void)
{
    static const int cards[9][2] = {{699, 4},  {701, 6},  {721, 16}, {722, 17}, {728, 18},
                                    {729, 19}, {730, 20}, {727, 23}, {5, 21}};
    int i;
    int side;

    cfg.nside = 1;
    cfg.loc = 0x0305;
    cfg.list = 0x2222;
    cfg.num = 2;
    cfg.grave = 3;
    cfg.gcount = 4;

    for (side = 0; side < 2; side++) {
        cfg.side = side;

        /* unknown type: nothing but the removal */
        assert(run(99, 0, 0, 0) == 0);
        expect((int)0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE);
        assert(rec.search_n == 0);

        /* list types */
        check_list_type(23, 0x90005, 0, 0, 5);
        check_list_type(73, 0x90005, 12, 0, 5);
        check_list_type(24, 0x90005, 13, 4, 5);
        check_list_type(30, 0x90005, 15, 0, 0);
        check_list_type(33, 0x90005, 91, 0, 0);
        check_list_type(34, 0x90005, 29, 0, 5);
        check_list_type(35, 0x90005, 13, 5, 5);
        check_list_type(36, 0x90005, 30, 0, (unsigned)side);

        /* type 54: no conversion step */
        assert(run(54, 2, 9, 0x10006) == 1);
        expect(66, 54, 0x2222, 6);
        expect_search(0, 0, 2, 9, 14);
        assert(rec.conv_n == 0 && rec.remove_word == 0x100 && rec.remove_side == side);
        cfg.num = 0;
        assert(run(54, 2, 9, 0x10006) == 1);
        expect(254, 0, 0xFFFF, 16);
        cfg.num = 2;

        /* type 18: the sequence depends on the unit's card */
        cfg.card = 739;
        assert(run(18, 1, 7, 0x90005) == 1);
        expect(90, 1, 0x2222, 5);
        expect_chain(1, 7);
        cfg.card = 740;
        assert(run(18, 1, 7, 0x90005) == 1);
        expect(92, 1, 0x2222, 5);
        cfg.card = 1;
        assert(run(18, 1, 7, 0x90005) == 1);
        expect(29, 1, 0x2222, 5);
        cfg.num = 0;
        assert(run(18, 1, 7, 0x90005) == 1);
        expect(254, 0, 0xFFFF, 16);
        cfg.num = 2;

        /* types 29, 38, 39, 40 and 32: fixed records */
        assert(run(29, 0, 0, 0) == 1);
        expect(14, 0, side == 0, 0);
        assert(run(38, 0, 0, 0) == 1);
        expect(33, 0, (unsigned)side, 0);
        assert(run(39, 0, 0, 0) == 1);
        expect(34, side == 0, (unsigned)side, (unsigned)(500 + (side == 0)));
        assert(run(40, 0, 0, 0) == 1);
        expect(35, (unsigned)side, 0, 0);
        assert(run(32, 0, 0, 0) == 1);
        expect(28, (unsigned)side, 0x101, 0);
        expect_search(0, 0, 3, 54, 14);
        assert(rec.conv_n == 1 && rec.remove_n == 0);

        /* type 20: owner selector, then the grave count of the kind */
        assert(run(20, 0, 29, 0) == 0);
        expect(1, (unsigned)side | 0x20000, 0x0305, 999);
        assert(rec.grave_n == 0);
        assert(run(20, 3, 29, 0) == 2);
        assert(rec.grave_n == 2 && rec.grave_index[0] == 0 && rec.grave_index[1] == 1 && rec.grave_kind[1] == 0);
        assert(pinfo.bType == 0 && pinfo.bKind == 0x21);
        assert(run(20, 1, 29, 0) == 2);
        assert(rec.grave_n == 1 && rec.grave_index[0] == (side != 0) && rec.grave_kind[0] == 0);
        assert(pinfo.bKind == (((side != 0) << 4) | 1));
        assert(run(20, 3, 30, 0) == 2);
        assert(rec.grave_n == 10 && pinfo.bKind == 0x22);
        for (i = 0; i < 10; i++) {
            assert(rec.grave_index[i] == (i & 1) && rec.grave_kind[i] == i / 2 + 1);
        }
        assert(run(20, 2, 30, 0) == 2);
        assert(rec.grave_n == 5 && pinfo.bKind == (((side == 0) << 4) | 2));
        for (i = 0; i < 5; i++) {
            assert(rec.grave_index[i] == (side == 0) && rec.grave_kind[i] == i + 1);
        }
        assert(run(20, 3, 31, 0) == 2);
        assert(rec.gcount_n == 2 && rec.gcount_index[0] == 0 && rec.gcount_index[1] == 1 && pinfo.bKind == 0x20);
        assert(run(20, 2, 31, 0) == 2);
        assert(rec.gcount_n == 1 && rec.gcount_index[0] == (side == 0) && pinfo.bKind == ((side == 0) << 4));
        assert(run(20, 3, 5, 0) == 0);
        assert(pinfo.bType == 0xEE);
        cfg.grave = 0;
        cfg.gcount = 0;
        assert(run(20, 3, 29, 0) == 0);
        assert(run(20, 1, 29, 0) == 0);
        assert(run(20, 3, 30, 0) == 0);
        assert(run(20, 1, 30, 0) == 0);
        assert(run(20, 3, 31, 0) == 0);
        assert(run(20, 1, 31, 0) == 0);
        assert(pinfo.bType == 0xEE);
        cfg.grave = 3;
        cfg.gcount = 4;

        /* type 2: the record is the same whether or not the low operand is 8 */
        assert(run(2, 0, 0, 0x30008) == 1);
        expect(2, 0x0305, 3, 8);
        assert(run(2, 0, 0, 0x30005) == 1);
        expect(2, 0x0305, 3, 5);

        /* type 72: a random cell whose field value is 8 */
        memset(cfg.field, 0, sizeof cfg.field);
        assert(run(72, 0, 0, 0) == 0);
        assert(rec.field_n == 49);
        cfg.field[2][3] = 8;
        cfg.field[6][6] = 8;
        cfg.rnd = 5;
        assert(run(72, 0, 0, 0) == 1);
        expect(3, 0x0606, 0x0305, 0);
        cfg.rnd = 4;
        assert(run(72, 0, 0, 0) == 1);
        expect(3, 0x0302, 0x0305, 0);

        /* type 4, operand 18: search around the unit, three conversions, record by card */
        cfg.card = 700;
        assert(run(4, 2, 9, 18 << 16) == 1);
        expect(5, 0x0305, 0x103, (unsigned)(side | (side << 16)));
        expect_search(5, 3, 2, 9, 18);
        assert(rec.conv_n == 3 && rec.conv_flag[0] == 1 && rec.conv_pos[0] == 9 && rec.conv_flag[1] == 1 &&
               rec.conv_pos[1] == 0 && rec.conv_flag[2] == 0 && rec.conv_pos[2] == 0 && rec.remove_n == 0);
        cfg.nside = 0;
        assert(run(4, 2, 9, 18 << 16) == 1);
        assert(rec.conv_flag[1] == 0 && rec.conv_flag[2] == 1);
        cfg.nside = 1;
        cfg.card = 702;
        assert(run(4, 2, 9, 18 << 16) == 1);
        expect(7, 0x0305, 0x103, (unsigned)(side | (side << 16)));
        cfg.num = 0;
        assert(run(4, 2, 9, 18 << 16) == 0);
        expect(7, 0x0305, 0x103, (unsigned)(side | (side << 16)));
        cfg.num = 2;
        cfg.card = 1;
        assert(run(4, 2, 9, 18 << 16) == 0);
        /* operand 19: the untouched cell list goes to Ta_SetEffectObjByField */
        assert(run(4, 0, 0, 19 << 16) == 1);
        expect(25, 0, 0, (unsigned)side);
        assert(rec.set_n == 1 && rec.set_out == (void *)&info && rec.set_ok);
        /* operand 0: sequence by card, list fallback */
        for (i = 0; i < 9; i++) {
            cfg.card = cards[i][0];
            assert(run(4, 1, 7, 5) == 1);
            expect(cards[i][1], i == 0 ? (unsigned)side : 0, 0x2222, 0);
            expect_chain(1, 7);
            cfg.num = 0;
            assert(run(4, 1, 7, 5) == 1);
            expect(254, 0, 0xFFFF, 16);
            cfg.num = 2;
        }
        assert(run(4, 0, 0, 7 << 16) == 0);
        assert(rec.search_n == 0);

        /* type 13: amount, multiplied by a unit count for kind 50 */
        assert(run(13, 1, 0, 0x20005) == 1);
        expect(8, 0, (unsigned)(side * 10 + 1), 0x20005);
        assert(run(13, 2, 0, 0x20005) == 1);
        expect(9, 0x0305, (unsigned)(side * 10 + 2), 0x20005);
        assert(run(13, 0, 0, 0x20005) == 0);
        assert(run(13, 3, 0, 0x20005) == 0);
        assert(run(13, 1, 0, 0x20000) == 0);
        cfg.unum = 3;
        assert(run(13, 1, 50, 0x20005) == 1);
        expect(8, 0, (unsigned)(side * 10 + 1), 0x2000F);
        assert(rec.unum_a == (side == 0) && rec.unum_kind == 3);
        cfg.unum = 0;
        assert(run(13, 1, 50, 0x20005) == 0);

        /* type 31 */
        cfg.nat = 1;
        assert(run(31, 1, 33, 0) == 0);
        assert(rec.search_n == 0);
        cfg.nat = 0;
        assert(run(31, 1, 33, 0) == 1);
        expect(26, (unsigned)side, 0x2222, 0xFFFE);
        expect_chain(1, 33);
        cfg.num = 0;
        assert(run(31, 1, 33, 0) == 1);
        expect(254, 0, 0xFFFF, 16);
        assert(run(31, 1, 51, 0) == 1);
        expect(254, 0, 0xFFFF, 16);
        cfg.num = 2;
        assert(run(31, 1, 51, 0) == 2);
        expect(26, (unsigned)side, 0x2222, 0);
        expect_chain(2, 52);
        assert(pinfo.bType == 1 && pinfo.bKind == ((side == 0) << 4));
        assert(run(31, 1, 5, 0) == 0);
        assert(run(31, 1, 34, 0) == 0);
        cfg.nat = -1;
        assert(run(31, 1, 33, 0) == 1);

        /* type 37 */
        assert(run(37, 1, 7, 33 << 16) == 1);
        expect(32, 0, 0x2222, 0);
        expect_search(0, 0, 1, 7, 14);
        assert(rec.conv_n == 3 && rec.conv_flag[0] == 1 && rec.conv_pos[0] == 9 && rec.conv_flag[1] == 0 &&
               rec.conv_pos[1] == 0 && rec.conv_flag[2] == 1 && rec.conv_pos[2] == 0);
        assert(rec.remove_word == 0x103 && rec.remove_side == side);
        cfg.num = 0;
        assert(run(37, 1, 7, 33 << 16) == 1);
        expect(254, 0, 0xFFFF, 16);
        cfg.num = 2;
        assert(run(37, 1, 34, 0) == 1);
        expect(31, 41, 0x777, 0x0305);
        assert(rec.conv2_n == 1 && rec.conv2_high == side && rec.owner_n == 2 && rec.search_n == 0);
        cfg.num = 0;
        assert(run(37, 1, 7, 0) == 1);
        expect(254, 0, 0xFFFF, 16);
        expect_chain(1, 7);
        assert(rec.owner_n == 0);
        cfg.num = 2;

        /* type 50: three sacrifices, then the unit that takes them */
        cfg.sacunit = 6;
        for (i = 0; i < 3; i++) {
            cfg.sac[0] = cfg.sac[1] = cfg.sac[2] = 0;
            cfg.sac[i] = -1;
            assert(run(50, 0, 0, 0x1234) == 0);
            assert(rec.cerem_obj == 0x1234);
            expect((int)0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE);
        }
        cfg.sac[2] = 0;
        assert(run(50, 0, 0, 0x1234) == 1);
        expect(56, 6, 0x1234, (unsigned)((side << 16) | 0x0305));
        assert(rec.sac_value == side);
        cfg.sacunit = 0;
        assert(run(50, 0, 0, 0x1234) == 1);
        cfg.sacunit = -1;
        assert(run(50, 0, 0, 0x1234) == 0);
    }
    return 0;
}
