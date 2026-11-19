#include "dotr/ee/lower_rules.h"

void TaLogic_CommandEfcMonster__001229f0(struct DotrRuleOrder *order)
{
    int side;
    int cell;
    unsigned char count;
    int i;

    count = 0;
    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0) {
        side = Ta_GetCurrentSide(order->position >> 8, order->position & 0xFF);
        cell = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
        switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xFF)) {
        case 565:
            for (i = 6; i < 11; i++) {
                if (SzDuel_GetUnitStatus(side, i, 4) != 0 && side == func_00250360_pair(side, i)) {
                    count++;
                }
                if (SzDuel_GetUnitStatus(side == 0 ? 1 : 0, i, 4) != 0 && side == func_00250360_pair(side == 0 ? 1 : 0, i)) {
                    count++;
                }
            }
            if (count > 0) {
                order->missions = 0x3FF;
                order->flags.word = (cell << 16) | order->flags.half;
            }
            break;
        }
        func_00122270_order(order, 0);
    }
}
