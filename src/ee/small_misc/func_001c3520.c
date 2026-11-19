#include "dotr/ee/small_misc.h"

void lSzDuel_SortTrade(int flag)
{
    struct DotrMiscControlTail *p = &data_00317490_tail;
    int value = flag ? p->unknown_6dc : p->unknown_6d8;
    int (*callback)(void) = flag ? lSzDuel_SortFunc2 : lSzDuel_SortFunc1;
    p->unknown_d5c = flag;
    func_00199bc0(value, 854, 2, callback);
}
