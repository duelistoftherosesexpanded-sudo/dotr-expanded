#include "dotr/ee/cell_verdicts.h"
/* The third argument is the caller's order (0x12f350 passes it); this body does not read it. */
int TaLogic_IsOpenUnit__00130940(int x, int y, struct DotrRuleOrder *unused, int flag)
{
    int kind;
    int r;
    unsigned char count;
    int i;
    (void)unused;
    kind = SzDuel_GetUnitCardNo(x, y);
    func_00250360_pair(x, y);
    r = SzDuel_GetUnitLoc(x, y);
    count = 0;
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (kind) {
        case 298: /* Castle of Dark Illusions */
            return 1;
        case 322: /* Midnight Fiend */
            if ((unsigned char)flag == 1) {
                return 1;
            }
            break;
        case 326: /* Candle of Fate */
            for (i = 0; i < 11; i++) {
                if (func_001df540_pick(0, i) != 0 && x != func_00250360_pair(0, i)) {
                    count++;
                }
                if (func_001df540_pick(1, i) != 0 && x != func_00250360_pair(1, i)) {
                    count++;
                }
            }
            if (count >= 4) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic007_IsDoCerem(func_00250360_pair(x, y), kind) == 1) {
            if (SzDuel_GetField(r & 0xff, r >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        return TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5 ? 2 : 1;
    case 2:
    case 3:
    case 4:
        break;
    }
    return 2;
}
