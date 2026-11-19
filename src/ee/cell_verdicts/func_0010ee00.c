#include "dotr/ee/cell_verdicts.h"
int TaLogic_IsOpenUnit__0010ee00(int x, int y, unsigned *record)
{
    unsigned *word;
    int kind;
    int r;
    int n;
    r = SzDuel_GetUnitLoc(x, y);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    kind = SzDuel_GetUnitCardNo(x, y);
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        if (kind == 427 || kind == 428 || kind == 402 || kind == 403) {
            return 1;
        }
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
        case 13: /* Alter LP (Heal/Damage) */
            return 1;
        case 2: /* Terrain Change */
            n = (record[2] >> 16) & 0xFFFF;
            if (n == SzDuel_GetUnitLoc(x, y)) {
                return 1;
            }
            break;
        }
        return 2;
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic002_IsDoCerem(func_00250360_pair(x, y), kind) == 1) {
            if (SzDuel_GetField(r & 0xff, r >> 8) != 9) {
                return 1;
            }
        }
        return 2;
    }
    return 2;
}
