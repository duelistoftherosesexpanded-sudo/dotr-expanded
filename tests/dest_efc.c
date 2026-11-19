/* Synthetic host tests for the dest_efc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/dest_efc.h"

enum { S1 = 1, ID1 = 7, S2 = 0, ID2 = 3, NX = 5, NZ = 6 };

static struct {
    int unit, has, side1, side2, loc, search, off_zero, trap76, trap46, piece, lp, rev, unum;
    int grave_own, grave_other, order_mask, card;
} cfg;
static struct {
    int search_n, search[6], off_n, off_flag[2], off_pos[2], trap_a, piece_a, rev_b, unum_a;
    int conv_n;
} rec;
static struct jp_102e6 eff;
static struct jp_10241 info;

void *SzDuel_GetUnit(int a, int b)
{
    assert(a == S1 && b == ID1);
    return cfg.unit ? (void *)&cfg : NULL;
}
struct jp_102e6 *Ta_GetUnitEffect(int a, int b, int n)
{
    assert(a == S1 && b == ID1 && n == 4);
    return cfg.has ? &eff : NULL;
}
int Ta_GetCurrentSide(int a, int b) { return a == S1 && b == ID1 ? cfg.side1 : cfg.side2; }
unsigned ConvUnitIDtoFlagList(unsigned word, int high, int position)
{
    assert(word == 0);
    rec.conv_n++;
    return 0x5000 + high * 16 + position;
}
int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == S1 && b == ID1);
    return cfg.loc;
}
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    rec.search_n++;
    rec.search[0] = a;
    rec.search[1] = b;
    rec.search[2] = c;
    rec.search[3] = d;
    rec.search[4] = e;
    rec.search[5] = f;
    return cfg.search;
}
unsigned ConvUnitIDtoFlagList_OFF(unsigned x, int flag, int n)
{
    assert(rec.off_n < 2);
    rec.off_flag[rec.off_n] = flag;
    rec.off_pos[rec.off_n] = n;
    rec.off_n++;
    return cfg.off_zero ? 0 : x + 1;
}
int Ta_GetEOwnerSide(int value, int mode) { return value * 10 + mode; }
int Ta_IsActiveTrapB(int a, int b)
{
    if (b == 76) {
        rec.trap_a = a;
        return cfg.trap76;
    }
    assert(a == 0 && b == 46);
    return cfg.trap46;
}
int Ta_GetSummonablePiece(int a, unsigned char flag)
{
    assert(flag == 1);
    rec.piece_a = a;
    return cfg.piece;
}
int SzDuel_GetLP(int side)
{
    assert(side == S1);
    return cfg.lp;
}
int Ta_GetLPRevByNaturalEffect(int a, int b)
{
    assert(a == 44);
    rec.rev_b = b;
    return cfg.rev;
}
int Ta_GetUnitNum(int a, unsigned char kind)
{
    assert(kind == 4);
    rec.unum_a = a;
    return cfg.unum;
}
int TaUtil_GetGraveCardNum(int index, int kind)
{
    assert(kind == 0);
    return index == cfg.side1 ? cfg.grave_own : cfg.grave_other;
}
int Ta_GetSummonablePieceWithOrder(int side, int i, int c)
{
    assert(side == cfg.side1 && c == 1 && i >= 0 && i < 8);
    return ((cfg.order_mask >> i) & 1) ? i : -1;
}
int SzDuel_GetUnitCardNo(int a, int b)
{
    assert(a == S1 && b == ID1);
    return cfg.card;
}

static void defaults(void)
{
    memset(&cfg, 0, sizeof(cfg));
    cfg.unit = 1;
    cfg.has = 1;
    cfg.side1 = 1;
    cfg.side2 = 0;
    cfg.loc = 0x0304;
    cfg.search = 0x100;
    cfg.trap76 = -1;
    cfg.trap46 = -1;
    cfg.piece = 4;
    cfg.lp = 4000;
    cfg.rev = 500;
    cfg.unum = 2;
    cfg.grave_own = 2;
    cfg.grave_other = 3;
    cfg.order_mask = 0x0F;
}

static int run5(unsigned type, unsigned sel, unsigned kind, unsigned param, int s2, int id2,
                unsigned flag)
{
    memset(&rec, 0, sizeof(rec));
    rec.trap_a = rec.piece_a = -9;
    memset(&info, 0xAA, sizeof(info));
    eff.dwID = (type << 8) | (sel << 6) | kind;
    eff.dwParam = param;
    return Ta_Manage_DestEfc(S1, ID1, s2, id2, NX, NZ, &info, flag);
}
static int run(unsigned type, unsigned sel, unsigned kind, unsigned param)
{
    return run5(type, sel, kind, param, S2, ID2, 0);
}
static void expect(int eno, unsigned from, unsigned to, unsigned par)
{
    assert(info.eNO == eno);
    assert(info.dwFrom == from);
    assert(info.dwTo == to);
    assert(info.dwPar == par);
}
static void untouched(void) { expect((int)0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA); }
static void searched(int a, int b, int c, int d, int e, int f)
{
    assert(rec.search_n == 1);
    assert(rec.search[0] == a && rec.search[1] == b && rec.search[2] == c);
    assert(rec.search[3] == d && rec.search[4] == e && rec.search[5] == f);
}

static void test_guards(void)
{
    defaults();
    cfg.unit = 0;
    assert(run(38, 0, 0, 0) == 0);
    untouched();
    cfg.unit = 1;
    cfg.has = 0;
    assert(run(38, 0, 0, 0) == 0);
    untouched();
    cfg.has = 1;
    cfg.side2 = cfg.side1;
    assert(run(38, 0, 0, 0) == 0);
    untouched();
    cfg.side2 = 0;
    assert(run(99, 0, 0, 0) == 0);
    untouched();
}

static void test_simple_types(void)
{
    defaults();
    assert(run(2, 0, 0, 0x00030008) == 1);
    expect(2, (NZ << 8) | NX, 3, 8);
    assert(run(2, 0, 0, 0x00040009) == 1);
    expect(2, (NZ << 8) | NX, 4, 9);

    assert(run(36, 0, 0, 0) == 1);
    expect(30, 0, 0x5000 + S1 * 16 + ID1, 1);

    assert(run(38, 0, 0, 0) == 1);
    expect(33, 0, 1, 0);

    assert(run(26, 0, 0, 0x00090123) == 1);
    expect(65, 0, 0, 0x123);
    cfg.side1 = 0;
    cfg.side2 = 1;
    assert(run(26, 0, 0, 0x00090123) == 1);
    expect(65, 0, 1, 0x123);
    defaults();

    assert(run(13, 1, 0, 0x00120034) == 1);
    expect(8, 0, 11, 0x00120034);
    assert(run(13, 2, 0, 0x00120034) == 1);
    expect(9, 0x0304, 12, 0x00120034);
    assert(run(13, 0, 0, 0x00120034) == 0);
    untouched();
    assert(run(13, 3, 0, 0x00120034) == 0);
    untouched();
}

static void test_type_19_23(void)
{
    defaults();
    /* Type 19, kind 26: the other unit, only with a valid second unit and no flag. */
    assert(run(19, 0, 26, 0x00020077) == 1);
    expect(57, 0, 0x5000 + S2 * 16 + ID2 + 1, 0x77);
    assert(rec.off_n == 1 && rec.off_flag[0] == S1 && rec.off_pos[0] == ID1 && rec.search_n == 0);
    assert(run5(19, 0, 26, 0, -1, ID2, 0) == 0);
    untouched();
    assert(run5(19, 0, 26, 0, S2, 0, 0) == 0);
    untouched();
    assert(run5(19, 0, 26, 0, S2, ID2, 1) == 0);
    untouched();
    /* Other kinds search: whole board for range 0, else around the unit's cell. */
    assert(run(19, 2, 5, 0x00000077) == 1);
    searched(0, 0, 1, 2, 5, 14);
    expect(57, 0, 0x101, 0x77);
    assert(run(19, 2, 5, 0x00030077) == 1);
    searched(4, 3, 1, 2, 5, 3);
    expect(57, 0, 0x101, 0x77);
    assert(run5(19, 2, 5, 0x00030077, S2, ID2, 1) == 1);
    expect(57, 0, 0x102, 0x77);
    assert(rec.off_n == 2 && rec.off_flag[1] == S2 && rec.off_pos[1] == ID2);
    cfg.off_zero = 1;
    assert(run(19, 2, 5, 0x00030077) == 0);
    untouched();
    cfg.off_zero = 0;

    assert(run(23, 0, 26, 0x00020077) == 0x401);
    expect(0, 0, 0x5000 + S2 * 16 + ID2, 0x77);
    assert(rec.off_n == 0);
    assert(run5(23, 0, 26, 0, -1, ID2, 0) == 0);
    untouched();
    assert(run5(23, 0, 26, 0, S2, 0, 0) == 0);
    untouched();
    assert(run5(23, 0, 26, 0, S2, ID2, 1) == 0);
    untouched();
    assert(run(23, 3, 9, 0x00020077) == 0x401);
    searched(0, 0, 1, 3, 9, 14);
    expect(0, 0, 0x100, 0x77);
    assert(run5(23, 3, 9, 0x00020077, S2, ID2, 1) == 0x401);
    expect(0, 0, 0x101, 0x77);
    assert(rec.off_n == 1 && rec.off_flag[0] == S2 && rec.off_pos[0] == ID2);
    cfg.search = 0;
    assert(run(23, 3, 9, 0x00020077) == 0);
    expect(0, 0, 0, 0x77);
}

static void test_type_20(void)
{
    unsigned list = 0x5000 + S1 * 16 + ID1;
    unsigned from = ((S1 << 8) | ID1) | 0x10000;

    defaults();
    assert(run(20, 0, 0, 0) == 0x101);
    expect(31, from, list, 4);
    assert(rec.trap_a == 0 && rec.piece_a == S1);
    cfg.piece = -1;
    assert(run(20, 0, 0, 0) == 0);
    untouched();
    cfg.piece = 0;
    cfg.trap76 = 0;
    assert(run(20, 0, 0, 0) == 1);
    expect(102, 1, 0, 0);
    cfg.trap76 = 6;
    assert(run(20, 0, 0, 0) == 1);
    expect(102, 1, 6, 0);
    cfg.trap46 = 0;
    assert(run(20, 0, 0, 0) == 0x103);
    expect(31, from, list, 0);
    cfg.piece = -1;
    assert(run(20, 0, 0, 0) == 0);
    untouched();
}

static void test_type_62(int trapped)
{
    int hit = trapped ? 3 : 1;

    defaults();
    if (trapped) {
        cfg.trap76 = 0;
        cfg.trap46 = 0;
    }
    /* Num1 = 5 - 2, Num2 = 2 + 3, Num3 = 4 summonable orders: the smallest is Num1. */
    assert(run(62, 0, 0, 0x00010321) == hit);
    expect(58, 500, 1, 3);
    assert(rec.rev_b == 0x321 && rec.piece_a == 1 && rec.unum_a == 1 && rec.trap_a == 0);
    cfg.grave_own = 1;
    cfg.grave_other = 1;
    assert(run(62, 0, 0, 0) == hit);
    expect(58, 500, 1, 2);
    cfg.order_mask = 0x80;
    assert(run(62, 0, 0, 0) == hit);
    expect(58, 500, 1, 1);
    cfg.order_mask = 0xFF;
    cfg.unum = 0;
    cfg.grave_own = 9;
    assert(run(62, 0, 0, 0) == hit);
    expect(58, 500, 1, 5);
    /* A negative life-point revision: its magnitude under the traps, the raw value without. */
    cfg.rev = -5;
    assert(run(62, 0, 0, 0) == hit);
    expect(58, trapped ? 5u : (unsigned)-5, 1, 5);
    cfg.rev = 0;
    assert(run(62, 0, 0, 0) == hit);
    expect(58, 0, 1, 5);
    /* Each empty count refuses. */
    cfg.unum = 5;
    assert(run(62, 0, 0, 0) == 0);
    untouched();
    cfg.unum = 6;
    assert(run(62, 0, 0, 0) == 0);
    cfg.unum = 4;
    assert(run(62, 0, 0, 0) == hit);
    expect(58, 0, 1, 1);
    cfg.grave_own = 0;
    cfg.grave_other = 0;
    assert(run(62, 0, 0, 0) == 0);
    untouched();
    cfg.grave_other = 1;
    assert(run(62, 0, 0, 0) == hit);
    cfg.order_mask = 0;
    assert(run(62, 0, 0, 0) == 0);
    untouched();
    cfg.order_mask = 1;
    cfg.piece = -1;
    assert(run(62, 0, 0, 0) == 0);
    untouched();
    cfg.piece = 0;
    assert(run(62, 0, 0, 0) == hit);
    /* Life points must exceed the revision. */
    cfg.rev = 4000;
    assert(run(62, 0, 0, 0) == 0);
    untouched();
    cfg.rev = 3999;
    assert(run(62, 0, 0, 0) == hit);
}

static void test_type_62_trap_only(void)
{
    defaults();
    cfg.trap76 = 5;
    assert(run(62, 0, 0, 0) == 1);
    expect(102, 1, 5, 0);
    cfg.trap76 = 0;
    assert(run(62, 0, 0, 0) == 1);
    expect(102, 1, 0, 0);
}

static void test_type_65(void)
{
    defaults();
    cfg.lp = 0x21;
    assert(run(65, 2, 9, 0x00200044) == 1);
    searched(0, 0, 1, 2, 9, 14);
    assert(rec.off_n == 1 && rec.off_flag[0] == S1 && rec.off_pos[0] == ID1);
    expect(59, 1, 0x101, 0x00200044);
    cfg.off_zero = 1;
    assert(run(65, 2, 9, 0x00200044) == 0);
    untouched();
    cfg.off_zero = 0;
    cfg.lp = 0x20;
    assert(run(65, 2, 9, 0x00200044) == 0);
    untouched();
    assert(rec.search_n == 0);
}

static void test_type_56_81(void)
{
    defaults();
    assert(run(56, 0, 0, 0x001F0002) == 1);
    expect(1, 1, 4, 0x001F0002);
    assert(rec.piece_a == 1 && eff.dwParam == 0x001F0002);
    assert(run(56, 0, 0, 0x001E0002) == 1);
    expect(1, 1, 4, 0x001E0002);
    assert(rec.piece_a == 0);
    assert(run(56, 0, 0, 0x001D0002) == 0);
    untouched();
    /* Type 56 compares the whole high half, type 81 only its low byte. */
    assert(run(56, 0, 0, 0x011F0002) == 0);
    untouched();
    cfg.piece = -1;
    assert(run(56, 0, 0, 0x001F0002) == 0);
    untouched();
    assert(run(56, 0, 0, 0x001E0002) == 0);
    untouched();
    cfg.piece = 0;
    assert(run(56, 0, 0, 0x001F0002) == 1);
    expect(1, 1, 0, 0x001F0002);

    cfg.piece = 4;
    assert(run(81, 0, 0, 0x011F0002) == 1);
    expect(1, 0x10001, 4, 0x011F0002);
    assert(rec.piece_a == 1 && eff.dwID == 0xFFFFFFFF && eff.dwParam == 0xFFFFFFFF);
    assert(run(81, 0, 0, 0x001E0002) == 1);
    expect(1, 0x10001, 4, 0x001E0002);
    assert(rec.piece_a == 0 && eff.dwID == 0xFFFFFFFF && eff.dwParam == 0xFFFFFFFF);
    assert(run(81, 0, 0, 0x001D0002) == 0);
    untouched();
    cfg.piece = -1;
    assert(run(81, 0, 0, 0x001F0002) == 0);
    untouched();
    assert(eff.dwParam == 0x001F0002);
    assert(run(81, 0, 0, 0x001E0002) == 0);
    untouched();
}

static void test_type_4(void)
{
    defaults();
    assert(run(4, 0, 26, 0) == 0x201);
    expect(21, 0, 0x5000 + S2 * 16 + ID2, 0);
    assert(run5(4, 0, 26, 0, -1, ID2, 0) == 0);
    untouched();
    assert(run5(4, 0, 26, 0, S2, 0, 0) == 0);
    untouched();
    assert(run5(4, 0, 26, 0, S2, ID2, 1) == 0);
    untouched();

    assert(run(4, 1, 30, 0x00030000) == 1);
    searched(NX, NZ, 1, 1, 30, 3);
    expect(21, 0, 0x100, 0);
    cfg.card = 617;
    assert(run(4, 1, 30, 0x00030000) == 1);
    expect(23, 0, 0x100, 0);
    cfg.search = 0;
    assert(run(4, 1, 30, 0x00030000) == 0);
    expect(23, 0, 0, 0);
    cfg.search = 0x100;

    cfg.card = 0;
    assert(run(4, 2, 31, 0x00030000) == 0x201);
    searched(NX, NZ, 1, 2, 31, 3);
    assert(rec.off_n == 1 && rec.off_flag[0] == S1 && rec.off_pos[0] == ID1);
    expect(21, (S1 << 8) | ID1, 0, 1);
    assert(run5(4, 2, 31, 0x00030000, S2, ID2, 1) == 0x201);
    assert(rec.off_n == 2 && rec.off_flag[1] == S2 && rec.off_pos[1] == ID2);
    expect(21, (S1 << 8) | ID1, 0, 1);
    cfg.card = 509;
    assert(run(4, 2, 31, 0x00030000) == 0x201);
    expect(46, (S1 << 8) | ID1, 0, 1);
    cfg.card = 506;
    assert(run(4, 2, 31, 0x00030000) == 0x201);
    expect(46, (S1 << 8) | ID1, 0, 1);

    assert(run(4, 0, 29, 0) == 0);
    untouched();
}

int main(void)
{
    test_guards();
    test_simple_types();
    test_type_19_23();
    test_type_20();
    test_type_62(0);
    test_type_62(1);
    test_type_62_trap_only();
    test_type_65();
    test_type_56_81();
    test_type_4();
    return 0;
}
