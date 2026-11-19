/* Synthetic host tests for the efc_monster group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/efc_monster.h"

/* Board fixture: the unit under command, its controller and the answers of the queries. */
static int unit_present, controller, unit_type, unit_loc, ready_card, deck_check, status4, unit_ap;
static int guard, gunner, natural, rank, near_result, curse[8][8], locs[0x300];
static int search_mask, search_count, search_units[8], grave_kind[2][8], grave_count[2], side_of_unit;
static int pref_hits;
static unsigned int effects[4][2];
static int has_effect[4];
static int atk_calls, line_calls, line_side, gunner_calls, l2_calls, l3_calls, removed;
static int unit_kind;

int Ta_IsUnitControl(int y, int x) { (void)y; (void)x; return controller; }
int SzDuel_GetUnitCardNo(int y, int x) { (void)x; return y == controller ? ready_card : unit_type; }
int SzDuel_GetUnit(int y, int x) { (void)y; (void)x; return unit_present; }
int SzDuel_GetUnitLoc(int y, int x)
{
    if (y == 0x13 && x == 0x24) {
        return unit_loc;
    }
    return locs[(y & 3) << 8 | (x & 0xff)];
}
void TaLogic_InitIgnoreLst(void *work) { memset(work, 0, 112); }
int SzData_CheckDeck(int side, int deck, int card)
{
    assert(side == controller && deck == 2 && card >= 0x36 && card <= 0x39);
    return card == deck_check ? -1 : 0;
}
int SzData_GetCurrentDeck(int side) { (void)side; return 2; }
int TaLogic_GetSommonPiecePreference__00170610(int side, int index)
{
    (void)side;
    pref_hits++;
    return index == 4 ? unit_loc : -1;
}
void *Ta_GetUnitEffect(int y, int x, int kind)
{
    assert(y == 0x13 && x == 0x24 && kind >= 1 && kind <= 3);
    return has_effect[kind] ? effects[kind] : 0;
}
int SearchUnitByRange(int a, int b, int side, int range, int key, int c)
{
    assert(a == 0 && b == 0 && c == 14 && side == controller);
    (void)range;
    (void)key;
    return search_mask;
}
int Ta_GetCurrentSide(int y, int x) { (void)y; (void)x; return side_of_unit; }
int SzDuel_GetUnitAP(int y, int x, int a) { (void)y; (void)x; assert(a == 0); return unit_ap; }
void TaLogic022_CommandAtkUnit(void *command) { assert(command != 0); atk_calls++; }
int TaLogic022_IsEmptyCntrGuard(int y, int x, int side) { (void)y; (void)x; assert(side == 1); return guard; }
void TaLogic022_CommandLineUnit(unsigned short *command, int side) { assert(command != 0); line_calls++; line_side = side; }
int TaLogic_RemoveHideUnit(int mask) { removed++; return mask; }
int Ta_GetUnitNumFrom22bitFlagList(int mask) { return mask ? search_count : 0; }
int Ta_GetUnitOwnerIDFromLstCount(int mask, int ordinal)
{
    (void)mask;
    assert(ordinal >= 0 && ordinal < 8);
    return search_units[ordinal];
}
int SzDuel_GetUnitCurseRest(int y, int x) { return curse[y & 7][x & 7]; }
int SzDuel_GetGraveCount(int side) { return grave_count[side]; }
int SzDuel_InqGrave(int side, int index) { return side * 100 + index; }
int TaHand_GetCardKind(int card) { return grave_kind[card / 100][card % 100]; }
int SzData_GetRank(int side, int card) { (void)side; (void)card; return rank; }
int SzDuel_GetUnitStatus(int y, int x, int bit) { (void)y; (void)x; assert(bit == 4); return status4; }
int TaLogic_CommandGunnerUnit(unsigned short *command, int a)
{
    assert(command != 0 && a == 0);
    gunner_calls++;
    if (gunner == 1) {
        /* The gunner command leaves its target in the word at +8. */
        *(unsigned int *)&command[4] = 0x0305u << 16 | command[4];
    }
    return gunner;
}
int SzDuel_GetUnitKind(int y, int x) { assert(y == 3 && x == 5); return unit_kind; }
int Ta_SearchUncTypeNaturalEffectUserNum(int effect) { assert(effect == 0x39); return natural; }
int TaLogic_GetNearUnitL2(int cursor, int card, int x, int y, int side, int attempt, int a6, int level,
                          struct DotrEfcMonsterCandidate *list, unsigned short *length, short *first,
                          short *second, void *work)
{
    assert(card == unit_present && x == 0x04 && y == 0x02 && attempt == l2_calls + 1 && a6 == 0);
    assert(list[0].card == 0xffff && list[21].index == 0xff && *first == 0 && *second == 0 && work != 0);
    (void)side;
    (void)level;
    *length = (unsigned short)(*length + 1);
    l2_calls++;
    return cursor + 1;
}
int TaLogic_GetNearUnitL3(int card, int x, int y, struct DotrEfcMonsterCandidate *list, int chase, int key,
                          int open, int def, int view, int level, int length)
{
    assert(card == unit_present && x == 0x04 && y == 0x02 && list != 0 && chase == 5 && key == 0x34);
    assert(open == 0 && def == 2 && view == 0 && length == l2_calls);
    (void)level;
    l3_calls++;
    return l3_calls == 3 ? near_result : -1;
}
int TaLogic_GetWarnHideLvl__00172310(int side) { return side + 7; }

static union {
    unsigned short h[8];
    unsigned int w[4];
} cmd;

static void reset(void)
{
    memset(&cmd, 0, sizeof(cmd));
    cmd.h[0] = 0x1324;
    cmd.h[2] = 0x1ff;
    cmd.h[4] = 0x55;
    unit_present = 0x77;
    controller = 0;
    unit_type = 1;
    unit_loc = 0x0204;
    ready_card = 0;
    deck_check = -1;
    status4 = 0;
    unit_ap = 0;
    guard = 0;
    gunner = 0;
    natural = 0;
    rank = 0;
    near_result = -1;
    memset(curse, 0, sizeof(curse));
    memset(locs, 0, sizeof(locs));
    search_mask = 0;
    search_count = 0;
    memset(search_units, 0, sizeof(search_units));
    memset(grave_kind, 0, sizeof(grave_kind));
    grave_count[0] = grave_count[1] = 0;
    side_of_unit = 0;
    pref_hits = 0;
    memset(effects, 0, sizeof(effects));
    memset(has_effect, 0, sizeof(has_effect));
    atk_calls = line_calls = line_side = gunner_calls = l2_calls = l3_calls = removed = 0;
    unit_kind = 0;
}

static void effect(int kind, unsigned int id, unsigned int param, unsigned int second)
{
    has_effect[kind] = 1;
    effects[kind][0] = id << 8 | param;
    effects[kind][1] = second << 16;
}

static int target(void) { return (int)(cmd.w[2] >> 16); }

static void test_no_unit(void)
{
    reset();
    unit_present = 0;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff && cmd.w[2] == 0x55 && atk_calls == 0);
}

static void test_ritual_pieces(void)
{
    /* Types 0x36..0x39 with the leader card 0x3a and all four pieces in the deck. */
    reset();
    unit_type = 0x37;
    ready_card = 0x3a;
    locs[0] = 0x0301;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(pref_hits == 9);
    assert(cmd.h[2] == 0x6ff && target() == 0x0301 && cmd.h[4] == 0x55);

    /* A missing piece: fall through to the final choice. */
    reset();
    unit_type = 0x37;
    ready_card = 0x3a;
    deck_check = 0x39;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(pref_hits == 0 && cmd.h[2] == 0x1ff && atk_calls == 1);

    /* Type 0x3a is out of the range. */
    reset();
    unit_type = 0x3a;
    ready_card = 0x3a;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(pref_hits == 0);
}

static void test_effects(void)
{
    static const unsigned int stay[] = { 0xc, 0x34, 0xd, 0xe, 0x22, 0x3b, 0x56, 0x43, 0xf };
    int i;

    for (i = 0; i < 9; i++) {
        reset();
        effect(2, stay[i], 0, 0);
        TaLogic022_CommandEfcMonster(cmd.h);
        assert(cmd.h[2] == 0x3ff && target() == 0x0204 && cmd.h[4] == 0x55 && atk_calls == 0);
    }

    /* 0xb family: stay when more than one unit is in range. */
    reset();
    effect(2, 0x4e, 3, 0);
    search_mask = 2;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x3ff);
    reset();
    effect(2, 0x4e, 3, 0);
    search_mask = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff && atk_calls == 1);

    /* 0x13 with bit 6 and a unit in range: straight to the final choice (line unit). */
    reset();
    effect(2, 0x13, 0x40, 0);
    search_mask = 1;
    guard = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(line_calls == 1 && line_side == 1 && atk_calls == 0 && removed == 0);
    /* Without bit 6: stay when more than two remain visible. */
    reset();
    effect(2, 0x17, 0, 0);
    search_mask = 3;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(removed == 1 && cmd.h[2] == 0x3ff);

    /* 0x47 and 0x55: stay when a unit in range has a curse count of 1..0xfe. */
    reset();
    effect(2, 0x47, 0, 0);
    search_mask = 1;
    search_count = 2;
    search_units[1] = 0x0203;
    curse[2][3] = 3;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x3ff);
    reset();
    effect(2, 0x55, 0, 0);
    search_mask = 1;
    search_count = 2;
    search_units[1] = 0x0203;
    curse[2][3] = 0xff;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff && atk_calls == 1);

    /* 0x53: stay when more than five monster cards lie in both graves. */
    reset();
    effect(2, 0x53, 0, 0);
    grave_count[0] = 3;
    grave_count[1] = 4;
    grave_kind[1][2] = 0x20;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x3ff);
    reset();
    effect(2, 0x53, 0, 0);
    grave_count[0] = 3;
    grave_count[1] = 2;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff);

    /* 0x54: stay when the first unit found is another unit of the same controller. */
    reset();
    effect(2, 0x54, 0, 0);
    search_mask = 1;
    search_count = 1;
    search_units[0] = 0x0102;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x3ff);
    reset();
    effect(2, 0x54, 0, 0);
    search_units[0] = 0x1324;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff);

    /* 0x58: stay against an opponent leader of rank 6 or more. */
    reset();
    effect(2, 0x58, 0, 0);
    rank = 6;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x3ff);
    reset();
    effect(2, 0x58, 0, 0);
    rank = 5;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff);
}

static void test_actions(void)
{
    reset();
    effect(3, 0x14, 0, 0);
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0xcff && atk_calls == 0);
    /* Status bit 4 set: the action record is ignored. */
    reset();
    effect(3, 0x14, 0, 0);
    status4 = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff && atk_calls == 1);

    /* Gunner actions. */
    reset();
    effect(3, 0x17, 0x1a, 0);
    gunner = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0xbff && gunner_calls == 1);
    reset();
    effect(3, 0x17, 0x1b, 0);
    gunner = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(gunner_calls == 0 && atk_calls == 1);
    reset();
    effect(3, 0x24, 0x1b, 0);
    gunner = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0xbff);
    reset();
    effect(3, 0x16, 0, 0);
    gunner = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0xbff);
    reset();
    effect(3, 0x46, 0, 0);
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(gunner_calls == 1 && cmd.h[2] == 0x1ff && atk_calls == 0);
    reset();
    effect(3, 0x51, 0, 0);
    gunner = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff && atk_calls == 0);

    /* 0x4: with value 2, return only when the gunner's target is a kind-2 unit of class 0. */
    reset();
    effect(3, 0x4, 0x1a, 2);
    gunner = 1;
    unit_kind = 2;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0xbff && atk_calls == 0);
    reset();
    effect(3, 0x4, 0x1a, 2);
    gunner = 1;
    unit_kind = 0x22;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(atk_calls == 1);
    /* 0x1f: blocked by a natural effect user; value 14 wants a kind-14 target. */
    reset();
    effect(3, 0x1f, 0x1a, 14);
    gunner = 1;
    natural = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(gunner_calls == 0 && atk_calls == 1);
    reset();
    effect(3, 0x1f, 0x1a, 14);
    gunner = 1;
    unit_kind = 14;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0xbff && atk_calls == 0);

    /* 0xc: stay. */
    reset();
    effect(3, 0xc, 0, 0);
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x3ff && target() == 0x0204);

    /* 0x25: up to five near-unit searches; the third finds a cell. */
    reset();
    effect(3, 0x25, 0, 0);
    near_result = 0x0102;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(l2_calls == 3 && l3_calls == 3 && cmd.h[2] == 0x2ff && target() == 0x0102);
    reset();
    effect(3, 0x25, 0, 0);
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(l2_calls == 5 && cmd.h[2] == 0x1ff && atk_calls == 1);

    /* 0x3c: the nearest found unit by cell distance (below 7). */
    reset();
    effect(3, 0x3c, 0, 0);
    search_mask = 1;
    search_count = 3;
    search_units[0] = 0x0100;
    search_units[1] = 0x0101;
    search_units[2] = 0x0102;
    locs[0x0100] = 0x0000;
    locs[0x0101] = 0x0303;
    locs[0x0102] = 0x0103;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0xbff && target() == 0x0101);
    reset();
    effect(3, 0x3c, 0, 0);
    search_mask = 1;
    search_count = 1;
    search_units[0] = 0x0100;
    locs[0x0100] = 0x0010;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff && atk_calls == 1);
}

static void test_final(void)
{
    /* Kind-1 effect 0x42 with mode 1 sets 0x6ff, then the final choice still runs. */
    reset();
    effect(1, 0x42, 0x40, 0);
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x6ff && target() == 0x0204 && atk_calls == 1);
    reset();
    effect(1, 0x42, 0x80, 0);
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(cmd.h[2] == 0x1ff);

    /* AP 2500 or more attacks; else an empty counter guard lines up; else attacks. */
    reset();
    unit_ap = 2500;
    guard = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(atk_calls == 1 && line_calls == 0);
    reset();
    unit_ap = 2499;
    guard = 1;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(atk_calls == 0 && line_calls == 1);
    reset();
    guard = 0;
    TaLogic022_CommandEfcMonster(cmd.h);
    assert(atk_calls == 1 && line_calls == 0);
}

int main(void)
{
    test_no_unit();
    test_ritual_pieces();
    test_effects();
    test_actions();
    test_final();
    return 0;
}
