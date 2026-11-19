#include "dotr/ee/list_reset.h"

void SzDuel_InitBuildDeck(int side)
{
    struct DotrListControl *c = DOTR_LIST_CONTROL;
    int deckNumber;
    short *ids;
    short *list;
    char *marks;
    int i;
    int j;
    int r;

    deckNumber = SzData_GetCurrentDeck(side);
    ids = DOTR_LIST_IDS;
    list = DOTR_LIST_ENTRIES;
    marks = DOTR_LIST_MARKS;
    r = SzData_GetOption(0, 0);
    data_00317490_lists.word_c58 = 0xfffff8;
    c->table = r ? gSzDuel_NameIndex1 : gSzDuel_NameIndex0;
    c->ids = DOTR_LIST_IDS;
    c->entries = DOTR_LIST_ENTRIES;
    c->marks = DOTR_LIST_MARKS;
    c->id_count = 0;
    for (i = 0; i < 854; i++) {
        if (SzData_GetBagCount(side, i, -1) > 0) {
            *ids = i;
            c->id_count++;
        } else {
            *ids = 999;
        }
        ids++;
    }
    c->entry_count = 0;
    *DOTR_LIST_ENTRIES = SzData_GetLeader(side, deckNumber);
    c->entry_count++;
    list++;
    for (j = 0; j < 40; j++) {
        *list = SzData_GetCardNo(side, deckNumber, j);
        list++;
        c->entry_count++;
    }
    for (i = 0; i < 8; i++) {
        marks[2] = 0;
        marks += 4;
    }
    c->count_c92 = 0;
    c->count_c94 = 0;
    c->byte_cb6 = 0;
    c->byte_cb7 = 4;
    c->byte_cb8 = 10;
    c->byte_cb9 = 0;
    c->byte_cba = 4;
    c->byte_cbb = 10;
    c->byte_d5d = 3;
    c->byte_d5e = 0;
    c->byte_d5f = 0;
    c->count_c8a = 0;
    c->count_c8e = 0;
    c->count_c96 = 0;
    c->count_c98 = 0;
    c->byte_d60 = 0;
    c->byte_d61 = 0;
    c->byte_d62 = 0;
    c->byte_cbc = 0;
    c->byte_cbd = 0;
    c->side = side;
    lSzDuel_SortBag();
    lSzDuel_SortDeck();
    SzData_CalcDeckCost(side, deckNumber);
    c->byte_d32 = 0;
}
