/* Synthetic host tests for the item_gates group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/item_gates.h"

struct DotrStateControl data_00317490_control[1];
static struct DotrStateCell cell_store, *cell_result;
static int cell_args[2], bits_result, counts[8], pad_result, pad_args[3], pad_calls;
static int query_result, query_calls, query_side, pair_result, pair_args[2];
static struct DotrStateItem *query_item;

struct DotrStateCell *SzDuel_GetMapInfo(int x, int y) { cell_args[0] = x; cell_args[1] = y; return cell_result; }
int SzGame_GetMenuNum(int unknown) { assert(unknown == 29); return bits_result; }
int SzDuel_GetPlayer(int index) { assert(index >= 0 && index < 8); return counts[index]; }
int SzPad_Check(int unknown, int zero, int mask)
{
    pad_calls++; pad_args[0] = unknown; pad_args[1] = zero; pad_args[2] = mask;
    return pad_result;
}
int Ta_IsUnitHide(struct DotrStateItem *p, int side) { query_calls++; query_item = p; query_side = side; return query_result; }
int func_00250360_pair(int a, int b) { pair_args[0] = a; pair_args[1] = b; return pair_result; }

/* lSzDuel_IsUnitHide (branch coverage): null item; item side byte 2 / 1; cell missing, owned by
 * another item, owned by the item; flag bit set / clear; each count at its bound
 * (31 / 32, 99 / 100); pad word zero / nonzero / nonzero only above bit 15; query
 * result nonzero; item bit 4 of +0x57 set; pair result equal / different. */
static void check_gate(void)
{
    struct DotrStateControl *c = data_00317490_control;
    static struct DotrStateItem item, stranger;

    memset(&item, 0, sizeof(item));
    assert(lSzDuel_IsUnitHide(0) == 0);

    item.locCol = 3; item.locRow = 4; item.team = 2; item.slot = 9;
    c->unknown_d3c = 1;
    cell_result = 0; bits_result = 0; pad_result = 0;
    assert(lSzDuel_IsUnitHide(&item) == 0);
    assert(cell_args[0] == 3 && cell_args[1] == 4);

    /* no cell, flag clear: the pad decides */
    item.team = 1;
    pad_calls = 0;
    assert(lSzDuel_IsUnitHide(&item) == 1);
    assert(pad_calls == 1 && pad_args[0] == 1 && pad_args[1] == 1 && pad_args[2] == 2);
    pad_result = 2;
    assert(lSzDuel_IsUnitHide(&item) == 0);
    pad_result = 0x10000;
    assert(lSzDuel_IsUnitHide(&item) == 1);

    /* no cell, flag set (bit 1 of the word for item side 1): the two counts decide */
    bits_result = 2; pad_result = 0;
    counts[1] = 31; counts[0] = 100;
    assert(lSzDuel_IsUnitHide(&item) == 1);
    counts[0] = 99;
    assert(lSzDuel_IsUnitHide(&item) == 0);
    counts[1] = 32; counts[0] = 100;
    assert(lSzDuel_IsUnitHide(&item) == 0);
    bits_result = 1;            /* bit 0 only: flag clear for side 1 */
    assert(lSzDuel_IsUnitHide(&item) == 1);

    /* cell owned by another item behaves like no cell */
    cell_result = &cell_store;
    cell_store.unit = (DotrStateWord)&stranger;
    query_calls = 0;
    bits_result = 2; counts[1] = 31; counts[0] = 100;
    assert(lSzDuel_IsUnitHide(&item) == 1 && query_calls == 0);

    /* cell owned by the item: the query result is returned when nonzero */
    cell_store.unit = (DotrStateWord)&item;
    query_result = 7;
    assert(lSzDuel_IsUnitHide(&item) == 7);
    assert(query_calls == 1 && query_item == &item && query_side == 1);
    query_result = 0;
    item.status.unknown_bit_4 = -1;
    assert(lSzDuel_IsUnitHide(&item) == 0);
    item.status.unknown_bit_4 = 0;

    /* flag set: three counts */
    pair_result = 5; counts[5] = 31;
    assert(lSzDuel_IsUnitHide(&item) == 1);
    assert(pair_args[0] == 1 && pair_args[1] == 9);
    counts[5] = 32;
    assert(lSzDuel_IsUnitHide(&item) == 0);
    counts[5] = 31; counts[0] = 99;
    assert(lSzDuel_IsUnitHide(&item) == 0);
    counts[0] = 100; counts[1] = 32;
    assert(lSzDuel_IsUnitHide(&item) == 0);

    /* flag clear: a pair result other than the control byte passes; equal asks the pad */
    bits_result = 0;
    assert(lSzDuel_IsUnitHide(&item) == 1);
    pair_result = 1; pad_result = 0; pad_calls = 0;
    assert(lSzDuel_IsUnitHide(&item) == 1 && pad_calls == 1 && pad_args[0] == 1);
    pad_result = 2;
    assert(lSzDuel_IsUnitHide(&item) == 0);
}

int main(void)
{
    check_gate();
    return 0;
}
