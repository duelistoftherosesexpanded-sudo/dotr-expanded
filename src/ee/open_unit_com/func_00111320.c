#include "dotr/ee/open_unit_com.h"

/* TaLogicCom_IsOpenUnit at 0x111320: 0 when the unit (side, pos) has status bit 4; 1 for an
 * Exodia piece (0x112960) when TaLogic_IsTactixExz__00112980 and TaLogic_IsExzodia__00112a40 (== 4)
 * agree; else by unit kind group (kind >> 5): 5 (ceremony check), 1 (the unit's own effect word,
 * effect kind in bits 8+) and 0 (effect lists 2 and 3 of an extra monster) decide 1 (open) or 2
 * (keep); every other group gives 2. The twin at 0x16b610 differs in its file-local helpers and in
 * the attack threshold of effect kind 0x21 (3000 here, 2500 there). The first
 * SzDuel_GetUnitCardNo call is written with (side, pos), which compiles to the same bytes as a
 * call without arguments (see open_unit_com.h). */
int TaLogicCom_IsOpenUnit__00111320(int side, int pos)
{
    short buffer[24];
    int mask;
    int count;
    int owner;
    unsigned int *effect;
    int card;
    int position;
    int unit;

    mask = 0;
    count = 0;
    card = SzDuel_GetUnitCardNo(side, pos);
    position = SzDuel_GetUnitLoc(side, pos);
    owner = Ta_IsUnitControl(side, pos);
    effect = SzDuel_GetUnitEffect(side, pos, 0);
    TaUtil_GetUnitLst(owner, 2, buffer);
    if (SzDuel_GetUnitStatus(side, pos, 4) != 0) {
        return 0;
    }
    if (TaLogicCom_IsExzParts__00112960(card) == 1) {
        if (TaLogic_IsTactixExz__00112980(owner) == 1 && TaLogic_IsExzodia__00112a40(owner) == 4) {
            return 1;
        }
    }
    switch (SzDuel_GetUnitKind(side, pos) >> 5) {
    case 5:
        if (TaLogic_GetActionReaction(side, pos, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogicCom_IsDoCerem__00112850(Ta_IsUnitControl(side, pos), card) == 1 &&
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
        switch (effect[0] >> 8) {
        case 0x17: /* spellbind */
        case 0x49: /* flip and spellbind */
            if (TaLogicCom_IsUseObjEffect__001123c0(owner, (effect[0] >> 6) & 3, effect[0] & 0x3F) == 1) {
                return 1;
            }
            break;
        case 0x18: /* flip */
        {
            int index;
            mask = SearchUnitByRange(0, 0, owner, 2, 0x34, 0xE);
            for (index = 0; index < Ta_GetUnitNumFrom22bitFlagList(mask); index++) {
                unit = Ta_GetUnitOwnerIDFromLstCount(mask, index);
                if (SzDuel_GetUnitStatus(unit >> 8, unit & 0xFF, 4) == 1) {
                    count++;
                }
            }
            if (count > 5) {
                return 1;
            }
            break;
        }
        case 2: /* terrain change */
            unit = TaLogic_GetLandChange(SzDuel_GetUnit(side, pos), position & 0xFF,
                                position >> 8, *((unsigned char *)effect + 4), 8, 2);
            if (unit >= 0 && unit == position) {
                return 1;
            }
            break;
        case 4: /* destruction */
            switch (effect[1] >> 16) {
            case 0x12:
                unit = TaLogicCom_GetUseXYRangeSpell__00112600(side, (int *)effect);
                if (unit >= 0 && ((position & 0xFF) == (unit & 0xFF) ||
                                  (position >> 8) == (unit >> 8))) {
                    return 1;
                }
                break;
            case 0x13:
            default:
                if (TaLogicCom_IsUseObjEffect__001123c0(owner, (effect[0] >> 6) & 3, effect[0] & 0x3F) == 1) {
                    return 1;
                }
                break;
            }
            break;
        case 0xD: /* alter LP */
            if (((effect[0] >> 6) & 3) == 1) {
                if ((effect[1] >> 16) == 4) {
                    if (SzDuel_GetLP(owner) <= 1200) {
                        return 1;
                    }
                    break;
                }
                return 1;
            }
            if ((effect[0] & 0x3F) == 0x32) {
                if (Ta_GetUnitNum(owner == 0 ? 1 : 0, 3) > 3) {
                    return 1;
                }
                break;
            }
            return 1;
        case 0x1F: /* revolt/steal */
            if (card == 0x2DD && TaLogicCom_GetRevoltObj__001110d0(owner) >= 0) {
                return 1;
            }
            break;
        case 0x20: /* magic-neutralizing field */
            mask = 0;
            do {
                if (SzDuel_GetUnitCurseRest(side, mask) == 0xFF) {
                    return 1;
                }
                mask++;
            } while (mask < 11);
            break;
        case 0x12: /* AP/DP buff */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, owner, 1, effect[0] & 0x3F, 0xE)) > 2) {
                return 1;
            }
            break;
        case 0x21: /* swap attack and defence */
            {
                int targetPosition;
                int sourcePosition;
                int targetY;

                TaLogic_GetUnitCtlLst2(owner == 0 ? 1 : 0, (unsigned short *)buffer, 0x1D, 0, 0);
                unit = TaLogic_GetMaxApUnit((unsigned short *)buffer);
                side = unit >> 8;
                pos = unit & 0xFF;
                sourcePosition = SzDuel_GetUnitLoc(side, pos);
                TaLogic_GetUnitCtlLst(owner, (unsigned short *)buffer, 0x1D, 1);
                unit = TaLogic_GetMaxApUnit((unsigned short *)buffer);
                card = unit >> 8;
                targetY = unit & 0xFF;
                targetPosition = SzDuel_GetUnitLoc(card, targetY);
                TaUtil_GetUnitLst(owner, 1, buffer);
                owner = sourcePosition & 0xFF;
                count = sourcePosition >> 8;
                if (TaUtil_GetUnitAP(side, pos, owner, count) >= 3000) {
                    count = TaUtil_GetUnitAP(side, pos, owner, count);
                    if (TaUtil_GetUnitAP(card, targetY, targetPosition & 0xFF, targetPosition >> 8) < count) {
                        return 1;
                    }
                }
            }
            break;
        case 0x36: /* transform card kind */
            return 1;
        case 0x25: /* teleport */
            switch (effect[0] & 0x3F) {
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
        switch (effect[0] >> 8) {
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
            mask = SearchUnitByRange(0, 0, side, 1, effect[0] & 0x3F, 0xE);
            if ((unsigned int)mask > 1) {
                return 1;
            }
            break;
        case 0x13: /* AP/DP reduction */
        case 0x17: /* spellbind */
            if (((effect[0] >> 6) & 1) && SearchUnitByRange(0, 0, side, 1, effect[0] & 0x3F, 0xE)) {
                return 2;
            }
            mask = TaLogic_RemoveHideUnit(SearchUnitByRange(0, 0, side, 2, effect[0] & 0x3F, 0xE));
            if ((unsigned int)mask > 2) {
                return 1;
            }
            break;
        case 0x47: /* pause spellbind timer */
        {
            int y;
            int x;
            int index;
            mask = SearchUnitByRange(0, 0, side, 2, 0x34, 0xE);
            for (index = 0; index < Ta_GetUnitNumFrom22bitFlagList(mask); index++) {
                unit = Ta_GetUnitOwnerIDFromLstCount(mask, index);
                x = unit >> 8;
                y = unit & 0xFF;
                if (SzDuel_GetUnitCurseRest(x, y) > 0 && SzDuel_GetUnitCurseRest(x, y) != 0xFF) {
                    return 1;
                }
            }
            break;
        }
        case 0x53: /* remove graveyard and buff */
        {
            int index;
            for (index = 0; index < SzDuel_GetGraveCount(side); index++) {
                if ((TaHand_GetCardKind(SzDuel_InqGrave(side, index)) >> 5) == 0) {
                    count++;
                }
            }
            for (index = 0; index < SzDuel_GetGraveCount(side == 0 ? 1 : 0); index++) {
                if ((TaHand_GetCardKind(SzDuel_InqGrave(side == 0 ? 1 : 0, index)) >> 5) == 0) {
                    count++;
                }
            }
            if (count > 5) {
                return 1;
            }
            break;
        }
        case 0x54: /* move all cards towards me */
            mask = SearchUnitByRange(0, 0, side, 3, 0x21, 0xE);
            unit = Ta_GetUnitOwnerIDFromLstCount(mask, 0);
            if (side == Ta_IsUnitControl(unit >> 8, unit & 0xFF) && unit != ((side << 8) | pos)) {
                return 1;
            }
            break;
        case 0x55: /* double spellbind duration */
        {
            int y;
            int x;
            int index;
            mask = SearchUnitByRange(0, 0, side, 1, 0x34, 0xE);
            for (index = 0; index < Ta_GetUnitNumFrom22bitFlagList(mask); index++) {
                unit = Ta_GetUnitOwnerIDFromLstCount(mask, index);
                x = unit >> 8;
                y = unit & 0xFF;
                if (SzDuel_GetUnitCurseRest(x, y) > 0 && SzDuel_GetUnitCurseRest(x, y) != 0xFF) {
                    return 1;
                }
            }
            break;
        }
        case 0x58: /* deny leader abilities */
            if (SzData_GetRank(side == 0 ? 1 : 0, SzDuel_GetUnitCardNo(side == 0 ? 1 : 0, 0)) >= 6) {
                return 1;
            }
            break;
        }
        effect = Ta_GetUnitEffect(side, pos, 3);
        if (effect == 0) {
            return 2;
        }
        switch (effect[0] >> 8) {
        case 0x12: /* AP/DP buff */
            if ((effect[1] >> 16) == 0x16) {
                unit = SearchUnitByRange(0, 0, owner, 1, effect[0] & 0x3F, 0x16);
            } else {
                unit = SearchUnitByRange(0, 0, owner, 1, ((effect[1] >> 16) << 16) | (effect[0] & 0x3F), 0xE);
            }
            if (Ta_GetUnitNumFrom22bitFlagList(unit) > 2) {
                return 1;
            }
            break;
        case 0x33: /* time wizard */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, owner, 1, ((effect[1] >> 16) << 16) | (effect[0] & 0x3F), 0xE)) > 2) {
                return 1;
            }
            break;
        case 0x17: /* spellbind */
            if ((effect[0] & 0x3F) == 0x1A) {
                return 2;
            }
            if (TaLogicCom_IsUseObjEffect__001123c0(owner, (effect[0] >> 6) & 3, effect[0] & 0x3F) == 1) {
                return 1;
            }
            break;
        case 0xD: /* alter LP */
            return 1;
        case 0x23: /* shift to attack */
            if (TaLogicCom_IsUseObjEffect__001123c0(owner, (effect[0] >> 6) & 3, effect[0] & 0x3F) == 1) {
                return 1;
            }
            break;
        case 0x18: /* flip */
        {
            int targets;
            int index;

            targets = SearchUnitByRange(0, 0, owner, 1, effect[0] & 0x3F, 0xE);
            for (index = 0; index < Ta_GetUnitNumFrom22bitFlagList(targets); index++) {
                unit = Ta_GetUnitOwnerIDFromLstCount(targets, index);
                if (SzDuel_GetUnitStatus(unit >> 8, unit & 0xFF, 4) == 1) {
                    count++;
                }
            }
            if (count > 2) {
                return 2;
            }
            {
                int unavailable;
                int others;
                int otherIndex;

                unavailable = 0;
                others = SearchUnitByRange(0, 0, owner, 2, effect[0] & 0x3F, 0xE);
                for (otherIndex = 0; otherIndex < Ta_GetUnitNumFrom22bitFlagList(others); otherIndex++) {
                    unit = Ta_GetUnitOwnerIDFromLstCount(others, otherIndex);
                    if (SzDuel_GetUnitStatus(unit >> 8, unit & 0xFF, 4) == 1) {
                        unavailable++;
                    }
                }
                if (unavailable > 5) {
                    return 1;
                }
            }
            break;
        }
        case 4: /* destruction */
        {
            int y;
            int x;
            int index;
            if ((effect[0] & 0x3F) == 0x1A) {
                return 2;
            }
            if ((effect[0] & 0x3F) == 0x30) {
                if (TaLogicCom_IsUseObjEffect__001123c0(owner, (effect[0] >> 6) & 3, effect[0] & 0x3F) == 1) {
                    return 1;
                }
            } else {
                if ((effect[1] >> 16) != 0) {
                    unit = SzDuel_GetUnitLoc(side, pos);
                    mask = SearchUnitByRange(unit & 0xFF, unit >> 8, owner, (effect[0] >> 6) & 3,
                                        effect[0] & 0x3F, effect[1] >> 16);
                } else {
                    mask = SearchUnitByRange(0, 0, owner, (effect[0] >> 6) & 3, effect[0] & 0x3F, 0xE);
                }
            }
            for (index = 0; index < Ta_GetUnitNumFrom22bitFlagList(mask); index++) {
                unit = Ta_GetUnitOwnerIDFromLstCount(mask, index);
                x = unit >> 8;
                y = unit & 0xFF;
                if (owner == Ta_IsUnitControl(x, y)) {
                    return 2;
                }
                if (SzDuel_GetUnitStatus(x, y, 4) != 0) {
                    count++;
                }
            }
            if (count > 1) {
                return 1;
            }
            break;
        }
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
            if ((effect[0] & 0x3F) == 0x1A) {
                return 2;
            }
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, owner, (effect[0] >> 6) & 3, effect[0] & 0x3F, 0xE)) > 2) {
                return 1;
            }
            break;
        case 0x3C:
            if (SearchUnitByRange(position & 0xFF, position >> 8, owner, 1,
                             ((unsigned int)*((unsigned short *)effect + 2) << 16) | (effect[0] & 0x3F), 1) &&
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
    default:
        break;
    }
    return 2;
}
