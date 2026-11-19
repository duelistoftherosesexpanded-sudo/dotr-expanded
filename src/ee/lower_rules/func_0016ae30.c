#include "dotr/ee/lower_rules.h"

/* The caller, TaLogic_GetMinimumTask_AtkMission__00169fe0, passes its order as a third
 * argument, like the other IsOpenUnit copies take; this body does not read it. */
int TaLogic_IsOpenUnit__0016ae30(int side, int index, struct DotrRuleOrder *order)
{
    int id;
    unsigned *p;

    (void)order;
    id = SzDuel_GetUnitCardNo(side, index);
    func_00250360_pair(side, index);
    p = SzDuel_GetUnitEffect(side, index, 0);
    SzDuel_GetUnitLoc(side, index);
    if (SzDuel_GetUnitStatus(side, index, 4) != 0) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(side, index) >> 5) {
    case 1:
        if (TaLogic_GetActionReaction(side, index, 0, 0, 6, 1) == 5) {
            return 2;
        }
        if (p == 0) {
            return 2;
        }
        switch (*p >> 8) {
        case 39: /* steal summoning power */
            if (SzDuel_GetSummonPower(side) < 4) {
                return 1;
            }
            break;
        case 4: /* destruction */
            if (TaLogic021_IsUseBlackHole(side) == 1) {
                return 1;
            }
            break;
        }
        break;
    case 0:
        switch (id) {
        case 224:
        case 364:
            if (SzDuel_GetSummonPower(side) < 4) {
                return 1;
            }
            break;
        }
        break;
    }
    return 2;
}
