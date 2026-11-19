/* Synthetic host tests for the kind_gates group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/kind_gates.h"

/* Every callee is a stub. The switch cell is (0x11, 0x22); the gated cell and
 * its two further arguments are (0x33, 0x44, 0x55, 0x66). */
static int sel_v, sel_calls, kind_v, kind_calls, veto_v, veto_calls;
static int score_v, score_calls, try_v, try_calls;

int SzDuel_GetUnitKindOfs(int a, int b)
{
    assert(a == 0x11 && b == 0x22);
    sel_calls++;
    return sel_v;
}
static int preset_mode;

int SzDuel_GetUnitKind(int a, int b)
{
    if (preset_mode) {
        assert(a == 0x77 && b == 0);
    } else {
        assert(a == 0x33 && b == 0x44);
    }
    kind_calls++;
    return kind_v;
}
int Ta_IsLDRangeAvilityWithLocation(int a, int b, int c, int d, int e)
{
    assert(a == 0x33 && b == 0x44 && c == 0x55 && d == 0x66 && e == 11);
    veto_calls++;
    return veto_v;
}
int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    assert(a == 0x33 && b == 0x44 && c == 0x55 && d == 0x66);
    score_calls++;
    return score_v;
}
int func_001e0290_try(int a, int b, int flag)
{
    assert(a == 0x33 && b == 0x44 && flag == 0);
    try_calls++;
    return try_v;
}

static int gate(int sel, int kind, int score, int try_answer, int veto)
{
    sel_v = sel;
    kind_v = kind;
    score_v = score;
    try_v = try_answer;
    veto_v = veto;
    sel_calls = kind_calls = veto_calls = score_calls = try_calls = 0;
    return TaTrapA_IsTrapAct(0x11, 0x22, 0x33, 0x44, 0x55, 0x66);
}

/* One scored case: `limit` is the largest passing answer of the scoring callee. */
static void check_scored(int sel, int limit, int uses_try)
{
    int *calls = uses_try ? &try_calls : &score_calls;
    int *other = uses_try ? &score_calls : &try_calls;

    assert(gate(sel, 31, limit, limit, 0) == 1);
    assert(sel_calls == 1 && kind_calls == 1 && *calls == 1 && *other == 0 && veto_calls == 1);
    assert(gate(sel, 31, limit, limit, 7) == 0);
    assert(*calls == 1 && veto_calls == 1);
    assert(gate(sel, 31, limit + 1, limit + 1, 0) == 0);
    assert(*calls == 1 && veto_calls == 0);
    assert(gate(sel, 32, limit, limit, 0) == 0);
    assert(kind_calls == 1 && *calls == 0 && veto_calls == 0);
}

static void check_gate(void)
{
    int sel;

    for (sel = 0; sel <= 5; sel++) {
        assert(gate(sel, 31, 9999, 9999, 0) == 1);
        assert(sel_calls == 1 && kind_calls == 1 && veto_calls == 1);
        assert(score_calls == 0 && try_calls == 0);
        assert(gate(sel, 31, 0, 0, 1) == 0);
        assert(veto_calls == 1);
        assert(gate(sel, 32, 0, 0, 0) == 0);
        assert(kind_calls == 1 && veto_calls == 0);
    }
    check_scored(6, 500, 0);
    check_scored(7, 1000, 0);
    check_scored(8, 1500, 1);
    check_scored(9, 2000, 0);
    check_scored(10, 3000, 0);
    assert(gate(11, 32, 9999, 9999, 1) == 1);
    assert(sel_calls == 1 && kind_calls == 0 && veto_calls == 0);
    assert(gate(12, 32, 9999, 9999, 0) == 1);
    assert(kind_calls == 1 && veto_calls == 1 && score_calls == 0 && try_calls == 0);
    assert(gate(12, 32, 0, 0, 3) == 0);
    assert(veto_calls == 1);
    assert(gate(12, 31, 0, 0, 0) == 0);
    assert(kind_calls == 1 && veto_calls == 0);
    assert(gate(13, 0, 0, 0, 0) == 0);
    assert(sel_calls == 1 && kind_calls == 0 && veto_calls == 0);
    assert(gate(-1, 0, 0, 0, 0) == 0);
    assert(kind_calls == 0 && veto_calls == 0);
}

/* 0x26f690 fixtures: record 0x77; the row table is host data. */
unsigned short Destiny_Tab[16][3];
static int flags_v, flags_calls, store_v, store_calls, side_calls;

int SzDuel_InqDestinyFlag(int index)
{
    assert(index == 0x77);
    flags_calls++;
    return flags_v;
}
void SzDuel_SetDestinyFlag(int index, int value)
{
    assert(index == 0x77);
    store_v = value;
    store_calls++;
}
int SzDuel_GetTurnOwner(void)
{
    side_calls++;
    return 0x4321;
}

static const short presets[21][3] = {
    {0, 0, 0}, {2, 1, -3}, {12, 6, -18}, {2, 1, -3}, {5, 3, -8}, {8, 4, -12}, {6, 3, -9},
    {0, 0, 0}, {12, 6, -18}, {10, 5, -15}, {14, 7, -21}, {15, 8, -23}, {8, 4, -12},
    {14, 7, -21}, {3, 1, -4}, {10, 5, -15}, {2, 1, -3}, {13, 6, -19}, {10, 5, -15},
    {10, 5, -15}, {-5, -2, 7},
};

static int preset(int kind, int flags, unsigned short *first, short *out, int *note)
{
    preset_mode = 1;
    kind_v = kind;
    flags_v = flags;
    flags_calls = kind_calls = store_calls = side_calls = 0;
    store_v = -1;
    first[0] = first[1] = first[2] = 0x5555;
    out[0] = out[1] = out[2] = 0x5555;
    kind = Ta_GetDestinyDraw(0x77, first, out, note);
    preset_mode = 0;
    return kind;
}

static void check_preset(void)
{
    unsigned short first[3];
    short out[3];
    int note[4];
    int kind, n;

    for (n = 0; n < 16; n++) {
        Destiny_Tab[n][0] = (unsigned short)(0x9000 + n);
        Destiny_Tab[n][1] = (unsigned short)(0xa000 + n);
        Destiny_Tab[n][2] = (unsigned short)(0xb000 + n);
    }
    /* Every case, flags 0x25: row 5, delta 0x20 * 5 = 160; bits above 0x1f of the kind are dropped. */
    for (kind = 0; kind <= 20; kind++) {
        int a = presets[kind][0] + 160, b = presets[kind][1] + 80;

        memset(note, 0x55, sizeof note);
        assert(preset(kind | 0x20, 0x25, first, out, note) == 1);
        assert(flags_calls == 1 && kind_calls == 1 && store_calls == 1 && side_calls == 1);
        assert(first[0] == 0x9005 && first[1] == 0xa005 && first[2] == 0xb005);
        assert(out[0] == a + 10 && out[1] == b + 20 && out[2] == -(a + b) + 70);
        assert(store_v == 0xa5);
        assert(note[0] == 114 && note[1] == 0x4321 && note[2] == 0 && note[3] == 0);
    }
    /* Null note: no 0x1de540 call; only the low byte of the flags is stored. */
    assert(preset(4, 0x30f, first, out, NULL) == 1);
    assert(side_calls == 0 && store_calls == 1 && store_v == 0x8f);
    assert(first[0] == 0x900f && first[1] == 0xa00f && first[2] == 0xb00f);
    assert(out[0] == 5 + 10 && out[1] == 3 + 20 && out[2] == -8 + 70);
    /* Bit 0x80 already set: -1 after the 999 stores and the preset. */
    assert(preset(4, 0x80, first, out, note) == -1);
    assert(store_calls == 0 && side_calls == 0);
    assert(first[0] == 999 && first[1] == 999 && first[2] == 999);
    assert(out[0] == 5 && out[1] == 3 && out[2] == -8);
    /* Kinds 21 and 31: -1 with out untouched. */
    for (kind = 21; kind <= 31; kind += 10) {
        assert(preset(kind, 0x25, first, out, note) == -1);
        assert(flags_calls == 1 && kind_calls == 1 && store_calls == 0 && side_calls == 0);
        assert(first[0] == 999 && first[1] == 999 && first[2] == 999);
        assert(out[0] == 0x5555 && out[1] == 0x5555 && out[2] == 0x5555);
    }
}

/* 0x1e0690 stub for 0x271c90: answers a set word (bits above the low five are noise). */
static int word_index, word_v;

int SzDuel_GetUnitConstKind(int a, int b)
{
    assert(a == word_index && b == 0);
    return word_v;
}

/* Ta_IsLD_PDownObj (branch coverage: every table case against every kind -1..18, and the
 * out-of-range words). */
static void check_kind(void)
{
    static const signed char accept[18][2] = {
        { -9, -9 }, { 7, -9 }, { 3, -9 }, { 0, 1 }, { -9, -9 }, { 12, -9 }, { 9, 12 }, { 1, -9 }, { -9, -9 },
        { 13, -9 }, { -9, -9 }, { 5, 6 }, { 15, -9 }, { -9, -9 }, { -9, -9 }, { 2, 14 }, { 14, 17 }, { 4, -9 },
    };
    int word, kind, want;

    word_index = 6;
    for (word = 0; word < 32; word++) {
        for (kind = -1; kind <= 18; kind++) {
            want = word < 18 && (kind == accept[word][0] || kind == accept[word][1]);
            word_v = word;
            assert(Ta_IsLD_PDownObj(6, kind) == want);
            /* only the low five bits select the case */
            word_v = word | 0x7e0;
            assert(Ta_IsLD_PDownObj(6, kind) == want);
        }
    }
    /* spot checks written out: one accepted and one neighbouring kind per two-kind case */
    word_v = 3;
    assert(Ta_IsLD_PDownObj(6, 0) == 1 && Ta_IsLD_PDownObj(6, 1) == 1 && Ta_IsLD_PDownObj(6, 2) == 0);
    word_v = 11;
    assert(Ta_IsLD_PDownObj(6, 4) == 0 && Ta_IsLD_PDownObj(6, 5) == 1 && Ta_IsLD_PDownObj(6, 6) == 1 && Ta_IsLD_PDownObj(6, 7) == 0);
    word_v = 16;
    assert(Ta_IsLD_PDownObj(6, 14) == 1 && Ta_IsLD_PDownObj(6, 17) == 1 && Ta_IsLD_PDownObj(6, 15) == 0);
    word_v = 18;
    assert(Ta_IsLD_PDownObj(6, 4) == 0);
}

/* Stubs for 0x271af0. The item is `gate_item`, the cell (0x21, 0x43); 0x1e08f0
 * answers `amount[0]` for the side and `amount[1]` for the other side and
 * counts its calls; 0x271c90 is the real function over the 0x1e0690 stub. */
static struct DotrGateItem gate_item;
static int side_v, hit_v, pre_v, pair_v, amount[2], amount_calls[2];

int func_00250370_gate(struct DotrGateItem *item)
{
    assert(item == &gate_item);
    return side_v;
}

int Ta_IsLDRangeAvilityFromLoc(int packed, int b, int c, int d)
{
    assert(packed == 0x4321 && b == side_v && c == (gate_item.kind & 0x1f) && d == 10);
    return hit_v;
}

int SzDuel_GetUnitLevel(int a, int b, int flag)
{
    int which = a == side_v ? 0 : 1;

    assert((which == 0 || a == (side_v == 0)) && b == 0 && flag == 0);
    amount_calls[which]++;
    return amount[which];
}

int Ta_IsLDRange(int a, int b, int c)
{
    assert(a == (side_v == 0) && b == 0x21 && c == 0x43);
    return pre_v;
}

int Ta_IsLDAbl(int a, int b)
{
    assert(a == 14 && b == (side_v == 0));
    return pair_v;
}

static int gate_total(int side, int hit, int pre, int pair, int word, int kind, int own, int theirs)
{
    side_v = side;
    hit_v = hit;
    pre_v = pre;
    pair_v = pair;
    word_index = side == 0;
    word_v = word;
    gate_item.kind = (unsigned char)kind;
    amount[0] = own;
    amount[1] = theirs;
    amount_calls[0] = amount_calls[1] = 0;
    return Ta_GetLDParamOfs2(&gate_item, 0x21, 0x43);
}

/* Ta_GetLDParamOfs2 (branch coverage: null item, each gate off and on, both sides of
 * the 500 bound on each amount, side 0 and not 0). */
static void check_total(void)
{
    assert(Ta_GetLDParamOfs2(NULL, 0x21, 0x43) == 0);
    /* nothing added, 0x271a40 answers 0: no 0x271740 / 0x1e08f0 call */
    assert(gate_total(0, 0, 0, 1, 1, 7, 3, 2) == 0 && amount_calls[0] == 0 && amount_calls[1] == 0);
    /* own amount below the bound (two calls), at it and above it (one call) */
    assert(gate_total(0, 1, 0, 1, 1, 7, 4, 2) == 400 && amount_calls[0] == 2);
    assert(gate_total(0, 1, 0, 1, 1, 7, 5, 2) == 500 && amount_calls[0] == 1);
    assert(gate_total(3, 1, 0, 1, 1, 7, 9, 2) == 500 && amount_calls[0] == 1 && amount_calls[1] == 0);
    /* 0x271740 answers 0 */
    assert(gate_total(0, 1, 1, 0, 1, 7, 4, 2) == 400 && amount_calls[1] == 0);
    /* 0x271c90 answers 0: word 1 accepts only kind 7 (the bits above the low five of the item byte
     * are dropped) */
    assert(gate_total(0, 1, 1, 1, 1, 8, 4, 2) == 400 && amount_calls[1] == 0);
    /* all three gates pass: other amount below, at and above the bound */
    assert(gate_total(0, 1, 1, 1, 1, 0xe7, 4, 2) == 200 && amount_calls[1] == 2);
    assert(gate_total(0, 1, 1, 1, 1, 7, 4, 5) == -100 && amount_calls[1] == 1);
    assert(gate_total(3, 0, 1, 1, 1, 7, 4, 77) == -500 && amount_calls[0] == 0 && amount_calls[1] == 1);
    assert(gate_total(3, 1, 1, 1, 2, 3, 1, 3) == -200 && amount_calls[0] == 2 && amount_calls[1] == 2);
}

static void check_position_table(void)
{
    static const int near_values[7] = { 0x200, 0x300, 0x400, 0x500, 0x600, 0x601, 0x602 };
    static const int far_values[7] = { 0x206, 0x306, 0x406, 0x506, 0x606, 0x605, 0x604 };
    int i;

    for (i = 0; i < 7; i++) {
        side_calls = 0;
        assert(TaLogic017_GetLine(i, 0) == near_values[i] && side_calls == 1);
        assert(TaLogic017_GetLine(i, 1) == far_values[i] && side_calls == 2);
    }
    /* Boundaries of the table: 7 and a negative index take the -1 exit, still after the call. */
    side_calls = 0;
    assert(TaLogic017_GetLine(7, 0) == -1 && TaLogic017_GetLine(-1, 1) == -1 && side_calls == 2);
}

static void check_mode_positions(void)
{
    static const int mode_one[7] = { 0x203, 0x302, 0x301, 0x300, 0x104, 0x203, 4 };
    static const int other[4] = { 0x102, 0x201, 0x200, 2 };
    int i;

    for (i = 0; i < 7; i++) {
        assert(TaLogic002_GetLinePiece(1, i) == mode_one[i]);
    }
    /* Boundary of the mode-1 table. */
    assert(TaLogic002_GetLinePiece(1, 7) == 0x203 && TaLogic002_GetLinePiece(1, 255) == 0x203);
    for (i = 0; i < 4; i++) {
        assert(TaLogic002_GetLinePiece(0, i) == other[i]);
        assert(TaLogic002_GetLinePiece(2, i) == other[i]);
    }
    assert(TaLogic002_GetLinePiece(0, 4) == 0x102 && TaLogic002_GetLinePiece(0, 255) == 0x102);
}

static int held_id[8][8], held_bad_high, held_calls;

int SzDuel_GetMapUnitID(int x, int y)
{
    assert(x >= 0 && x < 8 && y >= 0 && y < 8);
    held_calls++;
    return held_id[x][y];
}

int func_00250360_pair(int a, int b)
{
    assert(b != 0 || a == 1);
    return a == held_bad_high ? 0x1111 : 0x4321;
}

static void held_clear(void)
{
    int x, y;

    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            held_id[x][y] = -1;
        }
    }
    held_bad_high = 7;
}

static void check_held_positions(void)
{
    /* Mode 1 reads (3,2) (2,3) (1,3) (0,3) (4,1) and (3,2) again; other modes (2,1) (1,2) (0,2) (2,0). */
    held_clear();
    assert(TaLogic002_IsMakeDefense(1) == 0 && TaLogic002_IsMakeDefense(0) == 0);
    held_id[2][3] = 0x105;
    assert(TaLogic002_IsMakeDefense(1) == 0);
    assert(TaLogic002_IsMakeDefense(0) == 0);
    held_id[4][1] = 0x203;
    assert(TaLogic002_IsMakeDefense(1) == 1);
    held_id[4][1] = 0x705;
    assert(TaLogic002_IsMakeDefense(1) == 0);
    held_id[4][1] = 0x100;
    assert(TaLogic002_IsMakeDefense(1) == 0);
    held_id[0][3] = 0x301;
    assert(TaLogic002_IsMakeDefense(1) == 1);
    /* Index 0 and 5 name the same position, so one hit there counts twice. */
    held_clear();
    held_id[3][2] = 0x105;
    assert(TaLogic002_IsMakeDefense(1) == 1 && TaLogic002_IsMakeDefense(0) == 0);
    /* (4,0) belongs to index 6, which the six-position walk does not reach. */
    held_clear();
    held_id[4][0] = 0x105;
    held_id[1][3] = 0x105;
    assert(TaLogic002_IsMakeDefense(1) == 0);
    /* A mode-0 position does not count for mode 1 and the reverse. */
    held_clear();
    held_id[2][1] = 0x105;
    held_id[1][2] = 0x106;
    assert(TaLogic002_IsMakeDefense(1) == 0);
    assert(TaLogic002_IsMakeDefense(0) == 1 && TaLogic002_IsMakeDefense(2) == 1);
    held_id[1][2] = -1;
    assert(TaLogic002_IsMakeDefense(0) == 1);
    held_id[2][1] = 0x705;
    assert(TaLogic002_IsMakeDefense(0) == 0);
    held_id[2][1] = 0x100;
    assert(TaLogic002_IsMakeDefense(0) == 0);
    held_id[2][1] = -1;
    held_id[2][0] = 0x101;
    assert(TaLogic002_IsMakeDefense(0) == 1);
}

static void check_eight_positions(void)
{
    static const int expected[8] = { 2, 6, 0x406, 0x404, 0x604, 0x600, 0x200, 0x202 };
    int i;

    for (i = 0; i < 8; i++) {
        assert(TaLogic_GetCyclePoint(i) == expected[i]);
    }
    /* Boundaries of the table. */
    assert(TaLogic_GetCyclePoint(8) == 2 && TaLogic_GetCyclePoint(-1) == 2);
}

static int q_kind, q_id, q_side, *q_list, q_order, q_value, q_count, q_seq;

int TaHand_GetCardKind(int id)
{
    assert(q_seq++ == 0);
    q_id = id;
    return q_kind;
}

int TaLogic005_Is2Head_ThunderDragon(int side, int *list)
{
    assert(q_seq++ == 1);
    q_side = side;
    q_list = list;
    return q_order;
}

static int walk_on, walk_first, walk_count, walk_pos[8], walk_set[8], walk_made, walk_bits;

int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    if (walk_on) {
        assert(value == 0x5a5a && walk_made == 1);
        return walk_count;
    }
    assert(q_seq++ == 2);
    q_value = (int)value;
    return q_count;
}

static int kind_query(int kind, int order, int count)
{
    static int list[5];
    int r;

    q_kind = kind;
    q_order = order;
    q_count = count;
    q_seq = 0;
    r = TaLogic005_IsKeepHandCombo(1, list, 0x1234);
    assert(q_seq == 3 && q_id == 0x1234 && q_side == 1 && q_list == list && q_value == (order & 0xff));
    return r;
}

static void check_kind_query(void)
{
    /* A count of 3 wins over every kind. */
    assert(kind_query(0, 0x102, 3) == 0 && kind_query(15 << 5, 7, 3) == 0);
    assert(kind_query(0, 0x102, 2) == 1 && kind_query(31, 0, 4) == 1);
    assert(kind_query(15 << 5, 0, 0) == 1 && kind_query((15 << 5) | 31, 0, 0) == 1);
    assert(kind_query(18 << 5, 0, 0) == 1);
    /* Neighbours of the three accepted kinds. */
    assert(kind_query(1 << 5, 0, 0) == 0 && kind_query(14 << 5, 0, 0) == 0 && kind_query(16 << 5, 0, 0) == 0);
    assert(kind_query(17 << 5, 0, 0) == 0 && kind_query(19 << 5, 0, 0) == 0 && kind_query(-1, 0, 0) == 0);
}

int TaLogic_IsAttackedNow(int a, int b, int c)
{
    assert(a == 1 && b == 0 && c == 0 && walk_made == 0);
    return walk_first;
}

int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0 && b == 0 && c == 1 && d == 2 && e == 52 && f == 14);
    walk_made++;
    return 0x5a5a;
}

int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert(word == 0x5a5a && n >= 0 && n < walk_count);
    return walk_pos[n];
}

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    int n;

    assert(bit == 4);
    for (n = 0; n < walk_count; n++) {
        if (walk_pos[n] == ((a << 8) | b)) {
            walk_bits++;
            return walk_set[n];
        }
    }
    assert(0);
    return 0;
}

static int walk(int first, int count, int set_mask)
{
    int n, r;

    walk_on = 1;
    walk_first = first;
    walk_count = count;
    walk_made = 0;
    walk_bits = 0;
    for (n = 0; n < 8; n++) {
        walk_pos[n] = 0x300 + (n << 8) + n + 1;
        walk_set[n] = (set_mask >> n) & 1;
    }
    r = TaLogic019_IsUseGFK(1);
    walk_on = 0;
    assert(first == 1 ? walk_made == 0 && walk_bits == 0 : walk_made == 1 && walk_bits == count);
    return r;
}

static void check_clear_entries(void)
{
    /* The first answer of 1 returns before the list is made. */
    assert(walk(1, 8, 0) == 1);
    assert(walk(0, 0, 0) == 0 && walk(2, 0, 0) == 0);
    /* Five clear entries are needed. */
    assert(walk(0, 4, 0) == 0 && walk(0, 5, 0) == 1 && walk(0, 8, 0) == 1);
    assert(walk(0, 8, 0x0f) == 0 && walk(0, 8, 0x07) == 1 && walk(0, 8, 0xff) == 0);
    assert(walk(0, 6, 0x21) == 0 && walk(0, 6, 0x20) == 1);
}

static int drop_v, drop_calls, pair_calls, here_calls;

int func_001df540_word(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    drop_calls++;
    return drop_v;
}

int func_0024de70_pair(int a, int b)
{
    assert(a == 0x12 && b == 0x34 && drop_calls == 1);
    pair_calls++;
    return 0;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == 0x12 && b == 0x34 && pair_calls == 1);
    here_calls++;
    return 0;
}

static void spot_order(struct DotrPositionOrder *order, int mode, int hit, int found)
{
    order->position = 0x1234;
    order->unknown_02 = 0x1111;
    order->kind = 0x2222;
    order->unknown_06 = 0x3333;
    order->flags.word = 0xaaaa5555;
    drop_v = found;
    drop_calls = pair_calls = here_calls = held_calls = 0;
    TaLogic_CommandDefenseUnit(order, mode, hit);
    assert(order->position == 0x1234 && order->unknown_02 == 0x1111 && order->unknown_06 == 0x3333);
    if (found) {
        assert(drop_calls == 2 && pair_calls == 1 && here_calls == 1);
    }
}

static void check_spot_order(void)
{
    struct DotrPositionOrder order;
    int x, y;

    /* No block: nothing is written and nothing else is called. */
    held_clear();
    spot_order(&order, 1, 1, 0);
    assert(drop_calls == 1 && pair_calls == 0 && here_calls == 0 && held_calls == 0);
    assert(order.kind == 0x2222 && order.flags.word == 0xaaaa5555);
    /* An empty first position is taken; the kind byte follows the third argument. */
    spot_order(&order, 1, 1, 1);
    assert(order.kind == 0x3ff && order.flags.word == 0x02035555 && held_calls == 1);
    spot_order(&order, 1, 0, 1);
    assert(order.kind == 0x6ff && order.flags.word == 0x02035555);
    spot_order(&order, 0, 2, 1);
    assert(order.kind == 0x6ff && order.flags.word == 0x01025555 && held_calls == 1);
    /* Occupied positions are passed over until the order's own id or an empty one. */
    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            held_id[x][y] = 0x777;
        }
    }
    spot_order(&order, 1, 1, 1);
    assert(order.kind == 0x3ff && order.flags.word == 0xaaaa5555 && held_calls == 6);
    spot_order(&order, 0, 1, 1);
    assert(order.flags.word == 0xaaaa5555 && held_calls == 4);
    spot_order(&order, 2, 1, 1);
    assert(order.flags.word == 0xaaaa5555 && held_calls == 4);
    held_id[1][3] = 0x1234;
    spot_order(&order, 1, 1, 1);
    assert(order.flags.word == 0x03015555 && held_calls == 3);
    held_id[4][1] = 0;
    held_id[1][3] = 0x777;
    spot_order(&order, 1, 1, 1);
    assert(order.flags.word == 0xaaaa5555 && held_calls == 6);
    held_id[4][1] = -1;
    spot_order(&order, 1, 1, 1);
    assert(order.flags.word == 0x01045555 && held_calls == 5);
    held_id[2][0] = -5;
    spot_order(&order, 0, 1, 1);
    assert(order.flags.word == 0x00025555 && held_calls == 4);
}

/* 0x117b10: 0x1e36e0 answers the gate item or 0 and checks it is asked for
 * (3, 3); 0x250370 answers side_v and 0x1de540 answers 0x4321 as above. */
static int centre_on, centre_calls;

struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    assert(x == 3 && y == 3);
    centre_calls++;
    return centre_on ? (struct DotrStateItem *)&gate_item : 0;
}

static void check_centre_step(void)
{
    centre_calls = 0;
    centre_on = 1;
    side_v = 1;
    assert(TaLogic001_IsLandNearCntr(0x303) == 1 && centre_calls == 0);
    centre_on = 0;
    assert(TaLogic001_IsLandNearCntr(0x203) == 0 && centre_calls == 1);
    centre_on = 1;
    side_v = 0x4321;
    assert(TaLogic001_IsLandNearCntr(0x203) == 0);
    side_v = 1;
    assert(TaLogic001_IsLandNearCntr(0x203) == 1);
    assert(TaLogic001_IsLandNearCntr(0x403) == 1);
    assert(TaLogic001_IsLandNearCntr(0x302) == 1);
    assert(TaLogic001_IsLandNearCntr(0x304) == 1);
    assert(TaLogic001_IsLandNearCntr(0x202) == 0);
    assert(TaLogic001_IsLandNearCntr(0x404) == 0);
    assert(TaLogic001_IsLandNearCntr(0x103) == 0);
    assert(TaLogic001_IsLandNearCntr(0x503) == 0);
    assert(TaLogic001_IsLandNearCntr(0x301) == 0);
    assert(TaLogic001_IsLandNearCntr(0x305) == 0);
    assert(TaLogic001_IsLandNearCntr(0x000) == 0);
}

int main(void)
{
    check_centre_step();
    check_spot_order();
    check_clear_entries();
    check_kind_query();
    check_eight_positions();
    check_held_positions();
    check_mode_positions();
    check_position_table();
    check_total();
    check_kind();
    check_preset();
    check_gate();
    return 0;
}
