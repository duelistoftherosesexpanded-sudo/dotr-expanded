#include "dotr/ee/small_misc.h"

void lSzDuel_SetupTrade(int flag)
{
    struct DotrMiscControlLists *p = &data_00317490_lists;
    unsigned short *top = flag ? &p->pairs[1].top : &p->pairs[0].top;
    unsigned short *count = flag ? &p->pairs[1].count : &p->pairs[0].count;
    short *list = flag ? p->lists[1] : p->lists[0];
    unsigned char *a = flag ? p->counts[1] : p->counts[0];
    unsigned char *b = flag ? p->counts[0] : p->counts[1];
    unsigned char *rows = flag ? &p->rows[1] : &p->rows[0];
    int i;
    int v;

    *count = 0;
    switch (p->modes[flag]) {
    case 0:
        for (i = 0; i < 854; i++, list++) {
            if (SzData_GetBagCount(flag, i, 3) > 0) {
                *list = i;
                (*count)++;
            } else {
                *list = 999;
            }
        }
        break;
    case 1:
        for (i = 0; i < 854; i++, list++, a++) {
            if (*a > 0) {
                *list = i;
                (*count)++;
            } else {
                *list = 999;
            }
        }
        break;
    case 2:
        for (i = 0; i < 854; i++, list++, b++) {
            if (*b > 0) {
                *list = i;
                (*count)++;
            } else {
                *list = 999;
            }
        }
        break;
    }
    if (*count < *top + *rows) {
        v = *count - 5;
        *top = v > 0 ? v : 0;
    }
    lSzDuel_SortTrade(flag);
}
