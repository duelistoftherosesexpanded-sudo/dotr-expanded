#include "dotr/ee/cell_verdicts.h"

int TaLogic_IsOpenUnit__0011c140(int x, int y, unsigned *record)
{
    unsigned char item[32];
    unsigned *word;
    int cardId;
    int n;

    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        return 2;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        word = func_00250740_word(x, y, 0);
        if (word == 0) {
            return 2;
        }
        switch (*word >> 8) {
        case 2: /* Terrain Change */
            n = (record[2] >> 16) & 0xFFFF;
            if (n == SzDuel_GetUnitLoc(x, y)) {
                return 1;
            }
            break;
        case 13: /* Alter LP (Heal/Damage) */
            return 1;
        default:
            cardId = SzDuel_GetUnitCardNo(x, y);
            if (cardId == TaLogic003_IsSpellPlay(func_00250360_pair(x, y), item, func_001df540_pick(x, y))) {
                return 1;
            }
            break;
        }
        return 2;
    }
    return 2;
}
