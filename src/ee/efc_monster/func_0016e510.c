#include "dotr/ee/efc_monster.h"

/* TaLogic022_CommandEfcMonster (0x16e510): picks a monster unit's AI command. Two static inline
 * helpers give the larger of two ints and test whether the player is ready (card 0x3a, Exodia
 * the Forbidden One, in unit 0 and its four limbs 0x36 to 0x39 in the current deck). */

static inline int max_0016E510(int a, int b) {
    if (a < b) {
        return b;
    }
    return a;
}

static inline int ready_0016E510(int player) {
    if (SzDuel_GetUnitCardNo(player, 0) == 0x3A &&
        SzData_CheckDeck(player, SzData_GetCurrentDeck(player), 0x36) >= 0 &&
        SzData_CheckDeck(player, SzData_GetCurrentDeck(player), 0x37) >= 0 &&
        SzData_CheckDeck(player, SzData_GetCurrentDeck(player), 0x38) >= 0 &&
        SzData_CheckDeck(player, SzData_GetCurrentDeck(player), 0x39) >= 0) {
        return 1;
    }
    return 0;
}

void TaLogic022_CommandEfcMonster(unsigned short *actionUnit) {
    unsigned char work[112];
    struct DotrEfcMonsterCandidate list[22];
    short first;
    short second;
    unsigned short length;
    int player;
    int position;
    int distance;
    int count;
    int type;
    int x;
    int y;
    int x55;
    int y55;
    int retry_y;
    int retry_x;
    int nearest_y;
    int nearest_x;
    int mask;
    int mask55;
    int nearest_mask;
    int i;
    int i55;
    int nearest_i;
    int attempt;
    int card;
    int cursor;
    unsigned int *effect;
    unsigned int *action;
    int ready;
    int nearest;
    int maxy;
    int miny;
    int maxx;
    int minx;
    int other;
    int result;
    int result55;
    int nearest_card;
    int nearest_position;
    int nearest_delta;
    int n;
    struct DotrEfcMonsterCandidate *p;

    count = 0;
    player = Ta_IsUnitControl(actionUnit[0] >> 8, actionUnit[0] & 0xFF);
    type = SzDuel_GetUnitCardNo(actionUnit[0] >> 8, actionUnit[0] & 0xFF);
    SzDuel_GetUnit(actionUnit[0] >> 8, actionUnit[0] & 0xFF);
    position = SzDuel_GetUnitLoc(actionUnit[0] >> 8, actionUnit[0] & 0xFF);
    if ((card = SzDuel_GetUnit(actionUnit[0] >> 8, actionUnit[0] & 0xFF)) != 0) {
        p = list;
        length = 0;
        second = 0;
        first = 0;
        cursor = 0;
        n = 0;
        do {
            n++;
            p->card = 0xFFFF;
            p->index = 0xFF;
            p++;
        } while (n < 22);
        TaLogic_InitIgnoreLst(work);
        if (((unsigned int)(type - 0x36) < 4 ? 1 : 0) == 1) {
            ready = ready_0016E510(player);
            i = 0;
            if (ready == 1) {
                do {
                    if (position == TaLogic_GetSommonPiecePreference__00170610(player, i)) {
                        actionUnit[2] = 0x6FF;
                        *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                    }
                    i++;
                } while (i < 9);
                actionUnit[2] = 0x6FF;
                *(unsigned int *)&actionUnit[4] = actionUnit[4] | (SzDuel_GetUnitLoc(player, 0) << 16);
                return;
            }
        }
        effect = Ta_GetUnitEffect(actionUnit[0] >> 8, actionUnit[0] & 0xFF, 2);
        if (effect != 0) {
            switch (effect[0] >> 8) {
            case 0xC:
            case 0x34:
            case 0xD:
            case 0xE:
            case 0x22:
            case 0x3B:
            case 0x56:
            case 0x43:
            case 0xF:
                actionUnit[2] = 0x3FF;
                *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                return;
            case 0xB:
            case 0x4E:
            case 0x12:
            case 0x4D:
            case 0x6:
                if ((unsigned int)SearchUnitByRange(0, 0, player, 1, effect[0] & 0x3F, 14) > 1U) {
                    actionUnit[2] = 0x3FF;
                    *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                    return;
                }
                break;
            case 0x13:
            case 0x17:
                if (((effect[0] >> 6) & 1) && SearchUnitByRange(0, 0, player, 1, effect[0] & 0x3F, 14) != 0) {
                    if (SzDuel_GetUnit(actionUnit[0] >> 8, actionUnit[0] & 0xFF) != 0) {
                        other = (Ta_GetCurrentSide(actionUnit[0] >> 8, actionUnit[0] & 0xFF) == 0 ? 1 : 0) << 8;
                        SzDuel_GetUnitLoc(actionUnit[0] >> 8, actionUnit[0] & 0xFF);
                        SzDuel_GetUnitLoc(other >> 8, other & 0xFF);
                        SzDuel_GetUnit(actionUnit[0] >> 8, actionUnit[0] & 0xFF);
                        if (SzDuel_GetUnitAP(actionUnit[0] >> 8, actionUnit[0] & 0xFF, 0) >= 2500) {
                            TaLogic022_CommandAtkUnit(actionUnit);
                        } else if (TaLogic022_IsEmptyCntrGuard(actionUnit[0] >> 8, actionUnit[0] & 0xFF, 1) == 1) {
                            TaLogic022_CommandLineUnit(actionUnit, 1);
                        } else {
                            TaLogic022_CommandAtkUnit(actionUnit);
                        }
                    }
                    return;
                }
                if ((unsigned int)TaLogic_RemoveHideUnit(SearchUnitByRange(0, 0, player, 2, effect[0] & 0x3F, 14)) > 2U) {
                    actionUnit[2] = 0x3FF;
                    *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                    return;
                }
                break;
            case 0x47:
                mask = SearchUnitByRange(0, 0, player, 2, 0x34, 14);
                i = 0;
                if (0 < Ta_GetUnitNumFrom22bitFlagList(mask)) {
                    do {
                        result = Ta_GetUnitOwnerIDFromLstCount(mask, i);
                        y = result >> 8;
                        x = result & 0xFF;
                        if (SzDuel_GetUnitCurseRest(y, x) > 0 && SzDuel_GetUnitCurseRest(y, x) != 0xFF) {
                            actionUnit[2] = 0x3FF;
                            *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                            return;
                        }
                        i++;
                    } while (i < Ta_GetUnitNumFrom22bitFlagList(mask));
                }
                break;
            case 0x53:
                i = 0;
                if (0 < SzDuel_GetGraveCount(player)) {
                    do {
                        if ((TaHand_GetCardKind(SzDuel_InqGrave(player, i)) >> 5) == 0) {
                            count++;
                        }
                        i++;
                    } while (i < SzDuel_GetGraveCount(player));
                }
                i = 0;
                while (i < SzDuel_GetGraveCount((player == 0 ? 1 : 0))) {
                    if ((TaHand_GetCardKind(SzDuel_InqGrave((player == 0 ? 1 : 0), i)) >> 5) == 0) {
                        count++;
                    }
                    i++;
                }
                if (count > 5) {
                    actionUnit[2] = 0x3FF;
                    *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                    return;
                }
                break;
            case 0x54:
                result = Ta_GetUnitOwnerIDFromLstCount(SearchUnitByRange(0, 0, player, 3, 0x21, 14), 0);
                if (player == Ta_IsUnitControl(result >> 8, result & 0xFF) && result != actionUnit[0]) {
                    actionUnit[2] = 0x3FF;
                    *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                    return;
                }
                break;
            case 0x55:
                mask55 = SearchUnitByRange(0, 0, player, 1, 0x34, 14);
                i55 = 0;
                if (0 < Ta_GetUnitNumFrom22bitFlagList(mask55)) {
                    do {
                        result55 = Ta_GetUnitOwnerIDFromLstCount(mask55, i55);
                        y55 = result55 >> 8;
                        x55 = result55 & 0xFF;
                        if (SzDuel_GetUnitCurseRest(y55, x55) > 0 && SzDuel_GetUnitCurseRest(y55, x55) != 0xFF) {
                            actionUnit[2] = 0x3FF;
                            *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                            return;
                        }
                        i55++;
                    } while (i55 < Ta_GetUnitNumFrom22bitFlagList(mask55));
                }
                break;
            case 0x58:
                if (SzData_GetRank((player == 0 ? 1 : 0), SzDuel_GetUnitCardNo((player == 0 ? 1 : 0), 0)) >= 6) {
                    actionUnit[2] = 0x3FF;
                    *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                    return;
                }
                break;
            }
        }
        action = Ta_GetUnitEffect(actionUnit[0] >> 8, actionUnit[0] & 0xFF, 3);
        if (SzDuel_GetUnitStatus(actionUnit[0] >> 8, actionUnit[0] & 0xFF, 4) == 0 && action != 0) {
            switch (action[0] >> 8) {
            case 0x17:
                if ((action[0] & 0x3F) == 0x1A && TaLogic_CommandGunnerUnit(actionUnit, 0) == 1) {
                    actionUnit[2] = 0xBFF;
                    return;
                }
                break;
            case 0x14:
                actionUnit[2] = 0xCFF;
                return;
            case 0x4:
                if ((action[0] & 0x3F) == 0x1A && TaLogic_CommandGunnerUnit(actionUnit, 0) == 1) {
                    actionUnit[2] = 0xBFF;
                    if ((action[1] >> 16) != 2) {
                        return;
                    }
                    result = (unsigned short)SzDuel_GetUnitKind((unsigned int)(unsigned short)(*(unsigned int *)&actionUnit[4] >> 16) >> 8, (*(unsigned int *)&actionUnit[4] >> 16) & 0xFF);
                    if ((result >> 5) == 0 && (result & 0x1F) == 2) {
                        return;
                    }
                }
                break;
            case 0x16:
                if (TaLogic_CommandGunnerUnit(actionUnit, 0) == 1) {
                    actionUnit[2] = 0xBFF;
                    return;
                }
                break;
            case 0x24:
                if (((action[0] & 0x3F) == 0x1A || (action[0] & 0x3F) == 0x1B) && TaLogic_CommandGunnerUnit(actionUnit, 0) == 1) {
                    actionUnit[2] = 0xBFF;
                    return;
                }
                break;
            case 0x1F:
                if (Ta_SearchUncTypeNaturalEffectUserNum(0x39) <= 0 && (action[0] & 0x3F) == 0x1A && TaLogic_CommandGunnerUnit(actionUnit, 0) == 1) {
                    actionUnit[2] = 0xBFF;
                    if ((action[1] >> 16) != 14) {
                        return;
                    }
                    result = (unsigned short)SzDuel_GetUnitKind((unsigned int)(unsigned short)(*(unsigned int *)&actionUnit[4] >> 16) >> 8, (*(unsigned int *)&actionUnit[4] >> 16) & 0xFF);
                    if ((result >> 5) == 0 && (result & 0x1F) == 14) {
                        return;
                    }
                }
                break;
            case 0x25:
                attempt = 1;
                retry_y = position >> 8;
                retry_x = position & 0xFF;
                while (1) {
                    cursor = TaLogic_GetNearUnitL2(cursor, card, retry_x, retry_y, (player == 0 ? 1 : 0), (unsigned char)attempt, 0, TaLogic_GetWarnHideLvl__00172310(player), list, &length, &first, &second, work);
                    result = TaLogic_GetNearUnitL3(card, retry_x, retry_y, list, 5, 0x34, 0, 2, 0, TaLogic_GetWarnHideLvl__00172310(player), length);
                    if (result >= 0) {
                        actionUnit[2] = 0x2FF;
                        *(unsigned int *)&actionUnit[4] = (result << 16) | actionUnit[4];
                        return;
                    }
                    attempt++;
                    if (attempt <= 5) {
                        continue;
                    }
                    break;
                }
                break;
            case 0x3C:
                nearest_mask = SearchUnitByRange(0, 0, player, 3, 0x26D, 14);
                distance = 7;
                nearest = -1;
                if (nearest_mask != 0) {
                    nearest_i = 0;
                    if (0 < Ta_GetUnitNumFrom22bitFlagList(nearest_mask)) {
                        nearest_y = position >> 8;
                        nearest_x = position & 0xFF;
                        do {
                            nearest_card = Ta_GetUnitOwnerIDFromLstCount(nearest_mask, nearest_i);
                            nearest_position = SzDuel_GetUnitLoc(nearest_card >> 8, nearest_card & 0xFF);
                            miny = nearest_y < (nearest_position >> 8) ? nearest_y : (nearest_position >> 8);
                            maxy = max_0016E510(nearest_position >> 8, nearest_y);
                            minx = nearest_x < (nearest_position & 0xFF) ? nearest_x : (nearest_position & 0xFF);
                            maxx = max_0016E510(nearest_position & 0xFF, nearest_x);
                            nearest_delta = (maxx - minx) + (maxy - miny);
                            if (nearest_delta < distance) {
                                distance = nearest_delta;
                                nearest = nearest_card;
                            }
                            nearest_i++;
                        } while (nearest_i < Ta_GetUnitNumFrom22bitFlagList(nearest_mask));
                    }
                    if (nearest >= 0) {
                        actionUnit[2] = 0xBFF;
                        *(unsigned int *)&actionUnit[4] = (nearest << 16) | actionUnit[4];
                        return;
                    }
                }
                break;
            case 0xC:
                actionUnit[2] = 0x3FF;
                *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                return;
            case 0x46:
                if (TaLogic_CommandGunnerUnit(actionUnit, 0) == 1) {
                    actionUnit[2] = 0xBFF;
                }
                return;
            case 0x51:
                if (TaLogic_CommandGunnerUnit(actionUnit, 0) == 1) {
                    return;
                }
                break;
            }
        }
        action = Ta_GetUnitEffect(actionUnit[0] >> 8, actionUnit[0] & 0xFF, 1);
        if (action != 0) {
            switch (action[0] >> 8) {
            case 0x42:
                if (((action[0] >> 6) & 3) == 1) {
                    actionUnit[2] = 0x6FF;
                    *(unsigned int *)&actionUnit[4] = (position << 16) | actionUnit[4];
                }
                break;
            }
        }
        if (SzDuel_GetUnit(actionUnit[0] >> 8, actionUnit[0] & 0xFF) != 0) {
            other = (Ta_GetCurrentSide(actionUnit[0] >> 8, actionUnit[0] & 0xFF) == 0 ? 1 : 0) << 8;
            SzDuel_GetUnitLoc(actionUnit[0] >> 8, actionUnit[0] & 0xFF);
            SzDuel_GetUnitLoc(other >> 8, other & 0xFF);
            SzDuel_GetUnit(actionUnit[0] >> 8, actionUnit[0] & 0xFF);
            if (SzDuel_GetUnitAP(actionUnit[0] >> 8, actionUnit[0] & 0xFF, 0) >= 2500) {
                TaLogic022_CommandAtkUnit(actionUnit);
            } else if (TaLogic022_IsEmptyCntrGuard(actionUnit[0] >> 8, actionUnit[0] & 0xFF, 1) == 1) {
                TaLogic022_CommandLineUnit(actionUnit, 1);
            } else {
                TaLogic022_CommandAtkUnit(actionUnit);
            }
        }
    }
}
