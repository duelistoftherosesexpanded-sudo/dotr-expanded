#include "dotr/ee/small_misc.h"

void SzDuel_InitCustomDuel(int mode)
{
    struct DotrCustomDuelControl *c = (struct DotrCustomDuelControl *)&data_00317490_lists;
    unsigned short *p = DOTR_CUSTOM_DUEL->list;
    int deck = 101;
    int i;
    struct DotrMiscLargeEntry *src;
    struct DotrMiscLargeEntry *dst;

    if (mode >= 0) {
        DOTR_CUSTOM_DUEL->mode = mode;
    }
    switch (DOTR_CUSTOM_DUEL->mode) {
    case 0:
        break;
    case 1:
        src = SzData_GetData(0);
        dst = SzData_GetData(1);
        RhMem_CopyMemory((unsigned char *)dst, (unsigned char *)src, 0xffa0);
        ((struct DotrCustomDuelFlags *)dst)->b5 = 0;
        deck = 22;
        break;
    case 2:
        SzData_SetupFromConstDeck(1, 50);
        deck = 25;
        break;
    case 3:
        SzData_SetupFromConstDeck(1, 49);
        deck = 24;
        break;
    case 4:
        SzData_SetupFromConstDeck(1, 48);
        deck = 23;
        break;
    }
    SzText_Free(0, 1);
    SzText_Free(0, 0);
    SzText_Free(1, -1);
    if (mode >= 0) {
        c->side[0].deck = 100;
        c->side[0].lp = 4000;
        c->side[0].n = 3;
        c->side[1].deck = deck;
        c->side[1].lp = 4000;
        c->side[1].n = 3;
        DOTR_CUSTOM_DUEL->f0[0] = 0;
        DOTR_CUSTOM_DUEL->f2[0] = 0;
        DOTR_CUSTOM_DUEL->deck[0] = SzData_GetCurrentDeck(0);
        DOTR_CUSTOM_DUEL->lp[0] = 4000;
        DOTR_CUSTOM_DUEL->lp2[0] = 4000;
        DOTR_CUSTOM_DUEL->f6[0] = 3;
        DOTR_CUSTOM_DUEL->f8[0] = 1;
        DOTR_CUSTOM_DUEL->fa[0] = 1;
        DOTR_CUSTOM_DUEL->fc[0] = 1;
        DOTR_CUSTOM_DUEL->fe[0] = 1;
        DOTR_CUSTOM_DUEL->f0[1] = 0;
        DOTR_CUSTOM_DUEL->f2[1] = 0;
        DOTR_CUSTOM_DUEL->deck[1] = SzData_GetCurrentDeck(1);
        DOTR_CUSTOM_DUEL->lp[1] = 4000;
        DOTR_CUSTOM_DUEL->lp2[1] = 4000;
        DOTR_CUSTOM_DUEL->f6[1] = 3;
        DOTR_CUSTOM_DUEL->f8[1] = 1;
        DOTR_CUSTOM_DUEL->fa[1] = 1;
        DOTR_CUSTOM_DUEL->fc[1] = 1;
        DOTR_CUSTOM_DUEL->fe[1] = 1;
        DOTR_CUSTOM_DUEL->f8c = 0;
        DOTR_CUSTOM_DUEL->f8d = 0;
        DOTR_CUSTOM_DUEL->count = 24;
        for (i = 0; i < 24; i++) {
            *p++ = i | 0x200;
        }
        if (SzData_GetMapEditFlag(0)) {
            DOTR_CUSTOM_DUEL->count += 5;
            for (i = 0; i < 5; i++) {
                *p++ = i;
            }
            DOTR_CUSTOM_DUEL->f8d = 5;
            DOTR_CUSTOM_DUEL->f8c = DOTR_CUSTOM_DUEL->f8d - 2;
        }
        if (SzData_GetMapEditFlag(1)) {
            DOTR_CUSTOM_DUEL->count += 5;
            for (i = 0; i < 5; i++) {
                *p++ = i | 0x100;
            }
        }
        DOTR_CUSTOM_DUEL->last = DOTR_CUSTOM_DUEL->count - 2;
        DOTR_CUSTOM_DUEL->f8b = 0;
    } else {
        c->side[0].deck = 100;
        c->side[0].lp = DOTR_CUSTOM_DUEL->lp2[0];
        c->side[1].deck = deck;
        c->side[1].lp = DOTR_CUSTOM_DUEL->lp2[1];
    }
    SzText_LoadMerge(4, 1);
    SzText_LoadMerge(7, 1);
    c->a = 0;
    c->b = 0;
}
