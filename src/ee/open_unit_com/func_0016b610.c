#include "dotr/ee/open_unit_com.h"

/* TaLogicCom_IsOpenUnit at 0x16b610: the twin of TaLogicCom_IsOpenUnit__00111320 (see
 * func_00111320.c) calling the file-local helpers at 0x16b3c0..0x16cff0, with the attack threshold
 * 2500 for effect kind 0x21 and an unused third parameter. Its statements and declarations take a
 * different form from the 0x111320 copy's. The first SzDuel_GetUnitCardNo call is written with
 * (side, pos), which compiles to the same bytes as a call without arguments (see open_unit_com.h). */
int TaLogicCom_IsOpenUnit__0016b610(int side, int pos, void *info)
{
    int mask = 0;
    int count = 0;
    int owner;
    int *effect;
    int card;
    int position;
    int i;
    int y;
    int x;
    int y55;
    int x55;
    int index55;
    int value;
    int otherCount;
    int otherMask;
    int otherIndex;
    int handIndex;
    unsigned int word;
    short buffer[24];

    (void)info;
    card = SzDuel_GetUnitCardNo(side, pos);
    position = SzDuel_GetUnitLoc(side, pos);
    owner = Ta_IsUnitControl(side, pos);
    effect = SzDuel_GetUnitEffect(side, pos, 0);
    TaUtil_GetUnitLst(owner, 2, buffer);
    if (SzDuel_GetUnitStatus(side, pos, 4)) {
        return 0;
    }
    if (TaLogicCom_IsExzParts__0016cf10(card) == 1) {
        if (TaLogic_IsTactixExz__0016cf30(owner) == 1 && TaLogic_IsExzodia__0016cff0(owner) == 4) {
            return 1;
        }
    }

    switch (SzDuel_GetUnitKind(side, pos) >> 5) {
    case 5:
        if (TaLogic_GetActionReaction(side, pos, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogicCom_IsDoCerem__0016ce00(Ta_IsUnitControl(side, pos), card) == 1 &&
            SzDuel_GetField(position & 0xFF, position >> 8) != 9) {
            return 1;
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(side, pos, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (effect == 0) {
            return 2;
        }
        word = (unsigned int)effect[0];
        switch (word >> 8) {
        case 0x17: /* spellbind */
        case 0x49: /* flip and spellbind */
            if (TaLogicCom_IsUseObjEffect__0016c6b0(owner, (word >> 6) & 3, word & 0x3F) == 1) {
                return 1;
            }
            break;
        case 0x18: /* flip */
            mask = SearchUnitByRange(0, 0, owner, 2, 0x34, 0xE);
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(mask); i++) {
                value = Ta_GetUnitOwnerIDFromLstCount(mask, i);
                if (SzDuel_GetUnitStatus(value >> 8, value & 0xFF, 4) == 1) {
                    count++;
                }
            }
            if (count > 5) {
                return 1;
            }
            break;
        case 2: /* terrain change */
            value = TaLogic_GetLandChange(SzDuel_GetUnit(side, pos), position & 0xFF,
                                 position >> 8, *(unsigned char *)(effect + 1), 8, 2);
            if (value >= 0 && value == position) {
                return 1;
            }
            break;
        case 4: /* destruction */
            switch ((unsigned int)effect[1] >> 16) {
            case 0x12:
                value = TaLogicCom_GetUseXYRangeSpell__0016cbb0(side, effect);
                if (value >= 0 && ((position & 0xFF) == (value & 0xFF) ||
                                  (position >> 8) == (value >> 8))) {
                    return 1;
                }
                break;
            case 0x13:
            default:
                if (TaLogicCom_IsUseObjEffect__0016c6b0(owner, (word >> 6) & 3, word & 0x3F) == 1) {
                    return 1;
                }
                break;
            }
            break;
        case 0xD: /* alter LP */
            if (((word >> 6) & 3) == 1) {
                if (((unsigned int)effect[1] >> 16) == 4) {
                    if (SzDuel_GetLP(owner) < 1201) {
                        return 1;
                    }
                    break;
                }
                return 1;
            }
            if ((word & 0x3F) == 0x32) {
                if (Ta_GetUnitNum(owner == 0 ? 1 : 0, 3) > 3) {
                    return 1;
                }
                break;
            }
            return 1;
        case 0x1F: /* revolt/steal */
            if (card == 0x2DD && TaLogicCom_GetRevoltObj__0016b3c0(owner) >= 0) {
                return 1;
            }
            break;
        case 0x20: /* magic-neutralizing field */
            for (mask = 0; mask < 11; mask++) {
                if (SzDuel_GetUnitCurseRest(side, mask) == 0xFF) {
                    return 1;
                }
            }
            break;
        case 0x12: /* AP/DP buff */
            mask = SearchUnitByRange(0, 0, owner, 1, word & 0x3F, 0xE);
            if (Ta_GetUnitNumFrom22bitFlagList(mask) > 2) {
                return 1;
            }
            break;
        case 0x21: /* swap attack and defence */
            {
                int targetPosition;
                int sourcePosition;
                int targetY;

                TaLogic_GetUnitCtlLst2(owner == 0 ? 1 : 0, (unsigned short *)buffer, 0x1D, 0, 0);
                value = TaLogic_GetMaxApUnit((unsigned short *)buffer);
                side = value >> 8;
                pos = value & 0xFF;
                sourcePosition = SzDuel_GetUnitLoc(side, pos);
                TaLogic_GetUnitCtlLst(owner, (unsigned short *)buffer, 0x1D, 1);
                value = TaLogic_GetMaxApUnit((unsigned short *)buffer);
                card = value >> 8;
                targetY = value & 0xFF;
                targetPosition = SzDuel_GetUnitLoc(card, targetY);
                TaUtil_GetUnitLst(owner, 1, buffer);
                owner = sourcePosition & 0xFF;
                count = sourcePosition >> 8;
                if (TaUtil_GetUnitAP(side, pos, owner, count) >= 2500) {
                    count = TaUtil_GetUnitAP(side, pos, owner, count);
                    if (TaUtil_GetUnitAP(card, targetY, targetPosition & 0xFF,
                                     targetPosition >> 8) < count) {
                        return 1;
                    }
                }
            }
            break;
        case 0x36: /* transform card kind */
            return 1;
        case 0x25: /* teleport */
            switch (word & 0x3F) {
            case 0x21:
                if (TaLogic_IsAttackedNow(side, pos, 0) == 1) {
                    return 1;
                }
                break;
            case 0x22:
                if (TaLogic_IsAttackedNow(owner, 0, 0) == 1) {
                    return 1;
                }
                break;
            case 0x34:
            default:
                if (TaLogic_IsAttackedNow(owner, 0, 0) == 1) {
                    return 1;
                }
                break;
            }
            break;
        case 0x26: /* summon again */
            if (SzDuel_InqHandPhase(owner) == 0) {
                return 1;
            }
            break;
        case 0x27: /* steal summoning power */
            if (SzDuel_GetSummonPower(owner) < 4) {
                return 1;
            }
            break;
        case 0x28: /* earthshaker */
            if (TaLogic_IsAttackedNow(owner, 0, 0) == 1) {
                return 1;
            }
            break;
        }
        break;
    case 0:
        if (TaUtil_IsExMonster(side, pos) == 0) {
            return 2;
        }
        effect = Ta_GetUnitEffect(side, pos, 2);
        if (effect == 0) {
            return 2;
        }
        word = (unsigned int)effect[0];
        switch (word >> 8) {
        case 0xC: /* transform */
        case 0x34: /* boost power-ups */
        case 0xD: /* alter LP */
        case 0xE: /* deny magic and ritual */
        case 0x22: /* buff DP */
        case 0x3B: /* double LP recovery */
        case 0x56: /* reduce summoning power */
        case 0x43: /* cancel movement bonus */
        case 0xF: /* stop trap activation */
            return 1;
        case 0xB: /* immunity to spells */
        case 0x4E: /* buff by nature */
        case 0x12: /* AP/DP buff */
        case 0x4D: /* weaken by nature */
        case 6: /* double movement */
            mask = SearchUnitByRange(0, 0, side, 1, word & 0x3F, 0xE);
            if ((unsigned int)mask > 1) {
                return 1;
            }
            break;
        case 0x13: /* AP/DP reduction */
        case 0x17: /* spellbind */
            if (((word >> 6) & 1) &&
                SearchUnitByRange(0, 0, side, 1, word & 0x3F, 0xE) != 0) {
                return 2;
            }
            mask = TaLogic_RemoveHideUnit(SearchUnitByRange(0, 0, side, 2,
                                            (unsigned int)effect[0] & 0x3F, 0xE));
            if ((unsigned int)mask > 2) {
                return 1;
            }
            break;
        case 0x47: /* pause spellbind timer */
            mask = SearchUnitByRange(0, 0, side, 2, 0x34, 0xE);
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(mask); i++) {
                value = Ta_GetUnitOwnerIDFromLstCount(mask, i);
                x = value >> 8;
                y = value & 0xFF;
                if (SzDuel_GetUnitCurseRest(x, y) > 0 && SzDuel_GetUnitCurseRest(x, y) != 0xFF) {
                    return 1;
                }
            }
            break;
        case 0x53: /* remove graveyard and buff */
            for (handIndex = 0; handIndex < SzDuel_GetGraveCount(side); handIndex++) {
                if ((TaHand_GetCardKind(SzDuel_InqGrave(side, handIndex)) >> 5) == 0) {
                    count++;
                }
            }
            for (handIndex = 0; handIndex < SzDuel_GetGraveCount(side == 0 ? 1 : 0); handIndex++) {
                if ((TaHand_GetCardKind(SzDuel_InqGrave(side == 0 ? 1 : 0, handIndex)) >> 5) == 0) {
                    count++;
                }
            }
            if (count > 5) {
                return 1;
            }
            break;
        case 0x54: /* move all cards towards me */
            mask = SearchUnitByRange(0, 0, side, 3, 0x21, 0xE);
            value = Ta_GetUnitOwnerIDFromLstCount(mask, 0);
            if (side == Ta_IsUnitControl(value >> 8, value & 0xFF) &&
                value != ((side << 8) | pos)) {
                return 1;
            }
            break;
        case 0x55: /* double spellbind duration */
            mask = SearchUnitByRange(0, 0, side, 1, 0x34, 0xE);
            for (index55 = 0; index55 < Ta_GetUnitNumFrom22bitFlagList(mask); index55++) {
                value = Ta_GetUnitOwnerIDFromLstCount(mask, index55);
                x55 = value >> 8;
                y55 = value & 0xFF;
                if (SzDuel_GetUnitCurseRest(x55, y55) > 0 && SzDuel_GetUnitCurseRest(x55, y55) != 0xFF) {
                    return 1;
                }
            }
            break;
        case 0x58: /* deny leader abilities */
            if (SzData_GetRank(side == 0 ? 1 : 0,
                            SzDuel_GetUnitCardNo(side == 0 ? 1 : 0, 0)) >= 6) {
                return 1;
            }
            break;
        }
        effect = Ta_GetUnitEffect(side, pos, 3);
        if (effect == 0) {
            return 2;
        }
        word = (unsigned int)effect[0];
        switch (word >> 8) {
        case 0x12: /* AP/DP buff */
            if (((unsigned int)effect[1] >> 16) == 0x16) {
                mask = SearchUnitByRange(0, 0, owner, 1, word & 0x3F, 0x16);
            } else {
                mask = SearchUnitByRange(0, 0, owner, 1,
                                    (((unsigned int)effect[1] >> 16) << 16) | (word & 0x3F), 0xE);
            }
            if (Ta_GetUnitNumFrom22bitFlagList(mask) > 2) {
                return 1;
            }
            break;
        case 0x33: /* time wizard */
            mask = SearchUnitByRange(0, 0, owner, 1,
                                (((unsigned int)effect[1] >> 16) << 16) | (word & 0x3F), 0xE);
            if (Ta_GetUnitNumFrom22bitFlagList(mask) > 2) {
                return 1;
            }
            break;
        case 0x17: /* spellbind */
            if ((word & 0x3F) == 0x1A) {
                return 2;
            }
            if (TaLogicCom_IsUseObjEffect__0016c6b0(owner, (word >> 6) & 3, word & 0x3F) == 1) {
                return 1;
            }
            break;
        case 0xD: /* alter LP */
            return 1;
        case 0x23: /* shift to attack */
            if (TaLogicCom_IsUseObjEffect__0016c6b0(owner, (word >> 6) & 3, word & 0x3F) == 1) {
                return 1;
            }
            break;
        case 0x18: /* flip */
            mask = SearchUnitByRange(0, 0, owner, 1, word & 0x3F, 0xE);
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(mask); i++) {
                value = Ta_GetUnitOwnerIDFromLstCount(mask, i);
                if (SzDuel_GetUnitStatus(value >> 8, value & 0xFF, 4) == 1) {
                    count++;
                }
            }
            if (count > 2) {
                return 2;
            }
            otherCount = 0;
            otherMask = SearchUnitByRange(0, 0, owner, 2, (unsigned int)effect[0] & 0x3F, 0xE);
            for (otherIndex = 0; otherIndex < Ta_GetUnitNumFrom22bitFlagList(otherMask); otherIndex++) {
                value = Ta_GetUnitOwnerIDFromLstCount(otherMask, otherIndex);
                if (SzDuel_GetUnitStatus(value >> 8, value & 0xFF, 4) == 1) {
                    otherCount++;
                }
            }
            if (otherCount > 5) {
                return 1;
            }
            break;
        case 4: /* destruction */
            if ((word & 0x3F) == 0x1A) {
                return 2;
            }
            if ((word & 0x3F) == 0x30) {
                if (TaLogicCom_IsUseObjEffect__0016c6b0(owner, (word >> 6) & 3, word & 0x3F) == 1) {
                    return 1;
                }
            } else if (((unsigned int)effect[1] >> 16) != 0) {
                value = SzDuel_GetUnitLoc(side, pos);
                mask = SearchUnitByRange(value & 0xFF, value >> 8, owner,
                                    ((unsigned int)effect[0] >> 6) & 3, (unsigned int)effect[0] & 0x3F,
                                    (unsigned int)effect[1] >> 16);
            } else {
                mask = SearchUnitByRange(0, 0, owner, (word >> 6) & 3, word & 0x3F, 0xE);
            }
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(mask); i++) {
                value = Ta_GetUnitOwnerIDFromLstCount(mask, i);
                side = value >> 8;
                pos = value & 0xFF;
                if (owner == Ta_IsUnitControl(side, pos)) {
                    return 2;
                }
                if (SzDuel_GetUnitStatus(side, pos, 4)) {
                    count++;
                }
            }
            if (count > 1) {
                return 1;
            }
            break;
        case 0x1A: /* set summoning power */
            if (SzDuel_GetSummonPower(side) < 4) {
                return 1;
            }
            break;
        case 0x26: /* summon again */
            if (SzDuel_InqHandPhase(owner) == 0) {
                return 1;
            }
            break;
        case 0x1F: /* revolt/steal */
            if (Ta_SearchUncTypeNaturalEffectUserNum(0x39) > 0) {
                return 2;
            }
            word = (unsigned int)effect[0];
            if ((word & 0x3F) == 0x1A) {
                return 2;
            }
            mask = SearchUnitByRange(0, 0, owner, (word >> 6) & 3, word & 0x3F, 0xE);
            if (Ta_GetUnitNumFrom22bitFlagList(mask) > 2) {
                return 1;
            }
            break;
        case 0x3C:
            if (SearchUnitByRange(position & 0xFF, position >> 8, owner, 1,
                              ((unsigned int)*(unsigned short *)(effect + 1) << 16) | (word & 0x3F), 1) &&
                Ta_GetSummonablePiece(owner == 0 ? 1 : 0, 1) >= 0) {
                return 1;
            }
            break;
        case 0x46:
            if (TaLogic_IsAttackedNow(owner, 0, 0) == 1) {
                return 1;
            }
            break;
        case 0xC: /* transform */
            return 1;
        case 0x57:
            if (SzDuel_GetHandCount(owner) == 0) {
                return 1;
            }
            break;
        }
        break;
    }
    return 2;
}
