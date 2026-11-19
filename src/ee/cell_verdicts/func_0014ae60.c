#include "dotr/ee/cell_verdicts.h"
int TaLogic_IsOpenUnit__0014ae60(int x, int y)
{
    int cardId;
    int pos;
    unsigned *p;
    int r;
    cardId = SzDuel_GetUnitCardNo(x, y);
    pos = SzDuel_GetUnitLoc(x, y);
    func_00250360_pair(x, y);
    p = SzDuel_GetUnitEffect(x, y, 0);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (cardId) {
        case 291: /* Birdface */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 1, 6, 14)) >= 3) {
                return 1;
            }
            break;
        case 272: /* Harpie Lady */
        case 273: /* Harpie Lady Sisters */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 1, 0x1E001C, 14)) > 0) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic013_IsDoCerem(func_00250360_pair(x, y), cardId) == 1) {
            if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (p == 0) {
            return 2;
        }
        switch (*p >> 8) {
        case 13: /* Alter LP (Heal/Damage) */
            return 1;
        case 2: /* Terrain Change */
            r = TaLogic_GetLandChange(func_001df540_word(x, y), pos & 0xFF, pos >> 8, 2, 6, 2);
            if (r >= 0 && r == pos) {
                return 1;
            }
            break;
        case 4: /* Destruction */
            if (TaLogic013_IsUseHarpySweaper(x, pos & 0xFF, pos >> 8) == 1) {
                return 1;
            }
            break;
        }
        break;
    default:
        return 2;
    }
    return 2;
}
