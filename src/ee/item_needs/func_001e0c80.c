#include "dotr/ee/item_needs.h"

int SzDuel_GetUnitLeaderAbility3(struct DotrNeedItem *unit, int ability, int *extraDataOut, int ignoreMenu)
{
    int value;
    int rankRequirement;
    int v;

    value = 0;
    if (extraDataOut != 0) {
        *extraDataOut = 0;
    }
    if (unit == 0) {
        return 0;
    }
    if (unit->cardNo >= 854) {
        return 0;
    }
    if (unit->cardNo == 671) {
        return 0;
    }
    if (ignoreMenu == 0) {
        v = SzGame_GetMenuNum(28);
        if (!(v & (1 << unit->team))) {
            return 0;
        }
    }
    switch (ability) {
    case 0: /* extra slots */
        v = unit->leaderAbilities[0] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        return v != 0 ? 1 : 0;
    case 1: /* hidden card */
        v = unit->leaderAbilities[0] & 0xffff;
        if (v == 0xffff) {
            return 0;
        }
        return v != 0 ? 1 : 0;
    case 2: /* exodia (uncertain) */
        v = unit->leaderAbilities[1] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        value = v & 0xff;
        break;
    case 3: /* destiny draw */
        v = unit->leaderAbilities[1] & 0xffff;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 4: /* increased movement */
        v = unit->leaderAbilities[2] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 5: /* lp recovery */
        v = unit->leaderAbilities[2] & 0xffff;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 6: /* direct damage halved */
        v = unit->leaderAbilities[3] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 7: /* terrain change */
        v = unit->leaderAbilities[3] & 0xffff;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v & 0xff;
        value = v >> 8;
        break;
    case 8: /* level cost reduction for summoning same type */
        v = unit->leaderAbilities[4] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v & 0xff;
        value = v >> 8;
        break;
    case 9: /* extended support range */
        v = unit->leaderAbilities[4] & 0xffff;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 10: /* increased strength for same type friendlies */
        v = unit->leaderAbilities[5] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v & 0xff;
        value = v >> 8;
        break;
    case 11: /* improved resistance for same type friendlies */
        v = unit->leaderAbilities[5] & 0xffff;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 12: /* movement boost for same type friendlies */
        v = unit->leaderAbilities[6] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 13: /* open opponents card */
        v = unit->leaderAbilities[6] & 0xffff;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 14: /* weaken specific enemy type */
        v = unit->leaderAbilities[7] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v & 0xff;
        value = v >> 8;
        break;
    case 15: /* spellbind specific enemy type */
        v = unit->leaderAbilities[7] & 0xffff;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    case 16: /* destroy specific enemy type */
        v = unit->leaderAbilities[8] >> 16;
        if (v == 0xffff) {
            return 0;
        }
        rankRequirement = v >> 8;
        break;
    default:
        return 0;
    }
    v = SzData_GetRank(unit->team, unit->cardNo);
    if (extraDataOut != 0) {
        *extraDataOut = value;
    }
    if (v >= rankRequirement) {
        return 1;
    }
    return 0;
}
