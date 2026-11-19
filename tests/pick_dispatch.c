/* Synthetic host tests for the pick_dispatch group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/pick_dispatch.h"

#define SIDE 1

static char storage[16];
#define BLOCK ((struct DotrStateBlock *)storage)

static int first, second, fill_calls, count, kind_by[1000], afford_by[1000];
static int mark_calls, mark_id;
static int ids[8];
static int r15fc50, c15fc50, r15fb60[1000], c15fb60, r162490[1000], r15fa00, c15fa00;
static int r1625e0, r1626f0, r1627d0, r162a70;

int TaLogic019_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c15fc50++;
    return r15fc50;
}

int TaLogic019_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    assert(id == 833 || id == 853 || id == 700 || id == 715 || id == 685 || id == 43 || id == 796 || id == 795);
    c15fb60++;
    return r15fb60[id];
}

int TaLogic019_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block)
{
    assert(side == SIDE && item != NULL && (void *)block == (void *)BLOCK);
    c15fa00++;
    return r15fa00;
}

int TaLogic019_IsDoCerem(int value, int index) { assert(value == SIDE); return r162490[index]; }
int TaLogic019_GetUseThunderBolt(int side) { assert(side == SIDE); return r1625e0; }
int TaLogic019_IsUseGFK(int side) { assert(side == SIDE); return r1626f0; }
int TaLogic019_IsUseRevive(int side) { assert(side == SIDE); return r1627d0; }
int TaLogic019_IsUseSaint(int side) { assert(side == SIDE); return r162a70; }
static int r136980, c136980, r136890[1000], c136890, r138f90[1000], r138b60, r138db0, r138ea0;

int TaLogic009_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c136980++;
    return r136980;
}

int TaLogic009_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    assert(id == 830 || id == 849 || id == 732 || id == 733 || id == 683 || id == 713 || id == 687);
    c136890++;
    return r136890[id];
}

int TaLogic009_IsDoCerem(int value, int index) { assert(value == SIDE); return r138f90[index]; }
int TaLogic009_GetRevoltObj(int side) { assert(side == SIDE); return r138b60; }
int TaLogic009_IsUseCurseDragonPot(int side) { assert(side == SIDE); return r138db0; }
int TaLogic009_IsUseCopyCat(int side) { assert(side == SIDE); return r138ea0; }
static int r144250, c144250, r144150[1000], c144150, last144150, r1466a0, r1465a0, ok143a80[8][8], c143a80, score_by[8];
static int r128520, c128520, r128430[1000], c128430, r27bbc0, r12b000, c12b000, r27da40, r12b8b0, c12b8b0;
static int r10f450, c10f450, r10f290, c10f290, r10f6f0[3], c10f6f0;
/* answers and call counts of the stubbed pick routines */
static int r1514a0, c1514a0, r151170, c151170, r153f30, r1513b0, c1513b0;
static int r14d1e0, c14d1e0, r14d0f0_852, r14d0f0_701, r14d0f0_742, c14d0f0;
static int r14f8c0, r14f3a0, r1e01f0, r1e0150;
static int r142280, r13f540, c13f540, r13f3b0, c13f3b0, r13f640, c13f640;

int TaLogic_GetUnitNum(int a, int b)
{
    assert(a == SIDE);
    assert(b == 0 || b == 1);
    return 0x100 + (b ? second : first); /* only the low byte may be used */
}

void TaHand_GetNextHand(int index, int *out)
{
    assert(index == SIDE);
    fill_calls++;
    memcpy(out, ids, sizeof(ids));
}

int TaHand_GetNextHandNum(int index) { assert(index == SIDE); return count; }
int TaHand_IsCardSummon2(int index, int value) { assert(index == SIDE); return afford_by[value]; }
int TaHand_GetCardKind(int id) { return kind_by[id]; }

void SzDuel_InitCardInfo(struct DotrStateBlock *block, int unknown)
{
    assert(block == BLOCK);
    mark_calls++;
    mark_id = unknown;
}

int TaLogic015_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c1514a0++;
    return r1514a0;
}

int TaLogic015_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block)
{
    assert(side == SIDE && item != NULL && (void *)block == (void *)BLOCK);
    c151170++;
    return r151170;
}

int TaLogic015_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 686);
    c1513b0++;
    return r1513b0;
}

int TaLogic015_IsUseRevive(int side) { assert(side == SIDE); return r153f30; }

int TaLogic014_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c14d1e0++;
    return r14d1e0;
}

int TaLogic014_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    c14d0f0++;
    if (id == 852) {
        return r14d0f0_852;
    }
    if (id == 701) {
        return r14d0f0_701;
    }
    assert(id == 742);
    return r14d0f0_742;
}

int TaLogic014_IsDoCerem(int value, int index) { assert(value == SIDE && index == 852); return r14f8c0; }
int TaLogic014_IsActiveMode(void) { return r14f3a0; }
int SzDuel_GetUnitSpellCount(int side) { assert(side == SIDE); return r1e01f0; }
int SzDuel_GetUnitMonsterCount(int side) { assert(side == SIDE); return r1e0150; }
int TaLogic011_IsUseReversalGoddess(int side) { assert(side == SIDE); return r142280; }

int TaLogic011_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c13f640++;
    return r13f640;
}

int TaLogic011_GetSpecCardPlay(int side, int *list, struct DotrStateBlock *block, unsigned char *item, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 368);
    assert(memcmp(list, ids, sizeof(ids)) == 0);
    c13f540++;
    return r13f540;
}

int TaLogic011_GetSpecComboPlay(int side, unsigned char *item, struct DotrPickBlock *block)
{
    assert(side == SIDE && item != NULL && (void *)block == (void *)BLOCK);
    c13f3b0++;
    return r13f3b0;
}

static int r15a910, c15a910, r15a820, c15a820, r15d030;
static int r119d30, c119d30, id119d30, r119f30, c119f30, z118ce0, c118ce0;

int TaLogic003_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    c119d30++;
    id119d30 = id;
    return r119d30;
}

int TaLogic003_IsSpellPlay(int side, unsigned char *item, struct DotrPickBlock *block)
{
    assert(side == SIDE && item != NULL && (void *)block == (void *)BLOCK);
    c119f30++;
    return r119f30;
}

/* z118ce0: the key at which the stub answers 0 (-1: never) */
int TaLogic003_IsEmptyAtkLine(int key, unsigned char k)
{
    assert(key == SIDE && k == c118ce0 && k < 4);
    c118ce0++;
    return k == z118ce0 ? 0 : 1;
}
static int r164ae0, c164ae0, r164910_842, r164910_850, c164910, r167670_842, r167670_850;
static int r164a00, c164a00;
static int r1562e0, c1562e0, class_by[2], r158800, r155c90, c155c90;

int TaLogic018_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c15a910++;
    return r15a910;
}

int TaLogic018_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 835);
    c15a820++;
    return r15a820;
}

int TaLogic018_IsDoCerem(int value, int index) { assert(value == SIDE && index == 835); return r15d030; }

int TaLogic020_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c164ae0++;
    return r164ae0;
}

int TaLogic020_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    c164910++;
    if (id == 842) {
        return r164910_842;
    }
    assert(id == 850);
    return r164910_850;
}

int TaLogic020_IsDoCerem(int value, int index)
{
    assert(value == SIDE);
    if (index == 842) {
        return r167670_842;
    }
    assert(index == 850);
    return r167670_850;
}

int TaLogic020_GetTrapCardPlay(int index, unsigned char *item, void *block)
{
    assert(index == SIDE && item != NULL && block == (void *)BLOCK);
    c164a00++;
    return r164a00;
}

int TaLogic017_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c1562e0++;
    return r1562e0;
}

int SzDuel_Kind2Type(int value) { assert(value == 0 || value == 1); return class_by[value]; }
int TaLogic017_IsExzSafe(int side) { assert(side == SIDE); return r158800; }

int TaLogic017_PlayExzCombo(int side, unsigned char *item, struct DotrStateBlock *block)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    c155c90++;
    return r155c90;
}

static int r132b00, c132b00, r132a10_832, r132a10_746[2], c132a10, r135000;
static int slot_set[2][6], slot_word[2][6], slot_owner[2][6];

int TaLogic008_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c132b00++;
    return r132b00;
}

int TaLogic008_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(item != NULL && block == BLOCK);
    c132a10++;
    if (id == 832) {
        assert(side == SIDE);
        return r132a10_832;
    }
    assert(id == 746 && (side == 0 || side == 1));
    return r132a10_746[side];
}

int TaLogic008_IsDoCerem(int value, int index) { assert(value == SIDE && index == 832); return r135000; }

/* 0x16f760 */
static struct DotrDispatchPair pair022;
static int c_effect022, c_cerem022, apdp022, r_sac022, cell022[4], loc022, unitap022[4], c_unitap022;
static int r_spec022[1000], c_spec022, last_spec022, r_spell022, c_spell022, r_xy022, c_xy022;

static void reset022(void)
{
    c_effect022 = c_cerem022 = c_unitap022 = c_spec022 = last_spec022 = c_spell022 = c_xy022 = 0;
    r_sac022 = r_spell022 = r_xy022 = -1;
    apdp022 = 0x00640000;
    loc022 = 0x0204;
    memset(cell022, 0, sizeof(cell022));
    memset(unitap022, 0, sizeof(unitap022));
    memset(r_spec022, 0xff, sizeof(r_spec022));
}

struct DotrDispatchPair *TaHand_GetCardEffect(int a, int b)
{
    assert(b == 0);
    c_effect022++;
    pair022.unknown_0 = a;
    pair022.unknown_4 = a + 5000;
    return &pair022;
}

void Ta_GetCeremSacrifice(int *triple, int id)
{
    assert(id == pair022.unknown_4);
    c_cerem022++;
    triple[0] = id;
}

int TaHand_GetCardAPDP(int id) { assert(id == pair022.unknown_4); return apdp022; }

int Ta_GetSacrificeUnit(int *triple, int value)
{
    assert(triple[0] == pair022.unknown_4 && value == SIDE);
    return r_sac022;
}

int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert(word == 0x5E7 && n >= 0 && n < 4);
    return cell022[n];
}

static int loc_a022, loc_b022;

int SzDuel_GetUnitLoc(int a, int b)
{
    loc_a022 = a;
    loc_b022 = b;
    return loc022;
}

int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    assert(a == loc_a022 && b == loc_b022 && c == (loc022 & 0xff) && d == loc022 >> 8);
    assert(a == cell022[c_unitap022] >> 8 && b == (cell022[c_unitap022] & 0xff));
    return unitap022[c_unitap022++];
}

int TaLogic022_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id >= 0 && id < 1000);
    c_spec022++;
    last_spec022 = id;
    return r_spec022[id];
}

int TaLogic022_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c_spell022++;
    return r_spell022;
}

int TaLogicCom_GetUseXYRangeSpell__0016cbb0(int side, unsigned *record)
{
    assert(side == SIDE && (void *)record == (void *)&pair022);
    assert(pair022.unknown_0 == 700 || pair022.unknown_0 == 702);
    c_xy022++;
    return r_xy022;
}

/* 0x12d520: its unit scan covers slots 0..10 of both rows, unlike the 1..5 scans above. */
/* 0x11e6e0 asks positions 1..11 (11 after its early exit): its own tables, used when wide004 is set */
static int wide004, ctl004[2][12], card004[2][12], st004[2][12], c_ctl004;
static int wide007, unit007[2][11], owner007[2][11], c_unit007, c_ctl007;
static int r_spell007, c_spell007, r_curse007, c_curse007, r_cerem007, c_cerem007;
static int r_spec007[1000], c_spec007, last_spec007, r_revive007, c_revive007;

static void reset007(void)
{
    wide007 = c_unit007 = c_ctl007 = c_spell007 = c_curse007 = c_cerem007 = c_spec007 = c_revive007 = 0;
    r_cerem007 = last_spec007 = 0;
    r_spell007 = r_curse007 = r_revive007 = -1;
    memset(unit007, 0, sizeof(unit007));
    memset(owner007, 0, sizeof(owner007));
    memset(r_spec007, 0xff, sizeof(r_spec007));
}

int TaLogic007_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c_spell007++;
    return r_spell007;
}

int TaLogic007_GetCurseCardPlay(int side, int *list, struct DotrPickBlock *block, unsigned char *item)
{
    assert(side == SIDE && list != NULL && (void *)block == (void *)BLOCK && item != NULL);
    c_curse007++;
    return r_curse007;
}

int TaLogic007_IsDoCerem(int value, int index)
{
    assert(value == SIDE && index == 834);
    c_cerem007++;
    return r_cerem007;
}

int TaLogic007_GetSpecCardPlay(int side, int *list, struct DotrStateBlock *block, unsigned char *item, int id)
{
    assert(side == SIDE && list != NULL && block == BLOCK && item != NULL);
    assert(id == 834 || id == 326 || id == 305);
    c_spec007++;
    last_spec007 = id;
    return r_spec007[id];
}

int TaLogic007_IsRevivePlay(int side) { assert(side == SIDE); c_revive007++; return r_revive007; }

int Ta_IsUnitControl(int a, int b)
{
    if (wide004) {
        assert((a == 0 || a == 1) && b >= 1 && b <= 11);
        c_ctl004++;
        return ctl004[a][b];
    }
    assert((a == 0 || a == 1) && b >= 0 && b <= 10 && unit007[a][b]);
    c_ctl007++;
    return owner007[a][b];
}

struct DotrStateItem *SzDuel_GetUnit(int a, int b)
{
    if (wide007) {
        assert((a == 0 || a == 1) && b >= 0 && b <= 10);
        c_unit007++;
        return unit007[a][b] ? (struct DotrStateItem *)storage : NULL;
    }
    assert((a == 0 || a == 1) && b >= 1 && b <= 5);
    return slot_set[a][b] ? (struct DotrStateItem *)storage : NULL;
}

int SzDuel_GetUnitCardNo(int a, int b)
{
    if (wide004) {
        assert((a == 0 || a == 1) && b >= 1 && b <= 11);
        return card004[a][b];
    }
    assert(slot_set[a][b]);
    return slot_word[a][b];
}
int func_00250360_pair(int a, int b) { assert(slot_set[a][b]); return slot_owner[a][b]; }

static int r168e00, c168e00, r168d10[1000], c168d10, r16b0f0, r1de5c0;

int TaLogic021_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c168e00++;
    return r168e00;
}

int TaLogic021_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    assert(id == 699 || id == 825 || id == 748 || id == 224 || id == 364);
    c168d10++;
    return r168d10[id];
}

int TaLogic021_IsUseBlackHole(int side) { assert(side == SIDE); return r16b0f0; }
int SzDuel_GetSummonPower(int index) { assert(index == SIDE); return r1de5c0; }

static int r13b220, c13b220, r13b130[1000], c13b130, r13d980, r13d2c0, flag_1e1580[11];

int TaLogic010_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c13b220++;
    return r13b220;
}

int TaLogic010_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    assert(id == 840 || id == 700 || id == 796);
    c13b130++;
    return r13b130[id];
}

int TaLogic010_IsDoCerem(int value, int index) { assert(value == SIDE && index == 840); return r13d980; }
int TaLogic010_GetUseThunderBolt(int side) { assert(side == SIDE); return r13d2c0; }
int SzDuel_GetUnitCurseRest(int a, int b) { assert(a == SIDE && b >= 0 && b <= 10); return flag_1e1580[b]; }

static int r114b40, c114b40, ok1146c0[1000], c1146c0, r117bd0, r117600, r1173e0, r117550, r117680;
static int r1147b0, c1147b0, r24ca40_53;

int TaLogic001_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c114b40++;
    return r114b40;
}

/* ok1146c0[id]: nonzero makes the routine accept (answer 0), else it answers -1 */
int TaLogic001_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id >= 0 && id < 1000);
    c1146c0++;
    return ok1146c0[id] ? 0 : -1;
}

int TaLogic001_IsDoCerem(int value, int index) { assert(value == SIDE && index == 837); return r117bd0; }
int TaLogic001_IsUseLordOfDragon(int side) { assert(side == SIDE); return r117600; }
int TaLogic001_IsUseLifeHourGlass(int side) { assert(side == SIDE); return r1173e0; }
int TaLogic001_IsUseBraveHourGlass(int side) { assert(side == SIDE); return r117550; }
int TaLogic001_IsUseRevive(int side) { assert(side == SIDE); return r117680; }

int TaLogic001_GetProtectCardPlay(int side, unsigned char *item, struct DotrStateBlock *block)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    c1147b0++;
    return r1147b0;
}

static int r148960, c148960, r148870[1000], c148870, r14b240, r148630, c148630, r24e910, r14b490;

int TaLogic013_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c148960++;
    return r148960;
}

int TaLogic013_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    assert(id == 843 || id == 30 || id == 685 || id == 796);
    c148870++;
    return r148870[id];
}

int TaLogic013_IsDoCerem(int value, int index) { assert(value == SIDE && index == 843); return r14b240; }

int TaLogic013_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block)
{
    assert(side == SIDE && item != NULL && (void *)block == (void *)BLOCK);
    c148630++;
    return r148630;
}

int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0 && b == 0 && c == SIDE && d == 1 && f == 14);
    if (e == 0x53001C) {
        return r24ca40_53;
    }
    assert(e == 0x1E001C);
    return 0x5E7;
}

int Ta_GetUnitNumFrom22bitFlagList(unsigned value) { assert(value == 0x5E7 || value == 0xA7); return r24e910; }
int TaLogic013_IsUseRevive(int side) { assert(side == SIDE); return r14b490; }

/* Every pick routine refuses, both bytes are 0, the id list is empty. */
static void reset(void)
{
    c15fc50 = c15fb60 = c15fa00 = r15fa00 = 0;
    memset(r15fb60, 0xff, sizeof(r15fb60));
    memset(r162490, 0, sizeof(r162490));
    r15fc50 = r1625e0 = r1626f0 = r1627d0 = r162a70 = -1;
    c136980 = c136890 = r138db0 = 0;
    r136980 = -1;
    memset(r136890, 0xff, sizeof(r136890));
    memset(r138f90, 0, sizeof(r138f90));
    r138b60 = r138ea0 = -1;
    first = second = fill_calls = count = mark_calls = mark_id = 0;
    memset(kind_by, 0, sizeof(kind_by));
    memset(afford_by, 0, sizeof(afford_by));
    memset(ids, 0, sizeof(ids));
    r1514a0 = r1513b0 = r14d1e0 = r14d0f0_852 = r14d0f0_701 = r14d0f0_742 = -1;
    r13f540 = r13f3b0 = r13f640 = -1;
    r151170 = r153f30 = r14f8c0 = r14f3a0 = r1e01f0 = r1e0150 = r142280 = 0;
    r15a910 = r15a820 = r164ae0 = r164910_842 = r164910_850 = r164a00 = r1562e0 = r155c90 = -1;
    r15d030 = r167670_842 = r167670_850 = r158800 = class_by[0] = class_by[1] = 0;
    c15a910 = c15a820 = c164ae0 = c164910 = c164a00 = c1562e0 = c155c90 = 0;
    r119d30 = r119f30 = z118ce0 = -1;
    c119d30 = c119f30 = c118ce0 = id119d30 = 0;
    r132b00 = r132a10_832 = r132a10_746[0] = r132a10_746[1] = -1;
    r135000 = c132b00 = c132a10 = 0;
    memset(slot_set, 0, sizeof(slot_set));
    memset(slot_word, 0, sizeof(slot_word));
    memset(slot_owner, 0, sizeof(slot_owner));
    r168e00 = r168d10[699] = r168d10[825] = r168d10[748] = r168d10[224] = r168d10[364] = -1;
    r16b0f0 = c168e00 = c168d10 = 0;
    r1de5c0 = 4;
    r13b220 = r13b130[840] = r13b130[700] = r13b130[796] = r13d2c0 = -1;
    r13d980 = c13b220 = c13b130 = 0;
    memset(flag_1e1580, 0, sizeof(flag_1e1580));
    memset(ok1146c0, 0, sizeof(ok1146c0));
    r114b40 = r117680 = r1147b0 = -1;
    r24ca40_53 = 1;
    r117bd0 = r117600 = r1173e0 = r117550 = c114b40 = c1146c0 = c1147b0 = 0;
    r148960 = r148870[843] = r148870[30] = r148870[685] = r148870[796] = r14b490 = -1;
    r14b240 = r148630 = r24e910 = c148960 = c148870 = c148630 = 0;
    c1514a0 = c151170 = c1513b0 = c14d1e0 = c14d0f0 = c13f540 = c13f3b0 = c13f640 = 0;
    r10f450 = r10f290 = -1;
    r144250 = r144150[846] = r144150[731] = r144150[751] = r144150[3] = r144150[4] = -1;
    r1466a0 = r1465a0 = c144250 = c144150 = c143a80 = 0;
    memset(ok143a80, 0, sizeof(ok143a80));
    memset(score_by, 0, sizeof(score_by));
    r128520 = r128430[699] = r128430[108] = r128430[685] = r128430[851] = r27da40 = -1;
    r27bbc0 = r12b000 = r12b8b0 = c128520 = c128430 = c12b000 = c12b8b0 = 0;
    r10f6f0[1] = r10f6f0[2] = 1;
    c10f450 = c10f290 = c10f6f0 = 0;
    reset007();
    reset022();
}

int TaLogic002_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    c10f450++;
    return r10f450;
}

int TaLogic002_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block, unsigned char mode)
{
    assert(side == SIDE && item != NULL && (void *)block == (void *)BLOCK && mode == 0);
    c10f290++;
    return r10f290;
}

int TaLogic002_IsMakeDefense(int which)
{
    assert(which == 1 || which == 2);
    c10f6f0++;
    return r10f6f0[which];
}

int TaLogic006_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c128520++;
    return r128520;
}

int TaLogic006_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    assert(id == 699 || id == 108 || id == 685 || id == 851);
    c128430++;
    return r128430[id];
}

int TaLogic_IsAttackedNow(int a, int b, int c)
{
    assert(a == SIDE && b == 0 && c == 0);
    return r27bbc0;
}

int TaLogic006_IsBlackHoleCondition(int side)
{
    assert(side == SIDE);
    c12b000++;
    return r12b000;
}

int TaLogic_GetMaxAPGraveCard(int side, int mask)
{
    assert(side == SIDE && mask == 3);
    return r27da40;
}

int TaLogic006_IsDoCerem(int value, int index)
{
    assert(value == SIDE && index == 851);
    c12b8b0++;
    return r12b8b0;
}

int TaLogic012_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c144250++;
    return r144250;
}

int TaLogic012_GetSpecCardPlay(int side, int *list, struct DotrStateBlock *block, unsigned char *item, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    assert(memcmp(list, ids, sizeof(ids)) == 0);
    c144150++;
    last144150 = id;
    return r144150[id];
}

int TaLogic012_IsDoCerem(int value, int index)
{
    assert(value == SIDE && index == 846);
    return r1466a0;
}

int TaLogic012_IsPinch(int side)
{
    assert(side == SIDE);
    return r1465a0;
}

int TaLogic012_IsLineEmpty(int id, int y)
{
    assert(y >= 1 && y <= 5);
    c143a80++;
    return ok143a80[id][y];
}

/* the score of the id TaLogic012_GetSpecCardPlay was last asked about */
int SzDuel_GetUnitAP2(struct DotrStateItem *item, int flag)
{
    assert((void *)item == (void *)BLOCK && flag == 0);
    return score_by[last144150];
}

/* ids 3 (kind 32: skipped), 4 (kind 31: eligible when affordable) */
static void two_ids(int affordable)
{
    count = 2;
    ids[0] = 3;
    ids[1] = 4;
    kind_by[3] = 32;
    kind_by[4] = 31;
    afford_by[3] = 1;
    afford_by[4] = affordable;
}

typedef int (*dispatch_fn)(int side, struct DotrStateBlock *block);

/* The part the three functions share: null block, first byte 5, second byte 5.
 * marks: whether a found id is reported through SzDuel_InitCardInfo. */
static void check_shared(dispatch_fn fn, int *last_answer, int *last_calls)
{
    reset();
    assert(fn(SIDE, NULL) == -1);
    assert(fill_calls == 0);

    reset();
    first = 5;
    *last_answer = 0;
    assert(fn(SIDE, BLOCK) == 1);
    assert(fill_calls == 1 && *last_calls == 1);
    reset();
    first = 5;
    second = 5;
    *last_answer = -1;
    assert(fn(SIDE, BLOCK) == -1);
    assert(*last_calls == 1);

    reset();
    second = 5;
    assert(fn(SIDE, BLOCK) == -1); /* empty list */
    assert(*last_calls == 0);
    reset();
    second = 5;
    two_ids(1);
    assert(fn(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 4 && *last_calls == 0);
    reset();
    second = 5;
    two_ids(0);
    assert(fn(SIDE, BLOCK) == -1);
    assert(mark_calls == 0 && *last_calls == 0);
}

/* The shared tail: the id scan, then the last routine's answer or -1. */
static void check_tail(dispatch_fn fn, int *last_answer, int *last_calls, int marks)
{
    reset();
    two_ids(1);
    assert(fn(SIDE, BLOCK) == 0);
    assert(mark_calls == marks && *last_calls == 0);
    if (marks) {
        assert(mark_id == 4);
    }
    reset();
    two_ids(2);
    *last_answer = 7;
    assert(fn(SIDE, BLOCK) == 7);
    assert(mark_calls == 0 && *last_calls == 1);
    reset();
    *last_answer = 0;
    assert(fn(SIDE, BLOCK) == 0);
    reset();
    *last_answer = -2;
    assert(fn(SIDE, BLOCK) == -1);
    assert(*last_calls == 1);
}

/* 0x12d520 takes a third argument; the wrappers fix it. With the flag set the ceremony test
 * has to pass, otherwise the function reads a value it never assigned (see the header). */
static int hand007_clear(int side, struct DotrStateBlock *block)
{
    wide007 = 1;
    return TaLogic007_IsHandPlay(side, block, 0);
}

static int hand007_set(int side, struct DotrStateBlock *block)
{
    wide007 = 1;
    r_cerem007 = 1;
    return TaLogic007_IsHandPlay(side, block, 1);
}

static void check_12d520(void)
{
    check_shared(hand007_clear, &r_spell007, &c_spell007);
    check_tail(hand007_clear, &r_spell007, &c_spell007, 0);
    check_shared(hand007_set, &r_spell007, &c_spell007);
    check_tail(hand007_set, &r_spell007, &c_spell007, 0);

    /* flag clear: the first pick answers */
    reset();
    r_curse007 = 0;
    assert(hand007_clear(SIDE, BLOCK) == 2);
    assert(c_curse007 == 1 && c_cerem007 == 0 && c_spec007 == 0 && c_unit007 == 0);
    /* ceremony passes and its pick answers */
    reset();
    r_cerem007 = 1;
    r_spec007[834] = 0;
    assert(hand007_clear(SIDE, BLOCK) == 2);
    assert(c_curse007 == 1 && c_cerem007 == 1 && c_spec007 == 1 && last_spec007 == 834 && c_unit007 == 0);
    /* ceremony fails: no 834 pick; every slot of both rows is visited, owners asked for set slots only;
     * the pick answer is negative, so 326 is not tried; the revive test fails, so 305 is not tried */
    reset();
    unit007[0][0] = unit007[0][10] = unit007[1][0] = unit007[1][10] = 1;
    owner007[0][0] = owner007[1][10] = SIDE;
    assert(hand007_clear(SIDE, BLOCK) == -1);
    assert(c_cerem007 == 1 && c_spec007 == 0 && c_unit007 == 22 && c_ctl007 == 4 && c_revive007 == 1);
    assert(c_spell007 == 1);
    /* ceremony passes, pick fails, revive passes (zero counts) and 305 answers */
    reset();
    r_cerem007 = 1;
    r_revive007 = 0;
    r_spec007[305] = 0;
    assert(hand007_clear(SIDE, BLOCK) == 2);
    assert(c_spec007 == 2 && last_spec007 == 305 && c_unit007 == 22 && c_spell007 == 0);
    /* revive passes, 305 fails: on to the tail */
    reset();
    r_revive007 = 3;
    assert(hand007_clear(SIDE, BLOCK) == -1);
    assert(c_spec007 == 1 && last_spec007 == 305 && c_spell007 == 1);

    /* flag set: no first pick, no revive test */
    reset();
    r_spec007[834] = 0;
    assert(hand007_set(SIDE, BLOCK) == 2);
    assert(c_curse007 == 0 && c_cerem007 == 1 && c_spec007 == 1 && c_unit007 == 0);
    reset();
    unit007[0][5] = unit007[1][5] = 1;
    owner007[0][5] = SIDE;
    assert(hand007_set(SIDE, BLOCK) == -1);
    assert(c_curse007 == 0 && c_spec007 == 1 && last_spec007 == 834 && c_unit007 == 22 && c_ctl007 == 2);
    assert(c_revive007 == 0 && c_spell007 == 1);
}

static void one_cerem_card(void)
{
    count = 1;
    ids[0] = 9;
    kind_by[9] = 5 << 5;
    r_sac022 = 0x5E7;
    r_spec022[9] = 0;
}

static void check_16f760(void)
{
    int n;

    check_shared(TaLogic022_IsHandPlay, &r_spell022, &c_spell022);
    check_tail(TaLogic022_IsHandPlay, &r_spell022, &c_spell022, 1);

    /* a kind-5 card with no sacrifice list: not played */
    reset();
    one_cerem_card();
    r_sac022 = -1;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c_effect022 == 1 && c_cerem022 == 1 && c_spec022 == 0 && c_spell022 == 1);
    /* an empty list passes; the card's own pick decides */
    reset();
    one_cerem_card();
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c_spec022 == 1 && last_spec022 == 9 && c_unitap022 == 0 && c_spell022 == 0);
    reset();
    one_cerem_card();
    r_spec022[9] = -1;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c_spec022 == 1 && c_spell022 == 1);
    /* kind 4 (not 5) is not a ceremony card */
    reset();
    one_cerem_card();
    kind_by[9] = 4 << 5;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c_effect022 == 0 && c_spec022 == 0);
    /* every listed unit below the card's upper half word (100): played */
    reset();
    one_cerem_card();
    r24e910 = 2;
    cell022[0] = 0x0103;
    cell022[1] = 0x0005;
    unitap022[0] = 99;
    unitap022[1] = 0;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c_unitap022 == 2 && c_spec022 == 1);
    /* a unit equal to it: refused at that unit */
    reset();
    one_cerem_card();
    r24e910 = 2;
    cell022[0] = 0x0103;
    unitap022[0] = 100;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c_unitap022 == 1 && c_spec022 == 0);
    /* the comparison is unsigned on both sides: a negative unit value refuses, and a card
     * word with the top bit set is a large limit */
    reset();
    one_cerem_card();
    r24e910 = 1;
    unitap022[0] = -1;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1 && c_spec022 == 0);
    reset();
    one_cerem_card();
    r24e910 = 1;
    apdp022 = (int)0x80000000u;
    unitap022[0] = 0x9000;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1 && c_spec022 == 0);

    /* ids 700 and 702 ask the range routine; 701 does not */
    for (n = 700; n <= 702; n++) {
        reset();
        count = 1;
        ids[0] = n;
        r_xy022 = 0;
        r_spec022[n] = 0;
        if (n == 701) {
            assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1 && c_xy022 == 0 && c_spec022 == 0);
        } else {
            assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == 2);
            assert(c_xy022 == 1 && c_spec022 == 1 && last_spec022 == n);
        }
    }
    reset();
    count = 1;
    ids[0] = 700;
    r_spec022[700] = 0;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1); /* range routine refuses */
    assert(c_xy022 == 1 && c_spec022 == 0);
    reset();
    count = 1;
    ids[0] = 702;
    r_xy022 = 0;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1); /* its pick refuses */
    assert(c_xy022 == 1 && c_spec022 == 1 && c_spell022 == 1);

    /* a flagged slot (bounds 0 and 10) tries 796, then 795 */
    for (n = 0; n <= 10; n += 10) {
        reset();
        flag_1e1580[n] = 1;
        r_spec022[796] = 0;
        assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c_spec022 == 1 && last_spec022 == 796);
        reset();
        flag_1e1580[n] = 1;
        r_spec022[795] = 0;
        assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c_spec022 == 2 && last_spec022 == 795);
    }
    reset();
    flag_1e1580[4] = 1; /* both refuse */
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c_spec022 == 2 && c_spell022 == 1);
    reset(); /* no flagged slot: neither is tried */
    r_spec022[796] = r_spec022[795] = 0;
    assert(TaLogic022_IsHandPlay(SIDE, BLOCK) == -1 && c_spec022 == 0);
}

static void check_150f10(void)
{
    check_shared(TaLogic015_IsHandPlay, &r1514a0, &c1514a0);

    reset();
    r151170 = 1;
    assert(TaLogic015_IsHandPlay(SIDE, BLOCK) == 3);
    assert(c151170 == 1 && c1513b0 == 0 && c1514a0 == 0);
    reset();
    r153f30 = 1;
    r1513b0 = 0;
    assert(TaLogic015_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c1513b0 == 1 && c1514a0 == 0);
    reset();
    r153f30 = 1; /* the 686 routine refuses: falls to the tail */
    assert(TaLogic015_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1513b0 == 1 && c1514a0 == 1);
    reset();
    r153f30 = 2;
    r1513b0 = 0;
    assert(TaLogic015_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1513b0 == 0);

    check_tail(TaLogic015_IsHandPlay, &r1514a0, &c1514a0, 1);
}

static void check_14ce30(void)
{
    check_shared(TaLogic014_IsHandPlay, &r14d1e0, &c14d1e0);

    reset();
    r14d0f0_852 = 0;
    r14f8c0 = 1;
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c14d0f0 == 1);
    reset();
    r14d0f0_852 = 0; /* TaLogic014_IsDoCerem answers 0: next stage */
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c14d0f0 == 1 && c14d1e0 == 1);

    reset();
    r14f3a0 = 1;
    r14d0f0_701 = 0;
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c14d0f0 == 2);
    reset();
    r14f3a0 = 1;
    r1e01f0 = 1;
    r14d0f0_701 = 0;
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c14d0f0 == 1);
    reset();
    r14f3a0 = 2;
    r14d0f0_701 = 0;
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c14d0f0 == 1);
    reset();
    r14f3a0 = 1; /* the 701 routine refuses */
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c14d0f0 == 2);

    reset();
    r1e0150 = 4;
    r14d0f0_742 = 0;
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c14d0f0 == 2);
    reset();
    r1e0150 = 3;
    r14d0f0_742 = 0;
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c14d0f0 == 1);
    reset();
    r1e0150 = 4; /* the 742 routine refuses */
    assert(TaLogic014_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c14d0f0 == 2 && c14d1e0 == 1);

    check_tail(TaLogic014_IsHandPlay, &r14d1e0, &c14d1e0, 1);
}

static void check_13f170(void)
{
    check_shared(TaLogic011_IsHandPlay, &r13f640, &c13f640);

    reset();
    ids[5] = 6;
    r142280 = 1;
    r13f540 = 0;
    assert(TaLogic011_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c13f540 == 1 && c13f3b0 == 0);
    reset();
    r142280 = 1; /* the 368 routine refuses */
    assert(TaLogic011_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c13f540 == 1 && c13f3b0 == 1 && c13f640 == 1);
    reset();
    r142280 = 2;
    r13f540 = 0;
    assert(TaLogic011_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c13f540 == 0);

    reset();
    r13f3b0 = 0;
    assert(TaLogic011_IsHandPlay(SIDE, BLOCK) == 3);
    assert(c13f3b0 == 1 && c13f640 == 0);

    /* here the scan after the stages returns 0 without SzDuel_InitCardInfo */
    check_tail(TaLogic011_IsHandPlay, &r13f640, &c13f640, 0);
}

static void check_15a5f0(void)
{
    check_shared(TaLogic018_IsHandPlay, &r15a910, &c15a910);

    reset();
    r15a820 = 0;
    r15d030 = 1;
    assert(TaLogic018_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c15a820 == 1 && c15a910 == 0);
    reset();
    r15a820 = 0; /* TaLogic018_IsDoCerem answers 0: falls to the tail */
    assert(TaLogic018_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c15a820 == 1 && c15a910 == 1);
    reset();
    r15d030 = 1; /* the 835 routine refuses */
    assert(TaLogic018_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c15a820 == 1 && c15a910 == 1);

    check_tail(TaLogic018_IsHandPlay, &r15a910, &c15a910, 1);
}

static void check_119a40(void)
{
    check_shared(TaLogic_IsHandPlay__00119a40, &r119d30, &c119d30);

    reset(); /* the three-argument routine's answer is forwarded as the id */
    r119f30 = 3;
    r119d30 = 9;
    assert(TaLogic_IsHandPlay__00119a40(SIDE, BLOCK) == 9);
    assert(c119f30 == 1 && c119d30 == 1 && id119d30 == 3 && c118ce0 == 0);
    reset();
    r119f30 = 0;
    r119d30 = -5;
    assert(TaLogic_IsHandPlay__00119a40(SIDE, BLOCK) == -5); /* not clamped */
    assert(id119d30 == 0 && c118ce0 == 0);

    reset(); /* key 0 found: the scan does not mark */
    z118ce0 = 0;
    two_ids(1);
    assert(TaLogic_IsHandPlay__00119a40(SIDE, BLOCK) == 0);
    assert(c118ce0 == 1 && mark_calls == 0 && c119d30 == 0);
    reset();
    z118ce0 = 0;
    r119d30 = -2;
    assert(TaLogic_IsHandPlay__00119a40(SIDE, BLOCK) == -2); /* not clamped on this path */
    assert(c119d30 == 1 && id119d30 == 999);
    reset(); /* last key found */
    z118ce0 = 3;
    two_ids(0);
    r119d30 = 6;
    assert(TaLogic_IsHandPlay__00119a40(SIDE, BLOCK) == 6);
    assert(c118ce0 == 4 && mark_calls == 0 && id119d30 == 999);

    reset(); /* no key found: all four asked, then the marking tail */
    r119d30 = -2;
    assert(TaLogic_IsHandPlay__00119a40(SIDE, BLOCK) == -1);
    assert(c118ce0 == 4 && c119f30 == 1 && id119d30 == 999);
    check_tail(TaLogic_IsHandPlay__00119a40, &r119d30, &c119d30, 1);
}

static void check_164680(void)
{
    check_shared(TaLogic020_IsHandPlay, &r164ae0, &c164ae0);

    reset();
    r164910_842 = 0;
    r167670_842 = 1;
    assert(TaLogic020_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c164910 == 1 && c164a00 == 0);
    reset();
    r164910_842 = 0; /* TaLogic020_IsDoCerem answers 0 for 842: next stage */
    r167670_850 = 1;
    assert(TaLogic020_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c164910 == 2 && c164a00 == 1 && c164ae0 == 1);

    reset();
    r164910_850 = 0;
    r167670_850 = 1;
    assert(TaLogic020_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c164910 == 2 && c164a00 == 0);
    reset();
    r164910_850 = 0; /* TaLogic020_IsDoCerem answers 0 for 850 */
    r167670_842 = 1;
    assert(TaLogic020_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c164910 == 2 && c164a00 == 1);

    reset();
    r164a00 = 0;
    assert(TaLogic020_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c164910 == 2 && c164a00 == 1 && c164ae0 == 0);

    check_tail(TaLogic020_IsHandPlay, &r164ae0, &c164ae0, 1);
}

static void check_155a00(void)
{
    check_shared(TaLogic017_IsHandPlay, &r1562e0, &c1562e0);

    /* class scan: id 4 (kind 31 -> class index 0) is reported and answers 2 */
    reset();
    two_ids(0);
    class_by[0] = 2;
    assert(TaLogic017_IsHandPlay(SIDE, BLOCK) == 2);
    assert(mark_calls == 1 && mark_id == 4 && c155c90 == 0);
    reset();
    two_ids(0);
    class_by[1] = 2; /* id 3 (kind 32 -> class index 1) comes first */
    assert(TaLogic017_IsHandPlay(SIDE, BLOCK) == 2);
    assert(mark_calls == 1 && mark_id == 3);
    reset();
    two_ids(0);
    class_by[0] = class_by[1] = 1;
    assert(TaLogic017_IsHandPlay(SIDE, BLOCK) == -1);
    assert(mark_calls == 0 && c1562e0 == 1);

    reset();
    r158800 = 1;
    r155c90 = 0;
    assert(TaLogic017_IsHandPlay(SIDE, BLOCK) == 3);
    assert(c155c90 == 1 && c1562e0 == 0);
    reset();
    r158800 = 1; /* TaLogic017_PlayExzCombo refuses */
    assert(TaLogic017_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c155c90 == 1 && c1562e0 == 1);
    reset();
    r158800 = 2;
    r155c90 = 0;
    assert(TaLogic017_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c155c90 == 0);

    /* here the scan after the stages returns 0 without SzDuel_InitCardInfo */
    check_tail(TaLogic017_IsHandPlay, &r1562e0, &c1562e0, 0);
}

static void slot(int side, int n, int word, int owner)
{
    slot_set[side][n] = 1;
    slot_word[side][n] = word;
    slot_owner[side][n] = owner;
}

static void check_1326f0(void)
{
    int side;
    int n;

    check_shared(TaLogic008_IsHandPlay, &r132b00, &c132b00);

    reset();
    r132a10_832 = 0;
    r135000 = 1;
    assert(TaLogic008_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c132a10 == 1 && c132b00 == 0);
    reset();
    r132a10_832 = 0; /* TaLogic008_IsDoCerem answers 0: falls to the slots and the tail */
    assert(TaLogic008_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c132a10 == 1 && c132b00 == 1);
    reset();
    r135000 = 1; /* the 832 routine refuses */
    assert(TaLogic008_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c132a10 == 1 && c132b00 == 1);

    /* slots 1 and 5 (the bounds) of the own side and of the other side (0) */
    for (side = 0; side < 2; side++) {
        for (n = 1; n <= 5; n += 4) {
            reset();
            slot(side, n, 145, SIDE);
            r132a10_746[side] = 0;
            assert(TaLogic008_IsHandPlay(SIDE, BLOCK) == 2);
            assert(c132a10 == 2 && c132b00 == 0);
            reset();
            slot(side, n, 145, SIDE); /* the 746 routine refuses */
            assert(TaLogic008_IsHandPlay(SIDE, BLOCK) == -1);
            assert(c132a10 == 2 && c132b00 == 1);
            reset();
            slot(side, n, 144, SIDE); /* another word */
            r132a10_746[side] = 0;
            assert(TaLogic008_IsHandPlay(SIDE, BLOCK) == -1);
            assert(c132a10 == 1);
            reset();
            slot(side, n, 145, 0); /* owned by the other side */
            r132a10_746[side] = 0;
            assert(TaLogic008_IsHandPlay(SIDE, BLOCK) == -1);
            assert(c132a10 == 1);
        }
    }
    reset(); /* an empty slot is not asked for its word (the stubs assert) */
    r132a10_746[0] = r132a10_746[1] = 0;
    assert(TaLogic008_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c132a10 == 1);

    check_tail(TaLogic008_IsHandPlay, &r132b00, &c132b00, 1);
}

static void check_168a30(void)
{
    static const int late[3] = { 748, 224, 364 };
    int i;

    check_shared(TaLogic021_IsHandPlay, &r168e00, &c168e00);

    reset();
    r16b0f0 = 1;
    r168d10[699] = 0;
    assert(TaLogic021_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c168d10 == 1 && c168e00 == 0);
    reset();
    r16b0f0 = 1; /* the 699 routine refuses */
    assert(TaLogic021_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c168d10 == 2 && c168e00 == 1);
    reset();
    r16b0f0 = 2; /* 699 is not tried */
    r168d10[699] = 0;
    assert(TaLogic021_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c168d10 == 1);

    reset();
    r168d10[825] = 0;
    assert(TaLogic021_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c168d10 == 1 && c168e00 == 0);

    for (i = 0; i < 3; i++) {
        reset();
        r1de5c0 = 3; /* bound: below 4 */
        r168d10[late[i]] = 0;
        assert(TaLogic021_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c168d10 == 2 + i && c168e00 == 0);
        reset(); /* bound: 4 skips the three */
        r168d10[late[i]] = 0;
        assert(TaLogic021_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c168d10 == 1 && c168e00 == 1);
    }
    reset();
    r1de5c0 = 3; /* all three refuse */
    assert(TaLogic021_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c168d10 == 4 && c168e00 == 1);

    check_tail(TaLogic021_IsHandPlay, &r168e00, &c168e00, 1);
}

static void check_13ae00(void)
{
    int n;

    check_shared(TaLogic010_IsHandPlay, &r13b220, &c13b220);

    reset();
    r13b130[840] = 0;
    r13d980 = 1;
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c13b130 == 1 && c13b220 == 0);
    reset();
    r13b130[840] = 0; /* TaLogic010_IsDoCerem answers 0 */
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c13b130 == 1 && c13b220 == 1);
    reset();
    r13d980 = 1; /* the 840 routine refuses */
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c13b130 == 1 && c13b220 == 1);

    reset();
    r13d2c0 = 0; /* bound: 0 is accepted */
    r13b130[700] = 0;
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c13b130 == 2 && c13b220 == 0);
    reset();
    r13d2c0 = 0; /* the 700 routine refuses */
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c13b130 == 2 && c13b220 == 1);
    reset();
    r13b130[700] = 0; /* TaLogic010_GetUseThunderBolt answers -1: 700 is not tried */
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c13b130 == 1);

    /* class scan: id 4 (kind 31 -> class index 0) is reported and answers 1 */
    reset();
    two_ids(0);
    class_by[0] = 2;
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == 1);
    assert(mark_calls == 1 && mark_id == 4 && c13b220 == 0);
    reset();
    two_ids(0);
    class_by[1] = 2; /* id 3 (kind 32 -> class index 1) comes first */
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == 1);
    assert(mark_calls == 1 && mark_id == 3);
    reset();
    two_ids(0);
    class_by[0] = class_by[1] = 1;
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == -1);
    assert(mark_calls == 0 && c13b220 == 1);

    /* flags 0 and 10 are the bounds of the 796 loop */
    for (n = 0; n <= 10; n += 10) {
        reset();
        flag_1e1580[n] = 1;
        r13b130[796] = 0;
        assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c13b130 == 2 && c13b220 == 0);
        reset();
        flag_1e1580[n] = 1; /* the 796 routine refuses */
        assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c13b130 == 2 && c13b220 == 1);
    }
    reset();
    r13b130[796] = 0; /* no flag set: 796 is not tried */
    assert(TaLogic010_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c13b130 == 1);

    check_tail(TaLogic010_IsHandPlay, &r13b220, &c13b220, 1);
}

static void check_114310(void)
{
    int n;

    check_shared(TaLogic001_IsHandPlay, &r114b40, &c114b40);

    reset();
    ok1146c0[837] = 1;
    r117bd0 = 1;
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c1146c0 == 1 && c114b40 == 0);
    reset();
    ok1146c0[837] = 1; /* TaLogic001_IsDoCerem answers 0: falls through */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1146c0 == 1 && c114b40 == 1);
    reset();
    r117bd0 = 1; /* the 837 routine refuses */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1146c0 == 1 && c114b40 == 1);

    reset(); /* TaLogic001_IsUseLordOfDragon == 1 and the query answers 0: 83,
              * then the three-argument routine */
    r117600 = 1;
    r24ca40_53 = 0;
    ok1146c0[83] = 1;
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c1146c0 == 2 && c1147b0 == 0);
    reset();
    r117600 = 1;
    r24ca40_53 = 0;
    r1147b0 = 0;
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c1146c0 == 2 && c1147b0 == 1 && c114b40 == 0);
    reset();
    r117600 = 1;
    r24ca40_53 = 0; /* both refuse */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1147b0 == 1 && c114b40 == 1);
    reset();
    r117600 = 1; /* the query answers nonzero */
    ok1146c0[83] = 1;
    r1147b0 = 0;
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1147b0 == 0 && c1146c0 == 1);
    reset();
    r117600 = 2; /* not 1 */
    r24ca40_53 = 0;
    ok1146c0[83] = 1;
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1147b0 == 0);

    reset();
    r1173e0 = 1;
    ok1146c0[377] = 1;
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c1146c0 == 2);
    reset();
    r1173e0 = 1; /* 377 refuses */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1146c0 == 2);
    reset();
    ok1146c0[377] = 1; /* TaLogic001_IsUseLifeHourGlass not 1 */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);

    reset();
    r117550 = 1;
    ok1146c0[390] = 1;
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c1146c0 == 2);
    reset();
    r117550 = 1; /* 390 refuses */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1146c0 == 2);
    reset();
    ok1146c0[390] = 1; /* TaLogic001_IsUseBraveHourGlass not 1 */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);

    reset();
    ok1146c0[685] = 1; /* TaLogic001_IsUseRevive answers -1: 685 not tried */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1146c0 == 1);
    reset();
    r117680 = 0; /* bound: 0 tries 685 */
    ok1146c0[685] = 1;
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c1146c0 == 2);
    reset();
    r117680 = 0; /* 685 refuses */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1146c0 == 2 && c114b40 == 1);

    for (n = 0; n <= 10; n += 10) { /* both ends of the 0..10 loop */
        reset();
        flag_1e1580[n] = 1;
        ok1146c0[796] = 1;
        assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c1146c0 == 2 && c114b40 == 0);
    }
    reset();
    flag_1e1580[0] = flag_1e1580[10] = 1; /* 796 refuses each time */
    assert(TaLogic001_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c1146c0 == 3 && c114b40 == 1);

    check_tail(TaLogic001_IsHandPlay, &r114b40, &c114b40, 1);
}

static void check_10dfc0(void)
{
    check_shared(TaLogic_IsHandPlay__0010dfc0, &r10f450, &c10f450);

    reset();
    r10f290 = 0;
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 2);
    assert(c10f290 == 1 && c10f450 == 0 && c10f6f0 == 0);

    /* first byte 0: unmarked scan, then the last routine's answer unclamped */
    reset();
    two_ids(1);
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 0);
    assert(mark_calls == 0 && c10f450 == 0 && c10f6f0 == 0);
    reset();
    two_ids(0);
    r10f450 = -2;
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == -2);
    assert(c10f450 == 1 && c10f6f0 == 0);
    reset();
    r10f450 = 6; /* empty list */
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 6);

    /* first byte nonzero, query 1 answers 0: unmarked scan */
    reset();
    first = 1;
    r10f6f0[1] = 0;
    two_ids(1);
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 0);
    assert(mark_calls == 0 && c10f6f0 == 1 && c10f450 == 0);
    reset();
    first = 1;
    r10f6f0[1] = 0; /* empty list: falls to query 2 and the marked scan */
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == -1);
    assert(c10f6f0 == 2 && c10f450 == 1);
    reset();
    first = 1;
    r10f6f0[1] = 0;
    two_ids(0); /* nothing affordable */
    r10f450 = 3;
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 3);
    assert(mark_calls == 0 && c10f6f0 == 2 && c10f450 == 1);

    /* query 2 answers 0: the last routine first; a negative answer continues */
    reset();
    first = 1;
    r10f6f0[2] = 0;
    r10f450 = 0;
    two_ids(1);
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 0);
    assert(mark_calls == 0 && c10f450 == 1);
    reset();
    first = 1;
    r10f6f0[2] = 0;
    two_ids(1);
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 4 && c10f450 == 1);
    reset();
    first = 1;
    r10f6f0[2] = 0;
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == -1);
    assert(c10f450 == 2);

    /* both queries nonzero: marked scan, then the last routine's answer or -1 */
    reset();
    first = 1;
    two_ids(1);
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 4 && c10f450 == 0 && c10f6f0 == 2);
    reset();
    first = 1;
    two_ids(2);
    r10f450 = 7;
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == 7);
    assert(mark_calls == 0 && c10f450 == 1);
    reset();
    first = 1;
    r10f450 = -2;
    assert(TaLogic_IsHandPlay__0010dfc0(SIDE, BLOCK) == -1);
}

static void check_1280c0(void)
{
    check_shared(TaLogic006_IsHandPlay, &r128520, &c128520);

    /* 699: needs TaLogic_IsAttackedNow or TaLogic006_IsBlackHoleCondition to answer 1 */
    reset();
    r128430[699] = 0;
    r27bbc0 = 1;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c128430 == 1 && c12b000 == 0);
    reset();
    r128430[699] = 0;
    r12b000 = 1;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c128430 == 1 && c12b000 == 1);
    reset();
    r128430[699] = 0; /* neither answers 1: next stage */
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c128430 == 3 && c12b000 == 1 && c128520 == 1);

    reset();
    r128430[108] = 0;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c128430 == 2);

    /* 685 only when the low 16 bits of a non-negative TaLogic_GetMaxAPGraveCard answer reach 2400 */
    reset();
    r128430[685] = 0;
    r27da40 = 0x10000 + 2400;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c128430 == 3);
    reset();
    r128430[685] = 0;
    r27da40 = 2399;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c128430 == 3);
    reset();
    r128430[685] = 0; /* TaLogic_GetMaxAPGraveCard answers -1 */
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c128430 == 3);
    reset();
    r27da40 = 2400; /* 685 fails: next stage */
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c128430 == 4 && c12b8b0 == 0);

    reset();
    r128430[851] = 0;
    r12b8b0 = 1;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c12b8b0 == 1 && c128520 == 0);
    reset();
    r128430[851] = 0;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c12b8b0 == 1 && c128520 == 1);

    /* first byte 0: an unmarked scan comes first */
    reset();
    two_ids(1);
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 0);
    assert(mark_calls == 0 && c128520 == 0);
    reset();
    first = 1;
    two_ids(1);
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 4 && c128520 == 0);
    reset();
    first = 1;
    two_ids(2);
    r128520 = 7;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 7);
    assert(mark_calls == 0 && c128520 == 1);
    reset();
    two_ids(0);
    r128520 = 0;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == 0);
    reset();
    r128520 = -2;
    assert(TaLogic006_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c128520 == 1);
}

static void check_143de0(void)
{
    check_shared(TaLogic012_IsHandPlay, &r144250, &c144250);

    reset();
    r144150[846] = 0;
    r1466a0 = 1;
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c144150 == 1);
    reset();
    r144150[846] = 0; /* TaLogic012_IsDoCerem answers 0: next stage */
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c144150 == 1 && c144250 == 1);

    /* 731 and 751 only when TaLogic012_IsPinch answers 1 */
    reset();
    r1465a0 = 1;
    r144150[731] = 0;
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c144150 == 2);
    reset();
    r1465a0 = 1;
    r144150[751] = 0;
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c144150 == 3);
    reset();
    r144150[731] = r144150[751] = 0;
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c144150 == 1);

    /* best-score stage: every id is asked for y = 1..5 */
    reset();
    two_ids(0);
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c143a80 == 10 && c144150 == 1 && mark_calls == 0);
    reset();
    two_ids(0);
    ok143a80[3][1] = 1; /* first y */
    r144150[3] = 0;
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == 2);
    assert(mark_calls == 1 && mark_id == 3 && c144250 == 0);
    reset();
    two_ids(0);
    ok143a80[4][5] = 1; /* last y */
    r144150[4] = 0;
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == 2);
    assert(mark_calls == 1 && mark_id == 4);
    reset();
    two_ids(0);
    ok143a80[3][2] = ok143a80[4][2] = 1;
    r144150[3] = r144150[4] = 0;
    score_by[3] = score_by[4] = 5; /* an equal score does not replace the first */
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == 2);
    assert(mark_id == 3);
    reset();
    two_ids(0);
    ok143a80[3][2] = ok143a80[4][2] = 1;
    r144150[3] = r144150[4] = 0;
    score_by[3] = 5;
    score_by[4] = 6;
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == 2);
    assert(mark_id == 4);
    reset();
    two_ids(0);
    ok143a80[3][2] = 1; /* TaLogic012_GetSpecCardPlay refuses the id */
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c144150 == 2 && mark_calls == 0);
    reset();
    two_ids(0);
    ok143a80[3][2] = 1;
    r144150[3] = 0;
    score_by[3] = -1; /* a score of -1 is not above the start value */
    assert(TaLogic012_IsHandPlay(SIDE, BLOCK) == -1);
    assert(mark_calls == 0 && c144250 == 1);

    check_tail(TaLogic012_IsHandPlay, &r144250, &c144250, 0);
}

static void check_136490(void)
{
    static const int gated[2] = { 830, 849 };
    int i;

    check_shared(TaLogic009_IsHandPlay, &r136980, &c136980);

    for (i = 0; i < 2; i++) {
        reset();
        r136890[gated[i]] = 0;
        r138f90[gated[i]] = 1;
        assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c136890 == 1 + i && c136980 == 0);
        reset();
        r136890[gated[i]] = 0; /* TaLogic009_IsDoCerem answers 0 */
        assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c136890 == 3 && c136980 == 1);
        reset();
        r138f90[gated[i]] = 1; /* the routine refuses */
        assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c136890 == 3 && c136980 == 1);
    }

    reset();
    r138b60 = 0; /* bound: 0 is accepted */
    r136890[732] = 0;
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c136890 == 3 && c136980 == 0);
    reset();
    r138b60 = 0;
    r136890[732] = -2; /* the refusal's low half is not 0xffff: 733 is tried */
    r136890[733] = 0;
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c136890 == 4 && c136980 == 0);
    reset();
    r138b60 = 0;
    r136890[732] = -2; /* 733 refuses */
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c136890 == 5 && c136980 == 1);
    reset();
    r138b60 = 0;
    r136890[733] = 0; /* 732 answers -1, whose low half is 0xffff: 733 is not tried */
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c136890 == 4 && c136980 == 1);
    reset();
    r136890[732] = r136890[733] = 0; /* TaLogic009_GetRevoltObj answers -1: neither is tried */
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c136890 == 3);

    reset();
    r138db0 = 1;
    r136890[683] = 0;
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c136890 == 3 && c136980 == 0);
    reset();
    r138db0 = 1; /* the 683 routine refuses */
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c136890 == 4 && c136980 == 1);
    reset();
    r138db0 = 2; /* only 1 tries 683 */
    r136890[683] = 0;
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c136890 == 3);

    /* class scan: id 4 (kind 31 -> class index 0) is reported and answers 1 */
    reset();
    two_ids(0);
    class_by[0] = 2;
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == 1);
    assert(mark_calls == 1 && mark_id == 4 && c136890 == 2);
    reset();
    two_ids(0);
    class_by[1] = 2; /* id 3 (kind 32 -> class index 1) comes first */
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == 1);
    assert(mark_calls == 1 && mark_id == 3);
    reset();
    two_ids(0);
    class_by[0] = class_by[1] = 1;
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
    assert(mark_calls == 0 && c136980 == 1);

    reset();
    r136890[713] = 0;
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c136890 == 3 && c136980 == 0);

    reset();
    r138ea0 = 0; /* bound: 0 is accepted */
    r136890[687] = 0;
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c136890 == 4 && c136980 == 0);
    reset();
    r138ea0 = 0; /* the 687 routine refuses */
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c136890 == 4 && c136980 == 1);
    reset();
    r136890[687] = 0; /* TaLogic009_IsUseCopyCat answers -1 */
    assert(TaLogic009_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c136890 == 3);

    check_tail(TaLogic009_IsHandPlay, &r136980, &c136980, 1);
}

static void check_15f5e0(void)
{
    static const int gated[2] = { 833, 853 };
    static const int after[4] = { 700, 715, 685, 43 };
    static const int looped[2] = { 796, 795 };
    int *gate[4];
    int i;
    int n;

    gate[0] = &r1625e0;
    gate[1] = &r1626f0;
    gate[2] = &r1627d0;
    gate[3] = &r162a70;
    check_shared(TaLogic019_IsHandPlay, &r15fc50, &c15fc50);

    for (i = 0; i < 2; i++) {
        reset();
        r15fb60[gated[i]] = 0;
        r162490[gated[i]] = 1;
        assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c15fb60 == 1 + i && c15fc50 == 0);
        reset();
        r15fb60[gated[i]] = 0; /* TaLogic019_IsDoCerem answers 0 */
        assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c15fb60 == 2 && c15fc50 == 1);
        reset();
        r162490[gated[i]] = 1; /* the routine refuses */
        assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c15fb60 == 2 && c15fc50 == 1);
    }

    for (i = 0; i < 4; i++) {
        reset();
        *gate[i] = 0; /* bound: 0 is accepted */
        r15fb60[after[i]] = 0;
        assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c15fb60 == 3 && c15fa00 == 0 && c15fc50 == 0);
        reset();
        *gate[i] = 0; /* the routine refuses */
        assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c15fb60 == 3 && c15fa00 == 1 && c15fc50 == 1);
        reset();
        r15fb60[after[i]] = 0; /* the gate answers -1 */
        assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c15fb60 == 2);
    }

    reset();
    r15fa00 = 1;
    assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == 3);
    assert(c15fa00 == 1 && c15fc50 == 0);
    reset();
    r15fa00 = 2;
    assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c15fc50 == 1);

    /* flags 0 and 10 are the bounds of the 796 loop and of the 795 loop after it */
    for (i = 0; i < 2; i++) {
        for (n = 0; n <= 10; n += 10) {
            reset();
            flag_1e1580[n] = 1;
            r15fb60[looped[i]] = 0;
            assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == 2);
            assert(c15fb60 == 3 + i && c15fc50 == 0);
        }
        reset();
        r15fb60[looped[i]] = 0; /* no flag set */
        assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c15fb60 == 2);
    }
    reset();
    flag_1e1580[10] = 1; /* both routines refuse */
    assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c15fb60 == 4 && c15fc50 == 1);

    /* the last scan passes over id 297 even when it is eligible and affordable */
    reset();
    count = 1;
    ids[0] = 297;
    afford_by[297] = 1;
    r15fc50 = 7;
    assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == 7);
    assert(mark_calls == 0 && c15fc50 == 1);
    reset();
    second = 5; /* the second-byte scan does not make that exception */
    count = 1;
    ids[0] = 297;
    afford_by[297] = 1;
    assert(TaLogic019_IsHandPlay(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 297);

    check_tail(TaLogic019_IsHandPlay, &r15fc50, &c15fc50, 1);
}

static void check_1482f0(void)
{
    int n;

    check_shared(TaLogic013_IsHandPlay, &r148960, &c148960);

    reset();
    r148870[843] = 0;
    r14b240 = 1;
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c148870 == 1 && c148630 == 0);
    reset();
    r148870[843] = 0; /* TaLogic013_IsDoCerem answers 0 */
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c148870 == 1 && c148630 == 1 && c148960 == 1);
    reset();
    r14b240 = 1; /* the 843 routine refuses */
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c148870 == 1 && c148960 == 1);

    reset();
    r148630 = 1;
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == 3);
    assert(c148630 == 1 && c148960 == 0);
    reset();
    r148630 = 2;
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c148960 == 1);

    reset();
    r24e910 = 1; /* bound: above 0 */
    r148870[30] = 0;
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c148870 == 2 && c148960 == 0);
    reset();
    r24e910 = 1; /* the 30 routine refuses */
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c148870 == 2 && c148960 == 1);
    reset();
    r148870[30] = 0; /* bound: 0 does not try 30 */
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c148870 == 1);

    reset();
    r14b490 = 0; /* bound: 0 is accepted */
    r148870[685] = 0;
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == 2);
    assert(c148870 == 2 && c148960 == 0);
    reset();
    r14b490 = 0; /* the 685 routine refuses */
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c148870 == 2 && c148960 == 1);
    reset();
    r148870[685] = 0; /* TaLogic013_IsUseRevive answers -1 */
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c148870 == 1);

    /* flags 0 and 10 are the bounds of the 796 loop */
    for (n = 0; n <= 10; n += 10) {
        reset();
        flag_1e1580[n] = 1;
        r148870[796] = 0;
        assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == 2);
        assert(c148870 == 2 && c148960 == 0);
        reset();
        flag_1e1580[n] = 1; /* the 796 routine refuses */
        assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
        assert(c148870 == 2 && c148960 == 1);
    }
    reset();
    r148870[796] = 0; /* no flag set */
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == -1);
    assert(c148870 == 1);

    /* the last scan passes over id 272 even when it is eligible and affordable */
    reset();
    count = 1;
    ids[0] = 272;
    afford_by[272] = 1;
    r148960 = 7;
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == 7);
    assert(mark_calls == 0 && c148960 == 1);
    reset();
    second = 5; /* the second-byte scan does not make that exception */
    count = 1;
    ids[0] = 272;
    afford_by[272] = 1;
    assert(TaLogic013_IsHandPlay(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 272);

    check_tail(TaLogic013_IsHandPlay, &r148960, &c148960, 1);
}

/* 0x123740 */
static int r_spell005, c_spell005, r_combo005, c_combo005, r_sspell005, c_sspell005;
static int r_keep005, c_keep005, keep_id005, r_spec005[1000], c_spec005, last_spec005;
static int status005[2][11], kind005[2][6];

int TaLogic005_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id == 999);
    c_spell005++;
    return r_spell005;
}

int TaLogic005_Is2Head_ThunderDragon(int side, int *list)
{
    assert(side == SIDE && list[0] == ids[0] && list[7] == ids[7]);
    return 0x1A7; /* only the low byte may be forwarded */
}

int TaLogic_SetSpecialComboQue__00124c90(int side, unsigned char *item, struct DotrPickBlock *block, unsigned char mode)
{
    assert(side == SIDE && item != NULL && (void *)block == (void *)BLOCK && mode == 1);
    c_combo005++;
    return r_combo005;
}

int TaLogic005_GetSpecialSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    c_sspell005++;
    return r_sspell005;
}

int TaLogic005_IsKeepHandCombo(int side, int *list, unsigned short id)
{
    assert(side == SIDE && list[0] == ids[0]);
    c_keep005++;
    keep_id005 = id;
    return r_keep005;
}

int TaLogic005_GetSpecialCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK && id > 0 && id < 1000);
    c_spec005++;
    last_spec005 = id;
    return r_spec005[id];
}

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    if (wide004) {
        assert((a == 0 || a == 1) && b >= 1 && b <= 11 && bit == 4);
        return st004[a][b];
    }
    assert((a == 0 || a == 1) && b >= 6 && b <= 10 && bit == 4);
    return status005[a][b];
}

int SzDuel_GetUnitKind(int a, int b)
{
    assert((a == 0 || a == 1) && b >= 1 && b <= 5);
    return kind005[a][b];
}

static void reset005(void)
{
    int a, b;

    first = second = 1;
    count = 0;
    memset(ids, 0, sizeof(ids));
    memset(kind_by, 0, sizeof(kind_by));
    memset(afford_by, 0, sizeof(afford_by));
    mark_calls = mark_id = fill_calls = 0;
    r_spell005 = r_combo005 = r_sspell005 = -1;
    c_spell005 = c_combo005 = c_sspell005 = c_keep005 = c_spec005 = 0;
    r_keep005 = 1;
    keep_id005 = last_spec005 = 0;
    memset(r_spec005, 0xff, sizeof(r_spec005));
    memset(status005, 0, sizeof(status005));
    memset(kind005, 0, sizeof(kind005));
    memset(slot_word, 0, sizeof(slot_word));
    r24e910 = r27bbc0 = 0;
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 11; b++) {
            unit007[a][b] = 1;
            owner007[a][b] = 7; /* neither side */
        }
        for (b = 0; b < 6; b++) {
            slot_set[a][b] = 1;
        }
    }
}

static void check_123740(void)
{
    /* no block: nothing is read from the hand */
    reset005();
    assert(TaLogic_IsHandPlay__00123740(SIDE, NULL) == -1);
    assert(fill_calls == 0);

    /* first count 5 (low byte): only the spell pick, 1 when it answers, else -1 */
    reset005();
    first = 5;
    r_spell005 = 0;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 1);
    r_spell005 = -1;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spell005 == 2 && fill_calls == 2 && c_sspell005 == 0);

    /* second count 5: first summonable monster of the hand, or -1 */
    reset005();
    second = 5;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    count = 2;
    ids[0] = 10;
    ids[1] = 20;
    kind_by[10] = 0x20;
    kind_by[20] = 0x1f;
    afford_by[10] = 1;
    afford_by[20] = 1;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 20);
    afford_by[20] = 2;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(mark_calls == 1 && c_sspell005 == 0 && c_spell005 == 0);

    /* three flagged units: the combo queue answers 3; a refusal or another count goes on */
    reset005();
    r24e910 = 3;
    r_combo005 = 0;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 3);
    assert(c_combo005 == 1 && c_sspell005 == 0);
    r_combo005 = -1;
    r_sspell005 = 0;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 2);
    assert(c_combo005 == 2 && c_sspell005 == 1);
    r24e910 = 2;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 2);
    assert(c_combo005 == 2 && c_sspell005 == 2 && c_spec005 == 0);

    /* first count 0: a summonable monster that is not kept for a combo answers 0 unmarked */
    reset005();
    first = 0;
    count = 1;
    ids[0] = 20; /* not summonable yet */
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_keep005 == 0);
    afford_by[20] = 1;
    r_keep005 = 0;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 0);
    assert(c_keep005 == 1 && keep_id005 == 20 && mark_calls == 0);
    r_keep005 = 1; /* kept: falls through to the last walk, which marks it */
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 0);
    assert(c_keep005 == 2 && mark_calls == 1 && mark_id == 20);
    first = 1; /* any other count skips the walk */
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 0);
    assert(c_keep005 == 2 && mark_calls == 2);

    /* positions 6..10 with status bit 4 under this side's control: one is enough for 565 */
    reset005();
    r_spec005[565] = 0;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 0 && c_spell005 == 1);
    status005[SIDE][6] = 1; /* flagged but not controlled */
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 0);
    owner007[SIDE][6] = SIDE;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 2);
    assert(c_spec005 == 1 && last_spec005 == 565);
    status005[SIDE][6] = 0;
    status005[!SIDE][10] = 1;
    owner007[!SIDE][10] = SIDE;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 2);
    assert(c_spec005 == 2);
    owner007[SIDE][6] = 7; /* unflagged positions are not asked for their controller */
    c_ctl007 = 0;
    r_spec005[565] = -1;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 3 && c_ctl007 == 1);

    /* positions 1..5 of kind 16 (low five bits): three are needed for 593 */
    reset005();
    r_spec005[593] = 0;
    kind005[SIDE][1] = 0x30;
    kind005[!SIDE][5] = 0x10;
    kind005[SIDE][3] = 0x11;
    owner007[SIDE][1] = owner007[!SIDE][5] = owner007[SIDE][3] = SIDE;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 0);
    kind005[SIDE][3] = 0x10;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 2);
    assert(c_spec005 == 1 && last_spec005 == 593);
    owner007[!SIDE][5] = !SIDE;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 1);

    /* two units of card 600 ask for 560; two of card 560 ask for 600 */
    reset005();
    r_spec005[560] = 0;
    r_spec005[600] = 0;
    slot_word[SIDE][1] = 600;
    owner007[SIDE][1] = SIDE;
    slot_word[!SIDE][5] = 600;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 0);
    owner007[!SIDE][5] = SIDE;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 2);
    assert(c_spec005 == 1 && last_spec005 == 560);
    slot_word[SIDE][1] = slot_word[!SIDE][5] = 560;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 2);
    assert(c_spec005 == 2 && last_spec005 == 600);
    slot_word[SIDE][1] = 600; /* one of each */
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 2);

    /* under attack: 84 */
    reset005();
    r_spec005[84] = 0;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 0);
    r27bbc0 = 1;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 2);
    assert(c_spec005 == 1 && last_spec005 == 84);
    r_spec005[84] = -1;

    /* nothing taken: the spell pick's answer, or -1 when it is negative */
    r_spell005 = 4;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == 4);
    r_spell005 = -3;
    assert(TaLogic_IsHandPlay__00123740(SIDE, BLOCK) == -1);
    assert(c_spec005 == 3 && mark_calls == 0);
}

/* 0x11e6e0 */
static int r_spell004[1000], c_spell004, last_spell004, r_combo004[2], c_combo004[2];

int TaLogic004_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    assert(side == SIDE && item != NULL && block == BLOCK);
    assert(id == 999 || id == 738 || id == 823);
    c_spell004++;
    last_spell004 = id;
    return r_spell004[id];
}

int TaLogic004_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block, unsigned char mode)
{
    assert(side == SIDE && item != NULL && (void *)block == (void *)BLOCK && mode <= 1);
    c_combo004[mode]++;
    return r_combo004[mode];
}

static void reset004(void)
{
    int a, b;

    reset005();
    wide004 = 1;
    c_ctl004 = c_spell004 = last_spell004 = 0;
    c_combo004[0] = c_combo004[1] = 0;
    r_combo004[0] = r_combo004[1] = -1;
    memset(r_spell004, 0xff, sizeof(r_spell004));
    memset(card004, 0, sizeof(card004));
    memset(st004, 0, sizeof(st004));
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 12; b++) {
            ctl004[a][b] = 7; /* neither side */
        }
    }
}

static void check_11e6e0(void)
{
    int b;

    /* no block: nothing is read from the hand */
    reset004();
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, NULL) == -1);
    assert(fill_calls == 0);

    /* first count 5 (low byte): only the spell pick, 1 when it answers, else -1 */
    reset004();
    first = 5;
    r_spell004[999] = 0;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 1);
    r_spell004[999] = -1;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    assert(c_spell004 == 2 && fill_calls == 2 && c_combo004[0] == 0);

    /* second count 5: first summonable monster of the hand, or -1 */
    reset004();
    second = 5;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    count = 2;
    ids[0] = 10;
    ids[1] = 20;
    kind_by[10] = 0x20;
    kind_by[20] = 0x1f;
    afford_by[10] = 1;
    afford_by[20] = 1;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 20);
    afford_by[20] = 2;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    assert(mark_calls == 1 && c_combo004[0] == 0 && c_spell004 == 0);

    /* the combo pick in mode 0 answers 2 */
    reset004();
    r_combo004[0] = 0;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 2);
    assert(c_combo004[0] == 1 && c_ctl004 == 0 && c_spell004 == 0);

    /* positions 1..4 of kind 14 (low five bits) under this side's control: three are needed
     * for the combo pick in mode 1 (3) and then the pick of 738 (its answer) */
    reset004();
    r_combo004[1] = 0;
    r_spell004[738] = 6;
    kind005[SIDE][1] = 0x2e;
    kind005[!SIDE][4] = 0x0e;
    kind005[SIDE][3] = 0x0f;
    kind005[SIDE][5] = 0x0e; /* position 5 is not asked */
    ctl004[SIDE][1] = ctl004[!SIDE][4] = ctl004[SIDE][3] = ctl004[SIDE][5] = SIDE;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    assert(c_combo004[1] == 0 && c_ctl004 == 2 && last_spell004 == 999);
    kind005[SIDE][3] = 0x0e;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 3);
    assert(c_combo004[1] == 1);
    r_combo004[1] = -1;
    c_spell004 = 0;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 6);
    assert(c_combo004[1] == 2 && c_spell004 == 1 && last_spell004 == 738);
    r_spell004[738] = -1; /* refused: goes on to 823 and the last pick */
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    assert(c_spell004 == 4);
    ctl004[!SIDE][4] = !SIDE; /* a kind-14 unit the other side controls does not count */
    c_spell004 = 0;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    assert(c_combo004[1] == 3 && c_spell004 == 2);

    /* 823 can be played: taken when six or more of the positions 1..10 on both sides are
     * not controlled the expected way and have status bit 4 clear (this side's units count
     * when NOT controlled by it, the other side's when controlled by it) */
    reset004();
    r_spell004[823] = 9;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 9); /* 10 of this side's count */
    assert(c_spell004 == 1);
    for (b = 1; b <= 5; b++) {
        st004[SIDE][b] = 1;
    }
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1); /* five left */
    assert(c_spell004 == 3 && last_spell004 == 999);
    ctl004[!SIDE][2] = SIDE; /* the other side's unit under this side's control: six */
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 9);
    st004[!SIDE][2] = 1;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    st004[!SIDE][2] = 0;
    /* card 823 already out under this side's control: the walk ends with the count cleared;
     * the other side's half still runs once with position 11 */
    ctl004[SIDE][10] = SIDE;
    card004[SIDE][10] = 823;
    c_ctl004 = 0;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    assert(c_ctl004 == 20);
    card004[SIDE][10] = 822; /* another card: only that unit is left out */
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1); /* 4 + 1 */
    st004[SIDE][1] = 0;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 9);
    /* the same on the other side's half, for a unit this side does not control */
    card004[!SIDE][3] = 823;
    c_ctl004 = 0;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    assert(c_ctl004 == 6);

    /* last walk: the first summonable monster that is not card 342 is marked */
    reset004();
    count = 3;
    ids[0] = 10;
    ids[1] = 342;
    ids[2] = 20;
    kind_by[10] = 0x20;
    afford_by[10] = afford_by[342] = 1;
    r_spell004[999] = 4;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 4);
    assert(mark_calls == 0);
    afford_by[20] = 1;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == 0);
    assert(mark_calls == 1 && mark_id == 20);

    /* nothing taken: the spell pick's answer, or -1 when it is negative */
    afford_by[20] = 0;
    r_spell004[999] = -3;
    assert(TaLogic_IsHandPlay__0011e6e0(SIDE, BLOCK) == -1);
    wide004 = 0;
}

int main(void)
{
    check_1482f0();
    check_119a40();
    check_114310();
    check_10dfc0();
    check_1280c0();
    check_143de0();
    check_136490();
    check_12d520();
    check_16f760();
    check_15f5e0();
    check_13ae00();
    check_168a30();
    check_1326f0();
    check_15a5f0();
    check_164680();
    check_155a00();
    check_150f10();
    check_14ce30();
    check_13f170();
    check_123740();
    check_11e6e0();
    return 0;
}
