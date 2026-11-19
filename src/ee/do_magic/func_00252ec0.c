#include "dotr/ee/do_magic.h"

/* Builds the effect record *eInfo (sequence number eNO and its three operands) for the effect
 * *temp of the unit nSide/nID, by effect type (temp->dwID >> 8); bits 6-7 of dwID select the
 * side(s) searched and bits 0-5 the kind, dwParam carries a high and a low 16-bit operand. Every
 * path removes the unit with Ta_DelUnit. Returns 0 when the effect has no object or does not
 * apply, 1 when the record is filled, 2 when pInfo was filled too (types 20 and 31/51). The types
 * that collect a 22-bit unit list (TAIL) replace the record with sequence 254 (0, 0xFFFF, 16)
 * when the list is empty. Type 2 gathers the board cells within Manhattan distance dwParam >> 16
 * of the unit and type 72 the cells whose field value is 8, one of which is picked with the
 * library's pseudo-random number.
 * Name, prototype, parameter, local and member names from the Japanese build's debug information
 * (Magic.c); the locals are declared in the reverse of that order except eObj, which follows
 * sOwner (the order the register allocation requires). That prototype has five parameters;
 * the code reads `temp` from the sixth argument register and Ta_Open passes its step number
 * fifth, so PrNo (unused here) is assumed.
 * Which local holds which value is a hypothesis fitted to the register allocation: EFC also holds
 * the picked cell of type 72, k the flipped side, Num the loop counter of type 37. */
/* Shorthands for the stretches the function repeats; whether the original used macros is unknown. */
#define CUR Ta_GetCurrentSide(nSide, nID)
#define DEL Ta_DelUnit(nSide, nID)
#define SET(a, b, c, d) eInfo->eNO = a; eInfo->dwFrom = b; eInfo->dwTo = c; eInfo->dwPar = d
#define NONE eInfo->eNO = 254; eInfo->dwFrom = 0; eInfo->dwTo = 0xFFFF; eInfo->dwPar = 16
#define TAIL(u) if (Ta_GetUnitNumFrom22bitFlagList(u) == 0) { NONE; } return 1
#define SRCH(o)                                                                                   \
    Ta_RemoveObjFromFlagLst2(                                                                     \
        ConvUnitIDtoFlagList_OFF(SearchUnitByRange(0, 0, CUR, ((o) >> 6) & 3, (o) & 0x3F, 14),   \
                                 nSide, nID),                                                     \
        sOwner)
#define PARL ((unsigned short)temp->dwParam)
#define PARH (temp->dwParam >> 16)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int Ta_Do_Magic(int nSide, int nID, struct jp_1073d *eInfo, struct jp_107e2 *pInfo, int PrNo,
                struct jp_10845 *temp)
{
    int i;
    int j;
    int k;
    int Num;
    int Unit;
    int pKindP;
    int pKindS;
    int fLOC[49];
    int Sacri[3];
    int LOC;
    int EFC;
    int sOwner;
    unsigned int eObj;

    (void)PrNo;
    for (i = 0; i < 49; i++) {
        fLOC[i] = -1;
    }
    sOwner = CUR;
    eObj = temp->dwID;
    switch (eObj >> 8) {
    case 23: /* Spellbind */
        Unit = SRCH(eObj);
        SET(0, 0, Unit, PARL);
        DEL;
        if (Ta_GetUnitNumFrom22bitFlagList(Unit) == 0) {
            NONE;
            return 1;
        }
        return 1;
    case 20: /* Revive */
        eInfo->eNO = 1;
        eInfo->dwFrom = CUR | 0x20000;
        eInfo->dwTo = SzDuel_GetUnitLoc(nSide, nID);
        eInfo->dwPar = 999;
        switch ((temp->dwID >> 6) & 3) {
        case 3:
            pKindS = 2;
            break;
        case 1:
            pKindS = CUR != 0 ? 1 : 0;
            break;
        case 2:
            pKindS = CUR == 0 ? 1 : 0;
            break;
        default:
            DEL;
            return 0;
        }
        Num = 0;
        switch (temp->dwID & 0x3F) {
        case 29: /* search: monster */
            if (pKindS == 2) {
                Num += TaUtil_GetGraveCardNum(0, 0);
                Num += TaUtil_GetGraveCardNum(1, 0);
            } else {
                Num += TaUtil_GetGraveCardNum(pKindS, 0);
            }
            if (Num == 0) {
                DEL;
                return 0;
            }
            pKindP = 1;
            break;
        case 30: /* search: non monster */
            if (pKindS == 2) {
                Num += TaUtil_GetGraveCardNum(0, 1);
                Num += TaUtil_GetGraveCardNum(1, 1);
                Num += TaUtil_GetGraveCardNum(0, 2);
                Num += TaUtil_GetGraveCardNum(1, 2);
                Num += TaUtil_GetGraveCardNum(0, 3);
                Num += TaUtil_GetGraveCardNum(1, 3);
                Num += TaUtil_GetGraveCardNum(0, 4);
                Num += TaUtil_GetGraveCardNum(1, 4);
                Num += TaUtil_GetGraveCardNum(0, 5);
                Num += TaUtil_GetGraveCardNum(1, 5);
            } else {
                Num += TaUtil_GetGraveCardNum(pKindS, 1);
                Num += TaUtil_GetGraveCardNum(pKindS, 2);
                Num += TaUtil_GetGraveCardNum(pKindS, 3);
                Num += TaUtil_GetGraveCardNum(pKindS, 4);
                Num += TaUtil_GetGraveCardNum(pKindS, 5);
            }
            if (Num == 0) {
                DEL;
                return 0;
            }
            pKindP = 2;
            break;
        case 31: /* search: all cards */
            if (pKindS == 2) {
                Num += SzDuel_GetGraveCount(0);
                Num += SzDuel_GetGraveCount(1);
            } else {
                Num += SzDuel_GetGraveCount(pKindS);
            }
            if (Num == 0) {
                DEL;
                return 0;
            }
            pKindP = 0;
            break;
        default:
            DEL;
            return 0;
        }
        pInfo->bType = 0;
        pInfo->bKind = (pKindS << 4) | pKindP;
        DEL;
        return 2;
    case 2: /* Terrain Change */
        Num = 0;
        LOC = SzDuel_GetUnitLoc(nSide, nID);
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                if ((unsigned int)((MAX(i, LOC & 0xFF) - MIN(i, LOC & 0xFF)) +
                                   (MAX(j, LOC >> 8) - MIN(j, LOC >> 8))) <= PARH) {
                    fLOC[Num] = (j << 8) | i;
                    Num++;
                }
            }
        }
        switch (PARL) {
        case 8:
            SET(2, LOC, PARH, PARL);
            break;
        default:
            SET(2, LOC, PARH, PARL);
            break;
        }
        DEL;
        return 1;
    case 72: /* Magical Lab */
        Num = 0;
        LOC = SzDuel_GetUnitLoc(nSide, nID);
        for (i = 0; i < 49; i++) {
            fLOC[i] = -1;
        }
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                if (SzDuel_GetField(i, j) == 8) {
                    fLOC[Num] = (j << 8) | i;
                    Num++;
                }
            }
        }
        if (Num == 0) {
            DEL;
            return 0;
        }
        EFC = fLOC[func_0019a5c8() % Num];
        if (EFC < 0) {
            EFC = 0;
        }
        SET(3, EFC, LOC, 0);
        DEL;
        return 1;
    case 4: /* Destruction */
        switch (PARH) {
        case 18:
            LOC = SzDuel_GetUnitLoc(nSide, nID);
            Unit = ConvUnitIDtoFlagList_OFF(
                ConvUnitIDtoFlagList_OFF(
                    ConvUnitIDtoFlagList_OFF(SearchUnitByRange(LOC & 0xFF, LOC >> 8, CUR,
                                                               (temp->dwID >> 6) & 3,
                                                               temp->dwID & 0x3F, 18),
                                             nSide, nID),
                    nSide, 0),
                nSide == 0 ? 1 : 0, 0);
            switch (SzDuel_GetUnitCardNo(nSide, nID)) {
            case 700: /* Raigeki */
                SET(5, LOC, Unit, sOwner | (CUR << 16));
                DEL;
                return 1;
            case 702: /* Harpies Feather Duster */
                SET(7, LOC, Unit, sOwner | (CUR << 16));
                DEL;
                return Ta_GetUnitNumFrom22bitFlagList(Unit) == 0 ? 0 : 1;
            default:
                DEL;
                return 0;
            }
        case 19:
            SET(25, 0, 0, sOwner);
            Ta_SetEffectObjByField((struct DotrMiscBitWords *)eInfo, fLOC);
            DEL;
            return 1;
        case 0:
            Unit = SRCH(eObj);
            switch (SzDuel_GetUnitCardNo(nSide, nID)) {
            case 699: /* Dark Hole */
                SET(4, CUR, Unit, 0);
                DEL;
                TAIL(Unit);
            case 701: /* Heavy Storm */
                SET(6, 0, Unit, 0);
                DEL;
                TAIL(Unit);
            case 721: /* Eternal Rest */
                SET(16, 0, Unit, 0);
                DEL;
                TAIL(Unit);
            case 722: /* Stain Storm */
                SET(17, 0, Unit, 0);
                DEL;
                TAIL(Unit);
            case 728: /* Exile of the Wicked */
                SET(18, 0, Unit, 0);
                DEL;
                TAIL(Unit);
            case 729: /* Dust Tornado */
                SET(19, 0, Unit, 0);
                DEL;
                TAIL(Unit);
            case 730: /* Cold Wave */
                SET(20, 0, Unit, 0);
                DEL;
                TAIL(Unit);
            case 727: /* Last Day of Witch */
                SET(23, 0, Unit, 0);
                DEL;
                TAIL(Unit);
            default:
                SET(21, 0, Unit, 0);
                DEL;
                TAIL(Unit);
            }
        default:
            DEL;
            return 0;
        }
    case 13: /* Alter LP (Heal/Damage) */
        Num = (PARH << 16) | (temp->dwParam & 0xFFFF);
        if ((eObj & 0x3F) == 50) {
            Num = (PARH << 16) | ((temp->dwParam & 0xFFFF) * Ta_GetUnitNum(CUR == 0 ? 1 : 0, 3));
        }
        if ((Num & 0xFFFF) == 0) {
            DEL;
            return 0;
        }
        switch ((temp->dwID >> 6) & 3) {
        case 1:
            SET(8, 0, Ta_GetEOwnerSide(CUR, (temp->dwID >> 6) & 3), Num);
            DEL;
            return 1;
        case 2:
            SET(9, SzDuel_GetUnitLoc(nSide, nID), Ta_GetEOwnerSide(CUR, (temp->dwID >> 6) & 3), Num);
            DEL;
            return 1;
        default:
            DEL;
            return 0;
        }
    case 73: /* Flip and Spellbind (SORL) */
        Unit = SRCH(eObj);
        SET(12, 0, Unit, PARL);
        DEL;
        TAIL(Unit);
    case 24: /* Flip card */
        Unit = SRCH(eObj);
        SET(13, 4, Unit, PARL);
        DEL;
        TAIL(Unit);
    case 29: /* Reveal Hand */
        eInfo->eNO = 14;
        eInfo->dwFrom = 0;
        if (CUR == 0) {
            k = 1;
        } else {
            k = 0;
        }
        eInfo->dwTo = k;
        eInfo->dwPar = 0;
        DEL;
        return 1;
    case 30: /* Reveal Types */
        Unit = SRCH(eObj);
        SET(15, 0, Unit, 0);
        DEL;
        TAIL(Unit);
    case 31: /* Revolt/Steal */
        if (Ta_SearchUncTypeNaturalEffectUserNum(57) > 0) {
            DEL;
            return 0;
        }
        switch (temp->dwID & 0x3F) {
        case 33: /* search: card with best stats */
            Unit = SRCH(temp->dwID);
            SET(26, sOwner, Unit, 0xFFFE);
            DEL;
            TAIL(Unit);
        case 51:
            Unit = Ta_RemoveObjFromFlagLst2(
                ConvUnitIDtoFlagList_OFF(SearchUnitByRange(0, 0, CUR, 2, 52, 14), nSide, nID), sOwner);
            SET(26, sOwner, Unit, 0);
            pInfo->bType = 1;
            if (CUR == 0) {
                k = 1;
            } else {
                k = 0;
            }
            pInfo->bKind = k << 4;
            DEL;
            if (Ta_GetUnitNumFrom22bitFlagList(Unit) == 0) {
                NONE;
                return 1;
            }
            return 2;
        default:
            DEL;
            return 0;
        }
    case 32: /* Magic Neutralizing Force Field */
        Unit = ConvUnitIDtoFlagList_OFF(SearchUnitByRange(0, 0, CUR, 3, 54, 14), nSide, nID);
        SET(28, CUR, Unit, 0);
        DEL;
        return 1;
    case 18: /* AP/DP Buff */
        switch (SzDuel_GetUnitCardNo(nSide, nID)) {
        case 739: /* Rain of Mercy */
            EFC = 90;
            break;
        case 740: /* Windstorm of Etaqua */
            EFC = 92;
            break;
        default:
            EFC = 29;
            break;
        }
        eObj = temp->dwID;
        Unit = SRCH(eObj);
        SET(EFC, 1, Unit, PARL);
        DEL;
        TAIL(Unit);
    case 33: /* Swap All Atk & Def */
        Unit = SRCH(eObj);
        SET(91, 0, Unit, 0);
        DEL;
        TAIL(Unit);
    case 34: /* Buff DP */
        Unit = SRCH(eObj);
        SET(29, 0, Unit, PARL);
        DEL;
        TAIL(Unit);
    case 35: /* Shift to Attack */
        Unit = SRCH(eObj);
        SET(13, 5, Unit, PARL);
        DEL;
        TAIL(Unit);
    case 54: /* Transform Card Kind */
        Unit = Ta_RemoveObjFromFlagLst2(SearchUnitByRange(0, 0, CUR, (eObj >> 6) & 3, eObj & 0x3F, 14),
                                        sOwner);
        SET(66, 54, Unit, PARL);
        DEL;
        TAIL(Unit);
    case 36: /* Return To Deck */
        Unit = SRCH(eObj);
        SET(30, 0, Unit, CUR);
        DEL;
        TAIL(Unit);
    case 37: /* Teleport */
        switch (PARH) {
        case 33:
            Unit = Ta_RemoveObjFromFlagLst2(
                ConvUnitIDtoFlagList_OFF(
                    ConvUnitIDtoFlagList_OFF(
                        ConvUnitIDtoFlagList_OFF(
                            SearchUnitByRange(0, 0, CUR, (eObj >> 6) & 3, eObj & 0x3F, 14), nSide, nID),
                        0, 0),
                    1, 0),
                sOwner);
            SET(32, 0, Unit, 0);
            DEL;
            TAIL(Unit);
        default:
            break;
        }
        if ((eObj & 0x3F) == 34) {
            Unit = ConvUnitIDtoFlagList(0, sOwner, 0);
        } else {
            Unit = SRCH(eObj);
        }
        LOC = SzDuel_GetUnitLoc(nSide, nID);
        for (Num = 0; Num < Ta_GetUnitNumFrom22bitFlagList(Unit); Num++) {
            j = Ta_GetUnitOwnerIDFromLstCount(Unit, Num);
            SET(31, j, Unit, LOC);
        }
        DEL;
        TAIL(Unit);
    case 38: /* Summon Again */
        SET(33, 0, CUR, 0);
        DEL;
        return 1;
    case 39: /* Steal SP */
        eInfo->eNO = 34;
        if (CUR == 0) {
            k = 1;
        } else {
            k = 0;
        }
        eInfo->dwFrom = k;
        eInfo->dwTo = CUR;
        eInfo->dwPar = SzDuel_GetSummonPower(CUR == 0 ? 1 : 0);
        DEL;
        return 1;
    case 40: /* Earthshaker */
        SET(35, sOwner, 0, 0);
        DEL;
        return 1;
    case 50: /* Ritual */
        Ta_GetCeremSacrifice(Sacri, temp->dwParam);
        if (Sacri[0] < 0 || Sacri[1] < 0 || Sacri[2] < 0) {
            DEL;
            return 0;
        }
        Unit = Ta_GetSacrificeUnit(Sacri, CUR);
        if (Unit < 0) {
            DEL;
            return 0;
        }
        SET(56, Unit, temp->dwParam, (CUR << 16) | SzDuel_GetUnitLoc(nSide, nID));
        DEL;
        return 1;
    default:
        DEL;
        return 0;
    }
}
