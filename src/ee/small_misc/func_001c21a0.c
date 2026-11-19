#include "dotr/ee/small_misc.h"

/* MATCHING DEVICE: the fourth work address is an unsigned integer local (DotrTradeAddr),
 * not a pointer, stored through a cast to a signed word: the original stores it from the
 * saved register of the local (`sw s4,1764(s0)`), while a pointer local, or the same
 * store without the signed cast, rebuilds the constant (one word more). */
void SzDuel_InitTrade(void)
{
    struct DotrMiscControlLists *p = &data_00317490_lists;
    short *l0 = DOTR_TRADE_WORK->lists[0];
    short *l1 = DOTR_TRADE_WORK->lists[1];
    unsigned char *c0 = DOTR_TRADE_WORK->counts[0];
    DotrTradeAddr c1 = DOTR_TRADE_COUNTS1;
    int i;

    i = SzData_GetOption(0, 0);
    data_003180e8 = 0xfffff8;
    p->table = i ? gSzDuel_NameIndex1 : gSzDuel_NameIndex0;
    p->lists[0] = l0;
    p->lists[1] = l1;
    p->counts[0] = c0;
    *(DotrTradeWord *)&p->counts[1] = (DotrTradeWord)c1;
    for (i = 0; i < 854; i++) {
        *l0++ = 999;
        *c0++ = 0;
    }
    for (i = 0; i < 854; i++) {
        *l1++ = 999;
        *(unsigned char *)c1 = 0;
        c1++;
    }
    p->unknown_c92 = 0;
    p->unknown_c94 = 0;
    p->unknown_cb6[0] = 0;
    p->unknown_cb6[1] = 4;
    p->unknown_cb6[2] = 10;
    p->unknown_cb6[3] = 0;
    p->unknown_cb6[4] = 4;
    p->unknown_cb6[5] = 10;
    p->unknown_cbe[0] = 1;
    p->unknown_cbe[1] = 1;
    p->modes[0] = 0;
    p->modes[1] = 0;
    p->rows[0] = 0;
    p->rows[1] = 0;
    p->pairs[0].top = 0;
    p->pairs[1].top = 0;
    p->unknown_c96 = 0;
    p->unknown_c98 = 0;
    p->unknown_cbc[0] = 0;
    p->unknown_cbc[1] = 0;
    lSzDuel_SetupTrade(0);
    lSzDuel_SetupTrade(1);
}
