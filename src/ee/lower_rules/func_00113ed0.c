#include "dotr/ee/lower_rules.h"

void TaLogic001_CommandSpellUnit(struct DotrRuleOrder *order, int deep)
{
    int key;
    int cell;
    unsigned *record;
    int spot;

    (void)deep;
    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    cell = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
    record = func_00250740_word(order->position >> 8, order->position & 0xFF, 0);
    if (record == 0) {
        order->missions = 0x5FF;
        return;
    }
    switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
    case 1:
        switch (*record >> 8) {
        case 2:
            order->missions = 0x6FF;
            order->flags.word = order->flags.half | 0x3030000;
            return;
        case 20:
            order->missions = 0xCFF;
            return;
        case 4:
            spot = TaLogicCom_GetUseXYRangeSpell__00112600(key, record);
            if (spot >= 0) {
                order->missions = 0x6FF;
                if ((*record & 0x3F) == 0) {
                    order->flags.word = order->flags.half | ((cell >> 8) << 8 | (spot & 0xFF)) << 16;
                } else {
                    order->flags.word = spot << 16 | order->flags.half;
                }
                return;
            }
            break;
        }
        break;
    case 3:
        if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
            order->missions = 0xBFF;
            return;
        }
        break;
    }
    order->missions = 0x5FF;
}
