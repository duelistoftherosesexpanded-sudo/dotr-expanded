/* Synthetic host tests for the item_release group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/item_release.h"

struct DotrStateCell gSzDuel_aaMapInfo[7][7];
struct DotrReleaseSlot gSzDuel_aCardControl_release[6];

static struct DotrStateItem *item_result, *item_second;
static int item_args[2], item_calls;
static int seed_args[3], seed_calls;
static void *draw_arg, *slot_arg, *clear_arg;
static int draw_calls, slot_calls, clear_calls, clear_id;

struct DotrStateItem *SzDuel_GetCardInfo(int a, int b) { item_calls++; item_args[0] = a; item_args[1] = b; return item_calls > 1 && item_second ? item_second : item_result; }
int SzModel_Preset(int index, int value, int unknown) { seed_calls++; seed_args[0] = index; seed_args[1] = value; seed_args[2] = unknown; return 0; }
int SzDuel_ClearDrawCard(struct DotrOwnerDrawBlock *block) { draw_calls++; draw_arg = block; return 0; }
int SzDuel_ClearCardControl(struct DotrMiscSlot160 *p) { slot_calls++; slot_arg = p; return 0; }
void SzDuel_InitCardInfo(struct DotrStateBlock *block, int unknown) { clear_calls++; clear_arg = block; clear_id = unknown; }

struct DotrReleaseControl data_00317490_release;
struct DotrReleaseCurrent *data_00317b5c_release;
static int used[5], used_calls, used_index, count_result, count_arg;
static int place_calls, place_args[5][6];
static void *place_slot[5];
int SzDuel_InqHand(int index, int position) { used_calls++; used_index = index; return used[position]; }
int SzDuel_GetHandCount(int index) { count_arg = index; return count_result; }
int lSzDuel_SetCardCtl(struct DotrReleaseSlot *p, int a, int b, int c, int d, short e, short f)
{
    int *q = place_args[place_calls];
    place_slot[place_calls++] = p;
    q[0] = a; q[1] = b; q[2] = c; q[3] = d; q[4] = e; q[5] = f;
    return 0;
}

static void reset(void)
{
    item_calls = seed_calls = draw_calls = slot_calls = clear_calls = 0;
}

/* SzDuel_DelUnit (branch coverage): missing item, item byte +0x45 at 11 / 10, first argument
 * 1 and 2, byte +0x45 at 5 / 6, object present / absent, second argument 0, 1, 5, 6 and
 * a slot with and without an object. The null cell store is not run on the host. */
static void check_release(void)
{
    struct DotrStateItem item;
    struct DotrReleaseObject object;

    memset(&item, 0, sizeof(item));
    reset();
    item_result = 0;
    assert(SzDuel_DelUnit(1, 3) == 0 && item_calls == 1 && item_args[0] == 1 && item_args[1] == 3 && clear_calls == 0);
    item_result = &item;
    item.slot = 11;
    assert(SzDuel_DelUnit(1, 3) == 0 && clear_calls == 0);

    item.slot = 10;
    item.modelId = 0x91;
    item.locCol = 6;
    item.locRow = 2;
    item.cardModel = 0;
    gSzDuel_aaMapInfo[2][6].unit = 5;
    gSzDuel_aaMapInfo[6][2].unit = 5;
    assert(SzDuel_DelUnit(1, 3) == 1 && seed_calls == 0 && draw_calls == 0 && slot_calls == 0);
    assert(gSzDuel_aaMapInfo[2][6].unit == 0 && gSzDuel_aaMapInfo[6][2].unit == 5);
    assert(clear_calls == 1 && clear_arg == (void *)&item && clear_id == -1);

    item.slot = 6;
    item.cardModel = (struct DotrStateItemObject *)&object;
    object.unknown_90 = (struct DotrReleaseCurrent *)7;
    gSzDuel_aaMapInfo[2][6].unit = 5;
    assert(SzDuel_DelUnit(0, 3) == 1 && seed_calls == 0 && draw_calls == 1 && draw_arg == (void *)&object);
    assert(object.unknown_90 == 0 && gSzDuel_aaMapInfo[2][6].unit == 0 && clear_calls == 2);
    item.slot = 5;
    assert(SzDuel_DelUnit(0, 3) == 1 && seed_calls == 1 && draw_calls == 2);
    assert(seed_args[0] == 0x91 && seed_args[1] == -1 && seed_args[2] == 999);

    reset();
    gSzDuel_aaMapInfo[2][6].unit = 5;
    assert(SzDuel_DelUnit(2, 0) == 1 && clear_calls == 1 && slot_calls == 0);
    assert(SzDuel_DelUnit(2, 6) == 1 && clear_calls == 2 && slot_calls == 0);
    gSzDuel_aCardControl_release[1].object = 0;
    assert(SzDuel_DelUnit(2, 1) == 1 && slot_calls == 1 && slot_arg == (void *)&gSzDuel_aCardControl_release[1]);
    assert(draw_calls == 0 && clear_calls == 3);
    gSzDuel_aCardControl_release[5].object = &object;
    object.unknown_90 = (struct DotrReleaseCurrent *)7;
    assert(SzDuel_DelUnit(2, 5) == 1 && slot_calls == 2 && slot_arg == (void *)&gSzDuel_aCardControl_release[5]);
    assert(draw_calls == 1 && draw_arg == (void *)&object && object.unknown_90 == 0);
    assert(seed_calls == 0 && gSzDuel_aaMapInfo[2][6].unit == 5 && clear_calls == 4 && clear_id == -1);
}

/* lSzDuel_InitHandCancel (branch coverage): no current element; current without object; current with
 * object and slot 0 empty / holding an object (released, slot call, four bits cleared);
 * per position a negative word, a word whose low 12 bits are 999, a used word with an
 * empty slot and with an object (bits +0x9f, call arguments); list count 0 and 3. On the
 * host the pointer at +0x6cc and its absolute view are two objects and are set together. */
static void check_place(void)
{
    static struct DotrReleaseCurrent current;
    static struct DotrReleaseObject object, held, a, b;
    int i;

    reset();
    memset(&data_00317490_release, 0, sizeof(data_00317490_release));
    memset(gSzDuel_aCardControl_release, 0, sizeof(gSzDuel_aCardControl_release));
    data_00317b5c_release = 0;
    for (i = 0; i < 5; i++) used[i] = -1;
    used_calls = place_calls = 0;
    count_result = 0;
    data_00317490_release.unknown_d2c[0] = 9;
    assert(lSzDuel_InitHandCancel(4) == 1);
    assert(used_calls == 5 && used_index == 4 && count_arg == 4 && place_calls == 0);
    assert(draw_calls == 0 && slot_calls == 0);
    assert(data_00317490_release.unknown_d4a == 0 && data_00317490_release.unknown_d2c[0] == 9);

    /* current element without an object: nothing released, bits kept */
    memset(&current, 0xff, sizeof(current));
    current.object = 0;
    data_00317490_release.current = data_00317b5c_release = &current;
    gSzDuel_aCardControl_release[0].object = &held;
    assert(lSzDuel_InitHandCancel(4) == 1);
    assert(draw_calls == 0 && slot_calls == 0 && current.unknown_57_bit_7 != 0);

    /* object present, slot 0 empty: bits cleared, no release */
    memset(&object, 0xff, sizeof(object));
    current.object = &object;
    gSzDuel_aCardControl_release[0].object = 0;
    assert(lSzDuel_InitHandCancel(4) == 1);
    assert(draw_calls == 0 && slot_calls == 0);
    assert(current.unknown_57_bit_7 == 0 && current.unknown_58_bit_0 == 0 && current.unknown_58_bit_2 == 0);
    assert(current.unknown_57_bits_0_6 == -1 && current.unknown_58_bit_1 == -1 && current.unknown_58_bits_3_7 == -1);
    assert(object.unknown_83_bit_7 == 0 && object.unknown_83_bits_0_5 == -1 && object.unknown_83_bit_6 == -1);

    /* slot 0 holds an object: released, then the slot call */
    held.unknown_90 = (struct DotrReleaseCurrent *)5;
    gSzDuel_aCardControl_release[0].object = &held;
    used[0] = 0x3e7;           /* 999: skipped */
    used[1] = 0x13e7;          /* low 12 bits 999: skipped */
    used[2] = 0;               /* used, slot 3 empty */
    used[3] = 0x3e6;           /* used, slot 4 has an object */
    used[4] = 1;               /* used, slot 5 has an object */
    memset(&gSzDuel_aCardControl_release[4], 0, sizeof(struct DotrReleaseSlot));
    gSzDuel_aCardControl_release[4].object = &a;
    gSzDuel_aCardControl_release[4].unknown_9f_bit_4 = -1;
    gSzDuel_aCardControl_release[4].unknown_9f_bit_3 = -1;
    gSzDuel_aCardControl_release[5].object = &b;
    gSzDuel_aCardControl_release[1].object = &a;
    gSzDuel_aCardControl_release[2].object = &a;
    count_result = 3;
    assert(lSzDuel_InitHandCancel(7) == 1);
    assert(draw_calls == 1 && draw_arg == &held && held.unknown_90 == 0);
    assert(slot_calls == 1 && slot_arg == &gSzDuel_aCardControl_release[0]);
    assert(place_calls == 2);
    assert(place_slot[0] == &gSzDuel_aCardControl_release[4] && place_slot[1] == &gSzDuel_aCardControl_release[5]);
    assert(place_args[0][0] == 125 && place_args[0][1] == 448 && place_args[0][2] == 125);
    assert(place_args[0][3] == -8 && place_args[0][4] == 128 && place_args[0][5] == 128);
    assert(place_args[1][0] == 250 && place_args[1][2] == 250);
    assert(gSzDuel_aCardControl_release[4].unknown_9f_bit_2 == -1 && gSzDuel_aCardControl_release[4].unknown_9f_bit_4 == 0);
    assert(gSzDuel_aCardControl_release[4].unknown_9f_bit_3 == -1 && gSzDuel_aCardControl_release[4].unknown_9c_bits_0_25 == 0);
    assert(gSzDuel_aCardControl_release[1].unknown_9f_bit_2 == 0 && gSzDuel_aCardControl_release[3].unknown_9f_bit_2 == 0);
    assert(data_00317490_release.unknown_d4a == 3);
    assert(data_00317490_release.unknown_d2c[0] == 1 && data_00317490_release.unknown_d2c[1] == 2);
    assert(data_00317490_release.unknown_d2c[2] == 3 && data_00317490_release.unknown_d2c[3] == 0);
}

struct DotrReleaseObject gSzDuel_aCardRequest[32];
struct DotrReleaseSide data_00317490_sides[2];
static int query_calls, query_args[2], matrix_calls, view_calls, view_args[6];
static void *matrix_arg, *view_slot;
int Ta_GetSummonRev(int a, int id) { query_calls++; query_args[0] = a; query_args[1] = id; return 0x77; }
float func_00205c40_owner(int *out) { matrix_calls++; matrix_arg = out; return 0.0f; }
int lSzDuel_SetCardCtl2(struct DotrReleaseSlot *p, int a, int b, int c, int d, short e, short f)
{
    view_calls++; view_slot = p;
    view_args[0] = a; view_args[1] = b; view_args[2] = c; view_args[3] = d; view_args[4] = e; view_args[5] = f;
    return 0;
}

/* Branch coverage for lSzDuel_InitHandInfo (second == 0) and lSzDuel_InitHandInfo2 (second == 1):
 * no item, id 999, slot index below and above the range, a held object, a free
 * object that is not the first, and no free object. */
static struct DotrReleaseCurrent fill_item;
static int fill_call(int second, int b)
{
    struct DotrReleaseSide *sides = data_00317490_sides;

    sides[1].selectedCol = 0x21;
    sides[1].selectedRow = 0x22;
    item_calls = clear_calls = draw_calls = query_calls = matrix_calls = view_calls = place_calls = 0;
    return second ? lSzDuel_InitHandInfo2(1, b, 44, 10, 20) : lSzDuel_InitHandInfo(1, b, 44, 10, 20, 30, 40);
}
static void fill_common(int b)
{
    assert(item_calls == 1 && item_args[0] == 2 && item_args[1] == b);
    assert(clear_calls == 1 && clear_arg == (void *)&fill_item && clear_id == 44);
    assert(fill_item.team == 1 && fill_item.slot == (unsigned char)b);
    assert(fill_item.locCol == 0x21 && fill_item.locRow == 0x22);
    assert(fill_item.unknown_57_bit_7 == -1 && fill_item.unknown_58_bit_0 == -1 && fill_item.unknown_58_bit_2 == -1);
    assert(fill_item.unknown_58_bit_1 == 0 && fill_item.unknown_57_bits_0_6 == 0);
}
static void check_fill(int second)
{
    static struct DotrReleaseObject held;
    struct DotrReleaseSlot *s = &gSzDuel_aCardControl_release[5];
    struct DotrReleaseObject *o;
    int i, pass;

    memset(&data_00317490_release, 0, sizeof(data_00317490_release));
    memset(gSzDuel_aCardControl_release, 0, sizeof(gSzDuel_aCardControl_release));
    memset(gSzDuel_aCardRequest, 0, sizeof(gSzDuel_aCardRequest));
    item_result = 0;
    assert(fill_call(second, 5) == 0 && item_calls == 1 && clear_calls == 0);

    item_result = (struct DotrStateItem *)&fill_item;
    memset(&fill_item, 0, sizeof(fill_item));
    fill_item.cardNo = 999;
    assert(fill_call(second, 5) == 1);
    fill_common(5);
    assert(query_calls == 0 && draw_calls == 0 && s->object == 0 && fill_item.lvRev == 0);

    for (i = -1; i <= 6; i += 7) {
        memset(&fill_item, 0, sizeof(fill_item));
        fill_item.cardNo = 12;
        assert(fill_call(second, i) == 1);
        fill_common(i);
        assert(query_calls == 1 && query_args[0] == 1 && query_args[1] == 12 && fill_item.lvRev == 0x77);
        assert(draw_calls == 0 && matrix_calls == 0 && place_calls == 0 && view_calls == 0);
    }

    /* pass 0: slot 5 holds an object; pass 1: it takes the second pool object. */
    for (pass = 0; pass < 2; pass++) {
        memset(&fill_item, 0, sizeof(fill_item));
        memset(&held, 0, sizeof(held));
        memset(s, 0, sizeof(*s));
        fill_item.cardNo = 12;
        gSzDuel_aCardRequest[0].unknown_90 = &fill_item;
        s->object = pass ? 0 : &held;
        o = pass ? &gSzDuel_aCardRequest[1] : &held;
        assert(fill_call(second, 5) == 1);
        fill_common(5);
        assert(draw_calls == pass && (!pass || draw_arg == (void *)o));
        assert(s->object == o && fill_item.object == o && o->unknown_90 == &fill_item);
        assert(o->unknown_30 == 0.46875f && o->unknown_34 == 45.0f / 92.0f && o->unknown_38 == 1.0f && o->unknown_3c == 0);
        assert(o->unknown_83_bit_7 == -1 && o->unknown_83_bit_6 == (second ? -1 : 0));
        assert(o->unknown_81_bit_4 == (second ? -1 : 0) && o->unknown_81_bit_6 == (second ? -1 : 0));
        assert(o->unknown_81_bit_5 == -1 && o->unknown_81_bit_7 == 0 && o->unknown_81_bits_0_3 == 0);
        assert(matrix_calls == 1 && matrix_arg == (void *)o->unknown_20);
        assert(s->unknown_9f_bit_2 == (second ? 0 : -1) && s->unknown_9f_bit_3 == -1 && s->unknown_9f_bit_4 == 0);
        assert(s->unknown_9c_bits_0_25 == 5 && fill_item.lvRev == 0x77 && query_calls == 1);
        if (second) {
            assert(view_calls == 1 && place_calls == 0 && view_slot == (void *)s);
            assert(view_args[0] == 10 && view_args[1] == 20 && view_args[2] == 10 && view_args[3] == 20);
            assert(view_args[4] == 128 && view_args[5] == 128);
        } else {
            int *q = place_args[0];
            assert(place_calls == 1 && view_calls == 0 && place_slot[0] == (void *)s);
            assert(q[0] == 10 && q[1] == 20 && q[2] == 30 && q[3] == 40 && q[4] == 0 && q[5] == 128);
        }
    }

    for (i = 0; i < 32; i++) gSzDuel_aCardRequest[i].unknown_90 = &fill_item;
    memset(&fill_item, 0, sizeof(fill_item));
    memset(s, 0, sizeof(*s));
    fill_item.cardNo = 12;
    assert(fill_call(second, 5) == 1);
    fill_common(5);
    assert(draw_calls == 0 && s->object == 0 && fill_item.object == 0 && matrix_calls == 0);
    assert(place_calls == 0 && view_calls == 0 && fill_item.lvRev == 0x77);
}

static int get_calls, put_calls, get_index, put_index;
static struct DotrStateVector put_value;
int SzModel_GetPos(int index, struct DotrStateVector *out)
{
    get_calls++; get_index = index;
    out->x = 1.0f; out->y = 2.0f; out->z = 3.0f; out->unknown_w = 4.0f;
    return 0;
}
int SzModel_SetPos(int index, struct DotrStateVector *in) { put_calls++; put_index = index; put_value = *in; return 0; }

/* SzDuel_MoveUnit (branch coverage): missing item, byte +0x45 at 11; item already at (x, y);
 * x at -1 / 7, y at -1 / 7, the item's own cell out of range; target cell empty with
 * and without an item object; target cell holding an item that is given back through
 * the cell path (first byte 1) and the slot path (first byte 2), and one whose own
 * lookup fails. The inlined give-back's branches are those of SzDuel_DelUnit above. */
static void check_move(void)
{
    static struct DotrStateItem item, other;
    static struct DotrStateItemObject position;
    static struct DotrReleaseObject object;

    memset(&item, 0, sizeof(item));
    memset(&other, 0, sizeof(other));
    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
    memset(gSzDuel_aCardControl_release, 0, sizeof(gSzDuel_aCardControl_release));
    reset();
    get_calls = put_calls = 0;
    item_second = 0;
    item_result = 0;
    assert(SzDuel_MoveUnit(1, 4, 2, 3) == 0 && item_calls == 1 && item_args[0] == 1 && item_args[1] == 4);
    item_result = &item;
    item.slot = 11;
    assert(SzDuel_MoveUnit(1, 4, 2, 3) == 0);
    item.slot = 10;
    item.locCol = 2;
    item.locRow = 3;
    assert(SzDuel_MoveUnit(1, 4, 2, 3) == 0);
    assert(SzDuel_MoveUnit(1, 4, -1, 3) == 0 && SzDuel_MoveUnit(1, 4, 7, 3) == 0);
    assert(SzDuel_MoveUnit(1, 4, 2, -1) == 0 && SzDuel_MoveUnit(1, 4, 2, 7) == 0);
    item.locCol = 7;
    assert(SzDuel_MoveUnit(1, 4, 6, 0) == 0);
    item.locRow = 7;
    item.locCol = 2;
    assert(SzDuel_MoveUnit(1, 4, 6, 0) == 0 && get_calls == 0 && put_calls == 0);

    /* empty target, no object */
    item.locRow = 3;
    item.modelId = 0x92;
    item.cardModel = 0;
    memset(item.unknown_34, 0xff, sizeof(item.unknown_34));
    gSzDuel_aaMapInfo[3][2].unit = 9;
    reset();
    assert(SzDuel_MoveUnit(1, 4, 6, 0) == &item && item_calls == 1 && clear_calls == 0);
    assert(item.locCol == 6 && item.locRow == 0);
    assert(item.unknown_34[0] == 0 && item.unknown_34[1] == 0 && item.unknown_34[2] == 0 && item.unknown_34[3] == 0);
    assert(gSzDuel_aaMapInfo[3][2].unit == 0 && gSzDuel_aaMapInfo[0][6].unit == (DotrStateWord)&item);
    assert(get_calls == 1 && get_index == 0x92 && put_calls == 1 && put_index == 0x92);
    assert(put_value.x == 150.0f && put_value.y == 2.0f && put_value.z == -150.0f && put_value.unknown_w == 4.0f);

    /* with an object: y cleared, four floats copied */
    item.cardModel = &position;
    memset(&position, 0, sizeof(position));
    reset();
    assert(SzDuel_MoveUnit(1, 4, 3, 4) == &item);
    assert(put_value.x == 0.0f && put_value.y == 2.0f && put_value.z == 50.0f);
    assert(position.unknown_00.x == 0.0f && position.unknown_00.y == 0.0f);
    assert(position.unknown_00.z == 50.0f && position.unknown_00.unknown_w == 4.0f);
    assert(gSzDuel_aaMapInfo[0][6].unit == 0 && gSzDuel_aaMapInfo[4][3].unit == (DotrStateWord)&item);

    /* target holds another item, given back through the cell path */
    item.cardModel = 0;
    item_second = &other;
    other.team = 1;
    other.slot = 5;
    other.modelId = 0x93;
    other.locCol = 1;
    other.locRow = 1;
    other.cardModel = (struct DotrStateItemObject *)&object;
    object.unknown_90 = (struct DotrReleaseCurrent *)7;
    gSzDuel_aaMapInfo[1][1].unit = (DotrStateWord)&other;
    reset();
    assert(SzDuel_MoveUnit(1, 4, 1, 1) == &item && item_calls == 2 && item_args[0] == 1 && item_args[1] == 5);
    assert(seed_calls == 1 && seed_args[0] == 0x93 && seed_args[1] == -1 && seed_args[2] == 999);
    assert(draw_calls == 1 && draw_arg == (void *)&object && object.unknown_90 == 0);
    assert(clear_calls == 1 && clear_arg == (void *)&other && clear_id == -1 && slot_calls == 0);
    assert(gSzDuel_aaMapInfo[4][3].unit == 0 && gSzDuel_aaMapInfo[1][1].unit == (DotrStateWord)&item);

    /* slot path: first byte 2, second byte 0 (no slot) and 5 (slot with an object) */
    other.team = 2;
    other.slot = 0;
    gSzDuel_aaMapInfo[2][2].unit = (DotrStateWord)&other;
    reset();
    assert(SzDuel_MoveUnit(1, 4, 2, 2) == &item && slot_calls == 0 && seed_calls == 0 && clear_calls == 1);
    other.slot = 5;
    gSzDuel_aCardControl_release[5].object = &object;
    object.unknown_90 = (struct DotrReleaseCurrent *)7;
    gSzDuel_aaMapInfo[3][3].unit = (DotrStateWord)&other;
    reset();
    assert(SzDuel_MoveUnit(1, 4, 3, 3) == &item && item_args[0] == 2 && item_args[1] == 5);
    assert(slot_calls == 1 && slot_arg == (void *)&gSzDuel_aCardControl_release[5]);
    assert(draw_calls == 1 && object.unknown_90 == 0 && clear_calls == 1 && clear_arg == (void *)&other);

    /* the other item's own lookup fails: nothing given back, the move still happens */
    other.slot = 11;
    gSzDuel_aaMapInfo[4][4].unit = (DotrStateWord)&other;
    reset();
    assert(SzDuel_MoveUnit(1, 4, 4, 4) == &item && item_calls == 2 && clear_calls == 0 && slot_calls == 0);
    assert(gSzDuel_aaMapInfo[4][4].unit == (DotrStateWord)&item && item.locCol == 4);
    item_second = 0;
}

struct DotrStateItem gSzDuel_aaVarInfo[3][11];
struct DotrStateEntry8 EffIdx_L[854][5];
unsigned short AlternateCardIds[854];
struct DotrStateVector data_002915d0;
struct DotrReleaseAngles data_002915e0;
struct DotrStateVector data_002915f0;
static int calc_calls, calc_args[3], look_calls, look_args[3], model_calls, model_args[6][2];
static int request_calls;
static void *calc_arg, *request_cell;
static DotrStateWord request_unit;
static float speed_value;
static struct DotrReleaseAngles rot_value;
static struct DotrStateVector rotc_value;
static void *rgba_arg, *filter_arg;
static int filter_args[2];
void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrStateVector *v)
{
    calc_calls++; calc_args[0] = x; calc_args[1] = y; calc_args[2] = z; calc_arg = v;
}
int SzData_SetLookFlag(int side, int id, int flag)
{
    look_calls++; look_args[0] = side; look_args[1] = id; look_args[2] = flag; return 0;
}
static int model_log(int which, int index, int value)
{
    assert(model_calls == which);
    model_args[which][0] = index; model_args[which][1] = value; model_calls++;
    return 0;
}
int SzModel_SetAnimDirect(int index, int slot) { return model_log(0, index, slot); }
int SzModel_SetAnimDir(int index, int value) { return model_log(1, index, value); }
int SzModel_SetAnimSpeed(int index, float value) { speed_value = value; return model_log(2, index, 0); }
int SzModel_SetRot(int index, struct DotrReleaseAngles *in) { rot_value = *in; return model_log(3, index, 0); }
int SzModel_SetRotC(int index, struct DotrStateVector *in) { rotc_value = *in; return model_log(4, index, 0); }
int SzModel_SetRgba(int index, int value, void *in) { rgba_arg = in; return model_log(5, index, value); }
int SzModel_SetFilter(int index, int value, void *in)
{
    assert(model_calls == 6);
    filter_arg = in; filter_args[0] = index; filter_args[1] = value;
    return 0;
}
void lSzDuel_DoUnitRequest(struct DotrStateCell *cell, DotrStateWord unknown)
{
    request_calls++; request_cell = cell; request_unit = unknown;
}

static struct DotrStateItem add_src;
static void add_reset(int id)
{
    static const struct DotrStateVector pos = { 1.0f, 2.0f, 3.0f, 4.0f };
    static const struct DotrReleaseAngles rot = { 5, 6, 7, 8 };
    static const struct DotrStateVector rotc = { 9.0f, 10.0f, 11.0f, 12.0f };
    int side, n;

    reset();
    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    memset(gSzDuel_aCardRequest, 0, sizeof(gSzDuel_aCardRequest));
    memset(data_00317490_sides, 0, sizeof(data_00317490_sides));
    memset(AlternateCardIds, 0xff, sizeof(AlternateCardIds));
    memset(&add_src, 0, sizeof(add_src));
    for (side = 0; side < 3; side++) {
        for (n = 0; n < 11; n++) {
            gSzDuel_aaVarInfo[side][n].slot = 11;
        }
    }
    data_002915d0 = pos;
    data_002915e0 = rot;
    data_002915f0 = rotc;
    add_src.cardNo = id;
    add_src.unknown_30 = 0x1234;
    add_src.apBoost = 77;
    seed_calls = draw_calls = query_calls = put_calls = 0;
    calc_calls = look_calls = model_calls = request_calls = 0;
}

/* SzDuel_AddUnit2 (branch coverage): null source; x and y at -1 / 7; occupied cell; side -1 / 2;
 * id 671 (read as 999) and 854; leader with id 683 (refused) and 682 (slot 0, pairs,
 * model side + 30, index found in the table); ordinary unit with id 682 on side 1 with
 * slot 1 taken (slot 2, second free request, model 26, index absent) and with all five
 * slots taken; id 683 on side 0 (slot 6, no model) with an object already present and
 * with none and no free request. The result is compared as the low 32 bits of the
 * unit's address. The 23-word copy covers 92 bytes of the host layout, so
 * only members inside that span are asserted. */
static void check_add(void)
{
    struct DotrStateCell *c = &gSzDuel_aaMapInfo[4][2];
    struct DotrStateItem *unit;
    int result;
    static struct DotrStateItemObject own;
    int n;

    add_reset(100);
    assert(SzDuel_AddUnit2(0, 2, 4, 0, 0) == 0);
    assert(SzDuel_AddUnit2(0, -1, 4, (struct DotrStateBlock *)&add_src, 0) == 0 && SzDuel_AddUnit2(0, 7, 4, (struct DotrStateBlock *)&add_src, 0) == 0);
    assert(SzDuel_AddUnit2(0, 2, -1, (struct DotrStateBlock *)&add_src, 0) == 0 && SzDuel_AddUnit2(0, 2, 7, (struct DotrStateBlock *)&add_src, 0) == 0);
    c->unit = 1;
    assert(SzDuel_AddUnit2(0, 2, 4, (struct DotrStateBlock *)&add_src, 0) == 0);
    c->unit = 0;
    assert(SzDuel_AddUnit2(-1, 2, 4, (struct DotrStateBlock *)&add_src, 0) == 0 && SzDuel_AddUnit2(2, 2, 4, (struct DotrStateBlock *)&add_src, 0) == 0);
    add_src.cardNo = 671;
    assert(SzDuel_AddUnit2(0, 2, 4, (struct DotrStateBlock *)&add_src, 0) == 0);
    add_src.cardNo = 854;
    assert(SzDuel_AddUnit2(0, 2, 4, (struct DotrStateBlock *)&add_src, 0) == 0);
    add_src.cardNo = 683;
    assert(SzDuel_AddUnit2(0, 2, 4, (struct DotrStateBlock *)&add_src, 1) == 0);
    assert(query_calls == 0 && c->unit == 0);

    /* Leader. */
    add_reset(682);
    AlternateCardIds[40] = 682;
    ((struct DotrReleasePair *)EffIdx_L[682])[0].unknown_00 = 1.5f;
    ((struct DotrReleasePair *)EffIdx_L[682])[4].unknown_04 = 2.5f;
    result = SzDuel_AddUnit2(0, 2, 4, (struct DotrStateBlock *)&add_src, 1);
    unit = &gSzDuel_aaVarInfo[0][0];
    assert(result == (int)(DotrStateWord)unit && c->unit == (DotrStateWord)unit);
    assert(unit->cardNo == 682 && unit->unknown_30 == 0x1234 && unit->apBoost == 77);
    assert(query_calls == 1 && query_args[0] == 0 && query_args[1] == 682 && unit->lvRev == 0x77);
    assert(unit->locCol == 2 && unit->locRow == 4 && unit->team == 0 && unit->slot == 0);
    assert(((struct DotrReleasePair *)unit->cardEffects)[0].unknown_00 == 1.5f);
    assert(((struct DotrReleasePair *)unit->cardEffects)[4].unknown_04 == 2.5f);
    assert(calc_calls == 0 && look_calls == 0 && draw_calls == 0);
    assert(data_00317490_sides[0].numberOfUnitsSummoned == 0 && unit->cardModel == 0);
    assert(unit->modelId == 30);
    assert(seed_calls == 1 && seed_args[0] == 30 && seed_args[1] == -1 && seed_args[2] == 40);
    for (n = 0; n < 6; n++) {
        assert(model_args[n][0] == 30);
    }
    assert(model_args[0][1] == 0 && model_args[1][1] == 1 && speed_value == 1.0f);
    assert(put_calls == 1 && put_index == 30);
    assert(put_value.x == -50.0f && put_value.y == 2.0f && put_value.z == 50.0f && put_value.unknown_w == 4.0f);
    assert(rot_value.x == 5 && rot_value.y == 0x8000 && rot_value.z == 7 && rot_value.w == 8);
    assert(rotc_value.x == 9.0f && rotc_value.unknown_w == 12.0f);
    assert(model_args[5][1] == 0 && rgba_arg == 0 && filter_arg == 0);
    assert(filter_args[0] == 30 && filter_args[1] == 1);
    assert(request_calls == 1 && request_cell == c && request_unit == (DotrStateWord)unit);
    assert(data_002915e0.y == 6);

    /* Ordinary unit, side 1, slot 1 taken, first request busy. */
    add_reset(682);
    gSzDuel_aaVarInfo[1][1].slot = 10;
    gSzDuel_aCardRequest[0].unknown_90 = (struct DotrReleaseCurrent *)&add_src;
    data_00317490_sides[1].numberOfUnitsSummoned = 9;
    result = SzDuel_AddUnit2(1, 2, 4, (struct DotrStateBlock *)&add_src, 0);
    unit = &gSzDuel_aaVarInfo[1][2];
    assert(result == (int)(DotrStateWord)unit && unit->slot == 2 && unit->team == 1);
    assert(gSzDuel_aaVarInfo[1][1].slot == 10);
    assert(draw_calls == 1 && draw_arg == &gSzDuel_aCardRequest[1]);
    assert((void *)unit->cardModel == (void *)&gSzDuel_aCardRequest[1]);
    assert((void *)gSzDuel_aCardRequest[1].unknown_90 == (void *)unit);
    assert(calc_calls == 1 && calc_args[0] == 2 && calc_args[1] == 4 && calc_args[2] == 0);
    assert(calc_arg == &gSzDuel_aCardRequest[1]);
    assert(unit->summonOrder == 9 && data_00317490_sides[1].numberOfUnitsSummoned == 10);
    assert(look_calls == 1 && look_args[0] == 1 && look_args[1] == 682 && look_args[2] == 1);
    assert(unit->modelId == 26);
    assert(seed_calls == 1 && seed_args[0] == 26 && seed_args[2] == 999);
    assert(rot_value.y == 0 && request_calls == 1 && filter_args[0] == 26);
    assert(unit->cardEffects[0].unknown_words[0] == 0);

    /* All five slots taken. */
    add_reset(682);
    for (n = 1; n < 6; n++) {
        gSzDuel_aaVarInfo[0][n].slot = 10;
    }
    assert(SzDuel_AddUnit2(0, 2, 4, (struct DotrStateBlock *)&add_src, 0) == 0 && query_calls == 0 && c->unit == 0);

    /* Id 683 and up: slots 6..10, no model. The source's own object is kept. */
    add_reset(683);
    add_src.cardModel = &own;
    result = SzDuel_AddUnit2(0, 2, 4, (struct DotrStateBlock *)&add_src, 0);
    unit = &gSzDuel_aaVarInfo[0][6];
    assert(result == (int)(DotrStateWord)unit && unit->slot == 6);
    assert(draw_calls == 0 && unit->cardModel == &own);
    assert(calc_calls == 1 && calc_arg == &own && look_calls == 1);
    assert(seed_calls == 0 && put_calls == 0 && request_calls == 0);

    /* No object and no free request. */
    add_reset(683);
    for (n = 0; n < 32; n++) {
        gSzDuel_aCardRequest[n].unknown_90 = (struct DotrReleaseCurrent *)&add_src;
    }
    result = SzDuel_AddUnit2(0, 2, 4, (struct DotrStateBlock *)&add_src, 0);
    unit = &gSzDuel_aaVarInfo[0][6];
    assert(result == (int)(DotrStateWord)unit && unit->cardModel == 0);
    assert(draw_calls == 0 && calc_calls == 0 && look_calls == 1 && unit->summonOrder == 0);
}

static int point_calls, control_calls;
static void *point_out[12], *control_arg[6];
static float point_x[12], point_y[12];
void Ex3d_GetPointOnScreen(struct DotrStateVector *out, float x, float y)
{
    point_out[point_calls] = out;
    point_x[point_calls] = x;
    point_y[point_calls] = y;
    point_calls++;
}
int SzDuel_DoCardControl0(struct DotrReleaseSlot *c) { control_arg[control_calls++] = c; return 0; }

static struct DotrReleaseCurrent combo_current, combo_item;
static struct DotrReleaseObject combo_object;
static void combo_reset(void)
{
    int n;

    reset();
    memset(&data_00317490_release, 0, sizeof(data_00317490_release));
    memset(gSzDuel_aCardControl_release, 0, sizeof(gSzDuel_aCardControl_release));
    memset(gSzDuel_aCardRequest, 0, sizeof(gSzDuel_aCardRequest));
    memset(&combo_current, 0, sizeof(combo_current));
    memset(&combo_item, 0x11, sizeof(combo_item));
    memset(&combo_object, 0, sizeof(combo_object));
    memset(used, 0, sizeof(used));
    for (n = 0; n < 30; n++) {
        data_00317490_release.unknown_d2c[n] = 0x55;
    }
    for (n = 1; n < 6; n++) {
        gSzDuel_aCardControl_release[n].unknown_9c_bits_0_25 = 10 + n;
        gSzDuel_aCardControl_release[n].unknown_9f_bit_2 = -1;
        gSzDuel_aCardControl_release[n].unknown_30.x = 9.0f;
        gSzDuel_aCardControl_release[n].unknown_40[2] = 9;
    }
    item_result = 0;
    item_second = 0;
    used_calls = point_calls = control_calls = matrix_calls = 0;
}

/* The placed slot `n` at call pair `at`: both points, the cleared words and the control call. */
static void combo_placed(int n, int at, float x, float tx, float y, int bit4)
{
    struct DotrReleaseSlot *c = &gSzDuel_aCardControl_release[n];

    assert(point_out[at * 2] == &c->unknown_10 && point_x[at * 2] == x && point_y[at * 2] == -8.0f);
    assert(point_out[at * 2 + 1] == &c->unknown_20 && point_x[at * 2 + 1] == tx && point_y[at * 2 + 1] == y);
    assert(c->unknown_30.x == 0 && c->unknown_30.y == 0 && c->unknown_30.z == 0 && c->unknown_30.unknown_w == 0);
    assert(c->unknown_40[0] == 0 && c->unknown_40[1] == 0x10000 && c->unknown_40[2] == 0 && c->unknown_40[3] == 0);
    assert(c->unknown_98 == 128 && c->unknown_9a == 0);
    assert(c->unknown_9f_bit_2 == 0 && c->unknown_9f_bit_4 == bit4 && c->unknown_9c_bits_0_25 == 10 + n);
    assert(control_arg[at] == c);
}

/* lSzDuel_InitHandCombo (branch coverage): no current item, a current item without an object, no
 * item (2, 0), no free request, the full first block; a slot without an object, hand words
 * negative, 999 and usable; card numbers 0, 1, 2 and 3; mode byte 1 and not 1; the byte at
 * +0xd4e at 0, 1, 2 and 3; the position byte at +0xd4f matching and not. */
static void check_combo(void)
{
    struct DotrReleaseControl *ctl = &data_00317490_release;
    struct DotrReleaseSlot *s = &gSzDuel_aCardControl_release[0];
    struct DotrReleaseObject *o;
    float *f;
    int n;

    /* No current item: only the hand. Position 3 is the marked one (number forced to 1). */
    combo_reset();
    ctl->unknown_d4f = 3;
    for (n = 2; n < 6; n++) {
        gSzDuel_aCardControl_release[n].object = &combo_object;
    }
    used[0] = 5;
    used[1] = -1;
    used[2] = 999;
    used[3] = 5;
    used[4] = 0x3007;
    assert(lSzDuel_InitHandCombo(1) == 1);
    assert(item_calls == 0 && used_calls == 4 && used_index == 1);
    assert(point_calls == 4 && control_calls == 2 && matrix_calls == 0);
    combo_placed(4, 0, 125.0f, -125.0f, -8.0f, -1);
    combo_placed(5, 1, 250.0f, 141.0f, -8.0f, -1);
    assert(ctl->unknown_d2c[0] == 6 && ctl->unknown_d2c[1] == 14 && ctl->unknown_d2c[2] == 0x55);
    assert(ctl->unknown_d2c[3] == 15 && ctl->unknown_d2c[4] == 0x55);
    assert(gSzDuel_aCardControl_release[1].unknown_9f_bit_2 == -1 && gSzDuel_aCardControl_release[2].unknown_9f_bit_2 == -1);
    assert(gSzDuel_aCardControl_release[3].unknown_9f_bit_2 == -1);

    /* Numbers 0 take the places after max(+0xd4e, 1): +0xd4e at 0, 1, 2 and 3. */
    for (n = 0; n < 4; n++) {
        combo_reset();
        ctl->unknown_d4e = n;
        ctl->unknown_d4f = 9;
        gSzDuel_aCardControl_release[1].object = &combo_object;
        gSzDuel_aCardControl_release[3].object = &combo_object;
        used[0] = 5;
        used[2] = 0x0fff & 998;
        assert(lSzDuel_InitHandCombo(0) == 1);
        assert(used_calls == 2 && used_index == 0 && control_calls == 2);
        combo_placed(1, 0, -250.0f, -250.0f, 448.0f, 0);
        combo_placed(3, 1, 0.0f, 0.0f, 448.0f, 0);
        assert(ctl->unknown_d2c[(n > 1 ? n : 1) + 1] == 11 && ctl->unknown_d2c[(n > 1 ? n : 1) + 2] == 13);
        assert(ctl->unknown_d2c[(n > 1 ? n : 1) + 3] == 0x55 && ctl->unknown_d2c[0] == 6);
    }

    /* Mode byte 1: number 1 goes to (0, -8) with the flag, any other number stays. The
     * marked position counts only while +0xd4e is 0. */
    combo_reset();
    ctl->unknown_d4a = 1;
    ctl->unknown_d4e = 2;
    ctl->unknown_d4f = 2;
    for (n = 1; n < 6; n++) {
        gSzDuel_aCardControl_release[n].object = &combo_object;
    }
    used[0] = 0x1005;
    used[1] = 0x2005;
    used[2] = 5;
    used[3] = 999;
    used[4] = -5;
    assert(lSzDuel_InitHandCombo(1) == 1);
    assert(used_calls == 5 && control_calls == 3);
    combo_placed(1, 0, -250.0f, 0.0f, -8.0f, -1);
    combo_placed(2, 1, -125.0f, -125.0f, 448.0f, 0);
    combo_placed(3, 2, 0.0f, 0.0f, 448.0f, 0);
    assert(ctl->unknown_d2c[1] == 11 && ctl->unknown_d2c[2] == 12 && ctl->unknown_d2c[3] == 13);

    /* A current item without an object: nothing but the hand (empty here). */
    combo_reset();
    ctl->current = &combo_current;
    assert(lSzDuel_InitHandCombo(0) == 1);
    assert(item_calls == 0 && used_calls == 0 && ctl->unknown_d2c[0] == 6);
    assert(combo_current.unknown_58_bit_0 == 0 && combo_current.unknown_57_bit_7 == 0);

    /* No item (2, 0): only the flags of the current item and its object. */
    combo_reset();
    ctl->current = &combo_current;
    combo_current.object = &combo_object;
    assert(lSzDuel_InitHandCombo(0) == 1);
    assert(item_calls == 1 && item_args[0] == 2 && item_args[1] == 0 && draw_calls == 0);
    assert(combo_current.unknown_58_bit_0 == -1 && combo_current.unknown_57_bit_7 == -1);
    assert(combo_current.unknown_58_bit_2 == -1 && combo_current.unknown_58_bit_1 == 0);
    assert(combo_object.unknown_83_bit_7 == -1 && ctl->unknown_d2c[0] == 6 && control_calls == 0);

    /* No free request: the item is copied, slot 0 gets no object and the list keeps its 6. */
    combo_reset();
    ctl->current = &combo_current;
    combo_current.object = &combo_object;
    combo_current.cardNo = 123;
    combo_current.ctlRateM = 7;
    item_result = (struct DotrStateItem *)&combo_item;
    s->object = &combo_object;
    for (n = 0; n < 32; n++) {
        gSzDuel_aCardRequest[n].unknown_90 = &combo_current;
    }
    assert(lSzDuel_InitHandCombo(0) == 1);
    assert(combo_item.cardNo == 123 && combo_item.ctlRateM == 0 && combo_item.object == &combo_object);
    assert(s->object == 0 && draw_calls == 0 && control_calls == 0 && ctl->unknown_d2c[0] == 6);
    assert(combo_current.unknown_58_bit_0 == -1 && combo_current.ctlRateM == 7);

    /* The full first block: request 2 is the first free one. With a current item a hand
     * card numbered 2 goes to 141; number 1 to 125. */
    combo_reset();
    ctl->current = &combo_current;
    combo_current.object = &combo_object;
    combo_current.cardNo = 321;
    item_result = (struct DotrStateItem *)&combo_item;
    gSzDuel_aCardRequest[0].unknown_90 = &combo_current;
    gSzDuel_aCardRequest[1].unknown_90 = &combo_current;
    f = (float *)combo_object.unknown_00;
    for (n = 0; n < 4; n++) {
        f[n] = 1.0f + n;
        ((float *)combo_object.unknown_10)[n] = 5.0f + n;
        combo_object.unknown_20[n] = 70 + n;
    }
    combo_object.unknown_30 = 9.0f;
    combo_object.unknown_34 = 10.0f;
    combo_object.unknown_38 = 11.0f;
    combo_object.unknown_3c = 0x41400000;
    combo_object.unknown_82 = 0x5a;
    gSzDuel_aCardControl_release[1].object = &combo_object;
    gSzDuel_aCardControl_release[2].object = &combo_object;
    used[0] = 0x2005;
    used[1] = 0x1005;
    s->unknown_9c_bits_0_25 = 77;
    ctl->unknown_d4f = 9;
    assert(lSzDuel_InitHandCombo(1) == 1);
    o = &gSzDuel_aCardRequest[2];
    assert(draw_calls == 1 && draw_arg == o && s->object == o);
    assert(combo_item.cardNo == 321 && combo_item.object == o && o->unknown_90 == &combo_item);
    assert(memcmp(o, &combo_object, 0x40) == 0 && o->unknown_82 == 0x5a);
    assert(o->unknown_40[0] == 128 && o->unknown_40[1] == 128 && o->unknown_40[2] == 128 && o->unknown_40[3] == 128);
    assert(o->unknown_83_bit_7 == -1 && o->unknown_81_bit_5 == -1 && o->unknown_81_bit_4 == 0);
    assert(s->unknown_10.x == 1.0f && s->unknown_10.y == 2.0f && s->unknown_10.z == 3.0f && s->unknown_10.unknown_w == 4.0f);
    assert(point_out[0] == &s->unknown_20 && point_x[0] == -125.0f && point_y[0] == -8.0f);
    assert(s->unknown_30.x == 5.0f && s->unknown_30.y == 6.0f && s->unknown_30.z == 7.0f && s->unknown_30.unknown_w == 8.0f);
    assert(s->unknown_40[0] == 0 && s->unknown_40[1] == 0x10000 && s->unknown_40[2] == 0x10000 && s->unknown_40[3] == 0);
    assert(s->unknown_50[0] == 0 && s->unknown_50[1] == 0 && s->unknown_50[2] == 0 && s->unknown_50[3] == 0);
    assert(matrix_calls == 1 && matrix_arg == s->unknown_60);
    assert(s->unknown_70.x == 9.0f && s->unknown_70.y == 10.0f && s->unknown_70.z == 11.0f && s->unknown_70.unknown_w == 12.0f);
    assert(s->unknown_80 == 0.46875f && s->unknown_84 == 45.0f / 92.0f && s->unknown_88 == 1.0f && s->unknown_8c == 0);
    assert(s->unknown_98 == 128 && s->unknown_9a == 0 && s->unknown_9c_bits_0_25 == 0);
    assert(s->unknown_9f_bit_2 == 0 && s->unknown_9f_bit_3 == -1 && s->unknown_9f_bit_4 == -1);
    assert(s->unknown_9f_bit_5 == -1 && s->unknown_9f_bit_6 == -1 && s->unknown_9f_bit_7 == 0);
    assert(control_calls == 3 && control_arg[0] == s && point_calls == 5);
    assert(combo_current.unknown_58_bit_0 == -1 && combo_current.unknown_57_bit_7 == -1 && combo_current.unknown_58_bit_2 == -1);
    assert(combo_object.unknown_83_bit_7 == -1);
    n = point_calls;
    point_out[0] = point_out[1]; point_x[0] = point_x[1]; point_y[0] = point_y[1];
    point_out[1] = point_out[2]; point_x[1] = point_x[2]; point_y[1] = point_y[2];
    control_arg[0] = control_arg[1];
    combo_placed(1, 0, -250.0f, 141.0f, -8.0f, -1);
    assert(point_out[3] == &gSzDuel_aCardControl_release[2].unknown_10 && point_x[3] == -125.0f && point_y[3] == -8.0f);
    assert(point_out[4] == &gSzDuel_aCardControl_release[2].unknown_20 && point_x[4] == 125.0f && point_y[4] == -8.0f);
    assert(control_arg[2] == &gSzDuel_aCardControl_release[2] && gSzDuel_aCardControl_release[2].unknown_9f_bit_4 == -1);
    assert(ctl->unknown_d2c[0] == 0 && ctl->unknown_d2c[1] == 12 && ctl->unknown_d2c[2] == 11 && ctl->unknown_d2c[3] == 0x55);
    (void)n;
}

/* lSzDuel_InitHandDark (branch and loop coverage): menu bit set and clear, each side of the player bounds
 * 32 and 100, the pad answer with and without a low-halfword bit, flag 0, an empty slot, the
 * first and last slot, and the power below, at and above the level. */
static int dark_menu, dark_players[2], dark_pad, dark_power, dark_calls, dark_bad;
static int dark_levels[6];
static struct DotrReleaseCurrent dark_items[6];
int SzGame_GetMenuNum(int unknown)
{
    dark_bad |= unknown != 29;
    return dark_menu;
}
int SzDuel_GetPlayer(int index)
{
    return dark_players[index];
}
int SzPad_Check(int unknown, int zero, int mask)
{
    dark_bad |= zero != 1 || mask != 2;
    dark_calls += 100 + unknown;
    return dark_pad;
}
int SzDuel_GetSummonPower(int index)
{
    dark_calls += 1000 * (index + 1);
    return dark_power;
}
int SzDuel_GetUnitLevel2(struct DotrStateItem *item, int flag)
{
    dark_bad |= flag != 0;
    return dark_levels[(struct DotrReleaseCurrent *)item - dark_items];
}
/* Returns a bit per slot whose bytes are 64; asserts every object has 64 or 128 three times. */
static int run_dark(int side, int flag)
{
    static struct DotrReleaseObject objects[6];
    int i, bits = 0;

    memset(gSzDuel_aCardControl_release, 0, sizeof(gSzDuel_aCardControl_release));
    memset(objects, 0, sizeof(objects));
    for (i = 0; i < 6; i++) {
        if (i != 2) {
            gSzDuel_aCardControl_release[i].object = &objects[i];
        }
        objects[i].unknown_90 = &dark_items[i];
        objects[i].unknown_40[3] = 7;
    }
    dark_calls = dark_bad = 0;
    assert(lSzDuel_InitHandDark(side, flag) == 1 && !dark_bad);
    for (i = 0; i < 6; i++) {
        unsigned char *c = objects[i].unknown_40;

        if (i == 2) {
            assert(c[0] == 0 && c[1] == 0 && c[2] == 0);
            continue;
        }
        assert((c[0] == 64 || c[0] == 128) && c[1] == c[0] && c[2] == c[0] && c[3] == 7);
        bits |= (c[0] == 64) << i;
    }
    return bits;
}
static void check_hand_dark(void)
{
    /* power 5: levels above it are slots 0 and 5; slot 1 is equal, slot 3 below */
    static const int levels[6] = { 6, 5, 9, 4, 0, 100 };

    memcpy(dark_levels, levels, sizeof(levels));
    dark_power = 5;
    /* bit clear, pad answers: the flag stays; five live slots ask the power of the side */
    dark_menu = 2; dark_pad = 0x10001;
    assert(run_dark(0, 1) == 0x21 && dark_calls == 100 + 5 * 1000);
    /* only a high-halfword bit: dropped */
    dark_pad = 0x10000;
    assert(run_dark(0, 1) == 0 && dark_calls == 100 + 5 * 1000);
    /* bit set for side 1: the pad is not asked; players 31 / 100 drop the flag */
    dark_players[1] = 31; dark_players[0] = 100;
    assert(run_dark(1, 1) == 0 && dark_calls == 5 * 2000);
    dark_players[0] = 99;
    assert(run_dark(1, 1) == 0x21);
    dark_players[1] = 32; dark_players[0] = 100;
    assert(run_dark(1, 1) == 0x21);
    /* flag 0 stays 0 whatever the tests say */
    assert(run_dark(1, 0) == 0);
    dark_menu = 0; dark_pad = 1;
    assert(run_dark(1, 0) == 0 && dark_calls == 101 + 5 * 2000);
}

int main(void)
{
    check_release();
    check_move();
    check_place();
    check_fill(0);
    check_fill(1);
    check_add();
    check_combo();
    check_hand_dark();
    puts("item_release: ok");
    return 0;
}
