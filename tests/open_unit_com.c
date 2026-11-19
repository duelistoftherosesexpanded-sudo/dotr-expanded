/* Synthetic host tests for the open_unit_com group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. Every case runs against both twins (0x111320 and
 * 0x16b610), whose file-local helpers are stubbed alike. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/open_unit_com.h"

#define SIDE 1
#define ID 4
#define OWNER 1
#define POS 0x0305 /* row 3, column 5 */

static int status4;              /* SzDuel_GetUnitStatus(SIDE, ID, 4) */
static int list_status4[8];      /* status bit 4 of listed units */
static int kind_group;           /* SzDuel_GetUnitKind >> 5 */
static int card_no;
static int exz_parts, tactix, exzodia;
static int reaction;             /* TaLogic_GetActionReaction */
static int cerem, field;
static unsigned int own_effect[2], *own_effect_p; /* SzDuel_GetUnitEffect(.., 0) */
static unsigned int eff2[2], eff3[2], *eff2_p, *eff3_p; /* Ta_GetUnitEffect(.., 2 / 3) */
static int use_obj, use_obj_args[3], use_obj_calls;
static int land_change, land_arg;
static int range_spell;
static int lp;
static int unit_num, unit_num_arg;
static int revolt;
static int curse[2][16];
static int search_calls, search_args[8][6];
static int search_ret;           /* SearchUnitByRange result (a mask handle) */
static int list_count;           /* Ta_GetUnitNumFrom22bitFlagList */
static int list_units[8];        /* Ta_GetUnitOwnerIDFromLstCount */
static int control_of_listed;    /* Ta_IsUnitControl of a listed unit */
static int max_ap_unit[2], max_ap_calls;
static int unit_ap[2];           /* TaUtil_GetUnitAP for (side 6 / side 7) */
static int attacked;
static int hand_phase, summon_power, hand_count;
static int ex_monster;
static int hidden;               /* TaLogic_RemoveHideUnit */
static int grave_count[2], grave_kind;
static int rank;
static int unc_users;
static int summonable;
static int threshold_twin;       /* which twin runs: 0 = 0x111320, 1 = 0x16b610 */
static int helper_twin;

static void helper(int twin)
{
    helper_twin = twin;
}

int SzDuel_GetUnitCardNo(int side, int id)
{
    if (side == SIDE && id == ID) {
        return card_no;
    }
    assert(id == 0);
    return 600 + side;
}
int SzDuel_GetUnitCurseRest(int side, int id)
{
    assert(side >= 0 && side < 2 && id >= 0 && id < 16);
    return curse[side][id];
}
int SzDuel_GetGraveCount(int side)
{
    assert(side == 0 || side == 1);
    return grave_count[side];
}
int SzData_GetRank(int side, int card)
{
    assert(side == 1 - SIDE && card == 600 + side);
    return rank;
}
int SzDuel_GetSummonPower(int side)
{
    (void)side;
    return summon_power;
}
int SzDuel_InqHandPhase(int side)
{
    assert(side == OWNER);
    return hand_phase;
}
int SzDuel_InqGrave(int side, int index)
{
    return side * 100 + index;
}
int SzDuel_GetHandCount(int side)
{
    assert(side == OWNER);
    return hand_count;
}
int SzDuel_GetLP(int side)
{
    assert(side == OWNER);
    return lp;
}
int SzDuel_GetUnit(int side, int id)
{
    return side * 16 + id;
}
int SzDuel_GetUnitKind(int side, int id)
{
    assert(side == SIDE && id == ID);
    return kind_group << 5;
}
void *SzDuel_GetUnitEffect(int side, int id, int index)
{
    assert(side == SIDE && id == ID && index == 0);
    return own_effect_p;
}
int SzDuel_GetUnitLoc(int side, int id)
{
    if (side == SIDE && id == ID) {
        return POS;
    }
    return (side << 8) | id; /* the max-AP units of case 0x21 */
}
int SzDuel_GetUnitStatus(int side, int id, int bit)
{
    assert(bit == 4);
    if (side == SIDE && id == ID) {
        return status4;
    }
    assert(side == 2 && id >= 0 && id < 8);
    return list_status4[id];
}
int SzDuel_GetField(int x, int y)
{
    assert(x == (POS & 0xff) && y == (POS >> 8));
    return field;
}
void TaUtil_GetUnitLst(int side, int kind, short *out)
{
    assert(side == OWNER && (kind == 1 || kind == 2) && out != NULL);
}
int SearchUnitByRange(int x, int y, int side, int who, int kind, int range)
{
    if (search_calls < 8) {
        search_args[search_calls][0] = x;
        search_args[search_calls][1] = y;
        search_args[search_calls][2] = side;
        search_args[search_calls][3] = who;
        search_args[search_calls][4] = kind;
        search_args[search_calls][5] = range;
    }
    search_calls++;
    return search_ret;
}
int Ta_GetSummonablePiece(int side, int kind)
{
    assert(side == 1 - OWNER && kind == 1);
    return summonable;
}
int Ta_GetUnitNum(int side, unsigned char kind)
{
    assert(kind == 3);
    unit_num_arg = side;
    return unit_num;
}
int Ta_GetUnitNumFrom22bitFlagList(int mask)
{
    if (mask == 0) {
        return 0; /* the initial mask, when no search ran */
    }
    assert(mask == search_ret || mask == hidden);
    return list_count;
}
int Ta_GetUnitOwnerIDFromLstCount(int mask, int ordinal)
{
    assert(mask == search_ret && ordinal >= 0 && ordinal < 8);
    return list_units[ordinal];
}
int Ta_IsUnitControl(int side, int id)
{
    if (side == SIDE && id == ID) {
        return OWNER;
    }
    return control_of_listed;
}
void *Ta_GetUnitEffect(int side, int id, int index)
{
    assert(side == SIDE && id == ID && (index == 2 || index == 3));
    return index == 2 ? (void *)eff2_p : (void *)eff3_p;
}
int TaUtil_IsExMonster(int side, int id)
{
    assert(side == SIDE && id == ID);
    return ex_monster;
}
int TaUtil_GetUnitAP(int side, int id, int x, int y)
{
    /* Unit (6, 1) at its location (1, 6) is the opponent's strongest, (7, 2) the owner's. */
    if (side == 6) {
        assert(id == 1 && x == 1 && y == 6);
        return unit_ap[0];
    }
    assert(side == 7 && id == 2 && x == 2 && y == 7);
    return unit_ap[1];
}
int Ta_SearchUncTypeNaturalEffectUserNum(int kind)
{
    assert(kind == 0x39);
    return unc_users;
}
int TaHand_GetCardKind(int card)
{
    return card % 100 < grave_kind ? 0 : 0x20; /* the first grave_kind cards are group 0 */
}
int TaLogic_GetMaxApUnit(unsigned short *list)
{
    assert(list != NULL && max_ap_calls < 2);
    return max_ap_unit[max_ap_calls++];
}
int TaLogic_IsAttackedNow(int a, int b, int c)
{
    assert(c == 0 && ((a == SIDE && b == ID) || (a == OWNER && b == 0)));
    return attacked;
}
void TaLogic_GetUnitCtlLst(int side, unsigned short *list, int kind, unsigned char flag)
{
    assert(side == OWNER && list != NULL && kind == 0x1d && flag == 1);
}
void TaLogic_GetUnitCtlLst2(int side, unsigned short *list, int kind, int a, unsigned char flag)
{
    assert(side == 1 - OWNER && list != NULL && kind == 0x1d && a == 0 && flag == 0);
}
int TaLogic_GetLandChange(int unit, int x, int y, int land, int a, int b)
{
    assert(unit == SIDE * 16 + ID && x == (POS & 0xff) && y == (POS >> 8) && a == 8 && b == 2);
    land_arg = land;
    return land_change;
}
int TaLogic_GetActionReaction(int side, int id, int a, int b, int c, int d)
{
    assert(side == SIDE && id == ID && a == 0 && b == 0 && c == 6 && d == 0);
    return reaction;
}
int TaLogic_RemoveHideUnit(int mask)
{
    assert(mask == search_ret);
    return hidden;
}

/* File-local helpers of each copy: same assumed behaviour, the copy recorded. */
static int use_obj_effect(int twin, int side, int who, int kind)
{
    helper(twin);
    use_obj_calls++;
    use_obj_args[0] = side;
    use_obj_args[1] = who;
    use_obj_args[2] = kind;
    return use_obj;
}
static int range_spell_of(int twin, int side, int *effect)
{
    helper(twin);
    assert(side == SIDE && effect != NULL);
    return range_spell;
}
static int cerem_of(int twin, int side, int card)
{
    helper(twin);
    assert(side == OWNER && card == card_no);
    return cerem;
}
int TaLogicCom_GetRevoltObj__001110d0(int side) { helper(0); assert(side == OWNER); return revolt; }
int TaLogicCom_IsUseObjEffect__001123c0(int side, int who, int kind) { return use_obj_effect(0, side, who, kind); }
int TaLogicCom_GetUseXYRangeSpell__00112600(int side, int *effect) { return range_spell_of(0, side, effect); }
int TaLogicCom_IsDoCerem__00112850(int side, int card) { return cerem_of(0, side, card); }
int TaLogicCom_IsExzParts__00112960(int card) { helper(0); assert(card == card_no); return exz_parts; }
int TaLogic_IsTactixExz__00112980(int side) { helper(0); assert(side == OWNER); return tactix; }
int TaLogic_IsExzodia__00112a40(int side) { helper(0); assert(side == OWNER); return exzodia; }
int TaLogicCom_GetRevoltObj__0016b3c0(int side) { helper(1); assert(side == OWNER); return revolt; }
int TaLogicCom_IsUseObjEffect__0016c6b0(int side, int who, int kind) { return use_obj_effect(1, side, who, kind); }
int TaLogicCom_GetUseXYRangeSpell__0016cbb0(int side, int *effect) { return range_spell_of(1, side, effect); }
int TaLogicCom_IsDoCerem__0016ce00(int side, int card) { return cerem_of(1, side, card); }
int TaLogicCom_IsExzParts__0016cf10(int card) { helper(1); assert(card == card_no); return exz_parts; }
int TaLogic_IsTactixExz__0016cf30(int side) { helper(1); assert(side == OWNER); return tactix; }
int TaLogic_IsExzodia__0016cff0(int side) { helper(1); assert(side == OWNER); return exzodia; }

static void reset(void)
{
    status4 = 0;
    memset(list_status4, 0, sizeof list_status4);
    kind_group = 1;
    card_no = 0x123;
    exz_parts = tactix = exzodia = 0;
    reaction = 0;
    cerem = 0;
    field = 0;
    memset(own_effect, 0, sizeof own_effect);
    own_effect_p = own_effect;
    memset(eff2, 0, sizeof eff2);
    memset(eff3, 0, sizeof eff3);
    eff2_p = eff2;
    eff3_p = eff3;
    use_obj = 0;
    use_obj_calls = 0;
    memset(use_obj_args, 0, sizeof use_obj_args);
    land_change = -1;
    land_arg = -1;
    range_spell = -1;
    lp = 2000;
    unit_num = unit_num_arg = 0;
    revolt = -1;
    memset(curse, 0, sizeof curse);
    search_calls = 0;
    memset(search_args, 0, sizeof search_args);
    search_ret = 0x77;
    list_count = 0;
    memset(list_units, 0, sizeof list_units);
    control_of_listed = 0;
    max_ap_unit[0] = 0x0601;
    max_ap_unit[1] = 0x0702;
    max_ap_calls = 0;
    unit_ap[0] = unit_ap[1] = 0;
    attacked = 0;
    hand_phase = 1;
    summon_power = 4;
    hand_count = 1;
    ex_monster = 1;
    hidden = 0x55;
    grave_count[0] = grave_count[1] = 0;
    grave_kind = 0;
    rank = 0;
    unc_users = 0;
    summonable = -1;
    helper_twin = -1;
}

static int run(void)
{
    if (threshold_twin == 0) {
        return TaLogicCom_IsOpenUnit__00111320(SIDE, ID);
    }
    return TaLogicCom_IsOpenUnit__0016b610(SIDE, ID, NULL);
}

/* Effect word: kind in bits 8+, `who` in bits 6-7, target kind in bits 0-5. */
#define EFFECT(kind, who, low) (((unsigned int)(kind) << 8) | ((unsigned int)(who) << 6) | (unsigned int)(low))

static void check_common(void)
{
    reset();
    status4 = 1;
    assert(run() == 0);

    reset();
    exz_parts = 1;
    tactix = 1;
    exzodia = 4;
    assert(run() == 1 && helper_twin == threshold_twin);
    reset();
    exz_parts = 1;
    tactix = 1;
    exzodia = 3;
    own_effect_p = NULL;
    assert(run() == 2);

    /* Groups other than 0, 1 and 5 keep the unit closed. */
    reset();
    kind_group = 2;
    assert(run() == 2);
    reset();
    kind_group = 7;
    assert(run() == 2);
}

static void check_group5(void)
{
    reset();
    kind_group = 5;
    reaction = 5;
    cerem = 1;
    assert(run() == 2);
    reset();
    kind_group = 5;
    cerem = 1;
    assert(run() == 1 && helper_twin == threshold_twin);
    reset();
    kind_group = 5;
    cerem = 1;
    field = 9;
    assert(run() == 2);
}

static void check_group1(void)
{
    int i;

    reset();
    reaction = 5;
    assert(run() == 2);
    reset();
    own_effect_p = NULL;
    assert(run() == 2);

    /* 0x17 / 0x49: the helper on (owner, who, low). */
    reset();
    own_effect[0] = EFFECT(0x49, 2, 0x15);
    use_obj = 1;
    assert(run() == 1 && use_obj_args[0] == OWNER && use_obj_args[1] == 2 && use_obj_args[2] == 0x15);
    assert(helper_twin == threshold_twin);
    reset();
    own_effect[0] = EFFECT(0x17, 1, 3);
    assert(run() == 2 && use_obj_calls == 1);

    /* 0x18: more than five of the opponent-side listed units with status bit 4. */
    reset();
    own_effect[0] = EFFECT(0x18, 0, 0);
    list_count = 6;
    for (i = 0; i < 6; i++) {
        list_units[i] = (2 << 8) | i;
        list_status4[i] = 1;
    }
    assert(run() == 1);
    assert(search_args[0][2] == OWNER && search_args[0][3] == 2 && search_args[0][4] == 0x34 && search_args[0][5] == 0xe);
    list_status4[5] = 0;
    search_calls = 0;
    assert(run() == 2);

    /* 2: the land change result equal to the unit's own location. */
    reset();
    own_effect[0] = EFFECT(2, 0, 0);
    own_effect[1] = 0x1234560b;
    land_change = POS;
    assert(run() == 1 && land_arg == 0x0b);
    land_change = POS + 1;
    assert(run() == 2);

    /* 4 with 0x12 above bit 16: a range spell on the unit's row or column. */
    reset();
    own_effect[0] = EFFECT(4, 0, 0);
    own_effect[1] = 0x12u << 16;
    range_spell = 0x0905;
    assert(run() == 1);
    range_spell = 0x0309;
    assert(run() == 1);
    range_spell = 0x0909;
    assert(run() == 2);
    range_spell = -1;
    assert(run() == 2);
    reset();
    own_effect[0] = EFFECT(4, 1, 7);
    own_effect[1] = 0x13u << 16;
    use_obj = 1;
    assert(run() == 1 && use_obj_args[1] == 1 && use_obj_args[2] == 7);

    /* 0xd: who 1 opens unless the high half is 4 and the owner's LP are above 1200;
     * low 0x32 needs more than three of the opponent's units of kind 3. */
    reset();
    own_effect[0] = EFFECT(0xd, 1, 0);
    assert(run() == 1);
    own_effect[1] = 4u << 16;
    lp = 1200;
    assert(run() == 1);
    lp = 1201;
    assert(run() == 2);
    reset();
    own_effect[0] = EFFECT(0xd, 0, 0x32);
    unit_num = 4;
    assert(run() == 1 && unit_num_arg == 1 - OWNER);
    unit_num = 3;
    assert(run() == 2);
    reset();
    own_effect[0] = EFFECT(0xd, 2, 0x31);
    assert(run() == 1);

    /* 0x1f: card 0x2dd with a revolt object. */
    reset();
    own_effect[0] = EFFECT(0x1f, 0, 0);
    card_no = 0x2dd;
    revolt = 0;
    assert(run() == 1 && helper_twin == threshold_twin);
    revolt = -1;
    assert(run() == 2);
    card_no = 0x2dc;
    revolt = 0;
    assert(run() == 2);

    /* 0x20: any of the side's slots 0..10 with curse count 0xff. */
    reset();
    own_effect[0] = EFFECT(0x20, 0, 0);
    curse[SIDE][10] = 0xff;
    assert(run() == 1);
    curse[SIDE][10] = 0xfe;
    assert(run() == 2);

    /* 0x12: more than two units in the owner's who-1 range of the low kind. */
    reset();
    own_effect[0] = EFFECT(0x12, 0, 0x2a);
    list_count = 3;
    assert(run() == 1 && search_args[0][3] == 1 && search_args[0][4] == 0x2a);
    list_count = 2;
    assert(run() == 2);

    /* 0x21: the opponent's strongest unit at or above the copy's threshold (3000 / 2500)
     * and stronger than the owner's strongest. */
    reset();
    own_effect[0] = EFFECT(0x21, 0, 0);
    unit_ap[0] = threshold_twin == 0 ? 3000 : 2500;
    unit_ap[1] = unit_ap[0] - 1;
    assert(run() == 1 && max_ap_calls == 2);
    max_ap_calls = 0;
    unit_ap[1] = unit_ap[0];
    assert(run() == 2);
    max_ap_calls = 0;
    unit_ap[0] -= 1;
    unit_ap[1] = 0;
    assert(run() == 2);

    reset();
    own_effect[0] = EFFECT(0x36, 0, 0);
    assert(run() == 1);

    /* 0x25 / 0x28: attacked now (own unit for low 0x21, else the owner). */
    reset();
    own_effect[0] = EFFECT(0x25, 0, 0x21);
    attacked = 1;
    assert(run() == 1);
    own_effect[0] = EFFECT(0x25, 0, 0x22);
    assert(run() == 1);
    own_effect[0] = EFFECT(0x25, 0, 0x01);
    assert(run() == 1);
    own_effect[0] = EFFECT(0x28, 0, 0);
    assert(run() == 1);
    attacked = 0;
    assert(run() == 2);

    reset();
    own_effect[0] = EFFECT(0x26, 0, 0);
    hand_phase = 0;
    assert(run() == 1);
    hand_phase = 1;
    assert(run() == 2);
    reset();
    own_effect[0] = EFFECT(0x27, 0, 0);
    summon_power = 3;
    assert(run() == 1);
    summon_power = 4;
    assert(run() == 2);

    /* An unknown effect kind keeps the unit closed. */
    reset();
    own_effect[0] = EFFECT(0x50, 0, 0);
    assert(run() == 2);
}

static void check_group0_first(void)
{
    static const unsigned int opens[] = {0xc, 0x34, 0xd, 0xe, 0x22, 0x3b, 0x56, 0x43, 0xf};
    static const unsigned int counts[] = {0xb, 0x4e, 0x12, 0x4d, 6};
    unsigned int i;

    reset();
    kind_group = 0;
    ex_monster = 0;
    assert(run() == 2);
    reset();
    kind_group = 0;
    eff2_p = NULL;
    assert(run() == 2);

    for (i = 0; i < sizeof opens / sizeof opens[0]; i++) {
        reset();
        kind_group = 0;
        eff2[0] = EFFECT(opens[i], 0, 0);
        eff3_p = NULL;
        assert(run() == 1);
    }
    /* Mask above 1 (unsigned) opens; otherwise the second list decides (null: 2). */
    for (i = 0; i < sizeof counts / sizeof counts[0]; i++) {
        reset();
        kind_group = 0;
        eff2[0] = EFFECT(counts[i], 0, 9);
        search_ret = 2;
        assert(run() == 1 && search_args[0][2] == SIDE && search_args[0][3] == 1 && search_args[0][4] == 9);
        reset();
        kind_group = 0;
        eff2[0] = EFFECT(counts[i], 0, 9);
        search_ret = 1;
        eff3_p = NULL;
        assert(run() == 2);
    }

    /* 0x13 / 0x17: who bit 0 with any unit in range keeps it closed; else more than two
     * visible units. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x13, 1, 5);
    search_ret = 1;
    assert(run() == 2 && search_calls == 1);
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x17, 0, 5);
    hidden = 3;
    assert(run() == 1 && search_args[0][3] == 2);
    hidden = 2;
    search_calls = 0;
    eff3[0] = EFFECT(0x50, 0, 0);
    assert(run() == 2);

    /* 0x47 / 0x55: a listed unit with a curse count between 1 and 0xfe. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x47, 0, 0);
    list_count = 2;
    list_units[1] = 0x0003;
    curse[0][3] = 1;
    assert(run() == 1 && search_args[0][3] == 2);
    curse[0][3] = 0xff;
    eff3[0] = EFFECT(0x50, 0, 0);
    assert(run() == 2);
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x55, 0, 0);
    list_count = 1;
    list_units[0] = 0x0102;
    curse[1][2] = 0xfe;
    assert(run() == 1 && search_args[0][3] == 1);

    /* 0x53: more than five group-0 cards in both graves. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x53, 0, 0);
    grave_count[0] = grave_count[1] = 4;
    grave_kind = 3;
    assert(run() == 1);
    grave_kind = 2;
    eff3_p = NULL;
    assert(run() == 2);

    /* 0x54: the first unit in who-3 range of kind 0x21 controlled by side and not this one. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x54, 0, 0);
    list_units[0] = 0x0002;
    control_of_listed = SIDE;
    assert(run() == 1 && search_args[0][3] == 3 && search_args[0][4] == 0x21);
    control_of_listed = 0;
    eff3_p = NULL;
    assert(run() == 2);

    /* 0x58: the opponent's rank at 6 or above. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x58, 0, 0);
    rank = 6;
    assert(run() == 1);
    rank = 5;
    eff3_p = NULL;
    assert(run() == 2);
}

static void check_group0_second(void)
{
    int i;

    /* Effect list 3 after a first list that does not decide. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x12, 0, 0x11);
    eff3[1] = 0x16u << 16;
    list_count = 3;
    assert(run() == 1 && search_args[0][2] == OWNER && search_args[0][4] == 0x11 && search_args[0][5] == 0x16);
    eff3[1] = 0x5u << 16;
    search_calls = 0;
    assert(run() == 1 && search_args[0][4] == ((5 << 16) | 0x11) && search_args[0][5] == 0xe);
    list_count = 2;
    assert(run() == 2);
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x33, 0, 0x11);
    eff3[1] = 0x7u << 16;
    list_count = 3;
    assert(run() == 1 && search_args[0][4] == ((7 << 16) | 0x11));

    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0xd, 0, 0);
    assert(run() == 1);
    eff3[0] = EFFECT(0xc, 0, 0);
    assert(run() == 1);

    /* 0x17: low 0x1a keeps it closed; else the helper. 0x23: the helper. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x17, 2, 0x1a);
    use_obj = 1;
    assert(run() == 2 && use_obj_calls == 0);
    eff3[0] = EFFECT(0x17, 2, 0x1b);
    assert(run() == 1 && use_obj_args[1] == 2 && use_obj_args[2] == 0x1b);
    eff3[0] = EFFECT(0x23, 1, 0x1c);
    assert(run() == 1 && helper_twin == threshold_twin);

    /* 0x18: more than two own-range units with bit 4 keep it closed; more than five in
     * the who-2 range open it. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x18, 0, 0x2);
    list_count = 6;
    for (i = 0; i < 6; i++) {
        list_units[i] = (2 << 8) | i;
        list_status4[i] = 1;
    }
    assert(run() == 2 && search_calls == 1);
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x18, 0, 0x2);
    list_count = 6;
    for (i = 0; i < 6; i++) {
        list_units[i] = (2 << 8) | i;
        list_status4[i] = i < 2; /* 2 in the first pass: go on; 2 in the second: closed */
    }
    assert(run() == 2 && search_calls == 2 && search_args[1][3] == 2);

    /* 4: low 0x1a closed; listed units counted by status bit 4, closed if one is the owner's. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(4, 1, 0x1a);
    assert(run() == 2);
    eff3[0] = EFFECT(4, 1, 0x05);
    list_count = 2;
    list_units[0] = 0x0200;
    list_units[1] = 0x0201;
    list_status4[0] = list_status4[1] = 1;
    assert(run() == 1 && search_args[0][0] == 0 && search_args[0][3] == 1 && search_args[0][5] == 0xe);
    eff3[1] = 3u << 16;
    search_calls = 0;
    assert(run() == 1 && search_args[0][0] == (POS & 0xff) && search_args[0][1] == (POS >> 8) && search_args[0][5] == 3);
    control_of_listed = OWNER;
    assert(run() == 2);
    control_of_listed = 0;
    list_status4[1] = 0;
    assert(run() == 2);
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(4, 2, 0x30);
    use_obj = 1;
    assert(run() == 1 && use_obj_calls == 1 && use_obj_args[1] == 2 && use_obj_args[2] == 0x30);
    use_obj = 0;
    assert(run() == 2 && search_calls == 0); /* no search: the list of the initial mask 0 */

    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x1a, 0, 0);
    summon_power = 3;
    assert(run() == 1);
    eff3[0] = EFFECT(0x26, 0, 0);
    hand_phase = 0;
    assert(run() == 1);

    /* 0x1f: closed while effect 0x39 users exist or for low 0x1a; else more than two. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x1f, 1, 3);
    list_count = 3;
    unc_users = 1;
    assert(run() == 2);
    unc_users = 0;
    assert(run() == 1 && search_args[0][3] == 1 && search_args[0][4] == 3);
    eff3[0] = EFFECT(0x1f, 1, 0x1a);
    assert(run() == 2);

    /* 0x3c: a unit around the location and a summonable piece for the opponent. */
    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x3c, 0, 4);
    eff3[1] = 0x00090000u | 0x0008u;
    search_ret = 1;
    summonable = 0;
    assert(run() == 1 && search_args[0][4] == ((8 << 16) | 4) && search_args[0][5] == 1);
    summonable = -1;
    assert(run() == 2);

    reset();
    kind_group = 0;
    eff2[0] = EFFECT(0x50, 0, 0);
    eff3[0] = EFFECT(0x46, 0, 0);
    attacked = 1;
    assert(run() == 1);
    eff3[0] = EFFECT(0x57, 0, 0);
    hand_count = 0;
    assert(run() == 1);
    hand_count = 2;
    assert(run() == 2);
    eff3_p = NULL;
    assert(run() == 2);
}

int main(void)
{
    for (threshold_twin = 0; threshold_twin < 2; threshold_twin++) {
        check_common();
        check_group5();
        check_group1();
        check_group0_first();
        check_group0_second();
    }
    return 0;
}
