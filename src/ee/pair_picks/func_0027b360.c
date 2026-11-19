#include "dotr/ee/pair_picks.h"

int TaLogic_GetAtkUnitNow(int side, unsigned short *ownerIds, struct DotrMiscEntry16 *units)
{
    unsigned short picks[22];
    int count;
    int found;
    int i;
    int index;
    struct DotrMiscCellPair *pair;

    (void)side;
    found = 0;
    for (i = 0; i < 22; i++) {
        picks[i] = 0xffff;
    }
    for (i = 0, count = 0; i < TaLogic_GetUnitCtlLstLen(ownerIds); i++) {
        if (func_001df540_word(ownerIds[i] >> 8, ownerIds[i] & 0xff) == 0) {
            continue;
        }
        if (SzDuel_GetUnitStatus(ownerIds[i] >> 8, ownerIds[i] & 0xff, 0) != 0) {
            continue;
        }
        index = TaLogic_GetActionUnitFromOwnerID(units, ownerIds[i]);
        if (index < 0) {
            continue;
        }
        pair = (struct DotrMiscCellPair *)&units[index];
        /* The bytes at +5 and +4 are the entry's two missions; 2 is the attack mission. */
        if (((pair->unknown_02[1] >> 8) & 0xff) == 2 && TaLogic_IsAtkNowToGoal(pair, 1) == 1) {
            found = 1;
            picks[count++] = ownerIds[i];
        } else if (*(unsigned char *)&pair->unknown_02[1] == 2 && TaLogic_IsAtkNowToGoal(pair, 0) == 1) {
            found = 1;
            picks[count++] = ownerIds[i];
        }
    }
    if (found == 1) {
        return TaLogic_GetMaxApUnit(picks);
    }
    return -1;
}
