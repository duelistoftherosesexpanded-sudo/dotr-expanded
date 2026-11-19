#include "dotr/ee/position_rules.h"

/* File-local copy of the 0x15e440 body (pick_copies group), which the target contains
 * expanded in place twice with the constants 1 and 2. MATCHING DEVICE: it takes the order
 * instead of its two coordinates. Not a claim about an original function. */
static inline int column_free(struct DotrPositionOrder *order, unsigned char c)
{
    int key;
    int b;
    int a;
    int i;
    int t;
    int n;
    int cell;

    if (c == 1) {
        n = 3;
    } else {
        n = 4;
    }
    b = order->position & 0xff;
    a = order->position >> 8;
    key = (a << 8) | b;
    for (i = 1; i <= 5; i++) {
        cell = SzDuel_GetMapUnitID(i, n);
        if (cell != 0) {
            if (cell == key) {
                return 1;
            }
            t = func_00250360_pair(cell >> 8, cell & 0xff);
            if (t == func_00250360_pair(a, b)) {
                return 0;
            }
        }
    }
    return 1;
}

void TaLogic019_CommandSpellUnit(struct DotrPositionOrder *order)
{
    unsigned char bits;
    int kind;
    int side;
    unsigned *word;
    int here;
    int r;

    bits = SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff);
    kind = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff);
    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    word = func_00250740_word(order->position >> 8, order->position & 0xff, 0);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if (word == 0) {
        order->kind = 0x5ff;
        return;
    }
    switch (bits >> 5) {
    case 1:
        switch (*word >> 8) {
        case 20:
            order->kind = 0xcff;
            return;
        case 4:
            r = TaLogic019_GetUseThunderBolt(side);
            if (r >= 0) {
                order->kind = 0x6ff;
                order->flags.word = order->flags.half | ((((here >> 8) << 8) | (SzDuel_GetUnitLoc(r >> 8, r & 0xff) & 0xff)) << 16);
                return;
            }
            order->kind = 0x5ff;
            return;
        case 37:
            if (kind == 749) {
                order->kind = 0x6ff;
                order->flags.word = order->flags.half | 0x4060000;
                return;
            }
            if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
                order->kind = 0xbff;
                return;
            }
            break;
        case 31:
            if (kind == 732) {
                order->kind = 0xdff;
                return;
            }
            break;
        }
        break;
    case 3:
    case 4:
        if (kind == 749) {
            order->kind = 0x6ff;
            order->flags.word = order->flags.half | 0x6000000;
            return;
        }
        if (column_free(order, 1) == 1) {
            func_0015e520_order(order, 1);
            return;
        }
        r = TaLogic019_IsEmptyTrapGuard(order->position >> 8, order->position & 0xff);
        if (r >= 0) {
            order->kind = 0x6ff;
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
        if (column_free(order, 2) == 1) {
            func_0015e520_order(order, 2);
            return;
        }
        if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
            order->kind = 0xbff;
            return;
        }
        break;
    }
    order->kind = 0x5ff;
}
