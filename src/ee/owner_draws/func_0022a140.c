#include "dotr/ee/owner_draws.h"
#include "dotr/ee/camp_tikei.h"

DotrOwnerDrawWord Taku_DECK_CARD_DISP(DotrOwnerDrawWord cursor)
{
    struct DotrOwnerDrawItem item;
    struct DotrOwnerDrawBlock block;
    struct DotrTikeiCamp *camp = &data_00299e10_camp;
    struct DotrOwnerDeckBank *bank = DOTR_OWNER_DECK_BANK;
    short i;
    short card;
    short x;
    short idx;

    if (camp->stage >= 10070 && camp->stage <= 10080 && DOTR_OWNER_DECK_PHASE < 4) {
        cursor = SzDuel_StartCardSession_Pipe(cursor);
        SzDuel_SetCardDefPrioZ(0xfffff8);
        SzDuel_ClearDrawCard(&block);
        block.unknown_30 = 0.46875f;
        block.unknown_34 = 0.48913044f;
        block.unknown_38 = 1.0f;
        block.unknown_3c = 0;
        for (i = 0; i < 3; i++) {
            idx = (bank->top + 2 - i) % 3;
            if (bank->dir != 0) {
                if ((signed char)bank->dir == 1) {
                    switch (i) {
                    case 0:
                        idx = bank->top % 3;
                        x = bank->frac * 36 / 100;
                        break;
                    case 1:
                        idx = (bank->top + 2) % 3;
                        x = 36 - bank->frac * 18 / 100;
                        break;
                    case 2:
                        idx = (bank->top + 1) % 3;
                        x = 18 - bank->frac * 18 / 100;
                        break;
                    }
                } else {
                    switch (i) {
                    case 0:
                        x = 36 - bank->frac * 36 / 100;
                        break;
                    case 1:
                        x = bank->frac * 18 / 100 + 18;
                        break;
                    case 2:
                        x = bank->frac * 18 / 100;
                        break;
                    }
                }
            } else {
                switch (i) {
                case 0:
                    x = 36;
                    break;
                case 1:
                    x = 18;
                    break;
                case 2:
                    x = 0;
                    break;
                }
            }
            card = bank->cards[idx];
            SzDuel_GetCardNodeOnScreen(&block, 0, -10.0f + (float)x + (float)(bank->pos[i] * 640 / 100 - 640), 10.0f);
            func_001dcd70_owner(&item, card);
            cursor = func_001dbc40_item(cursor, &item, &block);
            cursor = SzDuel_DrawCard_Pipe(cursor, &block);
            if (bank->step < 2) {
                bank->last = 0;
            }
            if (bank->step == 2 && bank->phase == 0) {
                bank->pos[i] += 18;
                if (0 < bank->pos[i] && bank->seen[i] == 0) {
                    SzSound_PlaySE(0, 9);
                    bank->seen[i] = 1;
                }
                if (bank->pos[i] >= 100) {
                    bank->last = i;
                    bank->pos[i] = 100;
                    if (i == 2) {
                        bank->phase = 1;
                    }
                }
            } else if (bank->step == 2) {
                bank->pos[i] = 100;
            } else if (bank->step >= 3) {
                if (i == 2) {
                    if (bank->phase == 3) {
                        bank->pos[i] -= 6;
                        if (bank->pos[i] <= 0) {
                            bank->pos[i] = 0;
                            if (bank->phase < 4) {
                                bank->phase = 4;
                            }
                        }
                    } else {
                        bank->pos[i] = 100;
                    }
                } else {
                    bank->pos[i] += 6;
                    if (bank->pos[i] >= 400) {
                        bank->pos[i] = 400;
                        if (bank->phase < 3) {
                            bank->phase = 3;
                        }
                    }
                }
            }
        }
        cursor = func_001d9e70_owner(cursor);
    }
    return cursor;
}
