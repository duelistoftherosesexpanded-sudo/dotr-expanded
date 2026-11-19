/* Synthetic host tests for the list_reset group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/list_reset.h"

short dotr_list_work_01ed0000[0x730 / 2];
struct DotrListControl data_00317490_lists;
unsigned char gSzDuel_NameIndex1[1];
unsigned char gSzDuel_NameIndex0[1];

static int which;
static int order[4];
static int order_count;
static int kinds_bad;

int SzData_GetCurrentDeck(int side) { return side + 70; }
int SzData_GetOption(int side, int list) { assert(side == 0 && list == 0); return which; }
/* Ids divisible by 3 give 1 (taken), ids with remainder 1 give 0, the rest -1. */
int SzData_GetBagCount(int side, int id, int kind)
{
    if (side != 5 || kind != -1) {
        kinds_bad = 1;
    }
    return id % 3 == 0 ? 1 : id % 3 == 1 ? 0 : -1;
}
int SzData_GetLeader(int side, int list) { return side * 100 + list; }
int SzData_GetCardNo(int side, int list, int position) { return side * 1000 + list + position; }
int SzData_CalcDeckCost(int side, int list)
{
    assert(side == 5 && list == 75);
    /* Still set here: the function clears it after this call. */
    assert(DOTR_LIST_CONTROL->byte_d32 == 0xee);
    order[order_count++] = 3;
    return 0;
}
void lSzDuel_SortBag(void) { assert(DOTR_LIST_CONTROL->side == 5); order[order_count++] = 1; }
void lSzDuel_SortDeck(void) { order[order_count++] = 2; }

/* Branch coverage: both table choices, each outcome of the id query (positive, zero,
 * negative), the first and last index of each loop and the storage just past them. */
static void check_reset(int choice)
{
    struct DotrListControl *c = DOTR_LIST_CONTROL;
    char *marks = DOTR_LIST_MARKS;
    int i;

    memset(&data_00317490_lists, 0xee, sizeof(data_00317490_lists));
    memset(dotr_list_work_01ed0000, 0x77, sizeof(dotr_list_work_01ed0000));
    which = choice;
    order_count = 0;
    kinds_bad = 0;
    SzDuel_InitBuildDeck(5);

    assert(!kinds_bad);
    assert(c->word_c58 == 0xfffff8);
    assert(c->table == (choice ? gSzDuel_NameIndex1 : gSzDuel_NameIndex0));
    assert(c->ids == DOTR_LIST_IDS && c->entries == DOTR_LIST_ENTRIES && c->marks == marks);
    for (i = 0; i < 854; i++) {
        assert(DOTR_LIST_IDS[i] == (i % 3 == 0 ? i : 999));
    }
    assert(c->id_count == 285);
    assert(DOTR_LIST_ENTRIES[0] == 575);
    for (i = 0; i < 40; i++) {
        assert(DOTR_LIST_ENTRIES[i + 1] == 5075 + i);
    }
    assert(c->entry_count == 41);
    for (i = 0; i < 8; i++) {
        assert(marks[i * 4 + 2] == 0 && marks[i * 4] == 0x77 && marks[i * 4 + 1] == 0x77 && marks[i * 4 + 3] == 0x77);
    }
    assert(marks[8 * 4 + 2] == 0x77);
    assert(c->count_c8a == 0 && c->count_c8e == 0 && c->count_c92 == 0 && c->count_c94 == 0);
    assert(c->count_c96 == 0 && c->count_c98 == 0);
    assert(c->byte_cb6 == 0 && c->byte_cb7 == 4 && c->byte_cb8 == 10);
    assert(c->byte_cb9 == 0 && c->byte_cba == 4 && c->byte_cbb == 10);
    assert(c->byte_cbc == 0 && c->byte_cbd == 0 && c->byte_d32 == 0);
    assert(c->side == 5 && c->byte_d5d == 3 && c->byte_d5e == 0 && c->byte_d5f == 0);
    assert(c->byte_d60 == 0 && c->byte_d61 == 0 && c->byte_d62 == 0);
    /* Untouched neighbours. */
    assert(c->unknown_c9a[0] == 0xee && c->unknown_cbe[0] == 0xee && c->unknown_d33[0] == 0xee);
    assert(order_count == 3 && order[0] == 1 && order[1] == 2 && order[2] == 3);
}

int main(void)
{
    check_reset(0);
    check_reset(1);
    return 0;
}
